#pragma once
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "token.h"

class lexer {
    using Tokens = std::vector<Token>;

   private:
    Tokens tokens_;
    std::string source_;
    size_t start = 0;
    size_t cur = 0;
    size_t line = 1;

    bool looks_like_keyword(const std::string&, const std::string&, int) const;
    bool is_at_end() const;
    void scan_token();
    char advance();
    void add_token(token_type);
    bool match(char, char = '\0');

   public:
    lexer(const std::string& source);
    ~lexer();
    const Tokens& tokenize();
};

lexer::lexer(const std::string& source) : source_(source) {}

lexer::~lexer() {}

bool lexer::is_at_end() const { return cur >= source_.size(); }

char lexer::advance() { return source_[cur++]; }

void lexer::add_token(token_type type) {
    auto lexeme = source_.substr(start, cur);
    // TODO: line, col
    tokens_.push_back({token_type::EOF_, lexeme, -1, -1});
}

bool lexer::match(char expected, char expected2 = '\0') {
    if (expected2 != '\0' && cur < source_.size() - 1) {
        auto matched =
            source_[cur] == expected && source_[cur + 1] == expected2;
        if (matched) {
            cur += 2;
        }
        return matched;
    }
    if (is_at_end() || source_[cur] != expected) return false;
    cur++;
    return true;
}

const lexer::Tokens& lexer::tokenize() {
    while (!is_at_end()) {
        start = cur;
        scan_token();
    }
    tokens_.push_back({token_type::EOF_, "", -1, -1});
    return tokens_;
}

void lexer::scan_token() {
    char c = advance();
    switch (c) {
        // 1
        case '(':
            add_token(token_type::LPAREN);
            break;
        case ')':
            add_token(token_type::RPAREN);
            break;
        case '{':
            add_token(token_type::LBRACE);
            break;
        case '}':
            add_token(token_type::RBRACE);
            break;
        case '[':
            add_token(token_type::LBRACK);
            break;
        case ']':
            add_token(token_type::RBRACK);
            break;
        case ',':
            add_token(token_type::COMMA);
            break;
        case '.':  // TODO: ???
            add_token(token_type::PERIOD);
            break;
        case '\"':  // TODO ???
            add_token(token_type::QUOTE_MARK);
            break;
        case ';':
            add_token(token_type::SEMICOLON);
            break;
        // 2
        case ':':  // : :=
            add_token(match('=') ? token_type::DEFINE : token_type::COLON);
            break;
        case '-':  // - -= --
            if (match('-')) {
                add_token(token_type::DEC);
            } else if (match('=')) {
                add_token(token_type::SUB_ASSIGN);
            } else {
                add_token(token_type::SUB);
            }
            break;
        case '+':  // + += ++
            if (match('+')) {
                add_token(token_type::INC);
            } else if (match('=')) {
                add_token(token_type::ADD_ASSIGN);
            } else {
                add_token(token_type::ADD);
            }
            break;
        case '*':  // * *=
            add_token(match('=') ? token_type::MUL_ASSIGN : token_type::MUL);
            break;
        case '/':  // / /=
            add_token(match('=') ? token_type::QUO_ASSIGN : token_type::QUO);
            break;
        case '%':  // % %=
            add_token(match('=') ? token_type::REM_ASSIGN : token_type::REM);
            break;
        case '=':  // = ==
            add_token(match('=') ? token_type::EQL : token_type::ASSIGN);
            break;
        case '^':  // ^ ^=
            add_token(match('=') ? token_type::XOR_ASSIGN : token_type::XOR);
            break;
        case '<':  // < << <<=
            if (match('<', '=')) {
                add_token(token_type::SHL_ASSIGN);
            } else if (match('<')) {
                add_token(token_type::SHL);
            } else {
                add_token(token_type::LSS);
            }
            break;
        case '>':  // > >> >>=
            if (match('>', '=')) {
                add_token(token_type::SHR_ASSIGN);
            } else if (match('>')) {
                add_token(token_type::SHR);
            } else {
                add_token(token_type::GTR);
            }
            break;
        case '&':  // & &= && &^ &^=
            if (match('^', '=')) {
                add_token(token_type::AND_NOT_ASSIGN);
            } else if (match('^')) {
                add_token(token_type::AND_NOT);
            } else if (match('&')) {
                add_token(token_type::LAND);
            } else if (match('=')) {
                add_token(token_type::AND_ASSIGN);
            } else {
                add_token(token_type::AND);
            }
            break;
        case '|':  // | |= ||
            if (match('|')) {
                add_token(token_type::LOR);
            } else if (match('=')) {
                add_token(token_type::OR_ASSIGN);
            } else {
                add_token(token_type::OR);
            }
            break;
        case '!':  // ! !=
            add_token(match('=') ? token_type::NEQ : token_type::NOT);
            break;
        default:
            std::cout << "Unexpected character...\n";
    }
}

bool lexer::looks_like_keyword(const std::string& word,
                               const std::string& keyword,
                               int max_changes = 1) const {
    int len1 = word.size();
    int len2 = keyword.size();

    if (std::abs(len1 - len2) > max_changes) {
        return false;
    }

    int count = 0;
    int i = 0;
    int j = 0;

    while (i < len1 && j < len2) {
        if (word[i] != keyword[j]) {
            if (count == max_changes) {
                return false;
            }

            if (len1 > len2) {
                i++;
            } else if (len1 < len2) {
                j++;
            } else {
                i++;
                j++;
            }

            count++;
        } else {
            i++;
            j++;
        }
    }

    while (i++ < len1 || j++ < len2) {
        count++;
    }

    return count <= max_changes;
}
