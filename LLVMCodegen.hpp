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

#define LLVM_INT(x) ConstantInt::get(TheContext, APInt(32, x))
#define LLVM_DOUBLE(x) ConstantFP::get(TheContext, APFloat(x))

extern std::unique_ptr<Module> TheModule;
extern LLVMContext TheContext;
extern std::map<std::string, AllocaInst*> NamedValues;
extern IRBuilder<> builder;
extern std::unique_ptr<legacy::FunctionPassManager> TheFPM;

void InitializeModuleAndPassManager();
AllocaInst* CreateEntryBlockAlloca(Function* TheFunction, const std::string& name);

#endif /* ifndef LLVM_CODEGEN_HPP */
