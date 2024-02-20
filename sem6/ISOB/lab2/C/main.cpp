#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <chrono>
#include <iostream>
#include <unordered_map>

#include "des.h"
#include "json.h"
#include "keys.h"
#include "ports.h"
using namespace boost::asio;

std::unordered_map<std::string, std::string> response_;
std::unordered_map<std::string, std::string> request_;
std::string login{"tima"};
std::bitset<64> K_C_TGS;
std::bitset<64> K_C_SS;
int64_t T4;

void print(const std::string& msg) { std::cout << msg << "\n"; }

void parse_request(const std::string& json_request) {
    response_.clear();
    nlohmann::json json_data = nlohmann::json::parse(json_request);
    for (auto it = json_data.begin(); it != json_data.end(); ++it)
        response_[static_cast<std::string>(it.key())] =
            static_cast<std::string>(it.value());
}

void decrypt_AS_response() {
    print("STEP 2: AS response");
    for (const auto& [key, value] : response_) {
        print(key + " " + value);
    }
    std::cout << std::endl;

    auto tgt_c = std::bitset<64>(response_["tgt_c"]);
    tgt_c = des::decrypt(tgt_c, keys::K_C);
    request_["tgt_c"] = tgt_c.to_string();

    auto tgt_tgs = std::bitset<64>(response_["tgt_tgs"]);
    tgt_tgs = des::decrypt(tgt_tgs, keys::K_C);
    request_["tgt_tgs"] = tgt_tgs.to_string();

    auto tgt_t1 = std::bitset<64>(response_["tgt_t1"]);
    tgt_t1 = des::decrypt(tgt_t1, keys::K_C);
    request_["tgt_t1"] = tgt_t1.to_string();

    auto tgt_p1 = std::bitset<64>(response_["tgt_p1"]);
    tgt_p1 = des::decrypt(tgt_p1, keys::K_C);
    request_["tgt_p1"] = tgt_p1.to_string();

    auto tgt_k_c_tgs = std::bitset<64>(response_["tgt_k_c_tgs"]);
    tgt_k_c_tgs = des::decrypt(tgt_k_c_tgs, keys::K_C);
    request_["tgt_k_c_tgs"] = tgt_k_c_tgs.to_string();

    auto k_c_tgs = std::bitset<64>(response_["k_c_tgs"]);
    k_c_tgs = des::decrypt(k_c_tgs, keys::K_C);
    K_C_TGS = k_c_tgs;
}

void encrypt_request_to_TGS() {
    auto t2 = std::chrono::duration_cast<std::chrono::seconds>(
                  std::chrono::system_clock::now().time_since_epoch())
                  .count();
    auto enc_t2 = des::encrypt(std::bitset<64>(t2), K_C_TGS);
    request_["aut_t2"] = enc_t2.to_string();

    auto aut_c = des::str_to_bits(login);
    aut_c = des::encrypt(aut_c, K_C_TGS);
    request_["aut_c"] = aut_c.to_string();

    print("STEP 3: request to TGS");
    for (const auto& [key, value] : request_) {
        print(key + " " + value);
    }
    std::cout << std::endl;
}

void decrypt_TGS_response() {
    print("STEP 4: TGS response");
    for (const auto& [key, value] : response_) {
        print(key + " " + value);
    }
    std::cout << std::endl;

    auto tgs_c = std::bitset<64>(response_["tgs_c"]);
    tgs_c = des::decrypt(tgs_c, K_C_TGS);
    request_["tgs_c"] = tgs_c.to_string();

    auto tgs_ss = std::bitset<64>(response_["tgs_ss"]);
    tgs_ss = des::decrypt(tgs_ss, K_C_TGS);
    request_["tgs_ss"] = tgs_ss.to_string();

    auto tgs_t3 = std::bitset<64>(response_["tgs_t3"]);
    tgs_t3 = des::decrypt(tgs_t3, K_C_TGS);
    request_["tgs_t3"] = tgs_t3.to_string();

    auto tgs_p2 = std::bitset<64>(response_["tgs_p2"]);
    tgs_p2 = des::decrypt(tgs_p2, K_C_TGS);
    request_["tgs_p2"] = tgs_p2.to_string();

    auto tgs_k_c_ss = std::bitset<64>(response_["tgs_k_c_ss"]);
    tgs_k_c_ss = des::decrypt(tgs_k_c_ss, K_C_TGS);
    request_["tgs_k_c_ss"] = tgs_k_c_ss.to_string();

    auto k_c_ss = std::bitset<64>(response_["k_c_ss"]);
    k_c_ss = des::decrypt(k_c_ss, K_C_TGS);
    K_C_SS = k_c_ss;
}

void encrypt_request_to_SS() {
    auto t4 = std::chrono::duration_cast<std::chrono::seconds>(
                  std::chrono::system_clock::now().time_since_epoch())
                  .count();
    T4 = t4;
    auto enc_t4 = des::encrypt(T4, K_C_SS);
    request_["aut_t4"] = enc_t4.to_string();

    auto aut_c = des::str_to_bits(login);
    aut_c = des::encrypt(aut_c, K_C_SS);
    request_["aut_c"] = aut_c.to_string();

    print("STEP 5: request to SS");
    for (const auto& [key, value] : response_) {
        print(key + " " + value);
    }
    std::cout << std::endl;
}

void decrypt_SS_response() {
    print("STEP 6: SS response");
    for (const auto& [key, value] : response_) {
        print(key + " " + value);
    }
    std::cout << std::endl;

    auto t4 = std::bitset<64>(response_["t4"]);
    t4 = des::decrypt(t4, K_C_SS);
    request_["t4"] = std::to_string(t4.to_ullong());
}

std::string get_request_json() {
    nlohmann::json json_data;
    for (auto [k, v] : request_) json_data[k] = v;
    request_.clear();
    auto json = json_data.dump();
    return json;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "You must pass client login." << std::endl;
        return 0;
    }

    char* argument = argv[1];
    if (std::strlen(argument) > 8) {
        std::cout << "Login length have to be <= 8." << std::endl;
        return 1;
    }
    login = std::string(argument);

    io_context io_context;
    ip::tcp::socket sock(io_context);
    ip::tcp::resolver resolver(io_context);
    streambuf buff;
    buff.prepare(4096);

    connect(sock,
            resolver.resolve("127.0.0.1", std::to_string(ports::AS_PORT)));
    if (!sock.is_open()) return 0;

    // STEP 1
    print("STEP 1: sending login to AS: " + login);
    write(sock, buffer(login + '\0'));
    // END STEP 1

    // STEP 2
    if (!sock.is_open()) return 0;
    read_until(sock, buff, '\0');
    std::string as_response{std::istreambuf_iterator<char>(&buff),
                            std::istreambuf_iterator<char>()};
    as_response.pop_back();
    parse_request(as_response);
    decrypt_AS_response();
    // END STEP 2

    // STEP 3
    encrypt_request_to_TGS();
    auto json_to_tgs = get_request_json();
    sock.close();
    connect(sock,
            resolver.resolve("127.0.0.1", std::to_string(ports::TGS_PORT)));
    if (!sock.is_open()) return 0;
    write(sock, buffer(json_to_tgs + '\0'));
    // END STEP 3

    // STEP 4
    read_until(sock, buff, '\0');
    std::string tgs_response{std::istreambuf_iterator<char>(&buff),
                             std::istreambuf_iterator<char>()};
    tgs_response.pop_back();
    parse_request(tgs_response);
    decrypt_TGS_response();
    // END STEP 4

    // STEP 5
    encrypt_request_to_SS();
    auto json_to_ss = get_request_json();
    sock.close();
    connect(sock,
            resolver.resolve("127.0.0.1", std::to_string(ports::SS_PORT)));
    if (!sock.is_open()) return 0;
    write(sock, buffer(json_to_ss + '\0'));
    // END STEP 5

    // STEP 6
    read_until(sock, buff, '\0');
    std::string ss_response{std::istreambuf_iterator<char>(&buff),
                            std::istreambuf_iterator<char>()};
    ss_response.pop_back();
    parse_request(ss_response);
    decrypt_SS_response();
    auto ss_t4 = boost::lexical_cast<int64_t>(request_["t4"]);
    if (ss_t4 - 1 != T4) {
        sock.close();
        print("SS did not pass authentication.");
    } else {
        print("Connected to SS.");
    }
    // END STEP 6

    boost::system::error_code ec;
    sock.shutdown(ip::tcp::socket::shutdown_both, ec);
    sock.close();
}