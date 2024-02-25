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
    size_t pos_in_line = 0;

    bool looks_like_keyword(const std::string&, const std::string&, int) const;
    bool eof() const;
    void scan_token();
    char get_cur_char() const;
    char get_next_char() const;
    char next();
    void prev();
    void add_token(token_type);
    bool match(char, char = '\0');
    void scan_string_literal();
    void scan_multiline_string_literal();
    void scan_number();
    void scan_number_starts_with_period();
    void scan_number_starts_with_digit();
    void scan_number_bin();
    void scan_number_oct();
    void scan_number_hex();
    void scan_identifier();  // or keyword
    void insert_semicolon();

   public:
    lexer(const std::string& source);
    ~lexer();
    const Tokens& tokenize();
    void print_all() const;
};

lexer::lexer(const std::string& source) : source_(source) {}

lexer::~lexer() {}

void lexer::prev() {
    cur--;
    pos_in_line--;
}

void lexer::insert_semicolon() {
    auto last_token_type = tokens_.back().type;
    if (last_token_type == token_type::IDENT ||
        last_token_type == token_type::INT ||
        last_token_type == token_type::STRING ||
        last_token_type == token_type::BREAK ||
        last_token_type == token_type::CONTINUE ||
        last_token_type == token_type::FALLTHROUGH ||
        last_token_type == token_type::RETURN ||
        last_token_type == token_type::INC ||
        last_token_type == token_type::DEC ||
        last_token_type == token_type::RPAREN ||
        last_token_type == token_type::RBRACE) {
        auto insert_pos = source_.begin();
        std::advance(insert_pos, cur - 1);
        source_.insert(insert_pos, ';');
        prev();
        add_token(token_type::SEMICOLON);
    }
}

void lexer::print_all() const {
    std::cout << std::left << std::setw(5) << "ID" << std::setw(15) << "Type"
              << "Lexeme" << std::endl;
    std::cout << "--------------------------------" << std::endl;

    for (size_t i = 0; i < tokens_.size(); ++i) {
        const Token& token = tokens_[i];
        std::cout << std::setw(5) << i << std::setw(15)
                  << token_type_map.at(token.type) << token.lexeme << std::endl;
    }
}

bool lexer::eof() const { return cur >= source_.size(); }

char lexer::get_cur_char() const { return eof() ? '\0' : source_[cur]; }

char lexer::get_next_char() const {
    return cur + 1 < source_.size() ? source_[cur + 1] : '\0';
}

char lexer::next() {
    pos_in_line++;
    return source_[cur++];
}

void lexer::add_token(token_type type) {
    auto lexeme = source_.substr(start, cur - start);
    // TODO: line, col
    tokens_.push_back({type, lexeme, -1, -1});
}

bool lexer::match(char expected, char expected2) {
    expected = std::toupper(expected);
    expected2 = std::toupper(expected2);
    if (expected2 != '\0' && cur < source_.size() - 1) {
        auto matched = std::toupper(source_[cur]) == expected &&
                       std::toupper(source_[cur + 1]) == expected2;
        if (matched) {
            cur += 2;
        }
        return matched;
    }
    if (eof() || std::toupper(source_[cur]) != expected) return false;
    cur++;
    return true;
}

const lexer::Tokens& lexer::tokenize() {
    while (!eof()) {
        start = cur;
        scan_token();
    }
    tokens_.push_back({token_type::EOF_, "", -1, -1});
    return tokens_;
}

void lexer::scan_string_literal() {
    while (get_cur_char() != '"' && !eof()) {
        if (get_cur_char() == '\n') {
            // TODO: unterminated string literal error
            next();
            return;
        }
        next();
    }
    if (eof()) {
        // TODO: unterminated string literal error
        return;
    }
    next();  // skip closing '"'
    add_token(token_type::STRING);
}

void lexer::scan_multiline_string_literal() {
    while (get_cur_char() != '`' && !eof()) {
        if (get_cur_char() == '\n') line++;
        next();
    }
    if (eof()) {
        // TODO: unterminated string literal error
        return;
    }
    next();  // skip closing '`'
    add_token(token_type::STRING);
}

void lexer::scan_number_starts_with_period() {
    while (std::isdigit(get_cur_char())) next();
    if (get_cur_char() == 'e') {
        if (get_next_char() == '+' || get_next_char() == '-' ||
            std::isdigit(get_next_char())) {
            next();  // skip 'e'
            if ((get_cur_char() == '+' || get_cur_char() == '-') &&
                !std::isdigit(get_next_char())) {
                // TODO error exponenta has not digits
                return;
            }
            next();  // skip after 'e'
            while (std::isdigit(get_cur_char()) && !eof()) next();
        }
    }
    if (get_cur_char() == 'i' && !eof()) {
        next();  // take 'i
        add_token(token_type::IMAG);
    } else {
        add_token(token_type::FLOAT);
    }
}

void lexer::scan_number_starts_with_digit() {
    if (std::tolower(get_cur_char()) == 'b') {
        cur++;
        scan_number_bin();
    } else if (std::tolower(get_cur_char()) == 'o') {
        cur++;
        scan_number_oct();
    } else if (std::tolower(get_cur_char()) == 'x') {
        cur++;
        scan_number_hex();
    } else {
        while (std::isdigit(get_cur_char())) next();
        if (get_cur_char() == '.') {
            cur++;
            scan_number_starts_with_period();
        } else if (get_cur_char() == 'i') {
            next();  // take i
            add_token(token_type::IMAG);
        } else {
            add_token(token_type::INT);
        }
    }
}

void lexer::scan_number_bin() {
    while (get_cur_char() == '1' || get_cur_char() == '0') next();
    if (get_cur_char() == 'i') {
        next();
        add_token(token_type::IMAG);
    } else {
        add_token(token_type::INT);
    }
}

void lexer::scan_number_hex() {
    while ((get_cur_char() >= '0' && get_cur_char() <= '9') ||
           (std::toupper(get_cur_char()) >= 'A' &&
            std::toupper(get_cur_char()) <= 'F'))
        next();
    if (get_cur_char() == 'i') {
        next();
        add_token(token_type::IMAG);
    } else {
        add_token(token_type::INT);
    }
}

void lexer::scan_number_oct() {
    while (get_cur_char() >= '0' && get_cur_char() <= '7') next();
    if (get_cur_char() == 'i') {
        next();
        add_token(token_type::IMAG);
    } else {
        add_token(token_type::INT);
    }
}

void lexer::scan_number() {
    if (source_[cur - 1] == '.') {
        scan_number_starts_with_period();
    } else if (std::isdigit(source_[cur - 1])) {
        if (source_[cur] == '.') {
            cur++;
            scan_number_starts_with_period();
        } else {
            scan_number_starts_with_digit();
        }
    }
}

void lexer::scan_identifier() {
    while (is_alpha_numeric(get_cur_char()) || get_cur_char() == '_') next();
    auto lexeme = source_.substr(start, cur - start);
    auto is_kw = try_get_keyword(lexeme);
    add_token(is_kw.first ? is_kw.second : token_type::IDENT);
}

void lexer::scan_token() {
    // TODO: char
    char c = next();
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
        case ';':
            add_token(token_type::SEMICOLON);
            break;
        // 2
        case '.':  // .[0-9]
            if (cur - 2 > 0 && std::isspace(source_[cur - 2]) &&
                isdigit(source_[cur])) {
                scan_number();
            } else {
                add_token(token_type::PERIOD);
            }
            break;
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
        case '/':  // / /= '//' '/*'
            if (match('=')) {
                add_token(token_type::QUO_ASSIGN);
            } else if (match('/')) {
                while (get_cur_char() != '\n' && !eof()) next();
            } else if (match('*')) {
                while (get_cur_char() != '*' && !eof()) next();
                while (get_cur_char() != '/' && !eof()) next();
            } else {
                add_token(token_type::QUO);
            }
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
        case ' ':
        case '\r':
        case '\t':
            // Ignore whitespace chars.
            break;
        case '\n':
            // TODO: insert semicolon (;) by the rules
            line++;
            insert_semicolon();
            pos_in_line = 0;
            break;
        case '"':
            scan_string_literal();
            break;
        case '`':
            scan_multiline_string_literal();
            break;
        case '\'':
            // TODO char literal
            break;
        default:
            if (std::isdigit(c)) {
                scan_number();
            } else if (is_alpha(c)) {
                scan_identifier();
            } else {
                std::cout << c << ": unexpected character...\n";
            }
    }
}

bool lexer::looks_like_keyword(const std::string& word,
                               const std::string& keyword,
                               int max_changes = 1) const {
    auto word_copy = word, keyword_copy = keyword;
    std::transform(word_copy.begin(), word_copy.end(), word_copy.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    std::transform(keyword_copy.begin(), keyword_copy.end(),
                   keyword_copy.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    int len1 = word_copy.size();
    int len2 = keyword_copy.size();

    if (std::abs(len1 - len2) > max_changes) {
        return false;
    }

    int count = 0;
    int i = 0;
    int j = 0;

    while (i < len1 && j < len2) {
        if (word_copy[i] != keyword_copy[j]) {
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
