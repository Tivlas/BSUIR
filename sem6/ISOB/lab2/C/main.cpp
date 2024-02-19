#include <boost/asio.hpp>
#include <iostream>

int main() {
    boost::asio::io_context io_context;
    boost::asio::ip::tcp::socket socket(io_context);
    boost::asio::ip::tcp::resolver resolver(io_context);

    boost::asio::connect(socket, resolver.resolve("127.0.0.1", "7001"));

    std::string data{"a234"};
    data.push_back('\0');
    auto result = boost::asio::write(socket, boost::asio::buffer(data));

    boost::asio::streambuf resp;
    boost::asio::read_until(socket, resp, '\0');

    std::string resp_data{std::istreambuf_iterator<char>(&resp),
                          std::istreambuf_iterator<char>()};

    std::cout << "data recv: " << resp_data;

    boost::system::error_code ec;
    socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
    socket.close();
}