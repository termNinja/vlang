/*
 * llvm_codegen.cpp
 * Copyright (C) 2016 Nemanja Mićović <nmicovic@outlook.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "LLVMCOdegen.hpp"

std::unique_ptr<Module> TheModule;
LLVMContext TheContext;
std::map<std::string, AllocaInst*> NamedValues;
IRBuilder<> builder(TheContext);
std::unique_ptr<legacy::FunctionPassManager> TheFPM;

Function* mainFunction = nullptr;

void initializeModuleAndPassManager() {
	TheModule = make_unique<Module>("pasftw", TheContext);
	TheFPM = make_unique<legacy::FunctionPassManager>(TheModule.get());
	TheFPM->doInitialization();
}

AllocaInst* createEntryBlockAlloca(Function* TheFunction, const std::string& name) {
	IRBuilder<> b(&TheFunction->getEntryBlock(), TheFunction->getEntryBlock().begin());
	return b.CreateAlloca(Type::getInt32Ty(TheContext), 0, name.c_str());
}
