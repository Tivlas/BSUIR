#pragma once
#include <functional>

#include "parser.h"
#include "tree.h"

struct Visitor {
    virtual SP<Visitor> Visit(SP<Node> n) = 0;
};

using inspectorFunc = std::function<bool(SP<Node>)>;

struct Inspector : Visitor, std::enable_shared_from_this<Inspector> {
    inspectorFunc f;

    Inspector(inspectorFunc f) : f(f) {}

    SP<Visitor> Visit(SP<Node> n) override {
        if (f(n)) {
            return shared_from_this();
        }
    }
};

void Walk(SP<Visitor> v, SP<Node> n);

void Inspect(SP<Node> node, SP<Inspector> f) { Walk(f, node); }

void walkIdentList(SP<Visitor> v, V<SP<IdentExpr>> list) {
    for (auto x : list) {
        Walk(v, x);
    }
}

void walkExprList(SP<Visitor> v, V<SP<Expr>> list) {
    for (auto x : list) {
        Walk(v, x);
    }
}

void walkStmtList(SP<Visitor> v, V<SP<Stmt>> list) {
    for (auto x : list) {
        Walk(v, x);
    }
}

void walkDeclList(SP<Visitor> v, V<SP<Decl>> list) {
    for (auto x : list) {
        Walk(v, x);
    }
}

void Walk(SP<Visitor> v, SP<Node> node) {
    if (v = v->Visit(node); v == nullptr) {
        return;
    }

    auto GET = node.get();

    if (auto [n, is] = isOfType<Field>(GET); is) {
        walkIdentList(v, n->Names);
        if (n->Type != nullptr) {
            Walk(v, n->Type);
        }
        if (n->Tag != nullptr) {
            Walk(v, n->Tag);
        }
    } else if (auto [n, is] = isOfType<FieldList>(GET); is) {
        for (auto f : n->List) {
            Walk(v, f);
        }
    } else if (isOfType<BadExpr>(GET).second || isOfType<IdentExpr>(GET).second ||
               isOfType<BasicLitExpr>(GET).second) {
    } else if (auto [n, is] = isOfType<EllipsisExpr>(GET); is) {
        if (n->Elt != nullptr) {
            Walk(v, n->Elt);
        }
    } else if (auto [n, is] = isOfType<FuncLitExpr>(GET); is) {
        Walk(v, n->Type);
        Walk(v, n->Body);
    } else if (auto [n, is] = isOfType<CompositeLitExpr>(GET); is) {
        if (n->Type != nullptr) {
            Walk(v, n->Type);
        }
        walkExprList(v, n->Elts);
    } else if (auto [n, is] = isOfType<ParenExpr>(GET); is) {
        Walk(v, n->X);
    } else if (auto [n, is] = isOfType<SelectorExpr>(GET); is) {
        Walk(v, n->X);
        Walk(v, n->Sel);
    } else if (auto [n, is] = isOfType<IndexExpr>(GET); is) {
        Walk(v, n->X);
        Walk(v, n->Index);
    } else if (auto [n, is] = isOfType<IndexListExpr>(GET); is) {
        Walk(v, n->X);
        for (auto index : n->Indeces) {
            Walk(v, index);
        }
    } else if (auto [n, is] = isOfType<SliceExpr>(GET); is) {
        Walk(v, n->X);
        if (n->Low != nullptr) {
            Walk(v, n->Low);
        }
        if (n->High != nullptr) {
            Walk(v, n->High);
        }
        if (n->Max != nullptr) {
            Walk(v, n->Max);
        }
    } else if (auto [n, is] = isOfType<TypeAssertExpr>(GET); is) {
        Walk(v, n->X);
        if (n->Type != nullptr) {
            Walk(v, n->Type);
        }
    } else if (auto [n, is] = isOfType<CallExpr>(GET); is) {
        Walk(v, n->Fun);
        walkExprList(v, n->Args);
    } else if (auto [n, is] = isOfType<StarExpr>(GET); is) {
        Walk(v, n->X);
    } else if (auto [n, is] = isOfType<UnaryExpr>(GET); is) {
        Walk(v, n->X);
    } else if (auto [n, is] = isOfType<BinaryExpr>(GET); is) {
        Walk(v, n->X);
        Walk(v, n->Y);
    } else if (auto [n, is] = isOfType<KeyValueExpr>(GET); is) {
        Walk(v, n->Key);
        Walk(v, n->Value);
    } else if (auto [n, is] = isOfType<ArrayTypeExpr>(GET); is) {
        if (n->Len != nullptr) {
            Walk(v, n->Len);
        }
        Walk(v, n->Elt);
    } else if (auto [n, is] = isOfType<StructTypeExpr>(GET); is) {
        Walk(v, n->Fields);
    } else if (auto [n, is] = isOfType<FuncTypeExpr>(GET); is) {
        if (n->TypeParams != nullptr) {
            Walk(v, n->TypeParams);
        }
        if (n->Params != nullptr) {
            Walk(v, n->Params);
        }
        if (n->Results != nullptr) {
            Walk(v, n->Results);
        }
    } else if (auto [n, is] = isOfType<InterfaceTypeExpr>(GET); is) {
        Walk(v, n->Methods);
    } else if (auto [n, is] = isOfType<MapTypeExpr>(GET); is) {
        Walk(v, n->Key);
        Walk(v, n->Value);
        // TODO chan type expr
    } else if (auto [n, is] = isOfType<BadStmt>(GET); is) {
    } else if (auto [n, is] = isOfType<DeclStmt>(GET); is) {
        Walk(v, n->Decl_);
    } else if (auto [n, is] = isOfType<EmptyStmt>(GET); is) {
    } else if (auto [n, is] = isOfType<LabeledStmt>(GET); is) {
        Walk(v, n->Label);
        Walk(v, n->Stmt_);
    } else if (auto [n, is] = isOfType<ExprStmt>(GET); is) {
        Walk(v, n->X);
        // TODO send stmt
    } else if (auto [n, is] = isOfType<IncDecStmt>(GET); is) {
        Walk(v, n->X);
    } else if (auto [n, is] = isOfType<AssignStmt>(GET); is) {
        walkExprList(v, n->Lhs);
        walkExprList(v, n->Rhs);
        // TODO go stmt
    } else if (auto [n, is] = isOfType<DeferStmt>(GET); is) {
        Walk(v, n->Call);
    } else if (auto [n, is] = isOfType<ReturnStmt>(GET); is) {
        walkExprList(v, n->Results);
    } else if (auto [n, is] = isOfType<BranchStmt>(GET); is) {
        if (n->Label != nullptr) {
            Walk(v, n->Label);
        }
    } else if (auto [n, is] = isOfType<BlockStmt>(GET); is) {
        walkStmtList(v, n->List);
    } else if (auto [n, is] = isOfType<IfStmt>(GET); is) {
        if (n->Init != nullptr) {
            Walk(v, n->Init);
        }
        Walk(v, n->Cond);
        Walk(v, n->Body);
        if (n->Else != nullptr) {
            Walk(v, n->Else);
        }
    } else if (auto [n, is] = isOfType<CaseClauseStmt>(GET); is) {
        walkExprList(v, n->List);
        walkStmtList(v, n->Body);
    } else if (auto [n, is] = isOfType<SwitchStmt>(GET); is) {
        if (n->Init != nullptr) {
            Walk(v, n->Init);
        }
        if (n->Tag != nullptr) {
            Walk(v, n->Tag);
        }
        Walk(v, n->Body);
    } else if (auto [n, is] = isOfType<TypeSwitchStmt>(GET); is) {
        if (n->Init != nullptr) {
            Walk(v, n->Init);
        }
        Walk(v, n->Assign);
        Walk(v, n->Body);
        // TODO comm clause
        // TODO select stmt
    } else if (auto [n, is] = isOfType<ForStmt>(GET); is) {
        if (n->Init != nullptr) {
            Walk(v, n->Init);
        }
        if (n->Cond != nullptr) {
            Walk(v, n->Cond);
        }
        if (n->Post != nullptr) {
            Walk(v, n->Post);
        }
        Walk(v, n->Body);
    } else if (auto [n, is] = isOfType<RangeStmt>(GET); is) {
        if (n->Key != nullptr) {
            Walk(v, n->Key);
        }
        if (n->Value != nullptr) {
            Walk(v, n->Value);
        }
        Walk(v, n->X);
        Walk(v, n->Body);
    } else if (auto [n, is] = isOfType<ImportSpec>(GET); is) {
        if (n->Name != nullptr) {
            Walk(v, n->Name);
        }
        Walk(v, n->Path);
    } else if (auto [n, is] = isOfType<ValueSpec>(GET); is) {
        walkIdentList(v, n->Names);
        if (n->Type != nullptr) {
            Walk(v, n->Type);
        }
        walkExprList(v, n->Values);
    } else if (auto [n, is] = isOfType<TypeSpec>(GET); is) {
        Walk(v, n->Name);
        if (n->TypeParams != nullptr) {
            Walk(v, n->TypeParams);
        }
        Walk(v, n->Type);
    } else if (auto [n, is] = isOfType<BadDecl>(GET); is) {
    } else if (auto [n, is] = isOfType<GenDecl>(GET); is) {
        for (auto s : n->Specs) {
            Walk(v, s);
        }
    } else if (auto [n, is] = isOfType<FuncDecl>(GET); is) {
        if (n->Recv != nullptr) {
            Walk(v, n->Recv);
        }
        Walk(v, n->Name);
        Walk(v, n->Type);
        if (n->Body != nullptr) {
            Walk(v, n->Body);
        }
    }

    v->Visit(nullptr);
}