#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
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
    std::unordered_map<std::string, std::string> response_;
    std::unordered_map<std::string, std::string> query_;
    std::unordered_map<std::string, std::string> decrypted_query_;

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
    tcp_connection(boost::asio::io_context& io_context) : socket_(io_context) {}

    void handle_read_query(const boost::system::error_code& ec, std::size_t) {
        if (!ec) {
            std::string json_data{std::istreambuf_iterator<char>(&buff_),
                                  std::istreambuf_iterator<char>()};

            parse_query(json_data);
            decrypt_query();
            auto response_json = get_json_response();
            async_write(socket_, buffer(response_json + '\0'),
                        std::bind(&tcp_connection::handle_write_response,
                                  shared_from_this(), std::placeholders::_1,
                                  std::placeholders::_2));

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

    void parse_query(const std::string& json_query) {
        nlohmann::json json_data = nlohmann::json::parse(json_query);
        for (auto it = json_data.begin(); it != json_data.end(); ++it)
            query_[it.key()] = it.value();
    }

    void decrypt_query() {
        // c
        auto tgt_c = std::bitset<64>(query_["tgt_c"]);
        tgt_c = des::decrypt(tgt_c, keys::K_AS_TGS);
        decrypted_query_["tgt_c"] = des::bits_to_str(tgt_c);

        auto aut_c = std::bitset<64>(query_["aut_c"]);
        aut_c = des::decrypt(aut_c, keys::K_C_TGS);
        decrypted_query_["aut_c"] = des::bits_to_str(aut_c);

        // tgs
        auto tgt_tgs = std::bitset<64>(query_["tgt_tgs"]);
        tgt_tgs = des::decrypt(tgt_tgs, keys::K_AS_TGS);
        decrypted_query_["tgt_tgs"] = des::bits_to_str(tgt_tgs);

        // t1
        auto tgt_t1 = std::bitset<64>(query_["tgt_t1"]);
        tgt_t1 = des::decrypt(tgt_t1, keys::K_AS_TGS);
        decrypted_query_["tgt_t1"] = des::bits_to_str(tgt_t1);

        // p1
        auto tgt_p1 = std::bitset<64>(query_["tgt_p1"]);
        tgt_p1 = des::decrypt(tgt_p1, keys::K_AS_TGS);
        decrypted_query_["tgt_p1"] = des::bits_to_str(tgt_p1);

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