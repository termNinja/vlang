/*
 * Types.cpp
 * Copyright (C) 2016 Nemanja Mićović <nmicovic@outlook.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "Types.hpp"

namespace vlang {

std::string to_str(VLANG_TYPE type) {
	switch (type) {
		case INT32:
			return "int32";
		case INT64:
			return "int64";
		case DOUBLE:
			return "double";
		case BOOL:
			return "bool";
		case STRING:
			return "string";
		case VOID:
			return "void"
		default:
			return "unknown_type";
	}
}

std::string to_str(Type* llvm_type) {
	if (llvm_type == LLVM_INTTY)
		return "int";
	else if (llvm_type == LLVM_DOUBLETY)
		return "double";
	else
		return "unknown_type";
}

} // ;vlang
