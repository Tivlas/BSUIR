#pragma once
#include <bitset>
#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

#include "des.h"
#include "json.h"
#include "keys.h"

using namespace boost::asio;

class tgs_connection : public std::enable_shared_from_this<tgs_connection> {
   private:
    std::bitset<64> K_C_TGS;
    ip::tcp::socket socket_;
    streambuf buff_;
    std::unordered_map<std::string, std::string> response_;
    std::unordered_map<std::string, std::string> request_;
    std::unordered_map<std::string, std::string> decrypted_request_;

   public:
    using conn_shared_ptr = std::shared_ptr<tgs_connection>;
    ip::tcp::socket& socket() { return socket_; }

    void start() {
        async_read_until(
            socket_, buff_, '\0',
            std::bind(&tgs_connection::handle_read_request, shared_from_this(),
                      std::placeholders::_1, std::placeholders::_2));
    }

    static auto create(boost::asio::io_context& io_context) {
        return conn_shared_ptr(new tgs_connection(io_context));
    }

   private:
    tgs_connection(boost::asio::io_context& io_context) : socket_(io_context) {
        buff_.prepare(1024);
    }

    void print(const std::string& msg) {
        std::cout << "TGS: " << msg << std::endl;
    }

    void handle_read_request(const boost::system::error_code& ec, std::size_t) {
        if (!ec) {
            std::string json_data{std::istreambuf_iterator<char>(&buff_),
                                  std::istreambuf_iterator<char>()};
            json_data.pop_back();
            parse_request(json_data);
            decrypt_request();
            if (!ok()) {
                socket_.close();
                return;
            }
            encrypt_response();
            auto response_json = get_json_response();
            async_write(socket_, buffer(response_json + '\0'),
                        std::bind(&tgs_connection::handle_write_response,
                                  shared_from_this(), std::placeholders::_1,
                                  std::placeholders::_2));

        } else {
            print("error reading data.");
        }
    }

    void handle_write_response(const boost::system::error_code& ec,
                               std::size_t) {
        if (!ec) {
            print("response sent successfully.");
        } else {
            print("error sending response.");
        }
    }

    void parse_request(const std::string& json_request) {
        nlohmann::json json_data = nlohmann::json::parse(json_request);
        for (auto it = json_data.begin(); it != json_data.end(); ++it)
            request_[it.key()] = it.value();
    }

    void decrypt_request() {
        print("STEP 4: encrypted request from C");
        for (const auto& [key, value] : request_) {
            print(key + " " + value);
        }
        std::cout << std::endl;

        auto tgt_k_c_tgs = std::bitset<64>(request_["tgt_k_c_tgs"]);
        tgt_k_c_tgs = des::decrypt(tgt_k_c_tgs, keys::K_AS_TGS);
        decrypted_request_["tgt_k_c_tgs"] = tgt_k_c_tgs.to_string();
        K_C_TGS = tgt_k_c_tgs;  // remember key

        auto aut_c = std::bitset<64>(request_["aut_c"]);
        aut_c = des::decrypt(aut_c, K_C_TGS);
        decrypted_request_["aut_c"] = des::bits_to_str(aut_c);

        auto aut_t2 = std::bitset<64>(request_["aut_t2"]);
        aut_t2 = des::decrypt(aut_t2, K_C_TGS);
        decrypted_request_["aut_t2"] = std::to_string(aut_t2.to_ullong());

        auto tgt_c = std::bitset<64>(request_["tgt_c"]);
        tgt_c = des::decrypt(tgt_c, keys::K_AS_TGS);
        decrypted_request_["tgt_c"] = des::bits_to_str(tgt_c);

        auto tgt_tgs = std::bitset<64>(request_["tgt_tgs"]);
        tgt_tgs = des::decrypt(tgt_tgs, keys::K_AS_TGS);
        decrypted_request_["tgt_tgs"] = des::bits_to_str(tgt_tgs);

        auto tgt_t1 = std::bitset<64>(request_["tgt_t1"]);
        tgt_t1 = des::decrypt(tgt_t1, keys::K_AS_TGS);
        decrypted_request_["tgt_t1"] = std::to_string(tgt_t1.to_ullong());

        auto tgt_p1 = std::bitset<64>(request_["tgt_p1"]);
        tgt_p1 = des::decrypt(tgt_p1, keys::K_AS_TGS);
        decrypted_request_["tgt_p1"] = std::to_string(tgt_p1.to_ullong());

        decrypted_request_["ID"] = request_["ID"];

        print("STEP 4: decrypted request from C");
        for (const auto& [key, value] : decrypted_request_) {
            print(key + " " + value);
        }
        std::cout << std::endl;
    }

    bool ok() {
        bool ok;
        ok = decrypted_request_["tgt_c"] == decrypted_request_["aut_c"];
        if (!ok) return false;

        ok = decrypted_request_["tgt_tgs"] == "tgs";
        if (!ok) return false;

        int64_t t1, t2, p1;
        try {
            t1 = boost::lexical_cast<int64_t>(decrypted_request_["tgt_t1"]);
            t2 = boost::lexical_cast<int64_t>(decrypted_request_["aut_t2"]);
            p1 = boost::lexical_cast<int64_t>(decrypted_request_["tgt_p1"]);
        } catch (const std::exception& e) {
            std::cerr << e.what() << '\n';
        }
        ok = t2 < (t1 + p1);
        return ok;
    }

    void encrypt_response() {
        auto tgs_c = des::str_to_bits(decrypted_request_["tgt_c"]);
        tgs_c = des::encrypt(tgs_c, keys::K_TGS_SS);
        tgs_c = des::encrypt(tgs_c, K_C_TGS);
        response_["tgs_c"] = tgs_c.to_string();

        auto ss = des::str_to_bits("ss");
        ss = des::encrypt(ss, keys::K_TGS_SS);
        ss = des::encrypt(ss, K_C_TGS);
        response_["tgs_ss"] = ss.to_string();

        auto t3 = std::chrono::duration_cast<std::chrono::seconds>(
                      std::chrono::system_clock::now().time_since_epoch())
                      .count();
        auto enc_t3 = des::encrypt(t3, keys::K_TGS_SS);
        enc_t3 = des::encrypt(enc_t3, K_C_TGS);
        response_["tgs_t3"] = enc_t3.to_string();

        int64_t p2 = 10;
        auto enc_p2 = des::encrypt(p2, keys::K_TGS_SS);
        enc_p2 = des::encrypt(enc_p2, K_C_TGS);
        response_["tgs_p2"] = enc_p2.to_string();

        auto tgs_k_c_ss = des::encrypt(keys::K_C_SS, keys::K_TGS_SS);
        tgs_k_c_ss = des::encrypt(tgs_k_c_ss, K_C_TGS);
        response_["tgs_k_c_ss"] = tgs_k_c_ss.to_string();

        auto k_c_ss = des::encrypt(keys::K_C_SS, K_C_TGS);
        response_["k_c_ss"] = k_c_ss.to_string();

        print("STEP 4: response to C");
        for (const auto& [key, value] : response_) {
            print(key + " " + value);
        }
        std::cout << std::endl;
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
        auto tcp_conn = tgs_connection::create(io_context_);
        acceptor_.async_accept(tcp_conn->socket(),
                               std::bind(&TGS::handle_accept, this, tcp_conn,
                                         std::placeholders::_1));
    }

    void handle_accept(std::shared_ptr<tgs_connection> tcp_conn,
                       const boost::system::error_code& ec) {
        if (!ec) {
            tcp_conn->start();
        } else {
            std::cout << "TGS: error accepting connection." << std::endl;
        }
        accept();
    }
};