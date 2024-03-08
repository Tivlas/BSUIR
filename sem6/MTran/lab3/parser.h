#pragma once
#include <filesystem>
#include <functional>
#include <unordered_map>

#include "error.h"
#include "lexer.h"
#include "tree.h"

using parseSpecFunction = std::function<SP<Spec>(token_type keyword, int iota)>;

enum class operator_precedence { Lowest = 0, Unary = 6, Highest = 7 };

std::unordered_map<token_type, bool> stmtStart = {
    {token_type::BREAK, true},       {token_type::CONST, true},
    {token_type::CONTINUE, true},    {token_type::DEFER, true},
    {token_type::FALLTHROUGH, true}, {token_type::FOR, true},
    {token_type::GOTO, true},        {token_type::IF, true},
    {token_type::RETURN, true},      {token_type::SWITCH, true},
    {token_type::TYPE, true},        {token_type::VAR, true},
};

std::unordered_map<token_type, bool> declStart = {
    {token_type::IMPORT, true},
    {token_type::CONST, true},
    {token_type::TYPE, true},
    {token_type::VAR, true},
};

std::unordered_map<token_type, bool> exprEnd = {
    {token_type::COMMA, true},     {token_type::COLON, true},
    {token_type::SEMICOLON, true}, {token_type::RPAREN, true},
    {token_type::RBRACE, true},    {token_type::RBRACK, true},
};

class parser {
   private:
    const int maxNestLev = 1e5;

    lexer lxr;
    Tokens tokens_;
    Errors errors_;
    V<SP<Decl>> decls_;

    pos_t pos_;
    token_type tok_;   // one token look-ahead
    std::string lit_;  // token literal
    int nestLev_ = 0;
    int exprLev_ = 0;
    bool inRhs_ = false;
    size_t cur_ = 0;
    pos_t syncPos_;
    int syncCnt_ = 0;

    void incNestLev();
    void decNestLev();
    pos_t expect(token_type token);
    pos_t expect2(token_type token);
    void next();
    pos_t expectClosing(token_type tok, std::string context);
    void expectSemi();
    bool atComma(std::string context, token_type follow);
    void assert(bool cond, std::string msg);
    void advance(const std::unordered_map<token_type, bool> to);
    SP<IdentExpr> parseIdent();
    V<SP<IdentExpr>> parseIdentList();
    V<SP<Expr>> parseExprList();
    V<SP<Expr>> parseList(bool inRhs);
    SP<Expr> parseType();
    SP<Expr> parseQualifiedIdent(SP<IdentExpr> ident);
    SP<Expr> parseTypeName(SP<IdentExpr> ident);
    SP<ArrayTypeExpr> parseArrayType(pos_t lbrack, SP<Expr> len);
    std::pair<SP<IdentExpr>, SP<Expr>> parseArrayFieldOrTypeInstance(
        SP<IdentExpr> x);
    SP<Field> parseFieldDecl();
    SP<StructTypeExpr> parseStructType();

    SP<Expr> parseTypeInstance(SP<Expr> typ);
    GenDecl parseGenDecl(token_type keyword, parseSpecFunction f);
    SP<Spec> parseImportSpec(token_type _,
                             int __);  // do not need parameters, but match
                                       // parseSpecFunction interface
    SP<Decl> parseDecl(std::unordered_map<token_type, bool> sync);
    SP<Spec> parseValueSpec(token_type keyword, int iota);
    SP<Expr> tryIdentOrType();
    SP<Expr> packIndexExpr(SP<Expr> x, pos_t lbrack, V<SP<Expr>> exprs,
                           pos_t rbrack);
    SP<Expr> parseRhs();
    SP<Expr> parseExpr();
    SP<Expr> parseBinaryExpr(SP<Expr> x, operator_precedence prec1);
    SP<Expr> parseUnaryExpr();
    std::pair<token_type, int> tokPrec();
    SP<Expr> parsePrimaryExpr(SP<Expr> x);
    SP<Expr> parseOperand();
    SP<Expr> parseFuncTypeOrLit();
    SP<BlockStmt> parseBody();
    V<SP<Stmt>> parseStmtList();
    SP<StructTypeExpr> parseStructType();
    SP<FuncTypeExpr> parseFuncType();
    SP<InterfaceTypeExpr> parseInterfaceType();
    SP<MapTypeExpr> parseMapType();
    SP<Spec> parseTypeSpec(token_type _,
                           int __);  // do not need parameters, but match
                                     // parseSpecFunction interface
    FuncDecl parseFuncDecl();

    // Error methods
    void errorExpected(pos_t pos, std::string msg);

   public:
    parser(const std::filesystem::path&);
    ~parser();
    void parseFile();
};

parser::parser(const std::filesystem::path& path) : lxr(path) {
    lxr.tokenize();
    tokens_ = std::move(lxr.get_tokens());
    errors_ = std::move(lxr.get_errors());
}

void parser::incNestLev() {
    nestLev_++;
    if (nestLev_ > maxNestLev) {
        throw std::runtime_error("exceeded max nesting depth");
    }
}

void parser::decNestLev() { nestLev_--; }

void parser::next() {
    auto token = tokens_[cur_];
    pos_ = token.pos;
    tok_ = token.type;
    lit_ = token.lexeme;
    cur_++;
}

// TODO error()
// TODO errorExpected()

pos_t parser::expect(token_type tok) {
    auto pos = pos_;
    if (tok_ != tok) {
        // TODO error expected
    }
    next();
    return pos;
}

pos_t parser::expect2(token_type tok) {
    pos_t pos;
    if (tok_ == tok) {
        pos = pos_;
    } else {
        // TODO error expected
    }
    next();
    return pos;
}

pos_t parser::expectClosing(token_type tok, std::string context) {
    if (tok_ != tok && tok_ == token_type::SEMICOLON && lit_ == "\n") {
        // TODO error()
        next();
    }
    return expect(tok);
}

void parser::expectSemi() {
    if (tok_ != token_type::RPAREN && tok_ != token_type::RBRACE) {
        switch (tok_) {
            case token_type::COMMA:
                // TODO errorExpected()
                // no break intentionally
            case token_type::SEMICOLON:
                next();
                break;
            default:
                // TODO errorExpected()
                advance(stmtStart);
                break;
        }
    }
}

bool parser::atComma(std::string context, token_type follow) {
    if (tok_ == token_type::COMMA) {
        return true;
    }
    if (tok_ != follow) {
        std::string msg = "missing ';'";
        if (tok_ == token_type::SEMICOLON && lit_ == "\n") {
            msg += " begore newline";
        }
        // TODO error()
        return true;  // found error, report and continue parsing
    }
    return false;
}

void parser::assert(bool cond, std::string msg) {
    if (!cond) {
        throw std::runtime_error("internal parser error...");
    }
}

void parser::advance(std::unordered_map<token_type, bool> to) {
    while (tok_ != token_type::EOF_) {
        if (to[tok_]) {
            if (pos_ == syncPos_ && syncCnt_ < 10) {
                syncCnt_++;
                return;
            }
            if (pos_ > syncPos_) {
                syncPos_ = pos_;
                syncCnt_ = 0;
                return;
            }
        }
        next();
    }
}

// ----------------------------------------------------------------------------
// Identifiers

SP<IdentExpr> parser::parseIdent() {
    auto pos = pos_;
    std::string name = "_";
    if (tok_ == token_type::IDENT) {
        name = lit_;
        next();
    } else {
        expect(token_type::IDENT);
    }
    return std::make_shared<IdentExpr>(pos, name);
}

V<SP<IdentExpr>> parser::parseIdentList() {
    V<SP<IdentExpr>> list;
    list.push_back(parseIdent());
    while (tok_ == token_type::COMMA) {
        next();
        list.push_back(parseIdent());
    }
    return list;
}

// ----------------------------------------------------------------------------
// Common productions

V<SP<Expr>> parser::parseExprList() {
    V<SP<Expr>> list;
    list.push_back(parseExpr());
    while (tok_ == token_type::COMMA) {
        next();
        list.push_back(parseExpr());
    }
    return list;
}

V<SP<Expr>> parser::parseList(bool inRhs) {
    bool old = inRhs_;
    inRhs_ = inRhs;
    auto list = parseExprList();
    inRhs_ = old;
    return list;
}

SP<Expr> parser::parseType() {
    auto typ = tryIdentOrType();
    if (typ == nullptr) {
        auto pos = pos_;
        // TODO errorExpected()
        advance(exprEnd);
        return std::make_shared<BadExpr>(pos, pos_);
    }
    return typ;
}

SP<Expr> parser::parseQualifiedIdent(SP<IdentExpr> ident) {
    auto typ = parseTypeName(ident);
    if (tok_ == token_type::LBRACK) {
        typ = parseTypeInstance(typ);
    }
    return typ;
}

SP<Expr> parser::parseTypeName(SP<IdentExpr> ident) {
    if (ident == nullptr) {
        ident = parseIdent();
    }
    if (tok_ == token_type::PERIOD) {
        // ident is a package name (package.method)
        next();
        auto sel = parseIdent();
        return std::make_shared<SelectorExpr>(ident, sel);
    }
    return ident;
}

// "[" has already been consumed, and lbrack is its position.
// If len != nil it is the already consumed array length.
SP<ArrayTypeExpr> parser::parseArrayType(pos_t lbrack, SP<Expr> len) {
    if (len == nullptr) {
        exprLev_++;
        if (tok_ == token_type::ELLIPSIS) {
            len = std::make_shared<EllipsisExpr>(pos_);
            next();
        } else if (tok_ == token_type::RBRACK) {
            len = parseRhs();
        }
        exprLev_--;
    }
    if (tok_ == token_type::COMMA) {
        // Trailing commas are accepted in type parameter
        // lists but not in array type declarations.
        // Accept for better error handling but complain.
        // TODO error()
        next();
    }
    expect(token_type::RBRACK);
    auto elt = parseType();
    return std::make_shared<ArrayTypeExpr>(lbrack, len, elt);
}

std::pair<SP<IdentExpr>, SP<Expr>> parser::parseArrayFieldOrTypeInstance(
    SP<IdentExpr> x) {
    auto lbrack = expect(token_type::LBRACK);
    auto trailingComma = NoPos;
    V<SP<Expr>> args;
    if (tok_ == token_type::RBRACK) {
        exprLev_++;
        args.push_back(parseRhs());
        while (tok_ == token_type::COMMA) {
            auto comma = pos_;
            next();
            if (tok_ == token_type::RBRACK) {
                trailingComma = comma;
                break;
            }
            args.push_back(parseRhs());
        }
        exprLev_--;
    }

    auto rbrack = expect(token_type::RBRACK);
    if (args.empty()) {
        // x []E
        auto elt = parseType();
        return {x, std::make_shared<ArrayTypeExpr>(lbrack, nullptr, elt)};
    }

    if (args.size() == 1) {
        auto elt = tryIdentOrType();
        if (elt == nullptr) {
            if (trailingComma.IsValid()) {
                // TODO error()
            }
            return {x, std::make_shared<ArrayTypeExpr>(lbrack, args[0], elt)};
        }
    }

    return {nullptr, packIndexExpr(x, lbrack, args, rbrack)};
}

SP<Field> parser::parseFieldDecl() {
    V<SP<IdentExpr>> names;
    SP<Expr> typ;
    SP<IdentExpr> name = nullptr;
    pos_t star = NoPos;
    pos_t pos = NoPos;
    switch (tok_) {
        case token_type::IDENT:
            name = parseIdent();
            if (tok_ == token_type::PERIOD || tok_ == token_type::STRING ||
                tok_ == token_type::SEMICOLON || tok_ == token_type::RBRACE) {
                // embedded type
                typ = name;
                if (tok_ == token_type::PERIOD) {
                    typ = parseQualifiedIdent(name);
                }
            } else {
                // name1, name2, ... T
                names = {name};
                while (tok_ == token_type::COMMA) {
                    next();
                    names.push_back(parseIdent());
                }

                // Careful dance: We don't know if we have an embedded
                // instantiated type T[P1, P2, ...] or a field T of array type
                // []E or [P]E.
                if (names.size() == 1 && tok_ == token_type::LBRACK) {
                    std::tie(name, typ) = parseArrayFieldOrTypeInstance(name);
                    if (name == nullptr) {
                        names.clear();
                    }
                } else {
                    // T P
                    typ = parseType();
                }
            }
            break;
        case token_type::MUL:
            star = pos_;
            next();
            if (tok_ == token_type::LPAREN) {
                // *(T)
                // TODO error()
                next();
                typ = parseQualifiedIdent(nullptr);
                // expect closing ')' but no need to complain if missing
                if (tok_ == token_type::RPAREN) {
                    next();
                }
            } else {
                // *T
                typ = parseQualifiedIdent(nullptr);
            }
            typ = std::make_shared<StarExpr>(star, typ);
            break;
        case token_type::LPAREN:
            // TODO error()
            next();
            if (tok_ == token_type::MUL) {
                // (*T)
                star = pos_;
                next();
                typ = std::make_shared<StarExpr>(star,
                                                 parseQualifiedIdent(nullptr));
            } else {
                // (T)
                typ = parseQualifiedIdent(nullptr);
            }
            // expect closing ')' but no need to complain if missing
            if (tok_ == token_type::RPAREN) {
                next();
            }
            break;
        default:
            pos = pos_;
            // TODO errorExpected()
            advance(exprEnd);
            typ = std::make_shared<BadExpr>(pos, pos_);
            break;
    }

    SP<BasicLitExpr> tag;
    if (tok_ == token_type::STRING) {
        tag = std::make_shared<BasicLitExpr>(pos_, tok_, lit_);
    }

    expectSemi();
    return std::make_shared<Field>(names, typ, tag);
}

SP<StructTypeExpr> parser::parseStructType() {
    auto pos = expect(token_type::STRUCT);
    auto lbrace = expect(token_type::LBRACE);
    V<SP<Field>> list;
    while (tok_ == token_type::IDENT || tok_ == token_type::MUL || tok_ == token_type::LPAREN) {
        // a field declaration cannot start with a '(' but we accept
		// it here for more robust parsing and better error messages
		// (parseFieldDecl will check and complain if necessary)
        list.push_back(parseFieldDecl());
    }
    auto rbrace = expect(token_type::RBRACE);

    return std::make_shared<StructTypeExpr>(pos, std::make_shared<FieldList>(lbrace, list, rbrace));
}