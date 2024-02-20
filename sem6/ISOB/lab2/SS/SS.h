#pragma once
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
    ip::tcp::socket socket_;
    streambuf buff_;
    std::unordered_map<std::string, std::string> response_;
    std::unordered_map<std::string, std::string> query_;
    std::unordered_map<std::string, std::string> decrypted_query_;

   public:
    using conn_shared_ptr = std::shared_ptr<ss_connection>;
    ip::tcp::socket& socket() { return socket_; }

    void start() {
        async_read_until(
            socket_, buff_, '\0',
            std::bind(&ss_connection::handle_read_query, shared_from_this(),
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

    void handle_read_query(const boost::system::error_code& ec, std::size_t) {
        if (!ec) {
            std::string json_data{std::istreambuf_iterator<char>(&buff_),
                                  std::istreambuf_iterator<char>()};
            json_data.pop_back();
            parse_query(json_data);
            decrypt_query();
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

    void communicate(){};

    void handle_communicate_read(const boost::system::error_code& ec,
                                 std::size_t) {}
    void handle_communicate_write(const boost::system::error_code& ec,
                                  std::size_t) {}

    void handle_write_response(const boost::system::error_code& ec,
                               std::size_t) {
        if (!ec) {
            print("response sent successfully.");
            communicate();
        } else {
            print("error sending response.");
        }
    }

    void parse_query(const std::string& json_query) {
        nlohmann::json json_data = nlohmann::json::parse(json_query);
        for (auto it = json_data.begin(); it != json_data.end(); ++it)
            query_[it.key()] = it.value();
    }

    void decrypt_query() {
        auto tgs_c = std::bitset<64>(query_["tgs_c"]);
        tgs_c = des::decrypt(tgs_c, keys::K_TGS_SS);
        decrypted_query_["tgs_c"] = des::bits_to_str(tgs_c);

        auto aut_c = std::bitset<64>(query_["aut_c"]);
        aut_c = des::decrypt(aut_c, keys::K_C_SS);
        decrypted_query_["aut_c"] = des::bits_to_str(aut_c);

        auto tgs_ss = std::bitset<64>(query_["tgs_ss"]);
        tgs_ss = des::decrypt(tgs_ss, keys::K_TGS_SS);
        decrypted_query_["tgs_ss"] = des::bits_to_str(tgs_ss);

        auto tgs_t3 = std::bitset<64>(query_["tgs_t3"]);
        tgs_t3 = des::decrypt(tgs_t3, keys::K_TGS_SS);
        decrypted_query_["tgs_t3"] = des::bits_to_str(tgs_t3);

        auto tgs_p2 = std::bitset<64>(query_["tgs_p2"]);
        tgs_p2 = des::decrypt(tgs_p2, keys::K_TGS_SS);
        decrypted_query_["tgs_p2"] = des::bits_to_str(tgs_p2);

        auto tgs_k_c_ss = std::bitset<64>(query_["tgs_k_c_ss"]);
        tgs_k_c_ss = des::decrypt(tgs_k_c_ss, keys::K_TGS_SS);
        decrypted_query_["tgs_k_c_ss"] = tgs_k_c_ss.to_string();

        auto aut_t4 = std::bitset<64>(query_["aut_t4"]);
        aut_t4 = des::decrypt(aut_t4, keys::K_C_SS);
        decrypted_query_["aut_t4"] = des::bits_to_str(aut_t4);
        std::cout << decrypted_query_["aut_t4"] << std::endl;
    }

    bool ok() {
        bool ok;
        ok = decrypted_query_["tgs_c"] == decrypted_query_["aut_c"];
        if (!ok) {
            print("client identifiers are not equal.");
            print(decrypted_query_["tgs_c"] + " " + decrypted_query_["aut_c"]);
            return false;
        }

        ok = decrypted_query_["tgs_ss"] == "ss";
        if (!ok) {
            print("ss identifiers are not equal.");
            return false;
        }

        int64_t t3, t4, p2;
        try {
            t3 = boost::lexical_cast<int64_t>(decrypted_query_["tgs_t3"]);
            t4 = boost::lexical_cast<int64_t>(decrypted_query_["aut_t4"]);
            p2 = boost::lexical_cast<int64_t>(decrypted_query_["tgs_p2"]);
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
        int64_t t4 = boost::lexical_cast<int64_t>(decrypted_query_["aut_t4"]);
        t4++;
        auto enc_t4 = des::str_to_bits(std::to_string(t4));
        enc_t4 = des::encrypt(enc_t4, keys::K_C_SS);
        response_["t4"] = enc_t4.to_string();
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