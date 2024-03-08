#pragma once
#include "token.h"

struct Error {
    Token::token_position pos;
    std::string msg;
};

using Errors = std::vector<Error>;
