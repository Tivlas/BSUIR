#pragma once
#include <memory>
#include <vector>

#include "token.h"

/* namespace color {

const std::string error = "\033[1;91m";    // red
const std::string warning = "\033[1;95m";  // purple
const std::string keyword = "\033[1;93m";  // yellow
const std::string ident = "\033[1;94m";    // blue
const std::string oper = "\033[1;92m";     // green
const std::string reset = "\033[0m";       // no color

} */  // namespace color

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

const pos_t NoPos{};

// TODO implement Pos method for each type

template <class LHS, class RHS>
bool compare(const LHS& lhs, const RHS& rhs) {
    try {
        auto& casted = dynamic_cast<const LHS&>(rhs);
        // return lhs == casted; // TODO comparison (function ???)
        return true;
    } catch (std::bad_cast& error) {
        return false;
    }
}

const char shiftChar = '-';

struct Node {
    virtual pos_t Pos() const = 0;  // first node char
    virtual pos_t End() const = 0;  // after
    virtual std::string Print(size_t shift) const = 0;

    virtual bool operator==(const Node& rhs) const {
        return compare(*this, rhs);
    }
};

struct Expr : Node {
    // virtual void exprNode() const = 0;
    /*     virtual bool operator==(const Node& rhs) const {
            return compare(*this, rhs);
        } */
};

struct Stmt : Node {
    // virtual void stmtNode() const = 0;
    /* virtual bool operator==(const Node& rhs) const {
        return compare(*this, rhs);
    } */
};

struct Spec : Node {
    // virtual void specNode() const = 0;
    /* virtual bool operator==(const Node& rhs) const {
        return compare(*this, rhs);
    } */
};

struct Decl : Node {
    // virtual void declNode() const = 0;
    /* virtual bool operator==(const Node& rhs) const {
        return compare(*this, rhs);
    } */
};

// Expressions
struct BadExpr : Expr {
    pos_t From;
    pos_t To;

    BadExpr(pos_t from, pos_t to) : From(from), To(to) {}

    pos_t Pos() const override { return From; }

    pos_t End() const override { return To; }

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res += color::error + "BadExpr " + color::reset + "<" +
               Pos().ToString() + " --- " + End().ToString() + ">\n";
        return res;
    }
};

struct IdentExpr : Expr {
    pos_t NamePos;  // identifier position
    std::string Name;
    // TODO Obj *Object

    IdentExpr(pos_t pos, std::string name) : NamePos(pos), Name(name) {}

    pos_t Pos() const override { return NamePos; }

    pos_t End() const override { return NamePos + Name.size(); }

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res += "IdentExpr " + Name + " <" + Pos().ToString() + " --- " +
               End().ToString() + ">\n";
        return res;
    }
};

struct EllipsisExpr : Expr {
    pos_t Ellipsis;  // position of "..."
    SP<Expr> Elt;    // ellipsis element type (parameter lists only); or nullptr

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
        return compare(*this, rhs);
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res += "Ellipsis <" + Pos().ToString() + " --- " + End().ToString() +
               ">\n";
        if (Elt != nullptr) {
            // res += std::string(shiftChar, shiftSize + 1) + "Element type\n";
            res += Elt->Print(shiftSize + 2);
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
        return compare(*this, rhs);
    }

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res += "BasicLitExpr <" + Pos().ToString() + " --- " +
               End().ToString() + ">\n";
        res += std::string(shiftChar, shiftSize + 1) + "Kind and value\n";
        res += type_string_map.at(Kind) + ' ' + Value;
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

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res +=
            "Field <" + Pos().ToString() + " --- " + End().ToString() + ">\n";
        if (!Names.empty()) {
            for (const auto& name : Names) {
                // res += std::string(shiftChar, shiftSize + 1) + "Name\n";
                if (name) res += name->Print(shiftSize + 2);
            }
        }
        if (Type != nullptr) {
            // res += std::string(shiftChar, shiftSize + 1) + "Type\n";
            res += Type->Print(shiftSize + 2);
        }
        if (Tag != nullptr) {
            // res += std::string(shiftChar, shiftSize + 1) + "Tag";
            res += Tag->Print(shiftSize + 2);
        }
        return res;
    }

    Field(V<SP<IdentExpr>> names, SP<Expr> typ, SP<BasicLitExpr> tag)
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

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res += "Ellipsis <" + Pos().ToString() + " --- " + End().ToString() +
               ">\n";
        if (!List.empty()) {
            for (const auto& field : List) {
                // res += std::string(shiftChar, shiftSize + 1) + "Field\n";
                if (field) res += field->Print(shiftSize + 2);
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

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct FuncTypeExpr : Expr {
    pos_t Func;  // position of "func" keyword (token.NoPos if there is no
                 // "func")
    SP<FieldList> TypeParams;  // type parameters; or nullptr
    SP<FieldList> Params;      // (incoming) parameters; non-nullptr
    SP<FieldList> Results;     // (outgoing) results; or nullptr

    FuncTypeExpr(pos_t pos, SP<FieldList> tps, SP<FieldList> ps,
                 SP<FieldList> rs)
        : Func(pos), TypeParams(tps), Params(ps), Results(rs) {}

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res += "FuncTypeExpr <" + Pos().ToString() + " --- " +
               End().ToString() + ">\n";
        if (TypeParams) {
            res += std::string(shiftChar, shiftSize + 1);
            res += TypeParams->Print(shiftSize + 1);
        }
        if (Params) {
            res += Params->Print(shiftSize + 1);
        }
        if (Results) {
            res += Results->Print(shiftSize + 1);
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

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct BlockStmt : Stmt {
    pos_t Lbrace;
    V<SP<Stmt>> List;
    pos_t Rbrace;

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res += "BlockStmt <" + Pos().ToString() + " --- " + End().ToString() +
               ">\n";
        if (!List.empty()) {
            for (const auto& stmt : List) {
                if (stmt) res += stmt->Print(shiftSize + 2);
            }
        }
        return res;
    }

    BlockStmt(pos_t l, V<SP<Stmt>> list, pos_t r)
        : Lbrace(l), List(list), Rbrace(r) {}

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
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

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res += "FuncLitExpr <" + Pos().ToString() + " --- " + End().ToString() +
               ">\n";
        if (Type != nullptr) {
            res += Type->Print(shiftSize + 2);
        }
        if (Body != nullptr) {
            res += Body->Print(shiftSize + 2);
        }
        return res;
    }

    FuncLitExpr(SP<FuncTypeExpr> t, SP<BlockStmt> b) : Type(t), Body(b) {}

    pos_t Pos() const override { return Type->Pos(); }

    pos_t End() const override { return Body->End(); }

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct CompositeLitExpr : Expr {  // structs literals, slice literals, etc
    SP<Expr> Type;
    pos_t Lbrace;
    V<SP<Expr>> Elts;  // list of composite elements; or nullptr
    pos_t Rbrace;
    bool Incomplete;  // true if (source) expressions are missing in the
                      // Elts list

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res += "CompositeLitExpr <" + Pos().ToString() + " --- " +
               End().ToString() + ">\n";
        if (Type != nullptr) {
            res += Type->Print(shiftSize + 2);
        }
        if (!Elts.empty()) {
            for (const auto& elt : Elts) {
                if (elt) res += elt->Print(shiftSize + 2);
            }
        }
        return res;
    }

    CompositeLitExpr(SP<Expr> typ, pos_t l, V<SP<Expr>> elts, pos_t r)
        : Type(typ), Lbrace(l), Elts(elts), Rbrace(r) {}

    pos_t Pos() const override {
        return Type != nullptr ? Type->Pos() : Lbrace;
    }

    pos_t End() const override { return Rbrace + 1; }

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct ParenExpr : Expr {
    pos_t Lparen;
    SP<Expr> X;
    pos_t Rparen;

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res += "ParenExpr <" + Pos().ToString() + " --- " + End().ToString() +
               ">\n";
        if (X != nullptr) {
            res += X->Print(shiftSize + 2);
        }
        return res;
    }

    ParenExpr(pos_t l, SP<Expr> x, pos_t r) : Lparen(l), X(x), Rparen(r) {}

    pos_t Pos() const override { return Lparen; }

    pos_t End() const override { return Rparen + 1; }

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct SelectorExpr : Expr {
    SP<Expr> X;
    SP<IdentExpr> Sel;  // field selector

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res += "SelectorExpr <" + Pos().ToString() + " --- " +
               End().ToString() + ">\n";
        if (X != nullptr) {
            res += X->Print(shiftSize + 2);
        }
        if (Sel != nullptr) {
            res += Sel->Print(shiftSize + 2);
        }
        return res;
    }

    SelectorExpr() {}
    SelectorExpr(SP<Expr> x, SP<IdentExpr> sel) : X(x), Sel(sel) {}

    pos_t Pos() const override { return X->Pos(); }

    pos_t End() const override { return Sel->End(); }

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct IndexExpr : Expr {
    SP<Expr> X;
    pos_t Lbrack;
    SP<Expr> Index;
    pos_t Rbrack;

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res += "IndexExpr <" + Pos().ToString() + " --- " + End().ToString() +
               ">\n";
        if (X != nullptr) {
            res += X->Print(shiftSize + 2);
        }
        if (Index != nullptr) {
            res += Index->Print(shiftSize + 2);
        }
        return res;
    }

    IndexExpr(SP<Expr> x, pos_t l, SP<Expr> idx, pos_t r)
        : X(x), Lbrack(l), Index(idx), Rbrack(r) {}

    pos_t Pos() const override { return X->Pos(); }

    pos_t End() const override { return Rbrack + 1; }

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct IndexListExpr : Expr {
    SP<Expr> X;
    pos_t Lbrack;
    V<SP<Expr>> Indeces;
    pos_t Rbrack;

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res += "IndexListExpr <" + Pos().ToString() + " --- " +
               End().ToString() + ">\n";
        if (X != nullptr) {
            res += X->Print(shiftSize + 2);
        }
        if (!Indeces.empty()) {
            for (const auto& idx : Indeces) {
                if (idx) res += idx->Print(shiftSize + 2);
            }
        }
        return res;
    }

    IndexListExpr(SP<Expr> x, pos_t lbrack, V<SP<Expr>> indeces, pos_t rbrack)
        : X(x), Lbrack(lbrack), Indeces(indeces), Rbrack(rbrack) {}

    pos_t Pos() const override { return X->Pos(); }

    pos_t End() const override { return Rbrack + 1; }

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

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res += "SliceExpr <" + Pos().ToString() + " --- " + End().ToString() +
               ">\n";
        if (X != nullptr) {
            res += X->Print(shiftSize + 2);
        }
        if (Low != nullptr) {
            res += Low->Print(shiftSize + 2);
        }
        if (High != nullptr) {
            res += High->Print(shiftSize + 2);
        }
        if (Max != nullptr) {
            res += Max->Print(shiftSize + 2);
        }
        return res;
    }

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

    pos_t End() const override { return Rbrack + 1; }

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct TypeAssertExpr : Expr {
    SP<Expr> X;
    SP<Expr> Type;  // asserted type; nullptr means type switch X.(type)
    pos_t Lparen;
    pos_t Rparen;

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res += "TypeAssertExpr <" + Pos().ToString() + " --- " +
               End().ToString() + ">\n";
        if (X != nullptr) {
            res += X->Print(shiftSize + 2);
        }
        if (Type != nullptr) {
            res += Type->Print(shiftSize + 2);
        }
        return res;
    }

    TypeAssertExpr(SP<Expr> x, SP<Expr> typ, pos_t l, pos_t r)
        : X(x), Type(typ), Lparen(l), Rparen(r) {}

    pos_t Pos() const override { return X->Pos(); }

    pos_t End() const override { return Rparen + 1; }

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

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res += "CallExpr <" + Pos().ToString() + " --- " + End().ToString() +
               (Ellipsis == NoPos
                    ? ""
                    : "Ellipsis pos: " + Ellipsis.ToString() + ">\n");
        if (Fun != nullptr) {
            res += Fun->Print(shiftSize + 2);
        }
        if (!Args.empty()) {
            for (const auto& arg : Args) {
                if (arg) res += arg->Print(shiftSize + 2);
            }
        }
        return res;
    }

    CallExpr(SP<Expr> fun, pos_t l, V<SP<Expr>> args, pos_t ellipsis, pos_t r)
        : Fun(fun), Lparen(l), Args(args), Ellipsis(ellipsis), Rparen(r) {}

    pos_t Pos() const override { return Fun->Pos(); }

    pos_t End() const override { return Rparen + 1; }

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct StarExpr : Expr {
    pos_t Star;
    SP<Expr> X;  // operand (*ptr)

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res += "StarExpr <" + Pos().ToString() + " --- " + End().ToString() +
               ">\n";
        if (X != nullptr) {
            res += X->Print(shiftSize + 2);
        }
        return res;
    }

    StarExpr(pos_t pos, SP<Expr> x) : Star(pos), X(x) {}

    pos_t Pos() const override { return Star; }

    pos_t End() const override { return X->End(); }

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct UnaryExpr : Expr {
    pos_t OpPos;    // position of Op
    token_type Op;  // operator
    SP<Expr> X;     // operand

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res += "UnaryExpr <" + Pos().ToString() + " --- " + End().ToString() +
               ">\n";
        res += std::string(shiftChar, shiftSize + 1) + type_string_map.at(Op) +
               '\n';
        if (X != nullptr) {
            res += X->Print(shiftSize + 2);
        }
        return res;
    }

    UnaryExpr() {}
    UnaryExpr(pos_t pos, token_type op, SP<Expr> x)
        : OpPos(pos), Op(op), X(x) {}

    pos_t Pos() const override { return OpPos; }

    pos_t End() const override { return X->End(); }

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct BinaryExpr : Expr {
    SP<Expr> X;     // left operand
    pos_t OpPos;    // position of Op
    token_type Op;  // operator
    SP<Expr> Y;     // right operand

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res += "BinaryExpr <" + Pos().ToString() + " --- " + End().ToString() +
               ">\n";
        if (X != nullptr) {
            res += X->Print(shiftSize + 2);
        }
        res += std::string(shiftChar, shiftSize + 1) + type_string_map.at(Op) +
               '\n';
        if (Y != nullptr) {
            res += Y->Print(shiftSize + 2);
        }
        return res;
    }

    BinaryExpr() {}
    BinaryExpr(SP<Expr> x, pos_t pos, token_type op, SP<Expr> y)
        : X(x), OpPos(pos), Op(op), Y(y) {}

    pos_t Pos() const override { return X->Pos(); }

    pos_t End() const override { return Y->End(); }

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

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res += "KeyValueExpr <" + Pos().ToString() + " --- " +
               End().ToString() + ">\n";
        if (Key != nullptr) {
            res += Key->Print(shiftSize + 2);
        }
        if (Value != nullptr) {
            res += Value->Print(shiftSize + 2);
        }
        return res;
    }

    KeyValueExpr(SP<Expr> k, pos_t colon, SP<Expr> v)
        : Key(k), Colon(colon), Value(v) {}

    pos_t Pos() const override { return Key->Pos(); }

    pos_t End() const override { return Value->End(); }

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct ArrayTypeExpr : Expr {
    pos_t Lbrack;
    SP<Expr> Len;  // ellipsis node for [...]T arrays, nullptr for slice
    SP<Expr> Elt;  // elem type

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res += "ArrayTypeExpr <" + Pos().ToString() + " --- " +
               End().ToString() + ">\n";
        if (Len != nullptr) {
            res += Len->Print(shiftSize + 2);
        }
        if (Elt != nullptr) {
            res += Elt->Print(shiftSize + 2);
        }
        return res;
    }

    ArrayTypeExpr() {}
    ArrayTypeExpr(pos_t lbrack, SP<Expr> len, SP<Expr> elt)
        : Lbrack(lbrack), Len(len), Elt(elt) {}

    pos_t Pos() const override { return Lbrack; }

    pos_t End() const override { return Elt->End(); }

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct StructTypeExpr : Expr {
    pos_t Struct;             // pos of struct keyword
    SP<FieldList> Fields;     // list of field declarations
    bool Incomplete = false;  // true if (source) fields are missing in the
                              // Fields list

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res += "StructTypeExpr <" + Pos().ToString() + " --- " +
               End().ToString() + ">\n";
        if (Fields != nullptr) {
            res += Fields->Print(shiftSize + 2);
        }
        return res;
    }

    StructTypeExpr(pos_t pos, SP<FieldList> fields)
        : Struct(pos), Fields(fields) {}

    pos_t Pos() const override { return Struct; }

    pos_t End() const override { return Fields->End(); }

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct InterfaceTypeExpr : Expr {
    pos_t Interface;        // pos of interface keyword
    SP<FieldList> Methods;  // list of embedded interfaces, methods, or types
    bool Incomplete;        // true if (source) methods or types are missing in
    // the Methods list

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res += "InterfaceTypeExpr <" + Pos().ToString() + " --- " +
               End().ToString() + ">\n";
        if (Methods != nullptr) {
            res += Methods->Print(shiftSize + 2);
        }
        return res;
    }

    InterfaceTypeExpr(pos_t pos, SP<FieldList> ms)
        : Interface(pos), Methods(ms) {}

    pos_t Pos() const override { return Interface; }

    pos_t End() const override { return Methods->End(); }

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct MapTypeExpr : Expr {
    pos_t Map;  // pos of map keyword
    SP<Expr> Key;
    SP<Expr> Value;

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res += "MapTypeExpr <" + Pos().ToString() + " --- " + End().ToString() +
               ">\n";
        if (Key != nullptr) {
            res += Key->Print(shiftSize + 2);
        }
        if (Value != nullptr) {
            res += Value->Print(shiftSize + 2);
        }
        return res;
    }

    MapTypeExpr(pos_t pos, SP<Expr> key, SP<Expr> value)
        : Map(pos), Key(key), Value(value) {}

    pos_t Pos() const override { return Map; }

    pos_t End() const override { return Value->End(); }

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};
// end Expressions

// Statements
struct BadStmt : Stmt {
    pos_t From;
    pos_t To;

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res +=
            "BadStmt <" + Pos().ToString() + " --- " + End().ToString() + ">\n";
        return res;
    }

    BadStmt(pos_t from, pos_t to) : From(from), To(to) {}

    pos_t Pos() const override { return From; }

    pos_t End() const override { return To; }

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }
};

struct DeclStmt : Stmt {
    SP<Decl> Decl;

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res += "DeclStmt <" + Pos().ToString() + " --- " + End().ToString() +
               ">\n";
        if (Decl != nullptr) {
            res += Decl->Print(shiftSize + 2);
        }
        return res;
    }

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }

    pos_t Pos() const override { return Decl->Pos(); }

    pos_t End() const override { return Decl->End(); }
};

struct LabeledStmt : Stmt {
    SP<IdentExpr> Label;
    pos_t Colon;
    SP<Stmt> Stmt_;

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res += "LabeledStmt <" + Pos().ToString() + " --- " + End().ToString() +
               ">\n";
        if (Label != nullptr) {
            res += Label->Print(shiftSize + 2);
        }
        if (Stmt_ != nullptr) {
            res += Stmt_->Print(shiftSize + 2);
        }
        return res;
    }

    LabeledStmt(SP<IdentExpr> lbl, pos_t pos, SP<Stmt> stmt)
        : Label(lbl), Colon(pos), Stmt_(stmt) {}

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }

    pos_t Pos() const override { return Label->Pos(); }

    pos_t End() const override { return Stmt_->End(); }
};

struct ExprStmt : Stmt {
    SP<Expr> X;

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res += "ExprStmt <" + Pos().ToString() + " --- " + End().ToString() +
               ">\n";
        if (X != nullptr) {
            res += X->Print(shiftSize + 2);
        }
        return res;
    }

    ExprStmt(SP<Expr> x) : X(x) {}

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }

    pos_t Pos() const override { return X->Pos(); }

    pos_t End() const override { return X->End(); }
};

// TODO SendStmt (also need to support channels)

struct IncDecStmt : Stmt {
    SP<Expr> X;
    pos_t TokPos;
    token_type Tok;  // inc or dec

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res += "IncDecStmt <" + Pos().ToString() + " --- " + End().ToString() +
               ">\n";
        res += std::string(shiftChar, shiftSize + 1) + type_string_map.at(Tok) +
               '\n';
        if (X != nullptr) {
            res += X->Print(shiftSize + 2);
        }
        return res;
    }

    IncDecStmt(SP<Expr> x, pos_t pos, token_type tok)
        : X(x), TokPos(pos), Tok(tok) {}

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }

    pos_t Pos() const override { return X->Pos(); }

    pos_t End() const override { return TokPos + 2; }
};

struct AssignStmt : Stmt {
    V<SP<Expr>> Lhs;
    pos_t TokPos;
    token_type Tok;  // assignment token, DEFINE token
    V<SP<Expr>> Rhs;

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res += "AssignStmt <" + Pos().ToString() + " --- " + End().ToString() +
               ">\n";
        if (!Lhs.empty()) {
            for (const auto& lh : Lhs) {
                if (lh) res += lh->Print(shiftSize + 2);
            }
        }
        res += std::string(shiftChar, shiftSize + 1) + type_string_map.at(Tok) +
               '\n';
        if (!Rhs.empty()) {
            for (const auto& rh : Rhs) {
                if (rh) res += rh->Print(shiftSize + 2);
            }
        }
        return res;
    }

    AssignStmt(V<SP<Expr>> lhs, pos_t pos, token_type tok, V<SP<Expr>> rhs)
        : Lhs(lhs), TokPos(pos), Tok(tok), Rhs(rhs) {}

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }

    pos_t Pos() const override { return Lhs[0]->Pos(); }

    pos_t End() const override { return Rhs.back()->End(); }
};

// TODO GoStmt (also need to support channels)

struct DeferStmt : Stmt {
    pos_t Defer;  // pos of defer keyword
    SP<CallExpr> Call;

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res += "DeferStmt <" + Pos().ToString() + " --- " + End().ToString() +
               ">\n";
        if (Call != nullptr) {
            res += Call->Print(shiftSize + 2);
        }
        return res;
    }

    DeferStmt(pos_t pos, SP<CallExpr> call) : Defer(pos), Call(call) {}

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }

    pos_t Pos() const override { return Defer; }

    pos_t End() const override { return Call->End(); }
};

struct ReturnStmt : Stmt {
    pos_t Return;
    V<SP<Expr>> Results;

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res += "ReturnStmt <" + Pos().ToString() + " --- " + End().ToString() +
               ">\n";
        if (!Results.empty()) {
            for (const auto& rs : Results) {
                if (rs) res += rs->Print(shiftSize + 2);
            }
        }
        return res;
    }

    ReturnStmt(pos_t pos, V<SP<Expr>> res) : Return(pos), Results(res) {}
    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }

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

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res += "BranchStmt <" + Pos().ToString() + " --- " + End().ToString() +
               ">\n";
        res += std::string(shiftChar, shiftSize + 1) + type_string_map.at(Tok) +
               '\n';
        if (Label != nullptr) {
            res += Label->Print(shiftSize + 2);
        }
        return res;
    }

    BranchStmt(pos_t pos, token_type tok, SP<IdentExpr> lbl)
        : TokPos(pos), Tok(tok), Label(lbl) {}

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }

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

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res +=
            "IfStmt <" + Pos().ToString() + " --- " + End().ToString() + ">\n";
        if (Init != nullptr) {
            res += Init->Print(shiftSize + 2);
        }
        if (Cond != nullptr) {
            res += Cond->Print(shiftSize + 2);
        }
        if (Body != nullptr) {
            res += Body->Print(shiftSize + 2);
        }
        if (Else != nullptr) {
            res += Else->Print(shiftSize + 2);
        }
        return res;
    }

    IfStmt(pos_t pos, SP<Stmt> init, SP<Expr> cond, SP<BlockStmt> body,
           SP<Stmt> els)
        : If(pos), Init(init), Cond(cond), Body(body), Else(els) {}
    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }

    pos_t Pos() const override { return If; }

    pos_t End() const override {
        if (Else != nullptr) {
            return Else->End();
        }
        return Body->End();
    }
};

struct CaseClauseStmt : Stmt {
    pos_t Case;  // position of "case" or "default" keyword
    V<SP<Expr>>
        List;  // list of expressions or types; nullptr means default case
    pos_t Colon;
    V<SP<Stmt>> Body;

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res += "CaseClauseStmt <" + Pos().ToString() + " --- " +
               End().ToString() + ">\n";
        if (!List.empty()) {
            for (const auto& l : List) {
                if (l) res += l->Print(shiftSize + 2);
            }
        }
        if (!Body.empty()) {
            for (const auto& b : Body) {
                if (b) res += b->Print(shiftSize + 2);
            }
        }
        return res;
    }

    CaseClauseStmt(pos_t pos, V<SP<Expr>> list, pos_t colon, V<SP<Stmt>> body)
        : Case(pos), List(list), Colon(colon), Body(body) {}

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }

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

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res += "SwitchStmt <" + Pos().ToString() + " --- " + End().ToString() +
               ">\n";
        if (Init != nullptr) {
            res += Init->Print(shiftSize + 2);
        }
        if (Tag != nullptr) {
            res += Tag->Print(shiftSize + 2);
        }
        if (Body != nullptr) {
            res += Body->Print(shiftSize + 2);
        }
        return res;
    }

    SwitchStmt(pos_t pos, SP<Stmt> init, SP<Expr> tag, SP<BlockStmt> body)
        : Switch(pos), Init(init), Tag(tag), Body(body) {}

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }

    pos_t Pos() const override { return Switch; }

    pos_t End() const override { return Body->End(); }
};

struct TypeSwitchStmt : Stmt {
    pos_t Switch;
    SP<Stmt> Init;       // initialization statement; or nullptr
    SP<Stmt> Assign;     // x := y.(type) or y.(type)
    SP<BlockStmt> Body;  // CaseClauses only

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res += "TypeSwitchExpr <" + Pos().ToString() + " --- " +
               End().ToString() + ">\n";
        if (Init != nullptr) {
            res += Init->Print(shiftSize + 2);
        }
        if (Assign != nullptr) {
            res += Assign->Print(shiftSize + 2);
        }
        if (Body != nullptr) {
            res += Body->Print(shiftSize + 2);
        }
        return res;
    }

    TypeSwitchStmt(pos_t pos, SP<Stmt> init, SP<Stmt> assign,
                   SP<BlockStmt> body)
        : Switch(pos), Init(init), Assign(assign), Body(body) {}

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }

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

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res +=
            "ForStmt <" + Pos().ToString() + " --- " + End().ToString() + ">\n";
        if (Init != nullptr) {
            res += Init->Print(shiftSize + 2);
        }
        if (Cond != nullptr) {
            res += Cond->Print(shiftSize + 2);
        }
        if (Post != nullptr) {
            res += Post->Print(shiftSize + 2);
        }
        if (Body != nullptr) {
            res += Body->Print(shiftSize + 2);
        }
        return res;
    }

    ForStmt(pos_t forPos, SP<Stmt> init, SP<Expr> cond, SP<Stmt> post,
            SP<BlockStmt> body)
        : For(forPos), Init(init), Cond(cond), Post(post), Body(body) {}

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }

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

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res += "RangeStmt <" + Pos().ToString() + " --- " + End().ToString() +
               ">\n";
        if (Key != nullptr) {
            res += Key->Print(shiftSize + 2);
        }
        if (Value != nullptr) {
            res += Value->Print(shiftSize + 2);
        }
        res += std::string(shiftChar, shiftSize + 1) + type_string_map.at(Tok) +
               '\n';
        res += std::string(shiftChar, shiftSize + 1) + "Range pos " +
               Range.ToString() + '\n';
        if (X != nullptr) {
            res += X->Print(shiftSize + 2);
        }
        if (Body != nullptr) {
            res += Body->Print(shiftSize + 2);
        }
        return res;
    }

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

    pos_t Pos() const override { return For; }

    pos_t End() const override { return Body->End(); }
};

struct EmptyStmt : Stmt {
    pos_t Semicolon;
    bool Implicit;

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res += "EmptyStmt <" + Pos().ToString() + " --- " + End().ToString() +
               ">\n";
        return res;
    }

    EmptyStmt(pos_t sc, bool implicit) : Semicolon(sc), Implicit(implicit) {}

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }

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

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res += "ImportSpec <" + Pos().ToString() + " --- " + End().ToString() +
               ">\n";
        if (Name != nullptr) {
            res += Name->Print(shiftSize + 2);
        }
        if (Path != nullptr) {
            res += Path->Print(shiftSize + 2);
        }
        return res;
    }

    ImportSpec(SP<IdentExpr> name, SP<BasicLitExpr> path, pos_t endPos)
        : Name(name), Path(path), EndPos(endPos) {}

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }

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

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res += "ValueSpec <" + Pos().ToString() + " --- " + End().ToString() +
               ">\n";
        if (!Names.empty()) {
            for (const auto& name : Names) {
                if (name) res += name->Print(shiftSize + 2);
            }
        }
        if (Type != nullptr) {
            res += Type->Print(shiftSize + 2);
        }
        if (!Values.empty()) {
            for (const auto& val : Values) {
                if (val) res += val->Print(shiftSize + 2);
            }
        }
        return res;
    }

    ValueSpec(V<SP<IdentExpr>> names, SP<Expr> type, V<SP<Expr>> values)
        : Names(names), Type(type), Values(values) {}

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }

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

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res += "TypeSpec <" + Pos().ToString() + " --- " + End().ToString() +
               ">\n";
        if (Name != nullptr) {
            res += Name->Print(shiftSize + 2);
        }
        if (TypeParams != nullptr) {
            res += TypeParams->Print(shiftSize + 2);
        }
        if (Type != nullptr) {
            res += Type->Print(shiftSize + 2);
        }
        return res;
    }

    TypeSpec(SP<IdentExpr> name, SP<FieldList> typeParams, pos_t assign,
             SP<Expr> type)
        : Name(name), TypeParams(typeParams), Assign(assign), Type(type) {}

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }

    pos_t Pos() const override { return Name->Pos(); }

    pos_t End() const override { return Type->End(); }
};

// end Spec

// Declarations

struct BadDecl : Decl {
    pos_t From;
    pos_t To;

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res +=
            "BadDecl <" + Pos().ToString() + " --- " + End().ToString() + ">\n";
        return res;
    }

    BadDecl(pos_t from, pos_t to) : From(from), To(to) {}

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }

    pos_t Pos() const override { return From; }

    pos_t End() const override { return To; }
};

struct GenDecl : Decl {
    pos_t TokPos;
    token_type Tok;  // IMPORT, CONST, TYPE, or VAR
    pos_t Lparen;
    pos_t Rparen;
    V<SP<Spec>> Specs;

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res +=
            "GenDecl <" + Pos().ToString() + " --- " + End().ToString() + ">\n";

        res += std::string(shiftChar, shiftSize + 1) + type_string_map.at(Tok) +
               '\n';
        if (!Specs.empty()) {
            for (const auto& sp : Specs) {
                if (sp) res += sp->Print(shiftSize + 2);
            }
        }
        return res;
    }

    GenDecl(pos_t tokPos, token_type tok, pos_t lparen, V<SP<Spec>> specs,
            pos_t rparen)
        : TokPos(tokPos),
          Tok(tok),
          Lparen(lparen),
          Rparen(rparen),
          Specs(specs) {}

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }

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

    virtual std::string Print(size_t shiftSize) const override {
        std::string res(shiftChar, shiftSize);
        res += "FuncDecl <" + Pos().ToString() + " --- " + End().ToString() +
               ">\n";
        if (Recv != nullptr) {
            res += Recv->Print(shiftSize + 2);
        }
        if (Name != nullptr) {
            res += Name->Print(shiftSize + 2);
        }
        if (Type != nullptr) {
            res += Type->Print(shiftSize + 2);
        }
        if (Body != nullptr) {
            res += Body->Print(shiftSize + 2);
        }
        return res;
    }

    FuncDecl(SP<FieldList> recv, SP<IdentExpr> name, SP<FuncTypeExpr> type,
             SP<BlockStmt> body)
        : Recv(recv), Name(name), Type(type), Body(body) {}

    virtual bool operator==(const Node& rhs) const override {
        return compare(*this, rhs);
    }

    pos_t Pos() const override { return Type->Pos(); }

    pos_t End() const override {
        if (Body != nullptr) {
            return Body->End();
        }
        return Type->End();
    }
};

// end Declarations