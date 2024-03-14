#pragma once
#include <concepts>
#include <memory>
#include <vector>

#include "token.h"

template <typename T>
using V = std::vector<T>;

template <typename T>
using SP = std::shared_ptr<T>;

using pos_t = Token::token_position;

pos_t operator+(const pos_t& pos, int n) {
    auto tmp = pos;
    tmp.col += n;
    return tmp;
}

const pos_t NoPos = pos_t();

template <class LHS, class RHS>
bool sameType(const LHS& lhs, const RHS& rhs) {
    try {
        auto& casted = dynamic_cast<const LHS&>(rhs);
        return true;
    } catch (std::bad_cast& error) {
        return false;
    }
}

const char shiftChar = ' ';

struct Node {
    virtual pos_t Pos() const = 0;  // first node char
    virtual pos_t End() const = 0;  // after
    virtual std::string Print(size_t shift) const = 0;

    virtual bool operator==(const Node& rhs) const = 0;
};

bool onlyOneNull(const SP<Node>& a, const SP<Node>& b) {
    return (a == nullptr && b != nullptr) || (a != nullptr && b == nullptr);
}

bool bothNull(const SP<Node>& a, const SP<Node>& b) { return a == nullptr && b == nullptr; }

bool equal(const SP<Node>& a, const SP<Node>& b) {
    return !onlyOneNull(a, b) && (bothNull(a, b) || *a == *b);
}

template <typename T>
concept DerivedFromNode = std::derived_from<T, Node>;

template <DerivedFromNode T>
bool equal(const V<SP<T>>& a, const V<SP<T>>& b) {
    if (a.size() != b.size()) {
        return false;
    }
    for (size_t i = 0; i < a.size(); i++) {
        if (!equal(a[i], b[i])) {
            return false;
        }
    }
    return true;
}

struct Expr : Node {};

struct Stmt : Node {};

struct Spec : Node {};

struct Decl : Node {};

// Expressions
struct BadExpr : Expr {
    pos_t From;
    pos_t To;

    BadExpr(pos_t from, pos_t to) : From(from), To(to) {}

    pos_t Pos() const override { return From; }

    pos_t End() const override { return To; }

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const BadExpr&>(rhs);
            return From == casted.From && To == casted.To;
        }
        return false;
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += color::error + "BadExpr " + color::reset + "\n";
        return res;
    }
};

struct IdentExpr : Expr {
    pos_t NamePos;  // identifier position
    std::string Name;
    // TODO *Object ??? to store additional info

    IdentExpr(pos_t pos, std::string name) : NamePos(pos), Name(name) {}

    pos_t Pos() const override { return NamePos; }

    pos_t End() const override { return NamePos + Name.size(); }

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const IdentExpr&>(rhs);
            return NamePos == casted.NamePos && Name == casted.Name;
        }
        return false;
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "IdentExpr: " + Name + '\n';
        return res;
    }
};

struct EllipsisExpr : Expr {
    pos_t Ellipsis = NoPos;  // position of "..."
    SP<Expr> Elt;            // ellipsis element type (parameter lists only); or nullptr

    EllipsisExpr() {}
    EllipsisExpr(pos_t pos) : EllipsisExpr(pos, nullptr) {}
    EllipsisExpr(pos_t pos, SP<Expr> elt) : Elt(nullptr), Ellipsis(pos) {}

    pos_t Pos() const override { return Ellipsis; }

    pos_t End() const override {
        if (Elt != nullptr) {
            return Elt->Pos();
        }
        return Ellipsis + 3;
    }

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const EllipsisExpr&>(rhs);
            return Ellipsis == casted.Ellipsis && equal(Elt, casted.Elt);
        }
        return false;
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "Ellipsis:\n";
        if (Elt != nullptr) {
            res += Elt->Print(shiftSize + 4);
        }
        return res;
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

    pos_t End() const override { return ValuePos + Value.size(); }

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const BasicLitExpr&>(rhs);
            return ValuePos == casted.ValuePos && Kind == casted.Kind && Value == casted.Value;
        }
        return false;
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "BasicLitExpr: ";
        res += Value + '\n';
        return res;
    }
};

// A Field represents a Field declaration list in a struct type,
// a method list in an interface type, or a parameter/result declaration
// in a signature.
// Field.Names is nullptr for unnamed parameters (parameter lists which only
// contain types) and embedded struct fields. In the latter case, the field
// name is the type name.
struct Field : Node {
    V<SP<IdentExpr>> Names;  // field/method/(type) parameter names; or nullptr
    SP<Expr> Type;           // field/method/parameter type; or nullptr
    SP<BasicLitExpr> Tag;    // field tag; or nullptr

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const Field&>(rhs);
            return equal(Names, casted.Names) && equal(Type, casted.Type) && equal(Tag, casted.Tag);
        }
        return false;
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "Field:\n";
        if (!Names.empty()) {
            for (const auto& name : Names) {
                if (name) res += name->Print(shiftSize + 4);
            }
        }
        if (Type != nullptr) {
            res += Type->Print(shiftSize + 4);
        }
        if (Tag != nullptr) {
            res += Tag->Print(shiftSize + 4);
        }
        return res;
    }

    Field(const V<SP<IdentExpr>>& names, SP<Expr> typ, SP<BasicLitExpr> tag)
        : Names(names), Type(typ), Tag(tag) {}

    pos_t Pos() const override {
        if (!Names.empty()) return Names[0]->Pos();
        if (Type != nullptr) return Type->Pos();
        return NoPos;
    }

    pos_t End() const override {
        if (Tag != nullptr) {
            return Tag->End();
        }
        if (Type != nullptr) {
            return Type->End();
        }
        if (Names.size() > 0) {
            return Names.back()->End();
        }
        return NoPos;
    }
};

// A FieldList represents a list of Fields, enclosed by parentheses,
// curly braces, or square brackets.
struct FieldList : Node {
    pos_t Opening;      // position of opening parenthesis/brace/bracket, if any
    V<SP<Field>> List;  // field list; or nullptr
    pos_t Closing;      // position of closing parenthesis/brace/bracket, if any

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const FieldList&>(rhs);
            return Opening == casted.Opening && Closing == casted.Closing &&
                   equal(List, casted.List);
        }
        return false;
    }

    FieldList(pos_t opening, const V<SP<Field>>& list, pos_t closing)
        : Opening(opening), List(list), Closing(closing) {}

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "FieldList:\n";
        if (!List.empty()) {
            for (const auto& field : List) {
                if (field) res += field->Print(shiftSize + 4);
            }
        }
        return res;
    }

    pos_t Pos() const override {
        if (Opening.IsValid()) return Opening;
        if (!List.empty()) return List[0]->Pos();
        return NoPos;
    }

    pos_t End() const override {
        if (Closing.IsValid()) {
            return Closing + 1;
        }
        if (List.size() > 0) {
            return List.back()->End();
        }
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

struct FuncTypeExpr : Expr {
    pos_t Func;                // position of "func" keyword (token.NoPos if there is no
                               // "func")
    SP<FieldList> TypeParams;  // type parameters; or nullptr
    SP<FieldList> Params;      // (incoming) parameters; non-nullptr
    SP<FieldList> Results;     // (outgoing) results; or nullptr

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const FuncTypeExpr&>(rhs);
            return Func == casted.Func && equal(TypeParams, casted.TypeParams) &&
                   equal(Params, casted.Params) && equal(Results, casted.Results);
        }
        return false;
    }

    FuncTypeExpr(pos_t pos, SP<FieldList> tps, SP<FieldList> ps, SP<FieldList> rs)
        : Func(pos), TypeParams(tps), Params(ps), Results(rs) {}

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "FuncTypeExpr:\n";
        if (TypeParams) {
            res += TypeParams->Print(shiftSize + 4);
        }
        if (Params) {
            res += Params->Print(shiftSize + 4);
        }
        if (Results) {
            res += Results->Print(shiftSize + 4);
        }
        return res;
    }

    pos_t Pos() const override {
        return (Func.IsValid() || Params == nullptr) ? Func : Params->Pos();
    }

    pos_t End() const override {
        if (Results != nullptr) {
            return Results->End();
        }
        return Params->End();
    }
};

struct BlockStmt : Stmt {
    pos_t Lbrace;
    V<SP<Stmt>> List;
    pos_t Rbrace;

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "BlockStmt:\n";
        if (!List.empty()) {
            for (const auto& stmt : List) {
                if (stmt) res += stmt->Print(shiftSize + 4);
            }
        }
        return res;
    }

    BlockStmt(pos_t l, const V<SP<Stmt>>& list, pos_t r) : Lbrace(l), List(list), Rbrace(r) {}

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const BlockStmt&>(rhs);
            return Lbrace == casted.Lbrace && Rbrace == casted.Lbrace && equal(List, casted.List);
        }
        return false;
    }

    pos_t Pos() const override { return Lbrace; }

    pos_t End() const override {
        if (Rbrace.IsValid()) {
            return Rbrace + 1;
        }
        if (List.size() > 0) {
            return List.back()->End();
        }
        return Lbrace + 1;
    }
};
struct FuncLitExpr : Expr {
    SP<FuncTypeExpr> Type;
    SP<BlockStmt> Body;

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const FuncLitExpr&>(rhs);
            return equal(Type, casted.Type) && equal(Body, casted.Body);
        }
        return false;
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "FuncLitExpr:\n";
        if (Type != nullptr) {
            res += Type->Print(shiftSize + 4);
        }
        if (Body != nullptr) {
            res += Body->Print(shiftSize + 4);
        }
        return res;
    }

    FuncLitExpr(SP<FuncTypeExpr> t, SP<BlockStmt> b) : Type(t), Body(b) {}

    pos_t Pos() const override { return Type->Pos(); }

    pos_t End() const override { return Body->End(); }
};

struct CompositeLitExpr : Expr {  // structs literals, slice literals, etc
    SP<Expr> Type;
    pos_t Lbrace;
    V<SP<Expr>> Elts;  // list of composite elements; or nullptr
    pos_t Rbrace;
    bool Incomplete;  // true if (source) expressions are missing in the
                      // Elts list

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const CompositeLitExpr&>(rhs);
            return Lbrace == casted.Lbrace && Incomplete == casted.Incomplete &&
                   Rbrace == casted.Rbrace && equal(Type, casted.Type) && equal(Elts, casted.Elts);
        }
        return false;
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "CompositeLitExpr:\n";
        if (Type != nullptr) {
            res += Type->Print(shiftSize + 4);
        }
        if (!Elts.empty()) {
            for (const auto& elt : Elts) {
                if (elt) res += elt->Print(shiftSize + 4);
            }
        }
        return res;
    }

    CompositeLitExpr(SP<Expr> typ, pos_t l, const V<SP<Expr>>& elts, pos_t r)
        : Type(typ), Lbrace(l), Elts(elts), Rbrace(r) {}

    pos_t Pos() const override { return Type != nullptr ? Type->Pos() : Lbrace; }

    pos_t End() const override { return Rbrace + 1; }
};

struct ParenExpr : Expr {
    pos_t Lparen;
    SP<Expr> X;
    pos_t Rparen;

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const ParenExpr&>(rhs);
            return Lparen == casted.Lparen && Rparen == casted.Rparen && equal(X, casted.X);
        }
        return false;
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "ParenExpr:\n";
        if (X != nullptr) {
            res += X->Print(shiftSize + 4);
        }
        return res;
    }

    ParenExpr(pos_t l, SP<Expr> x, pos_t r) : Lparen(l), X(x), Rparen(r) {}

    pos_t Pos() const override { return Lparen; }

    pos_t End() const override { return Rparen + 1; }
};

struct SelectorExpr : Expr {
    SP<Expr> X;
    SP<IdentExpr> Sel;  // field selector

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const SelectorExpr&>(rhs);
            return equal(X, casted.X) && equal(Sel, casted.Sel);
        }
        return false;
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "SelectorExpr:\n";
        if (X != nullptr) {
            res += X->Print(shiftSize + 4);
        }
        if (Sel != nullptr) {
            res += Sel->Print(shiftSize + 4);
        }
        return res;
    }

    SelectorExpr() {}
    SelectorExpr(SP<Expr> x, SP<IdentExpr> sel) : X(x), Sel(sel) {}

    pos_t Pos() const override { return X->Pos(); }

    pos_t End() const override { return Sel->End(); }
};

struct IndexExpr : Expr {
    SP<Expr> X;
    pos_t Lbrack;
    SP<Expr> Index;
    pos_t Rbrack;

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const IndexExpr&>(rhs);
            return Lbrack == casted.Lbrack && Rbrack == casted.Rbrack && equal(X, casted.X) &&
                   equal(Index, casted.Index);
        }
        return false;
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "IndexExpr:\n";
        if (X != nullptr) {
            res += X->Print(shiftSize + 4);
        }
        if (Index != nullptr) {
            res += Index->Print(shiftSize + 4);
        }
        return res;
    }

    IndexExpr(SP<Expr> x, pos_t l, SP<Expr> idx, pos_t r)
        : X(x), Lbrack(l), Index(idx), Rbrack(r) {}

    pos_t Pos() const override { return X->Pos(); }

    pos_t End() const override { return Rbrack + 1; }
};

struct IndexListExpr : Expr {
    SP<Expr> X;
    pos_t Lbrack;
    V<SP<Expr>> Indeces;
    pos_t Rbrack;

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const IndexListExpr&>(rhs);
            return Lbrack == casted.Lbrack && Rbrack == casted.Rbrack && equal(X, casted.X) &&
                   equal(Indeces, casted.Indeces);
        }
        return false;
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "IndexListExpr:\n";
        if (X != nullptr) {
            res += X->Print(shiftSize + 4);
        }
        if (!Indeces.empty()) {
            for (const auto& idx : Indeces) {
                if (idx) res += idx->Print(shiftSize + 4);
            }
        }
        return res;
    }

    IndexListExpr(SP<Expr> x, pos_t lbrack, const V<SP<Expr>>& indeces, pos_t rbrack)
        : X(x), Lbrack(lbrack), Indeces(indeces), Rbrack(rbrack) {}

    pos_t Pos() const override { return X->Pos(); }

    pos_t End() const override { return Rbrack + 1; }
};

struct SliceExpr : Expr {
    SP<Expr> X;
    pos_t Lbrack;
    SP<Expr> Low;   // begin of slice range; or nullptr
    SP<Expr> High;  // end of slice range; or nullptr
    SP<Expr> Max;   // maximum capacity of slice; or nullptr
    bool Slice3;    // true if 3-index slice (2 colons present)
    pos_t Rbrack;

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const SliceExpr&>(rhs);
            return Lbrack == casted.Lbrack && Rbrack == casted.Rbrack && Slice3 == casted.Slice3 &&
                   equal(X, casted.X) && equal(Low, casted.Low) && equal(High, casted.High) &&
                   equal(Max, casted.Max);
        }
        return false;
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "SliceExpr:\n";
        if (X != nullptr) {
            res += X->Print(shiftSize + 4);
        }
        if (Low != nullptr) {
            res += Low->Print(shiftSize + 4);
        }
        if (High != nullptr) {
            res += High->Print(shiftSize + 4);
        }
        if (Max != nullptr) {
            res += Max->Print(shiftSize + 4);
        }
        return res;
    }

    SliceExpr(SP<Expr> x, pos_t lb, SP<Expr> low, SP<Expr> high, SP<Expr> max, bool slice3,
              pos_t rb)
        : X(x), Lbrack(lb), Low(low), High(high), Max(max), Slice3(slice3), Rbrack(rb) {}

    pos_t Pos() const override { return X->Pos(); }

    pos_t End() const override { return Rbrack + 1; }
};

struct TypeAssertExpr : Expr {
    SP<Expr> X;
    SP<Expr> Type;  // asserted type; nullptr means type switch X.(type)
    pos_t Lparen;
    pos_t Rparen;

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const TypeAssertExpr&>(rhs);
            return Lparen == casted.Lparen && Rparen == casted.Rparen && equal(X, casted.X) &&
                   equal(Type, casted.Type);
        }
        return false;
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "TypeAssertExpr:\n";
        if (X != nullptr) {
            res += X->Print(shiftSize + 4);
        }
        if (Type != nullptr) {
            res += Type->Print(shiftSize + 4);
        }
        return res;
    }

    TypeAssertExpr(SP<Expr> x, SP<Expr> typ, pos_t l, pos_t r)
        : X(x), Type(typ), Lparen(l), Rparen(r) {}

    pos_t Pos() const override { return X->Pos(); }

    pos_t End() const override { return Rparen + 1; }
};

struct CallExpr : Expr {
    SP<Expr> Fun;
    pos_t Lparen;
    V<SP<Expr>> Args;        // function arguments; or nullptr
    pos_t Ellipsis = NoPos;  // position of "..." (token.NoPos if there is no "...")
    pos_t Rparen;

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const CallExpr&>(rhs);
            return Lparen == casted.Lparen && Ellipsis == casted.Ellipsis &&
                   Rparen == casted.Rparen && equal(Args, casted.Args) && equal(Fun, casted.Fun);
        }
        return false;
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "CallExpr:\n";
        if (Fun != nullptr) {
            res += Fun->Print(shiftSize + 4);
        }
        if (!Args.empty()) {
            for (const auto& arg : Args) {
                if (arg) res += arg->Print(shiftSize + 4);
            }
        }
        return res;
    }

    CallExpr(SP<Expr> fun, pos_t l, const V<SP<Expr>>& args, pos_t ellipsis, pos_t r)
        : Fun(fun), Lparen(l), Args(args), Ellipsis(ellipsis), Rparen(r) {}

    pos_t Pos() const override { return Fun->Pos(); }

    pos_t End() const override { return Rparen + 1; }
};

struct StarExpr : Expr {
    pos_t Star;
    SP<Expr> X;  // operand (*ptr)

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const StarExpr&>(rhs);
            return Star == casted.Star && equal(X, casted.X);
        }
        return false;
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "StarExpr:\n";
        if (X != nullptr) {
            res += X->Print(shiftSize + 4);
        }
        return res;
    }

    StarExpr(pos_t pos, SP<Expr> x) : Star(pos), X(x) {}

    pos_t Pos() const override { return Star; }

    pos_t End() const override { return X->End(); }
};

struct UnaryExpr : Expr {
    pos_t OpPos;    // position of Op
    token_type Op;  // operator
    SP<Expr> X;     // operand

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const UnaryExpr&>(rhs);
            return OpPos == casted.OpPos && Op == casted.Op && equal(X, casted.X);
        }
        return false;
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "UnaryExpr:\n";
        res += std::string(shiftSize + 2, shiftChar) + type_string_map.at(Op) + '\n';
        if (X != nullptr) {
            res += X->Print(shiftSize + 4);
        }
        return res;
    }

    UnaryExpr() {}
    UnaryExpr(pos_t pos, token_type op, SP<Expr> x) : OpPos(pos), Op(op), X(x) {}

    pos_t Pos() const override { return OpPos; }

    pos_t End() const override { return X->End(); }
};

struct BinaryExpr : Expr {
    SP<Expr> X;     // left operand
    pos_t OpPos;    // position of Op
    token_type Op;  // operator
    SP<Expr> Y;     // right operand

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const BinaryExpr&>(rhs);
            return OpPos == casted.OpPos && Op == casted.Op && equal(X, casted.X) &&
                   equal(Y, casted.Y);
        }
        return false;
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "BinaryExpr:\n";
        if (X != nullptr) {
            res += X->Print(shiftSize + 4);
        }
        res += std::string(shiftSize + 2, shiftChar) + type_string_map.at(Op) + '\n';
        if (Y != nullptr) {
            res += Y->Print(shiftSize + 4);
        }
        return res;
    }

    BinaryExpr() {}
    BinaryExpr(SP<Expr> x, pos_t pos, token_type op, SP<Expr> y) : X(x), OpPos(pos), Op(op), Y(y) {}

    pos_t Pos() const override { return X->Pos(); }

    pos_t End() const override { return Y->End(); }
};

// A KeyValueExpr node represents (key : value) pairs
// in composite literals.
//
struct KeyValueExpr : Expr {
    SP<Expr> Key;
    pos_t Colon;  // position of ':'
    SP<Expr> Value;

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const KeyValueExpr&>(rhs);
            return Colon == casted.Colon && equal(Key, casted.Key) && equal(Value, casted.Value);
        }
        return false;
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "KeyValueExpr:\n";
        if (Key != nullptr) {
            res += Key->Print(shiftSize + 4);
        }
        if (Value != nullptr) {
            res += Value->Print(shiftSize + 4);
        }
        return res;
    }

    KeyValueExpr(SP<Expr> k, pos_t colon, SP<Expr> v) : Key(k), Colon(colon), Value(v) {}

    pos_t Pos() const override { return Key->Pos(); }

    pos_t End() const override { return Value->End(); }
};

struct ArrayTypeExpr : Expr {
    pos_t Lbrack;
    SP<Expr> Len;  // ellipsis node for [...]T arrays, nullptr for slice
    SP<Expr> Elt;  // elem type

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const ArrayTypeExpr&>(rhs);
            return Lbrack == casted.Lbrack && equal(Len, casted.Len) && equal(Elt, casted.Elt);
        }
        return false;
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "ArrayTypeExpr:\n";
        if (Len != nullptr) {
            res += Len->Print(shiftSize + 4);
        }
        if (Elt != nullptr) {
            res += Elt->Print(shiftSize + 4);
        }
        return res;
    }

    ArrayTypeExpr() {}
    ArrayTypeExpr(pos_t lbrack, SP<Expr> len, SP<Expr> elt) : Lbrack(lbrack), Len(len), Elt(elt) {}

    pos_t Pos() const override { return Lbrack; }

    pos_t End() const override { return Elt->End(); }
};

struct StructTypeExpr : Expr {
    pos_t Struct;             // pos of struct keyword
    SP<FieldList> Fields;     // list of field declarations
    bool Incomplete = false;  // true if (source) fields are missing in the
    // Fields list

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const StructTypeExpr&>(rhs);
            return Struct == casted.Struct && Incomplete == casted.Incomplete &&
                   equal(Fields, casted.Fields);
        }
        return false;
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "StructTypeExpr:\n";
        if (Fields != nullptr) {
            res += Fields->Print(shiftSize + 4);
        }
        return res;
    }

    StructTypeExpr(pos_t pos, SP<FieldList> fields) : Struct(pos), Fields(fields) {}

    pos_t Pos() const override { return Struct; }

    pos_t End() const override { return Fields->End(); }
};

struct InterfaceTypeExpr : Expr {
    pos_t Interface;        // pos of interface keyword
    SP<FieldList> Methods;  // list of embedded interfaces, methods, or types
    bool Incomplete;        // true if (source) methods or types are missing in
    // the Methods list

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const InterfaceTypeExpr&>(rhs);
            return Interface == casted.Interface && Incomplete == casted.Incomplete &&
                   equal(Methods, casted.Methods);
        }
        return false;
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "InterfaceTypeExpr:\n";
        if (Methods != nullptr) {
            res += Methods->Print(shiftSize + 4);
        }
        return res;
    }

    InterfaceTypeExpr(pos_t pos, SP<FieldList> ms) : Interface(pos), Methods(ms) {}

    pos_t Pos() const override { return Interface; }

    pos_t End() const override { return Methods->End(); }
};

struct MapTypeExpr : Expr {
    pos_t Map;  // pos of map keyword
    SP<Expr> Key;
    SP<Expr> Value;

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const MapTypeExpr&>(rhs);
            return Map == casted.Map && equal(Key, casted.Key) && equal(Value, casted.Value);
        }
        return false;
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "MapTypeExpr:\n";
        if (Key != nullptr) {
            res += Key->Print(shiftSize + 4);
        }
        if (Value != nullptr) {
            res += Value->Print(shiftSize + 4);
        }
        return res;
    }

    MapTypeExpr(pos_t pos, SP<Expr> key, SP<Expr> value) : Map(pos), Key(key), Value(value) {}

    pos_t Pos() const override { return Map; }

    pos_t End() const override { return Value->End(); }
};
// end Expressions

// Statements
struct BadStmt : Stmt {
    pos_t From;
    pos_t To;

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const BadStmt&>(rhs);
            return From == casted.From && To == casted.To;
        }
        return false;
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "BadStmt:\n";
        return res;
    }

    BadStmt(pos_t from, pos_t to) : From(from), To(to) {}

    pos_t Pos() const override { return From; }

    pos_t End() const override { return To; }
};

struct DeclStmt : Stmt {
    SP<Decl> Decl_;

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const DeclStmt&>(rhs);
            return equal(Decl_, casted.Decl_);
        }
        return false;
    }

    DeclStmt(SP<Decl> decl) : Decl_(decl) {}

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "DeclStmt:\n";
        if (Decl_ != nullptr) {
            res += Decl_->Print(shiftSize + 4);
        }
        return res;
    }

    pos_t Pos() const override { return Decl_->Pos(); }

    pos_t End() const override { return Decl_->End(); }
};

struct LabeledStmt : Stmt {
    SP<IdentExpr> Label;
    pos_t Colon;
    SP<Stmt> Stmt_;

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const LabeledStmt&>(rhs);
            return Colon == casted.Colon && equal(Label, casted.Label) &&
                   equal(Stmt_, casted.Stmt_);
        }
        return false;
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "LabeledStmt:\n";
        if (Label != nullptr) {
            res += Label->Print(shiftSize + 4);
        }
        if (Stmt_ != nullptr) {
            res += Stmt_->Print(shiftSize + 4);
        }
        return res;
    }

    LabeledStmt(SP<IdentExpr> lbl, pos_t pos, SP<Stmt> stmt)
        : Label(lbl), Colon(pos), Stmt_(stmt) {}

    pos_t Pos() const override { return Label->Pos(); }

    pos_t End() const override { return Stmt_->End(); }
};

struct ExprStmt : Stmt {
    SP<Expr> X;

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const ExprStmt&>(rhs);
            return equal(X, casted.X);
        }
        return false;
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "ExprStmt:\n";
        if (X != nullptr) {
            res += X->Print(shiftSize + 4);
        }
        return res;
    }

    ExprStmt(SP<Expr> x) : X(x) {}

    pos_t Pos() const override { return X->Pos(); }

    pos_t End() const override { return X->End(); }
};

// TODO SendStmt (also need to support channels)

struct IncDecStmt : Stmt {
    SP<Expr> X;
    pos_t TokPos;
    token_type Tok;  // inc or dec

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const IncDecStmt&>(rhs);
            return TokPos == casted.TokPos && Tok == casted.Tok && equal(X, casted.X);
        }
        return false;
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "IncDecStmt:\n";
        res += std::string(shiftSize + 2, shiftChar) + type_string_map.at(Tok) + '\n';
        if (X != nullptr) {
            res += X->Print(shiftSize + 4);
        }
        return res;
    }

    IncDecStmt(SP<Expr> x, pos_t pos, token_type tok) : X(x), TokPos(pos), Tok(tok) {}

    pos_t Pos() const override { return X->Pos(); }

    pos_t End() const override { return TokPos + 2; }
};

struct AssignStmt : Stmt {
    V<SP<Expr>> Lhs;
    pos_t TokPos;
    token_type Tok;  // assignment token, DEFINE token
    V<SP<Expr>> Rhs;

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const AssignStmt&>(rhs);
            return TokPos == casted.TokPos && Tok == casted.Tok && equal(Lhs, casted.Lhs) &&
                   equal(Rhs, casted.Rhs);
        }
        return false;
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "AssignStmt:\n";
        if (!Lhs.empty()) {
            for (const auto& lh : Lhs) {
                if (lh) res += lh->Print(shiftSize + 4);
            }
        }
        res += std::string(shiftSize + 2, shiftChar) + type_string_map.at(Tok) + '\n';
        if (!Rhs.empty()) {
            for (const auto& rh : Rhs) {
                if (rh) res += rh->Print(shiftSize + 4);
            }
        }
        return res;
    }

    AssignStmt(const V<SP<Expr>>& lhs, pos_t pos, token_type tok, const V<SP<Expr>>& rhs)
        : Lhs(lhs), TokPos(pos), Tok(tok), Rhs(rhs) {}

    pos_t Pos() const override { return Lhs[0]->Pos(); }

    pos_t End() const override { return Rhs.back()->End(); }
};

// TODO GoStmt (also need to support channels)

struct DeferStmt : Stmt {
    pos_t Defer;  // pos of defer keyword
    SP<CallExpr> Call;

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const DeferStmt&>(rhs);
            return Defer == casted.Defer && equal(Call, casted.Call);
        }
        return false;
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "DeferStmt:\n";
        if (Call != nullptr) {
            res += Call->Print(shiftSize + 4);
        }
        return res;
    }

    DeferStmt(pos_t pos, SP<CallExpr> call) : Defer(pos), Call(call) {}

    pos_t Pos() const override { return Defer; }

    pos_t End() const override { return Call->End(); }
};

struct ReturnStmt : Stmt {
    pos_t Return;
    V<SP<Expr>> Results;

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const ReturnStmt&>(rhs);
            return Return == casted.Return && equal(Results, casted.Results);
        }
        return false;
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "ReturnStmt:\n";
        if (!Results.empty()) {
            for (const auto& rs : Results) {
                if (rs) res += rs->Print(shiftSize + 4);
            }
        }
        return res;
    }

    ReturnStmt(pos_t pos, const V<SP<Expr>>& res) : Return(pos), Results(res) {}

    pos_t Pos() const override { return Return; }

    pos_t End() const override {
        if (Results.size() > 0) {
            return Results.back()->End();
        }
        return Return + 6;
    }
};

struct BranchStmt : Stmt {
    pos_t TokPos;
    token_type Tok;  // keyword token (BREAK, CONTINUE, GOTO, FALLTHROUGH)
    SP<IdentExpr> Label;

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const BranchStmt&>(rhs);
            return TokPos == casted.TokPos && Tok == casted.Tok && equal(Label, casted.Label);
        }
        return false;
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "BranchStmt:\n";
        res += std::string(shiftSize + 2, shiftChar) + type_string_map.at(Tok) + '\n';
        if (Label != nullptr) {
            res += Label->Print(shiftSize + 4);
        }
        return res;
    }

    BranchStmt(pos_t pos, token_type tok, SP<IdentExpr> lbl) : TokPos(pos), Tok(tok), Label(lbl) {}

    pos_t Pos() const override { return TokPos; }

    pos_t End() const override {
        if (Label != nullptr) {
            return Label->End();
        }
        return TokPos + type_string_map.at(Tok).size();
    }
};

struct IfStmt : Stmt {
    pos_t If;
    SP<Stmt> Init;  // initialization statement; or nullptr
    SP<Expr> Cond;  // condition
    SP<BlockStmt> Body;
    SP<Stmt> Else;

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const IfStmt&>(rhs);
            return If == casted.If && equal(Init, casted.Init) && equal(Cond, casted.Cond) &&
                   equal(Body, casted.Body) && equal(Else, casted.Else);
        }
        return false;
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "IfStmt:\n";
        if (Init != nullptr) {
            res += Init->Print(shiftSize + 4);
        }
        if (Cond != nullptr) {
            res += Cond->Print(shiftSize + 4);
        }
        if (Body != nullptr) {
            res += Body->Print(shiftSize + 4);
        }
        if (Else != nullptr) {
            res += Else->Print(shiftSize + 4);
        }
        return res;
    }

    IfStmt(pos_t pos, SP<Stmt> init, SP<Expr> cond, SP<BlockStmt> body, SP<Stmt> els)
        : If(pos), Init(init), Cond(cond), Body(body), Else(els) {}

    pos_t Pos() const override { return If; }

    pos_t End() const override {
        if (Else != nullptr) {
            return Else->End();
        }
        return Body->End();
    }
};

struct CaseClauseStmt : Stmt {
    pos_t Case;        // position of "case" or "default" keyword
    V<SP<Expr>> List;  // list of expressions or types; nullptr means default case
    pos_t Colon;
    V<SP<Stmt>> Body;

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const CaseClauseStmt&>(rhs);
            return Case == casted.Case && Colon == casted.Colon && equal(List, casted.List) &&
                   equal(Body, casted.Body);
        }
        return false;
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "CaseClauseStmt:\n";
        if (!List.empty()) {
            for (const auto& l : List) {
                if (l) res += l->Print(shiftSize + 4);
            }
        }
        if (!Body.empty()) {
            for (const auto& b : Body) {
                if (b) res += b->Print(shiftSize + 4);
            }
        }
        return res;
    }

    CaseClauseStmt(pos_t pos, const V<SP<Expr>>& list, pos_t colon, const V<SP<Stmt>>& body)
        : Case(pos), List(list), Colon(colon), Body(body) {}

    pos_t Pos() const override { return Case; }

    pos_t End() const override {
        if (Body.size() > 0) {
            return Body.back()->End();
        }
        return Colon + 1;
    }
};

struct SwitchStmt : Stmt {
    pos_t Switch;
    SP<Stmt> Init;  // initialization statement; or nullptr
    SP<Expr> Tag;
    SP<BlockStmt> Body;

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const SwitchStmt&>(rhs);
            return Switch == casted.Switch && equal(Init, casted.Init) && equal(Tag, casted.Tag) &&
                   equal(Body, casted.Body);
        }
        return false;
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "SwitchStmt:\n";
        if (Init != nullptr) {
            res += Init->Print(shiftSize + 4);
        }
        if (Tag != nullptr) {
            res += Tag->Print(shiftSize + 4);
        }
        if (Body != nullptr) {
            res += Body->Print(shiftSize + 4);
        }
        return res;
    }

    SwitchStmt(pos_t pos, SP<Stmt> init, SP<Expr> tag, SP<BlockStmt> body)
        : Switch(pos), Init(init), Tag(tag), Body(body) {}

    pos_t Pos() const override { return Switch; }

    pos_t End() const override { return Body->End(); }
};

struct TypeSwitchStmt : Stmt {
    pos_t Switch;
    SP<Stmt> Init;       // initialization statement; or nullptr
    SP<Stmt> Assign;     // x := y.(type) or y.(type)
    SP<BlockStmt> Body;  // CaseClauses only

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const TypeSwitchStmt&>(rhs);
            return Switch == casted.Switch && equal(Init, casted.Init) &&
                   equal(Assign, casted.Assign) && equal(Body, casted.Body);
        }
        return false;
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "TypeSwitchExpr:\n";
        if (Init != nullptr) {
            res += Init->Print(shiftSize + 4);
        }
        if (Assign != nullptr) {
            res += Assign->Print(shiftSize + 4);
        }
        if (Body != nullptr) {
            res += Body->Print(shiftSize + 4);
        }
        return res;
    }

    TypeSwitchStmt(pos_t pos, SP<Stmt> init, SP<Stmt> assign, SP<BlockStmt> body)
        : Switch(pos), Init(init), Assign(assign), Body(body) {}

    pos_t Pos() const override { return Switch; }

    pos_t End() const override { return Body->End(); }
};

// TODO CommClauseStmt

// TODO SelectClauseStmt (need channel support)

struct ForStmt : Stmt {
    pos_t For;
    SP<Stmt> Init;
    SP<Expr> Cond;
    SP<Stmt> Post;  // post iteration statement; or nullptr
    SP<BlockStmt> Body;

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const ForStmt&>(rhs);
            return For == casted.For && equal(Init, casted.Init) && equal(Cond, casted.Cond) &&
                   equal(Post, casted.Post) && equal(Body, casted.Body);
        }
        return false;
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "ForStmt:\n";
        if (Init != nullptr) {
            res += Init->Print(shiftSize + 4);
        }
        if (Cond != nullptr) {
            res += Cond->Print(shiftSize + 4);
        }
        if (Post != nullptr) {
            res += Post->Print(shiftSize + 4);
        }
        if (Body != nullptr) {
            res += Body->Print(shiftSize + 4);
        }
        return res;
    }

    ForStmt(pos_t forPos, SP<Stmt> init, SP<Expr> cond, SP<Stmt> post, SP<BlockStmt> body)
        : For(forPos), Init(init), Cond(cond), Post(post), Body(body) {}

    pos_t Pos() const override { return For; }

    pos_t End() const override { return Body->End(); }
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

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const RangeStmt&>(rhs);
            return For == casted.For && TokPos == casted.TokPos && Tok == casted.Tok &&
                   Range == casted.Range && equal(Key, casted.Key) && equal(Value, casted.Value) &&
                   equal(X, casted.X) && equal(Body, casted.Body);
        }
        return false;
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "RangeStmt:\n";
        if (Key != nullptr) {
            res += Key->Print(shiftSize + 4);
        }
        if (Value != nullptr) {
            res += Value->Print(shiftSize + 4);
        }
        res += std::string(shiftSize + 2, shiftChar) + type_string_map.at(Tok) + '\n';
        // res += std::string(shiftSize + 2, shiftChar) + "Range pos " +
        // Range.ToString() + '\n';
        if (X != nullptr) {
            res += X->Print(shiftSize + 4);
        }
        if (Body != nullptr) {
            res += Body->Print(shiftSize + 4);
        }
        return res;
    }

    RangeStmt(pos_t forPos, SP<Expr> key, SP<Expr> value, pos_t tokPos, token_type tok,
              pos_t rangePos, SP<Expr> x, SP<BlockStmt> body)
        : For(forPos),
          Key(key),
          Value(value),
          TokPos(tokPos),
          Tok(tok),
          Range(rangePos),
          X(x),
          Body(body) {}

    pos_t Pos() const override { return For; }

    pos_t End() const override { return Body->End(); }
};

struct EmptyStmt : Stmt {
    pos_t Semicolon;
    bool Implicit;

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const EmptyStmt&>(rhs);
            return Semicolon == casted.Semicolon && Implicit == casted.Implicit;
        }
        return false;
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "EmptyStmt:\n";
        return res;
    }

    EmptyStmt(pos_t sc, bool implicit) : Semicolon(sc), Implicit(implicit) {}

    pos_t Pos() const override { return Semicolon; }

    pos_t End() const override {
        if (Implicit) {
            return Semicolon;
        }
        return Semicolon + 1;
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

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const ImportSpec&>(rhs);
            return EndPos == casted.EndPos && equal(Name, casted.Name) && equal(Path, casted.Path);
        }
        return false;
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "ImportSpec:\n";
        if (Name != nullptr) {
            res += Name->Print(shiftSize + 4);
        }
        if (Path != nullptr) {
            res += Path->Print(shiftSize + 4);
        }
        return res;
    }

    ImportSpec(SP<IdentExpr> name, SP<BasicLitExpr> path, pos_t endPos)
        : Name(name), Path(path), EndPos(endPos) {}

    pos_t Pos() const override {
        if (Name != nullptr) {
            return Name->Pos();
        }
        return Path->Pos();
    }

    pos_t End() const override {
        if (EndPos != NoPos) {
            return EndPos;
        }
        return Path->End();
    }
};

// A ValueSpec node represents a constant or variable declaration
// (ConstSpec or VarSpec production).
//
struct ValueSpec : Spec {
    V<SP<IdentExpr>> Names;  // value names Names.size() > 0)
    SP<Expr> Type;           // value type; or nullptr
    V<SP<Expr>> Values;      // initial values; or nullptr

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const ValueSpec&>(rhs);
            return equal(Names, casted.Names) && equal(Type, casted.Type) &&
                   equal(Values, casted.Values);
        }
        return false;
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "ValueSpec:\n";
        if (!Names.empty()) {
            for (const auto& name : Names) {
                if (name) res += name->Print(shiftSize + 4);
            }
        }
        if (Type != nullptr) {
            res += Type->Print(shiftSize + 4);
        }
        if (!Values.empty()) {
            for (const auto& val : Values) {
                if (val) res += val->Print(shiftSize + 4);
            }
        }
        return res;
    }

    ValueSpec(const V<SP<IdentExpr>>& names, SP<Expr> type, const V<SP<Expr>>& values)
        : Names(names), Type(type), Values(values) {}

    pos_t Pos() const override { return Names[0]->Pos(); }

    pos_t End() const override {
        if (Values.size() > 0) {
            return Values.back()->End();
        }
        if (Type != nullptr) {
            return Type->End();
        }
        return Names.back()->End();
    }
};

// A TypeSpec node represents a type declaration (TypeSpec production).
struct TypeSpec : Spec {
    SP<IdentExpr> Name;
    SP<FieldList> TypeParams;
    pos_t Assign;   // position of '=', if any
    SP<Expr> Type;  // IdentExpr, ParenExpr, SelectorExpr, StarExpr, or any of
                    // the XxxTypes

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const TypeSpec&>(rhs);
            return Assign == casted.Assign && equal(Name, casted.Name) &&
                   equal(TypeParams, casted.TypeParams) && equal(Type, casted.Type);
        }
        return false;
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "TypeSpec:\n";
        if (Name != nullptr) {
            res += Name->Print(shiftSize + 4);
        }
        if (TypeParams != nullptr) {
            res += TypeParams->Print(shiftSize + 4);
        }
        if (Type != nullptr) {
            res += Type->Print(shiftSize + 4);
        }
        return res;
    }

    TypeSpec(SP<IdentExpr> name, SP<FieldList> typeParams, pos_t assign, SP<Expr> type)
        : Name(name), TypeParams(typeParams), Assign(assign), Type(type) {}

    pos_t Pos() const override { return Name->Pos(); }

    pos_t End() const override { return Type->End(); }
};

// end Spec

// Declarations

struct BadDecl : Decl {
    pos_t From;
    pos_t To;

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const BadDecl&>(rhs);
            return From == casted.From && To == casted.To;
        }
        return false;
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "BadDecl:\n";
        return res;
    }

    BadDecl(pos_t from, pos_t to) : From(from), To(to) {}

    pos_t Pos() const override { return From; }

    pos_t End() const override { return To; }
};

struct GenDecl : Decl {
    pos_t TokPos;
    token_type Tok;  // IMPORT, CONST, TYPE, or VAR
    pos_t Lparen;
    pos_t Rparen;
    V<SP<Spec>> Specs;

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const GenDecl&>(rhs);
            return TokPos == casted.TokPos && Tok == casted.Tok && Lparen == casted.Lparen &&
                   Rparen == casted.Rparen && equal(Specs, casted.Specs);
        }
        return false;
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "GenDecl: " + type_string_map.at(Tok) + '\n';
        if (!Specs.empty()) {
            for (const auto& sp : Specs) {
                if (sp) res += sp->Print(shiftSize + 4);
            }
        }
        return res;
    }

    GenDecl(pos_t tokPos, token_type tok, pos_t lparen, const V<SP<Spec>>& specs, pos_t rparen)
        : TokPos(tokPos), Tok(tok), Lparen(lparen), Rparen(rparen), Specs(specs) {}

    pos_t Pos() const override { return TokPos; }

    pos_t End() const override {
        if (Rparen.IsValid()) {
            return Rparen;
        }
        return Specs[0]->End();
    }
};

struct FuncDecl : Decl {
    SP<FieldList> Recv;  // receiver (methods); or nullptr (functions)
    SP<IdentExpr> Name;
    SP<FuncTypeExpr> Type;  // function signature: type and value parameters,
                            // results, and position of "func" keyword
    SP<BlockStmt> Body;

    virtual bool operator==(const Node& rhs) const override {
        if (sameType(*this, rhs)) {
            auto& casted = dynamic_cast<const FuncDecl&>(rhs);
            return equal(Recv, casted.Recv) && equal(Name, casted.Name) &&
                   equal(Type, casted.Type) && equal(Body, casted.Body);
        }
        return false;
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftSize, shiftChar);
        res += "FuncDecl:\n";
        if (Recv != nullptr) {
            res += Recv->Print(shiftSize + 4);
        }
        if (Name != nullptr) {
            res += Name->Print(shiftSize + 4);
        }
        if (Type != nullptr) {
            res += Type->Print(shiftSize + 4);
        }
        if (Body != nullptr) {
            res += Body->Print(shiftSize + 4);
        }
        return res;
    }

    FuncDecl(SP<FieldList> recv, SP<IdentExpr> name, SP<FuncTypeExpr> type, SP<BlockStmt> body)
        : Recv(recv), Name(name), Type(type), Body(body) {}

    pos_t Pos() const override { return Type->Pos(); }

    pos_t End() const override {
        if (Body != nullptr) {
            return Body->End();
        }
        return Type->End();
    }
};

// end Declarations