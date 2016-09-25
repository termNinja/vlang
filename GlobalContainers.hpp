/*
 * GlobalContainers.hpp
 * Copyright (C) 2016 Nemanja Mićović <nmicovic@outlook.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef GLOBALCONTAINERS_HPP
#define GLOBALCONTAINERS_HPP

#include <map>
#include <vector>
#include <stack>
#include <string>

#include "Types.hpp"
#include "Statement.hpp"

#define DRAGON_SEPARATOR() "*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*"

//class ProtoDefContainer;

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
namespace vlang {
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// TODO: Update this map from code (when block begins, open scope, when block ends, clear scope)
extern std::stack<std::map<std::string, VLANG_TYPE>> LocalVariableScopeContainer;
extern std::map<std::string, ProtoDefContainer*> FunctionContainer;

/// \brief This counter is used by lexer in order to count program lines (so error can point the exact line)
extern unsigned long long int ProgramLineCounter;

/// \brief Returns current variable type from current scope.
VLANG_TYPE GetVariableType(std::string name);

/// \brief Registers a given variable for given type.
/// Does NOT check if it's overridding.
/// This should be taken care by semantic analyzer.
void RegisterVariable(std::string name, VLANG_TYPE type);

/// \brief Begins a scope. Called by lexer when it finds '{' token.
void BeginScope();

/// \brief Ends a scope. Called by lexer when it finds '}' token.
void EndScope();

/// \brief Returns the return type for given function name.
/// extern std::map<std::string, VLANG_TYPE> FunctionReturnType;

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
} // ;vlang
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#endif /* !GLOBALCONTAINERS_HPP */
