#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <iostream>

#include "../ports.h"

using namespace boost::asio;

class tcp_connection : public std::enable_shared_from_this<tcp_connection> {
   private:
    ip::tcp::socket socket_;
    streambuf buff_;

   public:
    using conn_shared_ptr = std::shared_ptr<tcp_connection>;
    ip::tcp::socket& socket() { return socket_; }

    void start() {
        async_read_until(
            socket_, buff_, '\0',
            std::bind(&tcp_connection::handle_read_query, shared_from_this(),
                      std::placeholders::_1, std::placeholders::_2));
    }

    static auto create(io_context& io_context) {
        return conn_shared_ptr(new tcp_connection(io_context));
    }

   private:
    tcp_connection(io_context& io_context) : socket_(io_context) {}

    void handle_read_query(const boost::system::error_code& ec,
                     std::size_t bytes_transferred) {
        if (!ec) {
            std::string data{std::istreambuf_iterator<char>(&buff_),
                             std::istreambuf_iterator<char>()};
            std::cout << data << std::endl;

            async_write(
                socket_, buffer("abcdefgh\0"),
                std::bind(&tcp_connection::handle_write_response, shared_from_this(),
                          std::placeholders::_1, std::placeholders::_2));
        } else {
            std::cout << "error: " << ec.message() << std::endl;
        }
    }

    void handle_write_response(const boost::system::error_code& ec,
                      std::size_t /*bytes_transferred*/) {
        if (!ec) {
            start();
        } else {
            std::cout << "Error sending response: " << ec.message()
                      << std::endl;
        }
    }
};

class TGS {
   private:
    io_context& io_context_;
    ip::tcp::acceptor acceptor_;

   public:
    TGS(io_context& io_context, short port)
        : io_context_(io_context),
          acceptor_(io_context, ip::tcp::endpoint(ip::tcp::v4(), port)) {
        accept();
    }

   private:
    void accept() {
        auto tcp_conn = tcp_connection::create(io_context_);
        acceptor_.async_accept(tcp_conn->socket(),
                               std::bind(&TGS::handle_accept, this, tcp_conn,
                                         std::placeholders::_1));
    }

    void handle_accept(std::shared_ptr<tcp_connection> tcp_conn,
                       const boost::system::error_code& ec) {
        if (!ec) {
            tcp_conn->start();
        } else {
            std::cout << ec.message() << '\n';
        }
        accept();
    }
};

int main(int argc, char* argv[]) {
    io_context io_context;
    TGS s(io_context, ports::SS_PORT);
    io_context.run();
    return 0;
}