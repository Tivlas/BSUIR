#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <iostream>

using namespace boost::asio;
using namespace boost::asio::ip;

using namespace boost::asio;

class connection : public std::enable_shared_from_this<connection> {
   private:
    ip::tcp::socket socket_;
    streambuf buff_;

   public:
    using conn_shared_ptr = std::shared_ptr<connection>;
    ip::tcp::socket& socket() { return socket_; }

    void start() {
        /* async_read_until(
            socket_, buff_, '\0',
            std::bind(&connection::handle_read_request, shared_from_this(),
                      std::placeholders::_1, std::placeholders::_2)); */
    }

    static auto create(boost::asio::io_context& io_context) {
        return conn_shared_ptr(new connection(io_context));
    }

   private:
    connection(boost::asio::io_context& io_context) : socket_(io_context) {
        buff_.prepare(1024);
    }

    void print(const std::string& msg) { std::cout << msg << std::endl; }

    void handle_read_request(const boost::system::error_code& ec, std::size_t) {
        /* if (!ec) {
            std::string data{std::istreambuf_iterator<char>(&buff_),
                             std::istreambuf_iterator<char>()};
            std::cout << "Data: " << data << std::endl;
        } else {
            print("error reading data.");
        } */
    }
};

class server {
   private:
    io_context& io_context_;
    ip::tcp::acceptor acceptor_;
    size_t conn_cnt_ = 0;

   public:
    server(boost::asio::io_context& io_context, short port)
        : io_context_(io_context),
          acceptor_(io_context, ip::tcp::endpoint(ip::tcp::v4(), port)) {
        accept();
    }

   private:
    void accept() {
        auto tcp_conn = connection::create(io_context_);
        std::cout << conn_cnt_++ << std::endl;
        acceptor_.async_accept(tcp_conn->socket(),
                               std::bind(&server::handle_accept, this, tcp_conn,
                                         std::placeholders::_1));
    }

    void handle_accept(std::shared_ptr<connection> tcp_conn,
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
    if (argc != 2) {
        std::cerr << "Usage: server <port>\n";
        return 1;
    }

    unsigned short port = static_cast<unsigned short>(std::atoi(argv[1]));

    io_context io_context;
    server s(io_context, port);
    io_context.run();
    return 0;
}
