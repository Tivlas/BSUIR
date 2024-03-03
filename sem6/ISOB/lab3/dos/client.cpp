#include <boost/asio.hpp>
#include <iostream>

using namespace boost::asio;
using namespace boost::asio::ip;

void handle_connect(const boost::system::error_code& error) {
    if (error) {
        std::cerr << "false";
    } else {
        std::cerr << "true";
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    try {
        if (argc != 4) {
            std::cerr << "Usage: <target IP> <target port> <N>\n";
            return 1;
        }

        std::string target_ip = argv[1];
        unsigned short target_port =
            static_cast<unsigned short>(std::atoi(argv[2]));

        int N = static_cast<int>(std::atoi(argv[3]));

        io_context io_context;
        tcp::resolver resolver(io_context);
        tcp::resolver::query query(tcp::v4(), target_ip,
                                   std::to_string(target_port));
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
        std::vector<tcp::socket*> sockets;
        std::vector<boost::asio::io_context*> contexts;
        sockets.reserve(N);
        contexts.reserve(N);
        int cnt = 0;
        for (cnt = 0; cnt < N; ++cnt) {
            auto ctx_ptr = new boost::asio::io_context();
            auto skt_ptr = new tcp::socket(*ctx_ptr);
            contexts.push_back(ctx_ptr);
            sockets.push_back(skt_ptr);
            sockets.back()->async_connect(*endpoint_iterator, handle_connect);
            contexts.back()->run();
        }
        io_context.run();
        for (auto& skt : sockets) {
            skt->close();
            delete skt;
        }
        for (auto& ctx : contexts) {
            delete ctx;
        }
        std::cout << cnt << " connections to " << target_ip << ":"
                  << target_port << "\n";
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}
