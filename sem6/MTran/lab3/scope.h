#pragma once
#include <any>
#include <string>
#include <unordered_map>

#include "token.h"

template <typename T>
using V = std::vector<T>;

template <typename T>
using SP = std::shared_ptr<T>;

using pos_t = Token::token_position;

enum ObjKind : int {
    Bad,  // for error handling
    Pkg,  // package
    Con,  // constant
    Typ,  // type
    Var,  // variable
    Fun,  // function or method
    Lbl,
};

std::unordered_map<ObjKind, std::string> ObjKindStrings = {
    {Bad, "bad"}, {Pkg, "package"}, {Con, "const"}, {Typ, "type"},
    {Var, "var"}, {Fun, "func"},    {Lbl, "label"},
};

struct Object {
    ObjKind Kind;
    std::string Name;
    std::any Decl;
    std::any Data;
    std::any Type;

    pos_t Pos() const { throw std::runtime_error("Not implemented"); }

    Object() {}
    Object(ObjKind kind, std::string name) : Kind(kind), Name(name) {}
};

struct Scope {
    SP<Scope> Outer;
    std::unordered_map<std::string, SP<Object>> Objects;

    Scope(SP<Scope> outer) : Outer(outer) {}

    SP<Object> Lookup(std::string name) { return Objects[name]; }

    SP<Object> Insert(SP<Object> obj) {
        SP<Object> alt = nullptr;
        if (Objects.find(obj->Name) == Objects.end()) {
            Objects[obj->Name] = obj;
            return nullptr;
        }
        return Objects[obj->Name];
    }
};