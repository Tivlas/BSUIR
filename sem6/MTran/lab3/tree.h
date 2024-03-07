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

struct Node {
    virtual pos_t Pos() const = 0;  // first node char
};

struct Expr : Node {
    virtual void exprNode() const = 0;
};

struct Stmt : Node {
    virtual void stmtNode() const = 0;
};

struct Spec : Node {
    virtual void specNode() const = 0;
};

struct Decl : Node {
    virtual void declNode() const = 0;
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

    pos_t Pos() const override {
        if (!Names.empty()) return Names[0]->Pos();
        if (Type != nullptr) return Type->Pos();
        return NoPos;
    }
};

// A FieldList represents a list of Fields, enclosed by parentheses,
// curly braces, or square brackets.
struct FieldList : Node {
    pos_t Opening;      // position of opening parenthesis/brace/bracket, if any
    V<SP<Field>> List;  // field list; or nullptr
    pos_t Closing;      // position of closing parenthesis/brace/bracket, if any

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
};

// Expressions
struct BadExpr : Expr {
    pos_t From;
    pos_t To;

    pos_t Pos() const override { return From; }
};

struct IdentExpr : Expr {
    pos_t NamePos;  // identifier position
    std::string name;
    // TODO Obj *Object

    pos_t Pos() const override { return NamePos; }
};

struct EllipsisExpr : Expr {
    pos_t Ellipsis;  // position of "..."
    SP<Expr> Elt;    // ellipsis element type (parameter lists only); or nullptr

    pos_t Pos() const override { return Ellipsis; }
};

struct BasicLitExpr : Expr {
    pos_t ValuePos;
    token_type Kind;    // token.INT, token.FLOAT, token.IMAG, token.CHAR, or
                        // token.STRING
    std::string value;  // literal string; e.g. 42, 0x7f, 3.14, 1e-9, 2.4i, 'a'

    pos_t Pos() const override { return ValuePos; }
};

// TODO struct FuncLit ans methods

struct CompositeLitExpr : Expr {  // structs literals, slice literals, etc
    SP<Expr> Type;
    pos_t Lbrace;
    V<SP<Expr>> Elts;  // list of composite elements; or nullptr
    pos_t Rbrace;
    bool Incomplete;  // true if (source) expressions are missing in the Elts
                      // list

    pos_t Pos() const override {
        return Type != nullptr ? Type->Pos() : Lbrace;
    }
};

struct ParenExpr : Expr {
    pos_t Lparen;
    SP<Expr> X;
    pos_t Rparen;

    pos_t Pos() const override { return Lparen; }
};

struct SelectorExpr : Expr {
    SP<Expr> X;
    SP<IdentExpr> Sel;  // field selector

    pos_t Pos() const override { return X->Pos(); }
};

struct IndexExpr : Expr {
    SP<Expr> X;
    pos_t Lbrack;
    SP<Expr> Index;
    pos_t Rbrack;

    pos_t Pos() const override { return X->Pos(); }
};

struct IndexListExpr : Expr {
    SP<Expr> X;
    pos_t Lbrack;
    V<SP<Expr>> Indeces;
    pos_t Rbrack;

    pos_t Pos() const override { return X->Pos(); }
};

struct SliceExpr : Expr {
    SP<Expr> X;
    pos_t Lbrack;
    SP<Expr> Low;   // begin of slice range; or nullptr
    SP<Expr> High;  // end of slice range; or nullptr
    SP<Expr> Max;   // maximum capacity of slice; or nullptr
    bool Slice3;    // true if 3-index slice (2 colons present)
    pos_t Rbrack;

    pos_t Pos() const override { return X->Pos(); }
};

struct TypeAssertExpr : Expr {
    SP<Expr> X;
    pos_t Lparen;
    SP<Expr> Type;  // asserted type; nullptr means type switch X.(type)
    pos_t Rparen;

    pos_t Pos() const override { return X->Pos(); }
};

struct CallExpr : Expr {
    SP<Expr> Fun;
    pos_t Lparen;
    V<SP<Expr>> Args;  // function arguments; or nullptr
    pos_t Ellipsis;    // position of "..." (token.NoPos if there is no "...")
    pos_t Rparen;

    pos_t Pos() const override { return Fun->Pos(); }
};

struct StarExpr : Expr {
    pos_t Star;
    SP<Expr> X;  // operand (*ptr)

    pos_t Pos() const override { return Star; }
};

struct UnaryExpr : Expr {
    token_type Op;  // operator
    pos_t OpPos;    // position of Op
    SP<Expr> X;     // operand

    pos_t Pos() const override { return OpPos; }
};

struct BinaryExpr : Expr {
    token_type Op;  // operator
    pos_t OpPos;    // position of Op
    SP<Expr> X;     // left operand
    SP<Expr> Y;     // right operand

    pos_t Pos() const override { return X->Pos(); }
};

// A KeyValueExpr node represents (key : value) pairs
// in composite literals.
//
struct KeyValueExpr : Expr {
    pos_t Colon;  // position of ':'
    SP<Expr> Key;
    SP<Expr> Value;

    pos_t Pos() const override { return Key->Pos(); }
};

struct ArrayTypeExpr : Expr {
    pos_t Lbrack;
    SP<Expr> Len;  // ellipsis node for [...]T arrays, nullptr for slice
    SP<Expr> Elt;  // elem type

    pos_t Pos() const override { return Lbrack; }
};

struct StructTypeExpr : Expr {
    pos_t Struct;          // pos of struct keyword
    SP<FieldList> Fields;  // list of field declarations
    bool Incomplete;  // true if (source) fields are missing in the Fields list

    pos_t Pos() const override { return Struct; }
};

struct FuncTypeExpr : Expr {
    pos_t
        Func;  // position of "func" keyword (token.NoPos if there is no "func")
    SP<FieldList> TypeParams;  // type parameters; or nullptr
    SP<FieldList> Params;      // (incoming) parameters; non-nullptr
    SP<FieldList> Results;     // (outgoing) results; or nullptr

    pos_t Pos() const override {
        return (Func.IsValid() || Params == nullptr) ? Func : Params->Pos();
    }
};

struct InterfaceTypeExpr : Expr {
    pos_t Interface;        // pos of interface keyword
    SP<FieldList> Methods;  // list of embedded interfaces, methods, or types
    bool Incomplete;  // true if (source) methods or types are missing in the
                      // Methods list

    pos_t Pos() const override { return Interface; }
};

struct MapTypeExpr : Expr {
    pos_t Map;  // pos of map keyword
    SP<Expr> Key;
    SP<Expr> Value;

    pos_t Pos() const override { return Map; }
};
// end Expressions

// Statements
struct BadStmt : Stmt {
    pos_t From;
    pos_t To;
};

struct DeclStmt : Stmt {
    SP<Decl> Decl;
};

// TODO LabeledStmt

struct ExprStmt : Stmt {
    SP<Expr> X;
};

// TODO SendStmt (also need to support channels)

struct IncDecStms : Stmt {
    SP<Expr> X;
    pos_t TokPos;
    token_type Tok;  // inc or dec
};

struct AssignStmt : Stmt {
    V<SP<Expr>> Lhs;
    pos_t TokPos;
    token_type Tok;  // assignment token, DEFINE token
    V<SP<Expr>> Rhs;
};

// TODO GoStmt (also need to support channels)

struct DeferStmt : Stmt {
    pos_t Defer;  // pos of defer keyword
    SP<CallExpr> Call;
};

struct ReturnStmt : Stmt {
    pos_t Return;
    V<SP<Expr>> Results;
};

struct BranchStmt : Stmt {
    pos_t TokPos;
    token_type Tok;  // keyword token (BREAK, CONTINUE, GOTO, FALLTHROUGH)
    SP<IdentExpr> Label;
};

struct BlockStmt : Stmt {
    pos_t Lbrace;
    pos_t Rbrace;
    V<SP<Stmt>> List;
};

struct IfStmt : Stmt {
    pos_t If;
    SP<Stmt> Init;  // initialization statement; or nullptr
    SP<Expr> Cond;  // condition
    SP<BlockStmt> Body;
    SP<Stmt> Else;
};

struct CaseClauseStmt : Stmt {
    pos_t Case;  // position of "case" or "default" keyword
    V<SP<Expr>>
        List;  // list of expressions or types; nullptr means default case
    pos_t Colon;
    V<SP<Stmt>> Body;
};

struct SwitchStmt : Stmt {
    pos_t Switch;
    SP<Stmt> Init;  // initialization statement; or nullptr
    SP<Expr> Tag;
    SP<BlockStmt> Body;
};

// TODO CommClauseStmt

// TODO SelectClauseStmt (need channel support)

struct ForStmt : Stmt {
    pos_t For;
    SP<Stmt> Init;
    SP<Expr> Cond;
    SP<Stmt> Post;  // post iteration statement; or nullptr
    SP<BlockStmt> Body;
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
};

// A ValueSpec node represents a constant or variable declaration
// (ConstSpec or VarSpec production).
//
struct ValueSpec : Spec {
    V<SP<IdentExpr>> Names;  // value names Names.size() > 0)
    SP<Expr> Type;           // value type; or nullptr
    V<SP<Expr>> Values;      // initial values; or nullptr
};

// A TypeSpec node represents a type declaration (TypeSpec production).
struct TypeSpec : Spec {
    SP<IdentExpr> Name;
    SP<FieldList> TypeParams;
    pos_t Assign;   // position of '=', if any
    SP<Expr> Type;  // IdentExpr, ParenExpr, SelectorExpr, StarExpr, or any of
                    // the XxxTypes
};

// end Spec

// Declarations

struct BadDecl : Decl {
    pos_t From;
    pos_t To;
};

struct GenDecl : Decl {
    pos_t TokPos;
    token_type Tok;  // IMPORT, CONST, TYPE, or VAR
    pos_t Lparen;
    pos_t Rparen;
    V<SP<Spec>> Specs;
};

struct FuncDecl : Decl {
    SP<FieldList> Recv;  // receiver (methods); or nullptr (functions)
    SP<IdentExpr> Name;
    SP<FuncTypeExpr> Type;  // function signature: type and value parameters,
                            // results, and position of "func" keyword
    SP<BlockStmt> Body;
};

// end Declarations