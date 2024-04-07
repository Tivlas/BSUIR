#pragma once 
#include <unordered_set>
#include <string>

std::unordered_set<std::string> BUILT_IN_TYPES = {
    "int", 
    "int8", 
    "int16", 
    "int32", 
    "int64", 
    "uint", 
    "uint8", 
    "uint16", 
    "uint32", 
    "uint64", 

    "float32",
    "float64",
    "complex64",
    "complex128",

    "byte",
    "rune",
    "bool",
    "string"
};

std::unordered_set<std::string> NUMERIC_TYPES = {
    "int", 
    "int8", 
    "int16", 
    "int32", 
    "int64", 
    "uint", 
    "uint8", 
    "uint16", 
    "uint32", 
    "uint64", 

    "float32",
    "float64",
    "complex64",
    "complex128",

    "byte",
    "rune",
};
