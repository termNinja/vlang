/*
 * GlobalContainers.cpp
 * Copyright (C) 2016 Nemanja Mićović <nmicovic@outlook.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "GlobalContainers.hpp"

#include "Expression.hpp"

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
namespace vlang {
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

std::stack<std::map<std::string, VLANG_TYPE>> LocalVariableScopeContainer;
std::map<std::string, ProtoDefContainer*> FunctionContainer;
std::map<std::string, VLANG_TYPE> FunctionReturnType;

void RegisterVariable(std::string name, VLANG_TYPE type) {
    LocalVariableScopeContainer.top()[name] = type;
}

VLANG_TYPE GetVariableType(std::string name) {
    if (! LocalVariableScopeContainer.empty()) {
        auto finder = LocalVariableScopeContainer.top().find(name);
        if (finder == LocalVariableScopeContainer.top().end())
            return VLANG_TYPE::UNKNOWN;
        else return finder->second;
    } else return VLANG_TYPE::UNKNOWN;
}
void BeginScope() {
    if (LocalVariableScopeContainer.empty())
        LocalVariableScopeContainer.push(std::map<std::string, VLANG_TYPE>());
    auto m = LocalVariableScopeContainer.top();
    LocalVariableScopeContainer.push(m);
}
void EndScope() {
    if (! LocalVariableScopeContainer.empty())
        LocalVariableScopeContainer.pop();
}


// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
} // ;vlang
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

