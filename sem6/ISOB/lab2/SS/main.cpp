#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <iostream>

class session : public std::enable_shared_from_this<session> {
   private:
    boost::asio::ip::tcp::socket socket_;
    boost::asio::streambuf buff_;

   public:
    session(boost::asio::ip::tcp::socket socket)
        : socket_(std::move(socket)) {}

    void run() { wait_for_request(); }

   private:
    void wait_for_request() {
        boost::asio::async_read_until(
            socket_, buff_, "\0",
            std::bind(&session::handle_read, this, std::placeholders::_1,
                      std::placeholders::_2));
    }

    void handle_read(const boost::system::error_code& ec,
                     std::size_t /*bytes_transferred*/) {
        if (!ec) {
            std::string data{std::istreambuf_iterator<char>(&buff_),
                             std::istreambuf_iterator<char>()};
            std::cout << data << std::endl;

            boost::asio::async_write(
                socket_, boost::asio::buffer("abcdefgh\0"),
                std::bind(&session::handle_write, this, std::placeholders::_1,
                          std::placeholders::_2));
        } else {
            std::cout << "error: " << ec << std::endl;
        }
    }

    void handle_write(const boost::system::error_code& ec,
                      std::size_t /*bytes_transferred*/) {
        if (!ec) {
            wait_for_request();
        } else {
            std::cout << "Error sending response: " << ec << std::endl;
        }
    }
};

class server {
   private:
    boost::asio::ip::tcp::acceptor acceptor_;

   public:
    server(boost::asio::io_context& io_context, short port)
        : acceptor_(io_context, boost::asio::ip::tcp::endpoint(
                                    boost::asio::ip::tcp::v4(), port)) {
        accept();
    }

   private:
    void accept() {
        acceptor_.async_accept(std::bind(&server::handle_accept, this,
                                         std::placeholders::_1,
                                         std::placeholders::_2));
    }

    void handle_accept(const boost::system::error_code& ec,
                       boost::asio::ip::tcp::socket socket) {
        if (!ec) {
            std::make_shared<session>(std::move(socket))->run();
        } else {
            std::cout << ec.message() << '\n';
        }
        accept();
    }
};

int main(int argc, char* argv[]) {
    boost::asio::io_context io_context;
    server s(io_context, 1234);
    io_context.run();
    return 0;
}