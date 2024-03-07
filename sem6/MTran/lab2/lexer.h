#pragma once
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "token.h"

namespace color {
const std::string error = "\033[1;91m";
const std::string warning = "\033[1;95m";
const std::string reset = "\033[0m";
};  // namespace color

/*
TODO

semicolon token pos after inser

tests

errors
*/
class lexer {
    struct lexical_error {
        Token::token_position pos;
        std::string msg;
    };

    struct lexical_warning {
        Token::token_position pos;
        std::string msg;
    };

    using Tokens = std::vector<Token>;
    using Errors = std::vector<lexical_error>;
    using Warnings = std::vector<lexical_warning>;

   private:
    Tokens tokens_;
    Errors errors_;
    Warnings warnings_;
    std::string source_;
    size_t start_ = 0;
    size_t cur_ = 0;
    size_t line_ = 1;
    std::filesystem::path file_path_;

    bool looks_like_keyword(const std::string&, const std::string&, int) const;
    bool eof() const;
    void scan_token();
    char get_cur_char() const;
    char get_next_char() const;
    char get_prev_char() const;
    size_t get_lexem_col() const;
    char next();
    void add_token(token_type, size_t = -1);
    void add_error(const Token::token_position&, const std::string&);
    void add_warning(const Token::token_position&, const std::string&);
    bool match(char, char = '\0');
    void scan_string_literal();
    void scan_multiline_string_literal();
    void scan_char_literal();
    void scan_number();
    void scan_number_starts_with_period();
    void scan_number_starts_with_digit();
    void scan_number_bin();
    void scan_number_oct();
    void scan_number_hex();
    void scan_identifier();  // or keyword
    void insert_semicolon();

   public:
    lexer(const std::filesystem::path&);
    ~lexer();
    const Tokens& tokenize();
    void print_as_table() const;
    void print_as_token_sequence() const;
};

lexer::lexer(const std::filesystem::path& path) : file_path_(path) {
    file_path_ = std::filesystem::absolute(file_path_);
    std::ifstream file(file_path_);
    if (!file.is_open()) {
        throw std::invalid_argument("Invalid file! " + file_path_.string());
    }
    std::string code;
    std::string line;
    while (std::getline(file, line)) {
        code += line + '\n';
    }
    source_ = std::move(code);
    file.close();
}

lexer::~lexer() {}

void lexer::insert_semicolon() {
    auto last_token_type = tokens_.back().type;
    if (last_token_type == token_type::IDENT ||
        last_token_type == token_type::INT ||
        last_token_type == token_type::FLOAT ||
        last_token_type == token_type::CHAR ||
        last_token_type == token_type::IMAG ||
        last_token_type == token_type::STRING ||
        last_token_type == token_type::BREAK ||
        last_token_type == token_type::CONTINUE ||
        last_token_type == token_type::FALLTHROUGH ||
        last_token_type == token_type::RETURN ||
        last_token_type == token_type::INC ||
        last_token_type == token_type::DEC ||
        last_token_type == token_type::RPAREN ||
        last_token_type == token_type::RBRACK ||
        last_token_type == token_type::RBRACE) {
        auto insert_pos = source_.begin();
        std::advance(insert_pos, cur_ - 1);
        source_.insert(insert_pos, ';');
        add_token(token_type::SEMICOLON);
        next();
    }
}

void lexer::print_as_token_sequence() const {
    auto out_path = file_path_.parent_path() / "tokens" /
                    (file_path_.stem().string() + "_tokens" +
                     file_path_.extension().string());
    std::ofstream out(out_path);
    size_t prev_line = 1;
    int id = 0;
    bool first = true;
    for (const auto& t : tokens_) {
        if (t.pos.line != prev_line) {
            out << '\n';
            prev_line = t.pos.line;
            first = true;
        }
        if (first) {
            out << std::string(t.pos.col - 1, ' ');
        }
        if (t.type == token_type::IDENT) {
            out << '<' << id << "> ";
        } else {
            out << t.lexeme << ' ';
        }
        first = false;
        id++;
    }
    out << '\n';
}

void lexer::print_as_table() const {
    std::cout << std::left << std::setw(10) << "ID" << std::setw(15) << "POS"
              << std::setw(15) << "TYPE"
              << "LEXEME" << '\n';
    std::cout << "------------------------------------------------------"
              << '\n';

    for (size_t i = 0; i < tokens_.size(); ++i) {
        const Token& token = tokens_[i];
        std::cout << std::setw(10) << i << std::setw(15)
                  << std::to_string(token.pos.line) + ":" +
                         std::to_string(token.pos.col)
                  << std::setw(15) << type_string_map.at(token.type)
                  << token.lexeme << '\n';
    }

    std::cout << '\n';
    for (const auto& e : errors_) {
        std::cout << color::error << "Error at " << color::reset << e.pos.line
                  << ':' << e.pos.col << " --- " << e.msg << '\n';
    }
    std::cout << '\n';
    for (const auto& w : warnings_) {
        std::cout << color::warning << "Warning at " << color::reset
                  << w.pos.line << ':' << w.pos.col << " --- " << w.msg << '\n';
    }
    std::cout << '\n';
}

bool lexer::eof() const { return cur_ >= source_.size(); }

char lexer::get_cur_char() const { return eof() ? '\0' : source_[cur_]; }

char lexer::get_next_char() const {
    return cur_ + 1 < source_.size() ? source_[cur_ + 1] : '\0';
}

char lexer::get_prev_char() const {
    return cur_ - 1 >= 0 ? source_[cur_ - 1] : '\0';
}

char lexer::next() { return source_[cur_++]; }

size_t lexer::get_lexem_col() const {
    return start_ - source_.rfind('\n', start_);
}

void lexer::add_token(token_type type, size_t line) {
    auto lexeme = source_.substr(start_, cur_ - start_);
    tokens_.push_back(
        {type,
         lexeme,
         {file_path_, line == -1ULL ? line_ : line, get_lexem_col()}});
}

void lexer::add_error(const Token::token_position& pos,
                      const std::string& msg) {
    errors_.push_back({pos, msg});
}

void lexer::add_warning(const Token::token_position& pos,
                        const std::string& msg) {
    warnings_.push_back({pos, msg});
}

bool lexer::match(char expected, char expected2) {
    expected = std::toupper(expected);
    expected2 = std::toupper(expected2);
    if (expected2 != '\0' && cur_ < source_.size() - 1) {
        auto matched = std::toupper(source_[cur_]) == expected &&
                       std::toupper(source_[cur_ + 1]) == expected2;
        if (matched) {
            cur_ += 2;
        }
        return matched;
    }
    if (eof() || std::toupper(source_[cur_]) != expected) return false;
    cur_++;
    return true;
}

const lexer::Tokens& lexer::tokenize() {
    while (!eof()) {
        start_ = cur_;
        scan_token();
    }
    return tokens_;
}

void lexer::scan_string_literal() {
    bool escaped = false;
    while (!eof() && (get_cur_char() != '"' || escaped)) {
        if (get_cur_char() == '\n') {
            add_error({file_path_, line_, get_lexem_col()},
                      "string literal not terminated");
            add_token(token_type::STRING);
            return;
        }
        escaped = get_cur_char() == '\\' ? !escaped : false;
        next();
    }
    if (eof()) {
        add_error({file_path_, line_, get_lexem_col()},
                  "string literal not terminated");
        return;
    }
    next();  // skip closing '"'
    add_token(token_type::STRING);
}

void lexer::scan_multiline_string_literal() {
    size_t prev_line = line_;
    while (!eof() && get_cur_char() != '`') {
        if (get_cur_char() == '\n') line_++;
        next();
    }
    if (eof()) {
        cur_--;
        add_error({file_path_, line_, get_lexem_col()},
                  "string literal not terminated");
        add_token(token_type::STRING, prev_line);
        return;
    }
    next();  // skip closing '`'
    add_token(token_type::STRING, prev_line);
}

void lexer::scan_char_literal() {
    static const std::unordered_set<std::string> escape_sequences = {
        "\\n", "\\r", "\\t", "\\'", "\\\\", "\\'",
        "\\f", "\\0", "\\v", "\\r", "\\a",  "\\b"};
    bool escaped = false;
    while (!eof() && (get_cur_char() != '\'' || escaped)) {
        if (get_cur_char() == '\n') {
            add_error({file_path_, line_, get_lexem_col()},
                      "rune literal not terminated");
            add_token(token_type::CHAR);
            return;
        }
        escaped = get_cur_char() == '\\' ? !escaped : false;
        next();
    }
    if (eof()) {
        add_error({file_path_, line_, get_lexem_col()},
                  "rune literal not terminated");
        add_token(token_type::CHAR);
        return;
    }
    auto rune_literal = source_.substr(start_ + 1, cur_ - start_ - 1);
    if (rune_literal.size() > 1 &&
        escape_sequences.find(rune_literal) == escape_sequences.end()) {
        add_error({file_path_, line_, get_lexem_col()}, "illegal rune literal");
    }
    if (rune_literal.size() == 0) {
        add_error({file_path_, line_, get_lexem_col()}, "illegal rune literal");
    }
    next();  // skip closing '\''
    add_token(token_type::CHAR);
}

void lexer::scan_number_starts_with_period() {
    while (!eof() && std::isdigit(get_cur_char())) next();
    if (get_cur_char() == 'e') {
        if (get_next_char() == '+' || get_next_char() == '-' ||
            std::isdigit(get_next_char())) {
            next();  // skip 'e'
            if ((get_cur_char() == '+' || get_cur_char() == '-') &&
                !std::isdigit(get_next_char())) {
                next();  // take + or -
                add_token(token_type::FLOAT);
                add_error({file_path_, line_, get_lexem_col()},
                          "exponent has no digits");
                return;
            }
            next();  // skip e
            while (!eof() && std::isdigit(get_cur_char())) next();
        } else {
            next();  // take e
            add_token(token_type::FLOAT);
            add_error({file_path_, line_, get_lexem_col()},
                      "exponent has no digits");
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
        cur_++;
        scan_number_bin();
    } else if (std::tolower(get_cur_char()) == 'o') {
        cur_++;
        scan_number_oct();
    } else if (std::tolower(get_cur_char()) == 'x') {
        cur_++;
        scan_number_hex();
    } else {
        while (!eof() && std::isdigit(get_cur_char())) next();
        if (get_cur_char() == 'e') {
            if (get_next_char() == '+' || get_next_char() == '-' ||
                std::isdigit(get_next_char())) {
                next();
                if ((get_cur_char() == '+' || get_cur_char() == '-') &&
                    !std::isdigit(get_next_char())) {
                    next();  // take + or -
                    add_token(token_type::FLOAT);
                    add_error({file_path_, line_, get_lexem_col()},
                              "exponent has no digits");
                    return;
                }
                next();
                while (!eof() && std::isdigit(get_cur_char())) next();
                if (get_cur_char() == 'i') {
                    next();  // take i
                    add_token(token_type::IMAG);
                } else {
                    add_token(token_type::FLOAT);
                }
            } else {
                next();  // take e
                add_token(token_type::FLOAT);
                add_error({file_path_, line_, get_lexem_col()},
                          "exponent has no digits");
            }
        } else if (get_cur_char() == '.') {
            cur_++;
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
    while (!eof() && std::isdigit(get_cur_char())) next();
    if (get_prev_char() == 'b' || get_prev_char() == 'b') {
        add_error({file_path_, line_, get_lexem_col()},
                  "binary literal has no digits");
    }
    auto bin_num = source_.substr(start_, cur_ - start_);
    auto invalid_digit_pos = bin_num.find_first_not_of("bB01");
    if (invalid_digit_pos != std::string::npos) {
        add_error({file_path_, line_, get_lexem_col()},
                  "invalid digit in binary literal: " +
                      std::string(1, bin_num[invalid_digit_pos]));
    }
    if (get_cur_char() == 'i') {
        next();
        add_token(token_type::IMAG);
    } else {
        add_token(token_type::INT);
    }
}

void lexer::scan_number_hex() {
    while (!eof() && ((std::isdigit(get_cur_char())) ||
                      (std::toupper(get_cur_char()) >= 'A' &&
                       std::toupper(get_cur_char()) <= 'F'))) {
        next();
    }
    if (get_prev_char() == 'x' || get_prev_char() == 'X') {
        add_error({file_path_, line_, get_lexem_col()},
                  "hexadecimal literal has no digits");
    }
    if (get_cur_char() == 'i') {
        next();
        add_token(token_type::IMAG);
    } else {
        add_token(token_type::INT);
    }
}

void lexer::scan_number_oct() {
    while (!eof() && std::isdigit(get_cur_char())) next();
    if (get_prev_char() == 'o' || get_prev_char() == 'O') {
        add_error({file_path_, line_, get_lexem_col()},
                  "octal literal has no digits");
    }
    auto oct_num = source_.substr(start_, cur_ - start_);
    auto invalid_digit_pos = oct_num.find_first_not_of("oO01234567");
    if (invalid_digit_pos != std::string::npos) {
        add_error({file_path_, line_, get_lexem_col()},
                  "invalid digit in oct literal: " +
                      std::string(1, oct_num[invalid_digit_pos]));
    }
    if (get_cur_char() == 'i') {
        next();
        add_token(token_type::IMAG);
    } else {
        add_token(token_type::INT);
    }
}

void lexer::scan_number() {
    if (source_[cur_ - 1] == '.') {
        scan_number_starts_with_period();
    } else if (std::isdigit(source_[cur_ - 1])) {
        if (source_[cur_] == '.') {
            cur_++;
            scan_number_starts_with_period();
        } else {
            scan_number_starts_with_digit();
        }
    }
}

void lexer::scan_identifier() {
    while (!eof() && is_alpha_numeric(get_cur_char())) next();
    auto lexeme = source_.substr(start_, cur_ - start_);
    auto is_kw = try_get_keyword(lexeme);
    if (!is_kw.first && only_lowercase_letters(lexeme)) {
        for (const auto& [type, kw] : type_string_map) {
            if (type > token_type::keywords_beg &&
                type < token_type::keywords_end &&
                looks_like_keyword(lexeme, kw,
                                   lexeme.size() <= 3
                                       ? 1
                                       : std::min(lexeme.size() / 4, 3ULL))) {
                add_warning({file_path_, line_, get_lexem_col()},
                            "did you mean " + kw);
                break;
            }
        }
    }
    add_token(is_kw.first ? is_kw.second : token_type::IDENT);
}

void lexer::scan_token() {
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
            if (match('.', '.')) {
                add_token(token_type::ELLIPSIS);
            } else if (isdigit(source_[cur_])) {
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
                while (!eof() && get_cur_char() != '\n') next();
                add_token(token_type::COMMENT);
            } else if (match('*')) {
                size_t prev_line = line_;
                while (!eof() &&
                       (get_cur_char() != '*' ||
                        (get_cur_char() == '*' && get_next_char() != '/'))) {
                    if (get_cur_char() == '\n') line_++;
                    next();
                };
                if (eof()) {
                    add_error({file_path_, prev_line, get_lexem_col()},
                              "comment not terminated");
                }
                if (!eof()) next();
                if (!eof()) next();
                auto lexeme = source_.substr(start_, cur_ - start_);
                tokens_.push_back({token_type::COMMENT,
                                   lexeme,
                                   {file_path_, prev_line, get_lexem_col()}});
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
            insert_semicolon();
            line_++;
            break;
        case '"':
            scan_string_literal();
            break;
        case '`':
            scan_multiline_string_literal();
            break;
        case '\'':
            scan_char_literal();
            break;
        default:
            if (std::isdigit(c)) {
                scan_number();
            } else if (is_alpha(c)) {
                scan_identifier();
            } else {
                add_error({file_path_, line_, get_lexem_col()},
                          "illegal character");
                add_token(token_type::ILLEGAL);
            }
    }
}

bool lexer::looks_like_keyword(const std::string& word,
                               const std::string& keyword,
                               int max_changes = 1) const {
    if (word.size() < keyword.size()) return false;
    auto word_copy = word, keyword_copy = keyword;
    std::transform(word_copy.begin(), word_copy.end(), word_copy.begin(),
                   ::toupper);
    std::transform(keyword_copy.begin(), keyword_copy.end(),
                   keyword_copy.begin(), ::toupper);
    int w_size = word_copy.size();
    int kw_size = keyword_copy.size();

    if (std::abs(w_size - kw_size) > max_changes) {
        return false;
    }

    int count = 0;
    int i = 0;
    int j = 0;

    while (i < w_size && j < kw_size) {
        if (word_copy[i] != keyword_copy[j]) {
            if (count == max_changes) {
                return false;
            }

            if (w_size > kw_size) {
                i++;
            } else if (w_size < kw_size) {
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

    while (i++ < w_size || j++ < kw_size) {
        count++;
    }

    return count <= max_changes;
}
