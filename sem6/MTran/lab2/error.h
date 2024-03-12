#pragma once
#include "token.h"

struct Error {
    Token::token_position pos;
    std::string msg;

    Error(const Token::token_position& pos, const std::string& msg)
        : pos(pos), msg(msg) {}
};

using Errors = std::vector<Error>;
