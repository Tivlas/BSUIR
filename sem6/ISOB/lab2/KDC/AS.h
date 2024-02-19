#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <chrono>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

#include "des.h"
#include "json.hpp"
#include "keys.h"

using namespace boost::asio;

class tcp_connection : public std::enable_shared_from_this<tcp_connection> {
   private:
    ip::tcp::socket socket_;
    streambuf buff_;
    std::unordered_set<std::string> known_clients_;
    std::unordered_map<std::string, std::string> response_;

   public:
    using conn_shared_ptr = std::shared_ptr<tcp_connection>;
    ip::tcp::socket& socket() { return socket_; }

    void start() {
        async_read_until(
            socket_, buff_, '\0',
            std::bind(&tcp_connection::handle_read_query, shared_from_this(),
                      std::placeholders::_1, std::placeholders::_2));
    }

    static auto create(boost::asio::io_context& io_context) {
        return conn_shared_ptr(new tcp_connection(io_context));
    }

   private:
    tcp_connection(boost::asio::io_context& io_context)
        : socket_(io_context), known_clients_({"den", "tima", "vadik"}) {}

    void handle_read_query(const boost::system::error_code& ec, std::size_t) {
        if (!ec) {
            std::string login{std::istreambuf_iterator<char>(&buff_),
                              std::istreambuf_iterator<char>()};

            if (known_clients_.contains(login)) {
                encrypt_response(login);
                auto response_json = get_json_response();
                async_write(socket_, buffer(response_json + '\0'),
                            std::bind(&tcp_connection::handle_write_response,
                                      shared_from_this(), std::placeholders::_1,
                                      std::placeholders::_2));
            } else {
                socket_.close();
            }

        } else {
            std::cout << "Error reading login." << std::endl;
        }
    }

    void handle_write_response(const boost::system::error_code& ec,
                               std::size_t) {
        if (!ec) {
            std::cout << "Response sent successfully." << std::endl;
        } else {
            std::cout << "Error sending response." << std::endl;
        }
    }

    void encrypt_response(const std::string& login) {
        // c
        auto c = des::str_to_bits(login);
        c = des::encrypt(c, keys::K_AS_TGS);
        c = des::encrypt(c, keys::K_C);
        response_["tgt_c"] = c.to_string();

        // tgs
        auto tgs = des::str_to_bits("tgs");
        tgs = des::encrypt(tgs, keys::K_AS_TGS);
        tgs = des::encrypt(tgs, keys::K_C);
        response_["tgt_tgs"] = tgs.to_string();

        // t1
        auto t1 = std::chrono::duration_cast<std::chrono::seconds>(
                      std::chrono::system_clock::now().time_since_epoch())
                      .count();
        auto enc_t1 = des::str_to_bits(std::to_string(t1));
        enc_t1 = des::encrypt(enc_t1, keys::K_AS_TGS);
        enc_t1 = des::encrypt(enc_t1, keys::K_C);
        response_["tgt_t1"] = enc_t1.to_string();

        // p1
        int64_t p1 = 10;
        auto enc_p1 = des::str_to_bits(std::to_string(p1));
        enc_p1 = des::encrypt(enc_p1, keys::K_AS_TGS);
        enc_p1 = des::encrypt(enc_p1, keys::K_C);
        response_["tgt_p1"] = enc_p1.to_string();

        // tgt_k_c_tgs
        auto tgt_k_c_tgs = des::encrypt(keys::K_C_TGS, keys::K_AS_TGS);
        tgt_k_c_tgs = des::encrypt(tgt_k_c_tgs, keys::K_C);
        response_["tgt_k_c_tgs"] = tgt_k_c_tgs.to_string();

        // k_c_tgs
        auto k_c_tgs = des::encrypt(keys::K_C_TGS, keys::K_C);
        response_["k_c_tgs"] = k_c_tgs.to_string();
    }

    std::string get_json_response() {
        nlohmann::json json_data;
        for (const auto& [k, v] : response_) json_data[k] = v;
        return json_data.dump();
    }
};

class TGS {
   private:
    io_context& io_context_;
    ip::tcp::acceptor acceptor_;

   public:
    TGS(boost::asio::io_context& io_context, short port)
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