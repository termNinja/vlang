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

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
} // ;vlang
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#endif /* !GLOBALCONTAINERS_HPP */
