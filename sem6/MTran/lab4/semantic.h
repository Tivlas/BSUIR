#pragma once
#include "error.h"
#include "parser.h"
#include "types.h"
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

    void declare(SP<Node> decl, SP<Expr> type, SP<Scope> scope, ObjKind kind,
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
    bool checkDeclarationLeftType(SP<Expr> type);
    bool checkExprType(SP<Expr> expr, SP<Expr> typeToBeEqualTo);
    SP<Expr> validateExprAndReturnType(SP<Expr> expr);
    SP<Expr> callExprType(SP<Expr> call);
    bool isUnknown(SP<IdentExpr> ident);
    void compareCallArgs(V<SP<Expr>> callArgs, SP<FieldList> realArgs);
    SP<IdentExpr> getFuncName(SP<CallExpr> call);
    bool checkConstChange(SP<Expr> expr);
    SP<Expr> checkSelectorFieldAndReturnItsType(SP<SelectorExpr> n);
    // END RESOLVER

    void error(pos_t pos, std::string msg);
    void resolveFile();

   public:
    semantic(const std::filesystem::path&);
    void analyze();
};

semantic::semantic(const std::filesystem::path& path) : parser(path) {
    parser.parseFile();
    std::cout << parser.getTreeStr() << "\n";
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

void semantic::declare(SP<Node> decl, SP<Expr> type, SP<Scope> scope, ObjKind kind,
                       V<SP<IdentExpr>> idents) {
    for (auto ident : idents) {
        if (ident->Obj != nullptr) {
            throw std::runtime_error(ident->Pos().ToString() + ": identifier" + ident->Name +
                                     " already declared or resolved");
        }
        auto obj = std::make_shared<Object>(kind, ident->Name);

        obj->Decl = decl;
        obj->Type = type;

        if (isOfType<IdentExpr>(decl.get()).second == false) {
            ident->Obj = obj;
        }
        if (ident->Name != "_") {
            if (auto alt = scope->Insert(obj); alt != nullptr) {
                std::string prevDecl;
                if (auto pos = alt->Pos(); pos.IsValid()) {
                    prevDecl = "\n\tprevious declaration at " + pos.ToString();
                }
                errors_.push_back(
                    {ident->Pos(), ident->Name + " redeclared in this block" + prevDecl});
            }
        }
    }
}

void semantic::shortVarDecl(SP<AssignStmt> decl) {
    int n = 0;
    if (decl->Lhs.size() != decl->Rhs.size())
        throw std::runtime_error(decl->Pos().ToString() + " different number of values");
    size_t idx = 0;
    for (auto x : decl->Lhs) {
        if (auto [ident, isIdent] = isOfType<IdentExpr>(x.get()); isIdent) {
            assert(ident->Obj == nullptr, "identifier already declared or resolved");
            auto obj = std::make_shared<Object>(Var, ident->Name);
            obj->Decl = decl;
            obj->Type = validateExprAndReturnType(decl->Rhs[idx]);
            idx++;
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
            if (isOfType<IdentExpr>(obj->Decl.get()).second == false) {
                ident->Obj = obj;
            }
            return;
        }
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

bool semantic::checkConstChange(SP<Expr> expr) {
    if (auto [ident, is] = isOfType<IdentExpr>(expr.get()); is) {
        for (auto sc = topScope_; sc != nullptr; sc = sc->Outer) {
            auto obj = sc->Lookup(ident->Name);
            if (obj != nullptr && obj->Kind == Con) {
                return true;
            }
        }
    }
    return false;
}

SP<Expr> semantic::checkSelectorFieldAndReturnItsType(SP<SelectorExpr> n) {
    auto typ = validateExprAndReturnType(n->X);
    auto typIdent = isOfType<IdentExpr>(typ.get()).first;
    for (auto sc = topScope_; sc != nullptr; sc = sc->Outer) {
        auto obj = sc->Lookup(typIdent->Name);
        if (obj != nullptr && obj->Kind == Typ) {
            auto decl = obj->Decl;
            auto typeSpec = isOfType<TypeSpec>(decl.get()).first;
            auto sDecl = isOfType<StructTypeExpr>(typeSpec->Type.get()).first;
            if (sDecl == nullptr || sDecl->Fields == nullptr) {
                throw std::runtime_error(n->Pos().ToString() + " no such field");
            }
            for (auto field : sDecl->Fields->List) {
                for (auto name : field->Names) {
                    if (n->Sel->Name == name->Name) {
                        return field->Type;
                    }
                }
            }
            throw std::runtime_error(n->Pos().ToString() + " no such field");
        }
    }
    return nullptr;
}

SP<Visitor> semantic::Visit(SP<Node> node) {
    const auto get = node.get();
    if (auto [n, is] = isOfType<IdentExpr>(get); is) {
        resolve(shared(n), true);
    } else if (auto [n, is] = isOfType<CallExpr>(get); is) {
        validateExprAndReturnType(shared(n));
    } else if (auto [n, is] = isOfType<FuncLitExpr>(get); is) {
        openScope(n->Pos());
        walkFuncType(n->Type);
        walkBody(n->Body);
        closeScope();
    } else if (auto [n, is] = isOfType<SelectorExpr>(get); is) {
        checkSelectorFieldAndReturnItsType(shared(n));
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
    } else if (auto [n, is] = isOfType<IncDecStmt>(get); is) {
        validateExprAndReturnType(n->X);
        if (checkConstChange(n->X)) {
            throw std::runtime_error(n->Pos().ToString() +
                                     " ++ and -- are not allowed on const value");
        }
        Walk(shared_from_this(), n->X);
    } else if (auto [n, is] = isOfType<AssignStmt>(get); is) {
        walkExprs(n->Rhs);
        if (n->Tok == token_type::DEFINE) {
            shortVarDecl(shared(n));
        } else {
            walkExprs(n->Lhs);
            if (n->Lhs.size() != n->Rhs.size()) {
                throw std::runtime_error(n->Pos().ToString() +
                                         " diffenert number of values in assignment");
            }
            for (size_t i = 0; i < n->Lhs.size(); i++) {
                auto lExpr = n->Lhs[i];
                if (checkConstChange(lExpr)) {
                    throw std::runtime_error(n->Pos().ToString() + " assigning to const value");
                }
                auto rType = validateExprAndReturnType(n->Rhs[i]);
                if (checkExprType(lExpr, rType) == false) {
                    throw std::runtime_error(n->Pos().ToString() +
                                             " diffenert types of values in assignment");
                }
            }
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
                    if (spec->Names.size() != spec->Values.size()) {
                        throw std::runtime_error(spec->Pos().ToString() +
                                                 " different number of values in declaration");
                    }
                    auto kind = Con;
                    if (n->Tok == token_type::VAR) {
                        kind = Var;
                    }
                    walkExprs(spec->Values);
                    if (spec->Type != nullptr) {
                        if (checkDeclarationLeftType(spec->Type) == false) {
                            throw std::runtime_error(spec->Pos().ToString() + " invalid type");
                        }
                        for (auto e : spec->Values) {
                            if (checkExprType(e, spec->Type) == false) {
                                throw std::runtime_error(e->Pos().ToString() + " invalid type");
                            }
                        }
                        declare(shared(spec), spec->Type, topScope_, kind, spec->Names);
                    } else {
                        size_t idx = 0;
                        for (auto name : spec->Names) {
                            declare(shared(spec), validateExprAndReturnType(spec->Values[idx]),
                                    topScope_, kind, {name});
                            idx++;
                        }
                    }
                }
                break;
            }
            case token_type::TYPE: {
                for (size_t i = 0; i < n->Specs.size(); i++) {
                    auto spec = isOfType<TypeSpec>(n->Specs[i].get()).first;
                    declare(shared(spec), spec->Name, topScope_, Typ, {spec->Name});
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
        declare(shared(n), n->Type, pkgScope_, Fun, {n->Name});
        closeScope();
    } else {
        return shared_from_this();
    }

    return nullptr;
}

bool semantic::checkDeclarationLeftType(SP<Expr> type) {
    if (auto [ident, is] = isOfType<IdentExpr>(type.get()); is) {
        if (BUILT_IN_TYPES.contains(ident->Name)) {
            return true;
        }
        for (auto sc = topScope_; sc != nullptr; sc = sc->Outer) {
            auto obj = sc->Lookup(ident->Name);
            if (obj != nullptr && obj->Kind == Typ) return true;
        }
    }
    return false;
}

// First arg will be validated, do not validate it manually before calling this method
bool semantic::checkExprType(SP<Expr> expr, SP<Expr> typeToBeEqualTo) {
    auto typ = validateExprAndReturnType(expr);
    if (typ == nullptr) return false;
    return *typ == *typeToBeEqualTo;
}

void semantic::compareCallArgs(V<SP<Expr>> callArgs, SP<FieldList> realArgs) {
    size_t len = 0;
    for (auto f : realArgs->List) {
        len += f->Names.size();
    }
    if (len != callArgs.size())
        throw std::runtime_error(callArgs[0]->Pos().ToString() +
                                 " invalid args number in func call");
    size_t idx = 0;
    for (auto f : realArgs->List) {
        for (auto name : f->Names) {
            if (checkExprType(callArgs[idx], f->Type) == false)
                throw std::runtime_error(callArgs[0]->Pos().ToString() +
                                         " invalid args types in func call");
            idx++;
        }
    }
}

SP<IdentExpr> semantic::getFuncName(SP<CallExpr> call) {
    if (auto [ident, is] = isOfType<IdentExpr>(call->Fun.get()); is) {
        return shared(ident);
    } else if (auto [sel, is] = isOfType<SelectorExpr>(call->Fun.get()); is) {
        return sel->Sel;
    }
    return nullptr;
}

SP<Expr> semantic::callExprType(SP<Expr> call) {
    if (auto [ptr, is] = isOfType<CallExpr>(call.get()); is) {
        bool isMethod = isOfType<SelectorExpr>(ptr->Fun.get()).second;
        auto typ = callExprType(ptr->Fun);
        auto ident = getFuncName(shared(ptr));
        bool found = false;
        for (auto sc = pkgScope_; sc != nullptr; sc = sc->Outer) {
            auto obj = sc->Lookup(ident->Name);
            if (obj != nullptr && obj->Kind == Fun) {
                found = true;
                auto decl = obj->Decl;
                if (auto [funcDecl, is] = isOfType<FuncDecl>(decl.get()); is) {
                    auto funcType = funcDecl->Type;
                    auto params = funcType->Params;
                    if (isMethod && funcDecl->Recv == nullptr) {
                        throw std::runtime_error(ptr->Pos().ToString() + " no such method");
                    }
                    if (!isMethod && funcDecl->Recv != nullptr) {
                        throw std::runtime_error(ptr->Pos().ToString() + " no such function");
                    }
                    compareCallArgs(ptr->Args, params);
                }
            }
        }
        if (!found) {
            throw std::runtime_error(ptr->Pos().ToString() + " no such function or method");
        }
        return typ;
    } else if (auto [ptr, is] = isOfType<SelectorExpr>(call.get()); is) {
        auto typ = validateExprAndReturnType(ptr->X);
        auto typIdent = isOfType<IdentExpr>(typ.get()).first;
        bool found = false;
        for (auto sc = pkgScope_; sc != nullptr; sc = sc->Outer) {
            auto obj = sc->Lookup(ptr->Sel->Name);
            if (obj != nullptr && obj->Kind == Fun) {
                found = true;
                auto decl = obj->Decl;
                auto funcDecl = isOfType<FuncDecl>(decl.get()).first;
                if (funcDecl->Recv != nullptr) {
                    auto recv = funcDecl->Recv;
                    for (auto f : recv->List) {
                        if (auto [starE, is] = isOfType<StarExpr>(f->Type.get()); is) {
                            auto recvType = isOfType<IdentExpr>(starE->X.get()).first;
                            if (recvType->Name != typIdent->Name) {
                                throw std::runtime_error(ptr->Pos().ToString() + " no such method");
                            }
                        } else if (auto [ident, is] = isOfType<IdentExpr>(f->Type.get()); is) {
                            if (ident->Name != typIdent->Name) {
                                throw std::runtime_error(ptr->Pos().ToString() + " no such method");
                            }
                        }
                    }
                } else {
                    throw std::runtime_error(ptr->Pos().ToString() + " no such method");
                }
            }
        }
        if (!found) {
            throw std::runtime_error(ptr->Pos().ToString() + " no such function or method");
        }
        return callExprType(ptr->Sel);
    } else if (auto [ident, is] = isOfType<IdentExpr>(call.get()); is) {
        for (auto sc = pkgScope_; sc != nullptr; sc = sc->Outer) {
            auto obj = sc->Lookup(ident->Name);
            if (obj != nullptr && obj->Kind == Fun) {
                auto decl = obj->Decl;
                if (auto [funcDecl, is] = isOfType<FuncDecl>(decl.get()); is) {
                    auto funcType = funcDecl->Type;
                    auto results = funcType->Results;
                    if (results == nullptr || results->List.empty()) return nullptr;
                    auto res = results->List[0];
                    return res->Type;
                }
            }
        }
    }
    return nullptr;
}

bool semantic::isUnknown(SP<IdentExpr> ident) {
    for (auto sc = topScope_; sc != nullptr; sc = sc->Outer) {
        auto obj = sc->Lookup(ident->Name);
        if (obj != nullptr && (obj->Kind == Var || obj->Kind == Con)) {
            return false;
        }
    }
    return true;
}

SP<Expr> semantic::validateExprAndReturnType(SP<Expr> expr) {
    const auto get = expr.get();
    if (auto [ptr, is] = isOfType<ParenExpr>(get); is) {
        return validateExprAndReturnType(ptr->X);
    } else if (auto [ptr, is] = isOfType<BinaryExpr>(get); is) {
        auto l = validateExprAndReturnType(ptr->X);
        auto r = validateExprAndReturnType(ptr->Y);
        if (*l != *r) throw std::runtime_error(l->Pos().ToString() + " invalid types");

        auto typ = isOfType<IdentExpr>(l.get()).first;  // TODO check
        if (ptr->Op == token_type::ADD && typ->Name == "string") {
            return l;
        } else if (ptr->Op > token_type::logical_operators_beg &&
                   ptr->Op < token_type::logical_operators_end && typ->Name != "bool") {
            throw std::runtime_error(ptr->Pos().ToString() +
                                     " cannot use logical operators with not bool type");
        } else if (ptr->Op > token_type::numeric_operators_beg &&
                   ptr->Op < token_type::numeric_operators_end &&
                   !NUMERIC_TYPES.contains(typ->Name)) {
            throw std::runtime_error(ptr->Pos().ToString() +
                                     " cannot use numeric operator with non numeric type");
        } else if (ptr->Op > token_type::compare_operators_beg &&
                   ptr->Op < token_type::compare_operators_end) {
            if (BUILT_IN_TYPES.contains(typ->Name)) {
                return std::make_shared<IdentExpr>(ptr->Pos(), "bool");
            }
            throw std::runtime_error(ptr->Pos().ToString() +
                                     " cannot use comparison operator with non built-in type");
        }
        return l;
    } else if (auto [ptr, is] = isOfType<UnaryExpr>(get); is) {
        auto typ = validateExprAndReturnType(ptr->X);
        auto t = isOfType<IdentExpr>(typ.get()).first;  // TODO check
        if (ptr->Op == token_type::NOT) {
            if (t->Name == "bool") return std::make_shared<IdentExpr>(ptr->Pos(), "bool");
            throw std::runtime_error(ptr->Pos().ToString() + " cannot use ! on not bool type");
        } else if ((ptr->Op == token_type::INC || ptr->Op == token_type::DEC ||
                    ptr->Op == token_type::ADD || ptr->Op == token_type::SUB) &&
                   !NUMERIC_TYPES.contains(t->Name)) {
            throw std::runtime_error(ptr->Pos().ToString() +
                                     " cannot use \'++ -- + -\' on not numeric type");
        }
        return typ;
    } else if (auto [ptr, is] = isOfType<CallExpr>(get); is) {
        return callExprType(expr);
    } else if (auto [ptr, is] = isOfType<SelectorExpr>(get); is) {
        return checkSelectorFieldAndReturnItsType(shared(ptr));
    } else if (auto [ptr, is] = isOfType<CompositeLitExpr>(get); is) {
        return ptr->Type;
    } else if (auto [ptr, is] = isOfType<BasicLitExpr>(get); is) {
        switch (ptr->Kind) {
            case token_type::INT: {
                return std::make_shared<IdentExpr>(ptr->Pos(), "int");
            }
            case token_type::STRING: {
                return std::make_shared<IdentExpr>(ptr->Pos(), "string");
            }
            case token_type::CHAR: {
                return std::make_shared<IdentExpr>(ptr->Pos(), "rune");
            }
            case token_type::FLOAT: {
                return std::make_shared<IdentExpr>(ptr->Pos(), "float64");
            }
            case token_type::IMAG: {
                return std::make_shared<IdentExpr>(ptr->Pos(), "complex64");
            }

            default:
                return nullptr;
        }
    } else if (auto [ptr, is] = isOfType<IdentExpr>(get); is) {
        if (isUnknown(shared(ptr)))
            throw std::runtime_error(ptr->Pos().ToString() + " unknown ident " + ptr->Name);
        if (ptr->Name == "false" || ptr->Name == "true")
            return std::make_shared<IdentExpr>(ptr->Pos(), "bool");
        for (auto sc = topScope_; sc != nullptr; sc = sc->Outer) {
            auto obj = sc->Lookup(ptr->Name);
            if (obj != nullptr && (obj->Kind == Var || obj->Kind == Con)) return obj->Type;
        }
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
        declare(f, f->Type, topScope_, kind, f->Names);
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

    for (auto decl : decls_) {
        Walk(shared_from_this(), decl);
    }

    closeScope();
    assert(topScope_ == nullptr, "unbalanced scopes");
    assert(labelScope_ == nullptr, "unbalanced label scopes");

    // TODO rest code
    size_t i = 0;
    for (auto ident : unresolved_) {
        assert(ident->Obj == unresolved, "object already resolved");
        ident->Obj = pkgScope_->Lookup(ident->Name);
        if (ident->Obj == nullptr) {
            unresolved_[i] = ident;
            i++;
        }
        // errors_.push_back({ident->Pos(), "unknown var"});
    }
}

void semantic::analyze() {
    resolveFile();

    for (auto err : errors_) {
        std::cout << err.pos.ToString() + "---" + err.msg << "\n";
    }
}