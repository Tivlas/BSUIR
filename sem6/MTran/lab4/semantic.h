#pragma once
#include "error.h"
#include "parser.h"
#include "scope.h"
#include "walk.h"

template <typename T>
SP<T> shared(T* ptr) {
    return std::make_shared<T>(*ptr);
}

constexpr int maxScopeDepth = 1e3;
auto unresolved = std::make_shared<Object>();

class semantic : public Visitor, public std::enable_shared_from_this<semantic> {
   private:
    parser parser;
    V<Error> errors_;
    V<SP<Decl>> decls_;

    // RESOLVER
    SP<Scope> pkgScope_;
    SP<Scope> topScope_;
    V<SP<IdentExpr>> unresolved_;
    int depth_;

    SP<Scope> labelScope_;
    V<V<SP<IdentExpr>>> targetStack_;

    void openScope(pos_t pos);
    void closeScope();
    void openLabelScope();
    void closeLabelScope();

    void declare(std::any decl, std::any data, SP<Scope> scope, ObjKind kind,
                 V<SP<IdentExpr>> idents);
    void shortVarDecl(SP<AssignStmt> decl);
    void resolve(SP<IdentExpr> ident, bool collectUnresolved);
    void walkExprs(V<SP<Expr>> list);
    void walkLHS(V<SP<Expr>> list);
    void walkStmts(V<SP<Stmt>> list);
    SP<Visitor> Visit(SP<Node> node) override;
    void walkFuncType(SP<FuncTypeExpr> typ);
    void resolveList(SP<FieldList> list);
    void declareList(SP<FieldList> list, ObjKind kind);
    void walkRecv(SP<FieldList> recv);
    void walkFieldList(SP<FieldList> list, ObjKind kind);
    void walkTParams(SP<FieldList> list);
    void walkBody(SP<BlockStmt> body);
    // END RESOLVER

    void error(pos_t pos, std::string msg);
    void resolveFile();
   public:
    semantic(const std::filesystem::path&);
    void analyze();
};

semantic::semantic(const std::filesystem::path& path) : parser(path) {
    parser.parseFile();
    errors_ = std::move(parser.getErrors());
    decls_ = std::move(parser.getDecls());
}

void semantic::error(pos_t pos, std::string msg) { errors_.push_back({pos, msg}); }

void semantic::openScope(pos_t pos) {
    depth_++;
    if (depth_ > maxScopeDepth) {
        throw std::runtime_error(pos.ToString() +
                                 " exceeded max scope depth during object resolution");
    }
    topScope_ = std::make_shared<Scope>(topScope_);
}

void semantic::closeScope() {
    depth_--;
    topScope_ = topScope_->Outer;
}

void semantic::openLabelScope() {
    labelScope_ = std::make_shared<Scope>(labelScope_);
    targetStack_.push_back({});
}

void semantic::closeLabelScope() {
    size_t n = targetStack_.size() - 1;
    auto scope = labelScope_;
    for (auto ident : targetStack_[n]) {
        ident->Obj = scope->Lookup(ident->Name);
        if (ident->Obj == nullptr) {
            errors_.push_back({ident->Pos(), "label " + ident->Name + " is undefined"});
        }
    }

    targetStack_.pop_back();
    labelScope_ = labelScope_->Outer;
}

void semantic::declare(std::any decl, std::any data, SP<Scope> scope, ObjKind kind,
                       V<SP<IdentExpr>> idents) {
    for (auto ident : idents) {
        if (ident->Obj != nullptr) {
            throw std::runtime_error(ident->Pos().ToString() + ": identifier" + ident->Name +
                                     " already declared or resolved");
        }
        auto obj = std::make_shared<Object>(kind, ident->Name);

        obj->Decl = decl;
        obj->Data = data;

        if (decl.type() != typeid(SP<IdentExpr>)) {
            ident->Obj = obj;
        }
        if (ident->Name != "_") {
            if (auto alt = scope->Insert(obj); alt != nullptr) {
                std::string prevDecl;
                if (auto pos = alt->Pos(); pos.IsValid()) {
                    prevDecl = "\n\tprevious declaration at " + pos.ToString();
                }
                errors_.push_back(
                    {ident->Pos(), ident->Name + "redeclared in this block" + prevDecl});
            }
        }
    }
}

void semantic::shortVarDecl(SP<AssignStmt> decl) {
    int n = 0;
    for (auto x : decl->Lhs) {
        if (auto [ident, isIdent] = isOfType<IdentExpr>(x.get()); isIdent) {
            assert(ident->Obj == nullptr, "identifier already declared or resolved");
            auto obj = std::make_shared<Object>(Var, ident->Name);
            obj->Decl = decl;
            ident->Obj = obj;
            if (ident->Name != "_") {
                if (auto alt = topScope_->Insert(obj); alt != nullptr) {
                    ident->Obj = alt;  // redeclaration
                } else {
                    n++;
                }
            }
        }
    }
    if (n == 0) {
        errors_.push_back({decl->Lhs[0]->Pos(), " no new variables on left side of :="});
    }
}

void semantic::resolve(SP<IdentExpr> ident, bool collectUnresolved) {
    if (ident->Obj != nullptr) {
        throw std::runtime_error(ident->Pos().ToString() + ": identifier" + ident->Name +
                                 " already declared or resolved");
    }
    if (ident->Name == "_") {
        return;
    }

    for (auto s = topScope_; s != nullptr; s = s->Outer) {
        if (auto obj = s->Lookup(ident->Name); obj != nullptr) {
            assert(obj->Name != "", "obj with no name");
            if (obj->Decl.type() != typeid(SP<IdentExpr>)) {
                ident->Obj = obj;
            }
            return;
        }
    }

    if (collectUnresolved) {
        ident->Obj = unresolved;
        unresolved_.push_back(ident);
    }
}

void semantic::walkExprs(V<SP<Expr>> list) {
    for (auto node : list) {
        Walk(shared_from_this(), node);
    }
}

void semantic::walkLHS(V<SP<Expr>> list) {
    for (auto expr : list) {
        expr = unparen(expr);
        if (auto [_, is] = isOfType<IdentExpr>(expr.get()); !is && expr != nullptr) {
            Walk(shared_from_this(), expr);
        }
    }
}

void semantic::walkStmts(V<SP<Stmt>> list) {
    for (auto stmt : list) {
        Walk(shared_from_this(), stmt);
    }
}

SP<Visitor> semantic::Visit(SP<Node> node) {
    const auto get = node.get();
    if (auto [n, is] = isOfType<IdentExpr>(get); is) {
        resolve(shared(n), true);
    } else if (auto [n, is] = isOfType<FuncLitExpr>(get); is) {
        openScope(n->Pos());
        walkFuncType(n->Type);
        walkBody(n->Body);
        closeScope();
    } else if (auto [n, is] = isOfType<SelectorExpr>(get); is) {
        Walk(shared_from_this(), n->X);
    } else if (auto [n, is] = isOfType<StructTypeExpr>(get); is) {
        openScope(n->Pos());
        walkFieldList(n->Fields, Var);
        closeScope();
    } else if (auto [n, is] = isOfType<FuncTypeExpr>(get); is) {
        openScope(n->Pos());
        walkFuncType(shared(n));
        closeScope();
    } else if (auto [n, is] = isOfType<CompositeLitExpr>(get); is) {
        if (n->Type != nullptr) {
            Walk(shared_from_this(), n->Type);
        }
        for (auto e : n->Elts) {
            if (auto [kv, _] = isOfType<KeyValueExpr>(e.get()); kv != nullptr) {
                if (auto [ident, _] = isOfType<IdentExpr>(kv->Key.get()); ident != nullptr) {
                    resolve(shared(ident), false);
                } else {
                    Walk(shared_from_this(), kv->Key);
                }
                Walk(shared_from_this(), kv->Value);
            } else {
                Walk(shared_from_this(), e);
            }
        }
    } else if (auto [n, is] = isOfType<InterfaceTypeExpr>(get); is) {
        openScope(n->Pos());
        walkFieldList(n->Methods, Fun);
        closeScope();
    } else if (auto [n, is] = isOfType<LabeledStmt>(get); is) {
        declare(shared(n), nullptr, labelScope_, Lbl, {n->Label});
        Walk(shared_from_this(), n->Stmt_);
    } else if (auto [n, is] = isOfType<AssignStmt>(get); is) {
        walkExprs(n->Rhs);
        if (n->Tok == token_type::DEFINE) {
            shortVarDecl(shared(n));
        } else {
            walkExprs(n->Lhs);
        }
    } else if (auto [n, is] = isOfType<BranchStmt>(get); is) {
        if (n->Tok != token_type::FALLTHROUGH && n->Label != nullptr) {
            auto depth = targetStack_.size() - 1;
            targetStack_[depth].push_back(n->Label);
        }
    } else if (auto [n, is] = isOfType<BlockStmt>(get); is) {
        openScope(n->Pos());
        walkStmts(n->List);
        closeScope();
    } else if (auto [n, is] = isOfType<IfStmt>(get); is) {
        openScope(n->Pos());
        if (n->Init != nullptr) {
            Walk(shared_from_this(), n->Init);
        }
        Walk(shared_from_this(), n->Cond);
        Walk(shared_from_this(), n->Body);
        if (n->Else != nullptr) {
            Walk(shared_from_this(), n->Else);
        }
        closeScope();
    } else if (auto [n, is] = isOfType<CaseClauseStmt>(get); is) {
        walkExprs(n->List);
        openScope(n->Pos());
        walkStmts(n->Body);
        closeScope();
    } else if (auto [n, is] = isOfType<SwitchStmt>(get); is) {
        openScope(n->Pos());
        if (n->Init != nullptr) {
            Walk(shared_from_this(), n->Init);
        }
        if (n->Tag != nullptr) {
            if (n->Init != nullptr) {
                openScope(n->Pos());
            }
            Walk(shared_from_this(), n->Tag);
        }
        if (n->Body != nullptr) {
            Walk(shared_from_this(), n->Body);
        }
        closeScope();
        if (n->Init != nullptr) closeScope();
    } else if (auto [n, is] = isOfType<TypeSwitchStmt>(get); is) {
        if (n->Init != nullptr) {
            openScope(n->Pos());
            Walk(shared_from_this(), n->Init);
        }
        openScope(n->Pos());
        Walk(shared_from_this(), n->Assign);
        if (n->Body != nullptr) {
            walkStmts(n->Body->List);
        }
        closeScope();
        if (n->Init != nullptr) closeScope();
    }  // TODO comm clause, select stmt
    else if (auto [n, is] = isOfType<ForStmt>(get); is) {
        openScope(n->Pos());
        if (n->Init != nullptr) {
            Walk(shared_from_this(), n->Init);
        }
        if (n->Cond != nullptr) {
            Walk(shared_from_this(), n->Cond);
        }
        if (n->Post != nullptr) {
            Walk(shared_from_this(), n->Post);
        }
        Walk(shared_from_this(), n->Body);
        closeScope();
    } else if (auto [n, is] = isOfType<RangeStmt>(get); is) {
        openScope(n->Pos());
        Walk(shared_from_this(), n->X);
        V<SP<Expr>> lhs;
        if (n->Key != nullptr) {
            lhs.push_back(n->Key);
        }
        if (n->Value != nullptr) {
            lhs.push_back(n->Value);
        }
        if (lhs.size() > 0) {
            if (n->Tok == token_type::DEFINE) {
                auto as = std::make_shared<AssignStmt>(
                    lhs, n->TokPos, token_type::DEFINE,
                    V<SP<Expr>>{std::make_shared<UnaryExpr>(NoPos, token_type::RANGE, n->X)});
                walkLHS(lhs);
                shortVarDecl(as);
            } else {
                walkExprs(lhs);
            }
        }
        Walk(shared_from_this(), n->Body);
        closeScope();
    } else if (auto [n, is] = isOfType<GenDecl>(get); is) {
        switch (n->Tok) {
            case token_type::CONST:
            case token_type::VAR: {
                for (size_t i = 0; i < n->Specs.size(); i++) {
                    auto spec = isOfType<ValueSpec>(n->Specs[i].get()).first;
                    auto kind = Con;
                    if (n->Tok == token_type::VAR) {
                        kind = Var;
                    }
                    walkExprs(spec->Values);
                    if (spec->Type != nullptr) {
                        Walk(shared_from_this(), spec->Type);
                    }
                    declare(shared(spec), i, topScope_, kind, spec->Names);
                }
                break;
            }
            case token_type::TYPE: {
                for (size_t i = 0; i < n->Specs.size(); i++) {
                    auto spec = isOfType<TypeSpec>(n->Specs[i].get()).first;
                    declare(shared(spec), nullptr, topScope_, Typ, {spec->Name});
                    if (spec->TypeParams != nullptr) {
                        openScope(spec->Pos());
                        walkTParams(spec->TypeParams);
                    }
                    Walk(shared_from_this(), spec->Type);
                    if (spec->TypeParams != nullptr) closeScope();
                }
                break;
            }
            default:
                break;
        }
    } else if (auto [n, is] = isOfType<FuncDecl>(get); is) {
        openScope(n->Pos());
        walkRecv(n->Recv);
        if (n->Type->TypeParams != nullptr) {
            walkTParams(n->Type->TypeParams);
        }
        resolveList(n->Type->Params);
        resolveList(n->Type->Results);
        declareList(n->Recv, Var);
        declareList(n->Type->Params, Var);
        declareList(n->Type->Results, Var);

        walkBody(n->Body);
        if (n->Recv == nullptr && n->Name->Name != "init") {
            declare(shared(n), nullptr, pkgScope_, Fun, {n->Name});
        }
        closeScope();
    } else {
        return shared_from_this();
    }

    return nullptr;
}

void semantic::walkFuncType(SP<FuncTypeExpr> typ) {
    resolveList(typ->Params);
    resolveList(typ->Results);
    declareList(typ->Params, Var);
    declareList(typ->Results, Var);
}

void semantic::resolveList(SP<FieldList> list) {
    if (list == nullptr) {
        return;
    }
    for (auto f : list->List) {
        if (f->Type != nullptr) {
            Walk(shared_from_this(), f->Type);
        }
    }
}

void semantic::declareList(SP<FieldList> list, ObjKind kind) {
    if (list == nullptr) {
        return;
    }
    for (auto f : list->List) {
        declare(f, nullptr, topScope_, kind, f->Names);
    }
}

void semantic::walkRecv(SP<FieldList> recv) {
    if (recv == nullptr || recv->List.size() == 0) {
        return;
    }
    auto typ = recv->List[0]->Type;
    if (auto [ptr, is] = isOfType<StarExpr>(typ.get()); is) {
        typ = ptr->X;
    }

    V<SP<Expr>> declareExprs, resolveExprs;
    if (auto [typ2, is] = isOfType<IndexExpr>(typ.get()); is) {
        declareExprs = {typ2->Index};
        resolveExprs.push_back(typ2->X);
    } else if (auto [typ2, is] = isOfType<IndexListExpr>(typ.get()); is) {
        declareExprs = typ2->Indeces;
        resolveExprs.push_back(typ2->X);
    } else {
        resolveExprs.push_back(typ);
    }

    for (auto expr : declareExprs) {
        if (auto [id, _] = isOfType<IdentExpr>(expr.get()); id != nullptr) {
            declare(expr, nullptr, topScope_, Typ, {shared(id)});
        } else {
            resolveExprs.push_back(expr);
        }
    }

    for (auto expr : resolveExprs) {
        if (expr != nullptr) {
            Walk(shared_from_this(), expr);
        }
    }

    for (size_t i = 1; i < recv->List.size(); i++) {
        auto f = recv->List[i];
        if (f->Type != nullptr) {
            Walk(shared_from_this(), f->Type);
        }
    }
}

void semantic::walkFieldList(SP<FieldList> list, ObjKind kind) {
    if (list == nullptr) {
        return;
    }
    resolveList(list);
    declareList(list, kind);
}

void semantic::walkTParams(SP<FieldList> list) {
    declareList(list, Typ);
    resolveList(list);
}

void semantic::walkBody(SP<BlockStmt> body) {
    if (body == nullptr) {
        return;
    }
    openLabelScope();
    walkStmts(body->List);
    closeLabelScope();
}

void semantic::resolveFile() {
    pkgScope_ = std::make_shared<Scope>(nullptr);
    topScope_ = std::make_shared<Scope>(nullptr);
    depth_ = 1;

    for (auto decl: decls_) {
        Walk(shared_from_this(), decl);
    }

    closeScope();
    assert(topScope_ == nullptr, "unbalanced scopes");
    assert(labelScope_ == nullptr, "unbalanced label scopes");

    // TODO rest code
    size_t i = 0; 
    for (auto ident: unresolved_) {
        assert(ident->Obj == unresolved, "object already resolved");
        ident->Obj = pkgScope_->Lookup(ident->Name);
        if (ident->Obj == nullptr) {
            unresolved_[i] = ident;
            i++;
        }
        //errors_.push_back({ident->Pos(), "unknown var"});
    }
}

void semantic::analyze() {
    resolveFile();

    for (auto err: errors_) {
        std::cout << err.pos.ToString() + "---" + err.msg << "\n";
    }
}