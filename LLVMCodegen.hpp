/*
 * LLVMCodegen.hpp
 * Copyright (C) 2016 Nemanja Mićović <nmicovic@outlook.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef LLVM_CODEGEN_HPP
#define LLVM_CODEGEN_HPP

#include <map>
#include <string>
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Support/TargetSelect.h"

using namespace llvm;

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Useful LLVM macros
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#define LLVM_INT(x) ConstantInt::get(TheContext, APInt(32, x))
#define LLVM_INT_SIZE(x, y) ConstantInt::get(TheContext, APInt(x, y))
#define LLVM_INTTY() Type::getInt32Ty(TheContext)

#define LLVM_DOUBLE(x) ConstantFP::get(TheContext, APFloat(x))
#define LLVM_DOUBLETY() Type::getDoubleTy(TheContext)

#define LLVM_BOOL(x) ConstantInt::get(TheContext, APInt(1, x))
#define LLVM_BOOLTY() Type::getInt1Ty(TheContext)

#define LLVM_VOIDTY() Type::getVoidTy(TheContext)

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Global things we need for our compiler
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
extern std::unique_ptr<Module> TheModule;
extern LLVMContext TheContext;
extern std::map<std::string, AllocaInst*> NamedValues;
extern std::map<std::string, GlobalVariable*> GlobalValues;
extern IRBuilder<> Builder;
extern std::unique_ptr<legacy::FunctionPassManager> TheFPM;

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Helper functions
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void InitializeModuleAndPassManager();
Function* GetFunction(const std::string& name);
Value* logError(std::string err_msg);
AllocaInst* CreateEntryBlockAllocaInt32(Function* TheFunction, const std::string& name);
AllocaInst* CreateEntryBlockAllocaBool(Function* TheFunction, const std::string& name);
AllocaInst* CreateEntryBlockAllocaInt64(Function* TheFunction, const std::string& name);
AllocaInst* CreateEntryBlockAllocaIntDouble(Function* TheFunction, const std::string& name);

AllocaInst* GetEntryBlockAllocaForType(Function* TheFunction, Type* type, const std::string& name);

void write_llvm_to_bitcode();

#endif /* ifndef LLVM_CODEGEN_HPP */
