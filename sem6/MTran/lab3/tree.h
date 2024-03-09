#pragma once
#include <memory>
#include <vector>

#include "token.h"

template <typename T>
using V = std::vector<T>;

template <typename T>
using SP = std::shared_ptr<T>;

using pos_t = Token::token_position;

const pos_t NoPos{};

// TODO implement Pos method for each type

template <class LHS, class RHS>
bool compare(const LHS& lhs, const RHS& rhs) {
    try {
        auto& casted = dynamic_cast<const LHS&>(rhs);
        return lhs == casted;
    } catch (std::bad_cast& error) {
        return false;
    }
}

struct Node {
    virtual pos_t Pos() const = 0;  // first node char
    virtual pos_t End() const = 0;  // first node char

    virtual bool operator==(const Node& rhs) const {
        return compare(*this, rhs);
    }
};

struct Expr : Node {
    // virtual void exprNode() const = 0;
};

struct Stmt : Node {
    // virtual void stmtNode() const = 0;
};

struct Spec : Node {
    // virtual void specNode() const = 0;
};

struct Decl : Node {
    // virtual void declNode() const = 0;
};

// Expressions
struct BadExpr : Expr {
    pos_t From;
    pos_t To;

    BadExpr(pos_t from, pos_t to) : From(from), To(to) {}
    pos_t Pos() const override { return From; }

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct IdentExpr : Expr {
    pos_t NamePos;  // identifier position
    std::string Name;
    // TODO Obj *Object

    IdentExpr(pos_t pos, std::string name) : NamePos(pos), Name(name) {}

    pos_t Pos() const override { return NamePos; }

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct EllipsisExpr : Expr {
    pos_t Ellipsis;  // position of "..."
    SP<Expr> Elt;    // ellipsis element type (parameter lists only); or nullptr

    EllipsisExpr() {}
    EllipsisExpr(pos_t pos) : Ellipsis(pos) {}
    pos_t Pos() const override { return Ellipsis; }

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct BasicLitExpr : Expr {
    pos_t ValuePos;
    token_type Kind;    // token.INT, token.FLOAT, token.IMAG, token.CHAR, or
                        // token.STRING
    std::string Value;  // literal string; e.g. 42, 0x7f, 3.14, 1e-9, 2.4i, 'a'

    BasicLitExpr(pos_t pos, token_type kind, std::string value)
        : ValuePos(pos), Kind(kind), Value(value) {}
    pos_t Pos() const override { return ValuePos; }

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct FuncLitExpr : Expr {
    SP<FuncTypeExpr> Type;
    SP<BlockStmt> Body;

    FuncLitExpr(SP<FuncTypeExpr> t, SP<BlockStmt> b) : Type(t), Body(b) {}

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct CompositeLitExpr : Expr {  // structs literals, slice literals, etc
    SP<Expr> Type;
    pos_t Lbrace;
    V<SP<Expr>> Elts;  // list of composite elements; or nullptr
    pos_t Rbrace;
    bool Incomplete;  // true if (source) expressions are missing in the Elts
                      // list

    CompositeLitExpr(SP<Expr> typ, pos_t l, V<SP<Expr>> elts, pos_t r)
        : Type(typ), Lbrace(l), Elts(elts), Rbrace(r) {}
    pos_t Pos() const override {
        return Type != nullptr ? Type->Pos() : Lbrace;
    }

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct ParenExpr : Expr {
    pos_t Lparen;
    SP<Expr> X;
    pos_t Rparen;

    ParenExpr(pos_t l, SP<Expr> x, pos_t r) : Lparen(l), X(x), Rparen(r) {}
    pos_t Pos() const override { return Lparen; }

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct SelectorExpr : Expr {
    SP<Expr> X;
    SP<IdentExpr> Sel;  // field selector

    SelectorExpr() {}
    SelectorExpr(SP<Expr> x, SP<IdentExpr> sel) : X(x), Sel(sel) {}
    pos_t Pos() const override { return X->Pos(); }

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct IndexExpr : Expr {
    SP<Expr> X;
    pos_t Lbrack;
    SP<Expr> Index;
    pos_t Rbrack;

    IndexExpr(SP<Expr> x, pos_t l, SP<Expr> idx, pos_t r)
        : X(x), Lbrack(l), Index(idx), Rbrack(r) {}

    pos_t Pos() const override { return X->Pos(); }

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct IndexListExpr : Expr {
    SP<Expr> X;
    pos_t Lbrack;
    V<SP<Expr>> Indeces;
    pos_t Rbrack;

    pos_t Pos() const override { return X->Pos(); }

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct SliceExpr : Expr {
    SP<Expr> X;
    pos_t Lbrack;
    SP<Expr> Low;   // begin of slice range; or nullptr
    SP<Expr> High;  // end of slice range; or nullptr
    SP<Expr> Max;   // maximum capacity of slice; or nullptr
    bool Slice3;    // true if 3-index slice (2 colons present)
    pos_t Rbrack;

    SliceExpr(SP<Expr> x, pos_t lb, SP<Expr> low, SP<Expr> high, SP<Expr> max,
              bool slice3, pos_t rb)
        : X(x),
          Lbrack(lb),
          Low(low),
          High(high),
          Max(max),
          Slice3(slice3),
          Rbrack(rb) {}
    pos_t Pos() const override { return X->Pos(); }

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct TypeAssertExpr : Expr {
    SP<Expr> X;
    SP<Expr> Type;  // asserted type; nullptr means type switch X.(type)
    pos_t Lparen;
    pos_t Rparen;

    TypeAssertExpr(SP<Expr> x, SP<Expr> typ, pos_t l, pos_t r)
        : X(x), Type(typ), Lparen(l), Rparen(r) {}
    pos_t Pos() const override { return X->Pos(); }

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct CallExpr : Expr {
    SP<Expr> Fun;
    pos_t Lparen;
    V<SP<Expr>> Args;  // function arguments; or nullptr
    pos_t Ellipsis;    // position of "..." (token.NoPos if there is no "...")
    pos_t Rparen;

    CallExpr(SP<Expr> fun, pos_t l, V<SP<Expr>> args, pos_t ellipsis, pos_t r)
        : Fun(fun), Lparen(l), Args(args), Ellipsis(ellipsis), Rparen(r) {}
    pos_t Pos() const override { return Fun->Pos(); }

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct StarExpr : Expr {
    pos_t Star;
    SP<Expr> X;  // operand (*ptr)

    StarExpr(pos_t pos, SP<Expr> x) : Star(pos), X(x) {}
    pos_t Pos() const override { return Star; }

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct UnaryExpr : Expr {
    pos_t OpPos;    // position of Op
    token_type Op;  // operator
    SP<Expr> X;     // operand

    UnaryExpr() {}
    UnaryExpr(pos_t pos, token_type op, SP<Expr> x)
        : OpPos(pos), Op(op), X(x) {}
    pos_t Pos() const override { return OpPos; }

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct BinaryExpr : Expr {
    SP<Expr> X;     // left operand
    pos_t OpPos;    // position of Op
    token_type Op;  // operator
    SP<Expr> Y;     // right operand

    BinaryExpr() {}
    BinaryExpr(SP<Expr> x, pos_t pos, token_type op, SP<Expr> y)
        : X(x), OpPos(pos), Op(op), Y(y) {}
    pos_t Pos() const override { return X->Pos(); }

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

// A KeyValueExpr node represents (key : value) pairs
// in composite literals.
//
struct KeyValueExpr : Expr {
    SP<Expr> Key;
    pos_t Colon;  // position of ':'
    SP<Expr> Value;

    KeyValueExpr(SP<Expr> k, pos_t colon, SP<Expr> v)
        : Key(k), Colon(colon), Value(v) {}
    pos_t Pos() const override { return Key->Pos(); }

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct ArrayTypeExpr : Expr {
    pos_t Lbrack;
    SP<Expr> Len;  // ellipsis node for [...]T arrays, nullptr for slice
    SP<Expr> Elt;  // elem type

    ArrayTypeExpr() {}
    ArrayTypeExpr(pos_t lbrack, SP<Expr> len, SP<Expr> elt)
        : Lbrack(lbrack), Len(len), Elt(elt) {}

    pos_t Pos() const override { return Lbrack; }

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

// A Field represents a Field declaration list in a struct type,
// a method list in an interface type, or a parameter/result declaration
// in a signature.
// Field.Names is nullptr for unnamed parameters (parameter lists which only
// contain types) and embedded struct fields. In the latter case, the field name
// is the type name.
struct Field : Node {
    V<SP<IdentExpr>> Names;  // field/method/(type) parameter names; or nullptr
    SP<Expr> Type;           // field/method/parameter type; or nullptr
    SP<BasicLitExpr> Tag;    // field tag; or nullptr

    Field(V<SP<IdentExpr>> names, SP<Expr> typ, SP<BasicLitExpr> tag)
        : Names(names), Type(typ), Tag(tag) {}

    pos_t Pos() const override {
        if (!Names.empty()) return Names[0]->Pos();
        if (Type != nullptr) return Type->Pos();
        return NoPos;
    }

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

// A FieldList represents a list of Fields, enclosed by parentheses,
// curly braces, or square brackets.
struct FieldList : Node {
    pos_t Opening;      // position of opening parenthesis/brace/bracket, if any
    V<SP<Field>> List;  // field list; or nullptr
    pos_t Closing;      // position of closing parenthesis/brace/bracket, if any

    FieldList(pos_t opening, V<SP<Field>> list, pos_t closing)
        : Opening(opening), List(list), Closing(closing) {}

    pos_t Pos() const override {
        if (Opening.IsValid()) return Opening;
        if (!List.empty()) return List[0]->Pos();
        return NoPos;
    }

    int NumFields() const {
        int n = 0;
        for (const auto& field : List) {
            int m = field->Names.size();
            if (m == 0) m = 1;
            n += m;
        }
        return n;
    }

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct StructTypeExpr : Expr {
    pos_t Struct;          // pos of struct keyword
    SP<FieldList> Fields;  // list of field declarations
    bool Incomplete;  // true if (source) fields are missing in the Fields list

    StructTypeExpr(pos_t pos, SP<FieldList> fields)
        : Struct(pos), Fields(fields) {}
    pos_t Pos() const override { return Struct; }

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct FuncTypeExpr : Expr {
    pos_t
        Func;  // position of "func" keyword (token.NoPos if there is no "func")
    SP<FieldList> TypeParams;  // type parameters; or nullptr
    SP<FieldList> Params;      // (incoming) parameters; non-nullptr
    SP<FieldList> Results;     // (outgoing) results; or nullptr

    FuncTypeExpr(pos_t pos, SP<FieldList> tps, SP<FieldList> ps,
                 SP<FieldList> rs)
        : Func(pos), TypeParams(tps), Params(ps), Results(rs) {}

    pos_t Pos() const override {
        return (Func.IsValid() || Params == nullptr) ? Func : Params->Pos();
    }

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct InterfaceTypeExpr : Expr {
    pos_t Interface;        // pos of interface keyword
    SP<FieldList> Methods;  // list of embedded interfaces, methods, or types
    bool Incomplete;  // true if (source) methods or types are missing in the
                      // Methods list

    InterfaceTypeExpr(pos_t pos, SP<FieldList> ms)
        : Interface(pos), Methods(ms) {}

    pos_t Pos() const override { return Interface; }

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct MapTypeExpr : Expr {
    pos_t Map;  // pos of map keyword
    SP<Expr> Key;
    SP<Expr> Value;

    MapTypeExpr(pos_t pos, SP<Expr> key, SP<Expr> value)
        : Map(pos), Key(key), Value(value) {}
    pos_t Pos() const override { return Map; }

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};
// end Expressions

// Statements
struct BadStmt : Stmt {
    pos_t From;
    pos_t To;

    BadStmt(pos_t from, pos_t to) : From(from), To(to) {}
    pos_t Pos() const override { return From; }

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct DeclStmt : Stmt {
    SP<Decl> Decl;

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct LabeledStmt : Stmt {
    SP<IdentExpr> Label;
    pos_t Colon;
    SP<Stmt> Stmt_;

    LabeledStmt(SP<IdentExpr> lbl, pos_t pos, SP<Stmt> stmt)
        : Label(lbl), Colon(pos), Stmt_(stmt) {}

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct ExprStmt : Stmt {
    SP<Expr> X;

    ExprStmt(SP<Expr> x) : X(x) {}

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

// TODO SendStmt (also need to support channels)

struct IncDecStmt : Stmt {
    SP<Expr> X;
    pos_t TokPos;
    token_type Tok;  // inc or dec

    IncDecStmt(SP<Expr> x, pos_t pos, token_type tok)
        : X(x), TokPos(pos), Tok(tok) {}

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct AssignStmt : Stmt {
    V<SP<Expr>> Lhs;
    pos_t TokPos;
    token_type Tok;  // assignment token, DEFINE token
    V<SP<Expr>> Rhs;

    AssignStmt(V<SP<Expr>> lhs, pos_t pos, token_type tok, V<SP<Expr>> rhs)
        : Lhs(lhs), TokPos(pos), Tok(tok), Rhs(rhs) {}

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

// TODO GoStmt (also need to support channels)

struct DeferStmt : Stmt {
    pos_t Defer;  // pos of defer keyword
    SP<CallExpr> Call;

    DeferStmt(pos_t pos, SP<CallExpr> call) : Defer(pos), Call(call) {}
    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct ReturnStmt : Stmt {
    pos_t Return;
    V<SP<Expr>> Results;

    ReturnStmt(pos_t pos, V<SP<Expr>> res) : Return(pos), Results(res) {}
    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct BranchStmt : Stmt {
    pos_t TokPos;
    token_type Tok;  // keyword token (BREAK, CONTINUE, GOTO, FALLTHROUGH)
    SP<IdentExpr> Label;

    BranchStmt(pos_t pos, token_type tok, SP<IdentExpr> lbl)
        : TokPos(pos), Tok(tok), Label(lbl) {}
    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct BlockStmt : Stmt {
    pos_t Lbrace;
    V<SP<Stmt>> List;
    pos_t Rbrace;

    BlockStmt(pos_t l, V<SP<Stmt>> list, pos_t r)
        : Lbrace(l), List(list), Rbrace(r) {}

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct IfStmt : Stmt {
    pos_t If;
    SP<Stmt> Init;  // initialization statement; or nullptr
    SP<Expr> Cond;  // condition
    SP<BlockStmt> Body;
    SP<Stmt> Else;

    IfStmt(pos_t pos, SP<Stmt> init, SP<Expr> cond, SP<BlockStmt> body,
           SP<Stmt> els)
        : If(pos), Init(init), Cond(cond), Body(body), Else(els) {}
    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct CaseClauseStmt : Stmt {
    pos_t Case;  // position of "case" or "default" keyword
    V<SP<Expr>>
        List;  // list of expressions or types; nullptr means default case
    pos_t Colon;
    V<SP<Stmt>> Body;

    CaseClauseStmt(pos_t pos, V<SP<Expr>> list, pos_t colon, V<SP<Stmt>> body)
        : Case(pos), List(list), Colon(colon), Body(body) {}

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct SwitchStmt : Stmt {
    pos_t Switch;
    SP<Stmt> Init;  // initialization statement; or nullptr
    SP<Expr> Tag;
    SP<BlockStmt> Body;

    SwitchStmt(pos_t pos, SP<Stmt> init, SP<Expr> tag, SP<BlockStmt> body)
        : Switch(pos), Init(init), Tag(tag), Body(body) {}
    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct TypeSwitchStmt : Stmt {
    pos_t Switch;
    SP<Stmt> Init;       // initialization statement; or nullptr
    SP<Stmt> Assign;     // x := y.(type) or y.(type)
    SP<BlockStmt> Body;  // CaseClauses only

    TypeSwitchStmt(pos_t pos, SP<Stmt> init, SP<Stmt> assign,
                   SP<BlockStmt> body)
        : Switch(pos), Init(init), Assign(assign), Body(body) {}

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

// TODO CommClauseStmt

// TODO SelectClauseStmt (need channel support)

struct ForStmt : Stmt {
    pos_t For;
    SP<Stmt> Init;
    SP<Expr> Cond;
    SP<Stmt> Post;  // post iteration statement; or nullptr
    SP<BlockStmt> Body;

    ForStmt(pos_t forPos, SP<Stmt> init, SP<Expr> cond, SP<Stmt> post,
            SP<BlockStmt> body)
        : For(forPos), Init(init), Cond(cond), Post(post), Body(body) {}

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct RangeStmt : Stmt {
    pos_t For;
    SP<Expr> Key;  // Key, Value may be nullptr
    SP<Expr> Value;
    pos_t TokPos;    // position of Tok; invalid if Key == nullptr
    token_type Tok;  // ILLEGAL if Key == nullptr, ASSIGN, DEFINE
    pos_t Range;
    SP<Expr> X;
    SP<BlockStmt> Body;

    RangeStmt(pos_t forPos, SP<Expr> key, SP<Expr> value, pos_t tokPos,
              token_type tok, pos_t rangePos, SP<Expr> x, SP<BlockStmt> body)
        : For(forPos),
          Key(key),
          Value(value),
          TokPos(tokPos),
          Tok(tok),
          Range(rangePos),
          X(x),
          Body(body) {}

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct EmptyStmt : Stmt {
    pos_t Semicolon;
    bool Implicit;

    EmptyStmt(pos_t sc, bool implicit) : Semicolon(sc), Implicit(implicit) {}

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

// end Statements

// Spec
// A Spec node represents a single (non-parenthesized) import,
// constant, type, or variable declaration.

// An ImportSpec node represents a single package import.
struct ImportSpec : Spec {
    SP<IdentExpr> Name;     // local package name (including "."); or nullptr
    SP<BasicLitExpr> Path;  // import path
    pos_t EndPos;

    ImportSpec(SP<IdentExpr> name, SP<BasicLitExpr> path, pos_t endPos)
        : Name(name), Path(path), EndPos(endPos) {}

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

// A ValueSpec node represents a constant or variable declaration
// (ConstSpec or VarSpec production).
//
struct ValueSpec : Spec {
    V<SP<IdentExpr>> Names;  // value names Names.size() > 0)
    SP<Expr> Type;           // value type; or nullptr
    V<SP<Expr>> Values;      // initial values; or nullptr

    ValueSpec(V<SP<IdentExpr>> names, SP<Expr> type, V<SP<Expr>> values)
        : Names(names), Type(type), Values(values) {}

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

// A TypeSpec node represents a type declaration (TypeSpec production).
struct TypeSpec : Spec {
    SP<IdentExpr> Name;
    SP<FieldList> TypeParams;
    pos_t Assign;   // position of '=', if any
    SP<Expr> Type;  // IdentExpr, ParenExpr, SelectorExpr, StarExpr, or any of
    // the XxxTypes

    TypeSpec(SP<IdentExpr> name, SP<FieldList> typeParams, pos_t assign,
             SP<Expr> type)
        : Name(name), TypeParams(typeParams), Assign(assign), Type(type) {}

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

// end Spec

// Declarations

struct BadDecl : Decl {
    pos_t From;
    pos_t To;

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct GenDecl : Decl {
    pos_t TokPos;
    token_type Tok;  // IMPORT, CONST, TYPE, or VAR
    pos_t Lparen;
    pos_t Rparen;
    V<SP<Spec>> Specs;

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct FuncDecl : Decl {
    SP<FieldList> Recv;  // receiver (methods); or nullptr (functions)
    SP<IdentExpr> Name;
    SP<FuncTypeExpr> Type;  // function signature: type and value parameters,
                            // results, and position of "func" keyword
    SP<BlockStmt> Body;

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

// end Declarations