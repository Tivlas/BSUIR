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

class ss_connection : public std::enable_shared_from_this<ss_connection> {
   private:
    std::bitset<64> K_C_SS;
    ip::tcp::socket socket_;
    streambuf buff_;
    std::unordered_map<std::string, std::string> response_;
    std::unordered_map<std::string, std::string> request_;
    std::unordered_map<std::string, std::string> decrypted_request_;

   public:
    using conn_shared_ptr = std::shared_ptr<ss_connection>;
    ip::tcp::socket& socket() { return socket_; }

    void start() {
        async_read_until(
            socket_, buff_, '\0',
            std::bind(&ss_connection::handle_read_request, shared_from_this(),
                      std::placeholders::_1, std::placeholders::_2));
    }

    static auto create(boost::asio::io_context& io_context) {
        return conn_shared_ptr(new ss_connection(io_context));
    }

   private:
    ss_connection(boost::asio::io_context& io_context) : socket_(io_context) {
        buff_.prepare(1024);
    }

    void print(const std::string& msg) {
        std::cout << "SS: " << msg << std::endl;
    }

    void handle_read_request(const boost::system::error_code& ec, std::size_t) {
        if (!ec) {
            std::string json_data{std::istreambuf_iterator<char>(&buff_),
                                  std::istreambuf_iterator<char>()};
            json_data.pop_back();
            parse_request(json_data);
            decrypt_request();
            if (!ok()) {
                print("client is not ok.");
                socket_.close();
                return;
            }
            encrypt_response();
            auto response_json = get_json_response();
            async_write(socket_, buffer(response_json + '\0'),
                        std::bind(&ss_connection::handle_write_response,
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
        print("STEP 5: encrypted request from C");
        for (const auto& [key, value] : request_) {
            print(key + " " + value);
        }
        std::cout << std::endl;

        auto tgs_k_c_ss = std::bitset<64>(request_["tgs_k_c_ss"]);
        tgs_k_c_ss = des::decrypt(tgs_k_c_ss, keys::K_TGS_SS);
        decrypted_request_["tgs_k_c_ss"] = tgs_k_c_ss.to_string();
        K_C_SS = tgs_k_c_ss;

        auto aut_t4 = std::bitset<64>(request_["aut_t4"]);
        aut_t4 = des::decrypt(aut_t4, K_C_SS);
        decrypted_request_["aut_t4"] = std::to_string(aut_t4.to_ullong());

        auto aut_c = std::bitset<64>(request_["aut_c"]);
        aut_c = des::decrypt(aut_c, K_C_SS);
        decrypted_request_["aut_c"] = des::bits_to_str(aut_c);

        auto tgs_c = std::bitset<64>(request_["tgs_c"]);
        tgs_c = des::decrypt(tgs_c, keys::K_TGS_SS);
        decrypted_request_["tgs_c"] = des::bits_to_str(tgs_c);

        auto tgs_ss = std::bitset<64>(request_["tgs_ss"]);
        tgs_ss = des::decrypt(tgs_ss, keys::K_TGS_SS);
        decrypted_request_["tgs_ss"] = des::bits_to_str(tgs_ss);

        auto tgs_t3 = std::bitset<64>(request_["tgs_t3"]);
        tgs_t3 = des::decrypt(tgs_t3, keys::K_TGS_SS);
        decrypted_request_["tgs_t3"] = std::to_string(tgs_t3.to_ullong());

        auto tgs_p2 = std::bitset<64>(request_["tgs_p2"]);
        tgs_p2 = des::decrypt(tgs_p2, keys::K_TGS_SS);
        decrypted_request_["tgs_p2"] = std::to_string(tgs_p2.to_ullong());

        print("STEP 6: decrypted request from C");
        for (const auto& [key, value] : decrypted_request_) {
            print(key + " " + value);
        }
        std::cout << std::endl;
    }

    bool ok() {
        bool ok;
        ok = decrypted_request_["tgs_c"] == decrypted_request_["aut_c"];
        if (!ok) {
            print("client identifiers are not equal.");
            print(decrypted_request_["tgs_c"] + " " +
                  decrypted_request_["aut_c"]);
            return false;
        }

        ok = decrypted_request_["tgs_ss"] == "ss";
        if (!ok) {
            print("ss identifiers are not equal.");
            return false;
        }

        int64_t t3, t4, p2;
        try {
            t3 = boost::lexical_cast<int64_t>(decrypted_request_["tgs_t3"]);
            t4 = boost::lexical_cast<int64_t>(decrypted_request_["aut_t4"]);
            p2 = boost::lexical_cast<int64_t>(decrypted_request_["tgs_p2"]);
        } catch (const std::exception& e) {
            std::cerr << e.what() << '\n';
            return false;
        }
        ok = t4 < (t3 + p2);
        if (!ok) {
            print("time limit.");
        }
        return ok;
    }

    void encrypt_response() {
        int64_t t4 = boost::lexical_cast<int64_t>(decrypted_request_["aut_t4"]);
        t4++;
        auto enc_t4 = des::encrypt(t4, K_C_SS);
        response_["t4"] = enc_t4.to_string();

        print("STEP 6: response C");
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

class SS {
   private:
    io_context& io_context_;
    ip::tcp::acceptor acceptor_;

   public:
    SS(boost::asio::io_context& io_context, short port)
        : io_context_(io_context),
          acceptor_(io_context, ip::tcp::endpoint(ip::tcp::v4(), port)) {
        accept();
    }

   private:
    void accept() {
        auto tcp_conn = ss_connection::create(io_context_);
        acceptor_.async_accept(tcp_conn->socket(),
                               std::bind(&SS::handle_accept, this, tcp_conn,
                                         std::placeholders::_1));
    }

    void handle_accept(std::shared_ptr<ss_connection> tcp_conn,
                       const boost::system::error_code& ec) {
        if (!ec) {
            tcp_conn->start();
        } else {
            std::cout << ec.message() << '\n';
        }
        accept();
    }
};