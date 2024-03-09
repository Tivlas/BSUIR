#pragma once
#include <filesystem>
#include <functional>
#include <unordered_map>

#include "error.h"
#include "lexer.h"
#include "tree.h"

// TODO IMPORTANT interface comparison (check all == usage)
// TODO check all switch case var decls (use {} instead of outer declaration)

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

struct field {
    SP<IdentExpr> name;
    SP<Expr> typ;
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
    SP<StarExpr> parsePointerType();
    SP<EllipsisExpr> parseDotsType();
    field parseParamDecl(SP<IdentExpr> name, bool typeSetsOk);
    V<SP<Field>> parseParameterList(SP<IdentExpr> name0, SP<Expr> typ0,
                                    token_type closing);
    std::pair<SP<FieldList>, SP<FieldList>> parseParameters(bool acceptTParams);
    SP<FieldList> parseResult();
    SP<FuncTypeExpr> parseFuncType();
    SP<Field> parseMethodSpec();
    SP<Expr> embeddedElem(SP<Expr> x);
    SP<Expr> embeddedTerm();
    SP<InterfaceTypeExpr> parseInterfaceType();
    SP<MapTypeExpr> parseMapType();
    SP<Expr> parseTypeInstance(SP<Expr> typ);
    SP<Expr> tryIdentOrType();
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    GenDecl parseGenDecl(token_type keyword, parseSpecFunction f);
    SP<Spec> parseImportSpec(token_type _,
                             int __);  // do not need parameters, but match
                                       // parseSpecFunction interface
    SP<Decl> parseDecl(std::unordered_map<token_type, bool> sync);
    SP<Spec> parseValueSpec(token_type keyword, int iota);
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
    while (tok_ == token_type::IDENT || tok_ == token_type::MUL ||
           tok_ == token_type::LPAREN) {
        // a field declaration cannot start with a '(' but we accept
        // it here for more robust parsing and better error messages
        // (parseFieldDecl will check and complain if necessary)
        list.push_back(parseFieldDecl());
    }
    auto rbrace = expect(token_type::RBRACE);

    return std::make_shared<StructTypeExpr>(
        pos, std::make_shared<FieldList>(lbrace, list, rbrace));
}

SP<StarExpr> parser::parsePointerType() {
    auto star = expect(token_type::MUL);
    auto base = parseType();
    return std::make_shared<StarExpr>(star, base);
}

SP<EllipsisExpr> parser::parseDotsType() {
    auto pos = expect(token_type::ELLIPSIS);
    auto elt = parseType();
    return std::make_shared<EllipsisExpr>(pos, elt);
}

field parser::parseParamDecl(SP<IdentExpr> name, bool typeSetsOk) {
    auto ptok = tok_;
    field f;
    if (name == nullptr) {
        tok_ = token_type::IDENT;
    } else if (typeSetsOk && tok_ == token_type::TILDE) {
        // "~" ...
        return field{nullptr, embeddedElem(nullptr)};
    }

    switch (tok_) {
        case token_type::IDENT:
            if (name == nullptr) {
                f.name = name;
                tok_ = ptok;
            } else {
                f.name = parseIdent();
            }
            switch (tok_) {
                case token_type::IDENT:
                case token_type::MUL:
                // TODO case token_type ::ARROW:
                case token_type::FUNC:
                // TODO case token_type ::CHAN:
                case token_type::MAP:
                case token_type::STRUCT:
                case token_type::INTERFACE:
                case token_type::LPAREN:
                    f.typ = parseType();
                    break;
                case token_type::LBRACK:
                    std::tie(f.name, f.typ) =
                        parseArrayFieldOrTypeInstance(f.name);
                    break;
                case token_type::ELLIPSIS:
                    f.typ = parseDotsType();
                    break;
                case token_type::PERIOD:
                    f.typ = parseQualifiedIdent(f.name);
                    f.name = nullptr;
                    break;
                case token_type::TILDE:
                    if (typeSetsOk) {
                        f.typ = embeddedElem(nullptr);
                        return f;
                    }
                    break;
                case token_type::OR:
                    if (typeSetsOk) {
                        f.typ = embeddedElem(f.name);
                        f.name = nullptr;
                        return f;
                    }
                    break;
            }
            break;

        case token_type::MUL:
        // TODO case token_type ::ARROW:
        case token_type::FUNC:
        case token_type::LBRACK:
        // TODO case token_type ::CHAN:
        case token_type::MAP:
        case token_type::STRUCT:
        case token_type::INTERFACE:
        case token_type::LPAREN:
            f.typ = parseType();
            break;
        case token_type::ELLIPSIS:
            f.typ = parseDotsType();
            return f;
        default:
            // TODO errorExpected()
            advance(exprEnd);
            break;
    }

    if (typeSetsOk && tok_ == token_type::OR && f.typ != nullptr) {
        f.typ = embeddedElem(f.typ);
    }

    return f;
}

V<SP<Field>> parser::parseParameterList(SP<IdentExpr> name0, SP<Expr> typ0,
                                        token_type closing) {
    V<SP<Field>> params;
    bool tparams = closing == token_type::RBRACK;
    bool typeSetOk = tparams;
    pos_t pos = pos_;
    if (name0 != nullptr) {
        pos = name0->Pos();
    }

    V<field> list;
    int named = 0;
    while (name0 != nullptr || tok_ != closing && tok_ != token_type::EOF_) {
        field par;
        if (typ0 != nullptr) {
            if (typeSetOk) {
                typ0 = embeddedElem(typ0);
            }
            par = field{name0, typ0};
        } else {
            par = parseParamDecl(name0, typeSetOk);
        }
        name0 = nullptr;  // 1st name was consumed if present
        typ0 = nullptr;   // 1st typ was consumed if present
        if (par.name != nullptr || par.typ != nullptr) {
            list.push_back(par);
            if (par.name != nullptr && par.typ != nullptr) {
                named++;
            }
        }
        if (!atComma("parameter list", closing)) {
            break;
        }
        next();
    }

    if (list.empty()) {
        return params;
    }

    if (named == 0) {
        // all unnamed => found names are type names
        for (size_t i = 0; i < list.size(); i++) {
            auto par = list[i];
            if (auto typ = par.name; typ != nullptr) {
                par.typ = typ;
                par.name = nullptr;
            }
        }
        if (tparams) {
            // TODO error()
        }
    } else if (named != list.size()) {
        // some named => all must be named
        bool ok = true;
        SP<Expr> typ;
        pos_t missingName = pos;
        for (size_t i = list.size() - 1; i >= 0; i--) {
            if (auto par = list[i]; par.typ != nullptr) {
                typ = par.typ;
                if (par.name == nullptr) {
                    ok = false;
                    missingName = par.typ->Pos();
                    auto n = std::make_shared<IdentExpr>(NoPos, "_");
                    n->NamePos = typ->Pos();
                    par.name = n;
                }
            } else if (typ != nullptr) {
                par.typ = typ;
            } else {
                // par.typ == nulltpr && typ == nullptr => we only have a
                // par.name
                ok = false;
                missingName = par.name->Pos();
                par.typ = std::make_shared<BadExpr>(par.name->Pos(), pos_);
            }
        }
        if (!ok) {
            if (tparams) {
                // TODO error()
            } else {
                // TODO error()
            }
        }
    }

    if (named == 0) {
        for (const auto& par : list) {
            assert(par.typ != nullptr,
                   "nullptr type in unnamed parameter list");
            params.push_back(
                std::make_shared<Field>(nullptr, par.typ, nullptr));
        }
        return params;
    }

    V<SP<IdentExpr>> names;
    SP<Expr> typ;
    auto addParams = [&params, &typ, &names, this]() {
        assert(typ != nullptr, "nullptr type in named parameter list");
        auto field = std::make_shared<Field>(names, typ, nullptr);
        params.push_back(field);
        names.clear();
    };
    for (const auto& par : list) {
        if (*par.typ != *typ) {
            if (names.size() > 0) {
                addParams();
            }
            typ = par.typ;
        }
        names.push_back(par.name);
    }
    if (names.size() > 0) {
        addParams();
    }
    return params;
}

std::pair<SP<FieldList>, SP<FieldList>> parser::parseParameters(
    bool acceptTParams) {
    SP<FieldList> tparams, params;

    if (acceptTParams && tok_ == token_type::LBRACK) {
        auto opening = pos_;
        next();
        auto list = parseParameterList(nullptr, nullptr, token_type::RBRACK);
        pos_t rbrack = expect(token_type::RBRACK);
        tparams = std::make_shared<FieldList>(opening, list, rbrack);
        if (tparams->NumFields() == 0) {
            // TODO error
            tparams = nullptr;
        }
    }

    pos_t opening = expect(token_type::LPAREN);

    V<SP<Field>> fields;
    if (tok_ != token_type::RPAREN) {
        fields = parseParameterList(nullptr, nullptr, token_type::RPAREN);
    }

    pos_t rparen = expect(token_type::RPAREN);
    params = std::make_shared<FieldList>(opening, fields, rparen);

    return {tparams, params};
}

SP<FieldList> parser::parseResult() {
    if (tok_ == token_type::LPAREN) {
        auto [_, results] = parseParameters(false);
        return results;
    }
    auto typ = tryIdentOrType();
    if (typ == nullptr) {
        V<SP<Field>> list;
        list.push_back(std::make_shared<Field>(nullptr, typ, nullptr));
        return std::make_shared<FieldList>(NoPos, list, NoPos);
    }
    return nullptr;
}

SP<FuncTypeExpr> parser::parseFuncType() {
    auto pos = expect(token_type::FUNC);
    auto [tparams, params] = parseParameters(true);
    if (tparams != nullptr) {
        // TODO error()
    }
    auto results = parseResult();
    return std::make_shared<FuncTypeExpr>(pos, nullptr, params, results);
}

SP<Field> parser::parseMethodSpec() {
    V<SP<IdentExpr>> idents;
    SP<Expr> typ;
    auto x = parseTypeName(nullptr);
    if (auto ident = dynamic_cast<IdentExpr*>(x.get()); ident != nullptr) {
        switch (tok_) {
            case token_type::LBRACK: {
                pos_t lbrack = pos_;
                next();
                exprLev_++;
                auto x = parseExpr();
                exprLev_--;
                if (auto name0 = dynamic_cast<IdentExpr*>(x.get());
                    name0 != nullptr && tok_ != token_type::COMMA &&
                    tok_ != token_type::RBRACK) {
                    // generic method m[T any]
                    //
                    // Interface methods do not have type parameters. We parse
                    // them for a better error message and improved error
                    // recovery.
                    parseParameterList(std::shared_ptr<IdentExpr>(name0),
                                       nullptr, token_type::RBRACK);
                    expect(token_type::RBRACK);
                    // TODO error()

                    auto [_, params] = parseParameters(false);
                    auto results = parseResult();
                    idents = {std::shared_ptr<IdentExpr>(ident)};
                    typ = std::make_shared<FuncTypeExpr>(NoPos, nullptr, params,
                                                         results);
                } else {
                    V<SP<Expr>> list = {x};
                    if (atComma("type argument list", token_type::RBRACK)) {
                        exprLev_++;
                        next();
                        while (tok_ != token_type::RBRACK &&
                               tok_ != token_type::EOF_) {
                            list.push_back(parseType());
                            if (!atComma("type argument list",
                                         token_type::RBRACK)) {
                                break;
                            }
                            next();
                        }
                        exprLev_--;
                    }
                    auto rbrack =
                        expectClosing(token_type::RBRACK, "type argument list");
                    typ = packIndexExpr(std::shared_ptr<IdentExpr>(ident),
                                        lbrack, list, rbrack);
                }
                break;
            }
            case token_type::LPAREN: {
                auto [_, params] = parseParameters(false);
                auto results = parseResult();
                idents = {std::shared_ptr<IdentExpr>(ident)};
                typ = std::make_shared<FuncTypeExpr>(NoPos, nullptr, params,
                                                     results);
                break;
            }
            default:
                typ = x;
                break;
        }
    } else {
        typ = x;
        if (tok_ == token_type::LBRACK) {
            typ = parseTypeInstance(typ);
        }
    }

    return std::make_shared<Field>(idents, typ, nullptr);
}

SP<Expr> parser::embeddedElem(SP<Expr> x) {
    if (x == nullptr) {
        x = embeddedTerm();
    }
    while (tok_ == token_type::OR) {
        auto t = BinaryExpr();
        t.OpPos = pos_;
        t.Op = token_type::OR;
        next();
        t.X = x;
        t.Y = embeddedTerm();
        x = std::make_shared<BinaryExpr>(t);
    }
    return x;
}

SP<Expr> parser::embeddedTerm() {
    if (tok_ == token_type::TILDE) {
        auto t = UnaryExpr();
        t.OpPos = pos_;
        t.Op = token_type::TILDE;
        next();
        t.X = parseType();
        return std::make_shared<UnaryExpr>(t);
    }

    auto t = tryIdentOrType();
    if (t == nullptr) {
        pos_t pos = pos_;
        // TODO errorExpected()
        advance(exprEnd);
        return std::make_shared<BadExpr>(pos, pos_);
    }

    return t;
}

SP<InterfaceTypeExpr> parser::parseInterfaceType() {
    pos_t pos = expect(token_type::INTERFACE);
    pos_t lbrace = expect(token_type::LBRACE);
    V<SP<Field>> list;
    while (true) {
        switch (tok_) {
            case token_type::IDENT: {
                auto f = parseMethodSpec();
                if (f->Names.empty()) {
                    f->Type = embeddedElem(f->Type);
                }
                expectSemi();
                list.push_back(f);
                break;
            }
            case token_type::TILDE: {
                auto typ = embeddedElem(nullptr);
                expectSemi();
                list.push_back(std::make_shared<Field>(nullptr, typ, nullptr));
                break;
            }
            default: {
                if (auto t = tryIdentOrType(); t != nullptr) {
                    auto typ = embeddedElem(t);
                    expectSemi();
                    list.push_back(
                        std::make_shared<Field>(nullptr, typ, nullptr));
                } else {
                    goto finish;
                }
            }
        }
    }
finish:
    pos_t rbrace = expect(token_type::RBRACE);
    return std::make_shared<InterfaceTypeExpr>(
        pos, std::make_shared<FieldList>(lbrace, list, rbrace));
}

SP<MapTypeExpr> parser::parseMapType() {
    pos_t pos = expect(token_type::MAP);
    expect(token_type::LBRACK);
    auto key = parseType();
    expect(token_type::RBRACK);
    auto value = parseType();
    return std::make_shared<MapTypeExpr>(pos, key, value);
}

SP<Expr> parser::parseTypeInstance(SP<Expr> typ) {
    pos_t opening = expect(token_type::LBRACK);
    exprLev_++;
    V<SP<Expr>> list;
    while (tok_ != token_type::RBRACK && tok_ != token_type::EOF_) {
        list.push_back(parseType());
        if (!atComma("type argument list", token_type::RBRACK)) {
            break;
        }
        next();
    }
    exprLev_--;

    pos_t closing = expectClosing(token_type::RBRACK, "type argument list");

    if (list.empty()) {
        // TODO errorEcpected()
        return std::make_shared<IndexExpr>(
            typ, opening,
            std::make_shared<BadExpr>(opening /*TODO + 1*/, closing), closing);
    }

    return packIndexExpr(typ, opening, list, closing);
}

SP<Expr> parser::tryIdentOrType() {
    incNestLev();
    SP<Expr> ret = nullptr;
    switch (tok_) {
        case token_type::IDENT: {
            auto typ = parseTypeName(nullptr);
            if (tok_ == token_type::LBRACK) {
                typ = parseTypeInstance(typ);
            }
            ret = typ;
            break;
        }
        case token_type::LBRACK: {
            pos_t lbrack = expect(token_type::LBRACK);
            ret = parseArrayType(lbrack, nullptr);
            break;
        }
        case token_type::STRUCT:
            ret = parseStructType();
            break;
        case token_type::MUL:
            ret = parsePointerType();
            break;
        case token_type::FUNC:
            ret = parseFuncType();
            break;
        case token_type::INTERFACE:
            ret = parseInterfaceType();
            break;
        case token_type::MAP:
            ret = parseMapType();
            break;
        // TODO case CHAN, ARROW
        case token_type::LPAREN: {
            pos_t lparen = pos_;
            next();
            auto typ = parseType();
            pos_t rparen = expect(token_type::RPAREN);
            ret = std::make_shared<ParenExpr>(lparen, typ, rparen);
            break;
        }
    }
    decNestLev();
    return ret;
}