#pragma once
#include <string>
#include <unordered_map>
#include <vector>

enum class token_type : uint8_t {
    ILLEGAL,
    EOF_,
    COMMENT,
    // Literals
    literal_beg,  // BEG
    IDENT,        // main
    INT,          // 123
    FLOAT,        // 123.45
    IMAG,         // 123.45i
    CHAR,         // 'a'
    STRING,       // 'abc'
    literal_end,  // END
    // Operators
    operators_beg,   // BEG
    ADD,             // +
    SUB,             // -
    MUL,             // *
    QUO,             // /
    REM,             // %
    AND,             // &
    OR,              // |
    XOR,             // ^
    SHL,             // <<
    SHR,             // >>
    AND_NOT,         // &^
    ADD_ASSIGN,      // +=
    SUB_ASSIGN,      // -=
    MUL_ASSIGN,      // *=
    QUO_ASSIGN,      // /=
    REM_ASSIGN,      // %=
    AND_ASSIGN,      // &=
    OR_ASSIGN,       // |=
    XOR_ASSIGN,      // ^=
    SHL_ASSIGN,      // <<=
    SHR_ASSIGN,      // >>=
    AND_NOT_ASSIGN,  // &^=
    LAND,            // &&
    LOR,             // ||
    INC,             // ++
    DEC,             // --
    EQL,             // ==
    LSS,             // <
    GTR,             // >
    ASSIGN,          // =
    NOT,             // !
    NEQ,             // !=
    LEQ,             // <=
    GEQ,             // >=
    DEFINE,          // :=
    LPAREN,          // (
    LBRACK,          // [
    LBRACE,          // {
    COMMA,           // ,
    PERIOD,          // .
    RPAREN,          // )
    RBRACK,          // ]
    RBRACE,          // }
    SEMICOLON,       // ;
    COLON,           // :
    operators_end,   // END
    // Keywords
    keywords_beg,  // BEG
    BREAK,
    CASE,
    CONST,
    CONTINUE,
    DEFAULT,
    DEFER,
    ELSE,
    FALLTHROUGH,
    FOR,
    FUNC,
    GOTO,
    IF,
    IMPORT,
    INTERFACE,
    MAP,
    PACKAGE,
    RANGE,
    RETURN,
    STRUCT,
    SWITCH,
    TYPE,
    VAR,
    keywords_end,  // END
};

const std::unordered_map<token_type, std::string> token_type_map = {
    {token_type::ILLEGAL, "ILLEGAL"},
    {token_type::EOF_, "EOF_"},
    {token_type::COMMENT, "COMMENT"},
    {token_type::IDENT, "IDENT"},
    {token_type::INT, "INT"},
    {token_type::FLOAT, "FLOAT"},
    {token_type::IMAG, "IMAG"},
    {token_type::CHAR, "CHAR"},
    {token_type::STRING, "STRING"},
    {token_type::ADD, "+"},
    {token_type::SUB, "-"},
    {token_type::MUL, "*"},
    {token_type::QUO, "/"},
    {token_type::REM, "%"},
    {token_type::AND, "&"},
    {token_type::OR, "|"},
    {token_type::XOR, "^"},
    {token_type::SHL, "<<"},
    {token_type::SHR, ">>"},
    {token_type::AND_NOT, "&^"},
    {token_type::ADD_ASSIGN, "+="},
    {token_type::SUB_ASSIGN, "-="},
    {token_type::MUL_ASSIGN, "*="},
    {token_type::QUO_ASSIGN, "/="},
    {token_type::REM_ASSIGN, "%="},
    {token_type::AND_ASSIGN, "&="},
    {token_type::OR_ASSIGN, "|="},
    {token_type::XOR_ASSIGN, "^="},
    {token_type::SHL_ASSIGN, "<<="},
    {token_type::SHR_ASSIGN, ">>="},
    {token_type::AND_NOT_ASSIGN, "&^="},
    {token_type::LAND, "&&"},
    {token_type::LOR, "||"},
    {token_type::INC, "++"},
    {token_type::DEC, "--"},
    {token_type::EQL, "=="},
    {token_type::LSS, "<"},
    {token_type::GTR, ">"},
    {token_type::ASSIGN, "="},
    {token_type::NOT, "!"},
    {token_type::NEQ, "!="},
    {token_type::LEQ, "<="},
    {token_type::GEQ, ">="},
    {token_type::DEFINE, ":="},
    {token_type::LPAREN, "("},
    {token_type::LBRACK, "["},
    {token_type::LBRACE, "{"},
    {token_type::COMMA, ","},
    {token_type::PERIOD, "."},
    {token_type::RPAREN, ")"},
    {token_type::RBRACK, "]"},
    {token_type::RBRACE, "}"},
    {token_type::SEMICOLON, ";"},
    {token_type::COLON, ":"},
    {token_type::BREAK, "BREAK"},
    {token_type::CASE, "CASE"},
    {token_type::CONST, "CONST"},
    {token_type::CONTINUE, "CONTINUE"},
    {token_type::DEFAULT, "DEFAULT"},
    {token_type::DEFER, "DEFER"},
    {token_type::ELSE, "ELSE"},
    {token_type::FALLTHROUGH, "FALLTHROUGH"},
    {token_type::FOR, "FOR"},
    {token_type::FUNC, "FUNC"},
    {token_type::GOTO, "GOTO"},
    {token_type::IF, "IF"},
    {token_type::IMPORT, "IMPORT"},
    {token_type::INTERFACE, "INTERFACE"},
    {token_type::MAP, "MAP"},
    {token_type::PACKAGE, "PACKAGE"},
    {token_type::RANGE, "RANGE"},
    {token_type::RETURN, "RETURN"},
    {token_type::STRUCT, "STRUCT"},
    {token_type::SWITCH, "SWITCH"},
    {token_type::TYPE, "TYPE"},
    {token_type::VAR, "VAR"}};

bool is_keyword(const std::string& name) {
    constexpr auto init_lambda = []() {
        std::unordered_map<std::string, token_type> ump;
        for (int i = static_cast<int>(token_type::keywords_beg) + 1;
             i < static_cast<int>(token_type::keywords_end); i++) {
            ump[token_type_map.at(static_cast<token_type>(i))] =
                static_cast<token_type>(i);
        }
        return ump;
    };
    static const std::unordered_map<std::string, token_type> keywords =
        init_lambda();
    return keywords.find(name) != keywords.cend();
}

bool is_identifier(const std::string& name) {
    if (name == "" || is_keyword(name)) return false;
    for (size_t i = 0; i < name.size(); i++) {
        if (!std::isalpha(name[i]) && name[i] != '_' &&
            (i == 0 || !std::isdigit(name[i])))
            return false;
    }
    return true;
}

struct Token {
    token_type type;
    std::string lexeme;
    std::string error = "";
    int line;
    int col;

    Token() {}

    Token(token_type type, std::string lexeme, int line, int col,
          std::string error = "")
        : type(type), lexeme(lexeme), error(error), line(line), col(col) {}

    bool operator==(const Token& other) const {
        return type == other.type && lexeme == other.lexeme &&
               error == other.error;
    }

    bool is_keyword() const;
    bool is_literal() const;
    bool is_operator() const;
};

bool Token::is_keyword() const {
    return type > token_type::keywords_beg && type < token_type::keywords_end;
}

bool Token::is_literal() const {
    return type > token_type::literal_beg && type < token_type::literal_end;
}

bool Token::is_operator() const {
    return type > token_type::operators_beg && type < token_type::operators_end;
}