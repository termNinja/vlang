/*
 * LLVMCodegen.cpp
 * Copyright (C) 2016 Nemanja Mićović <nmicovic@outlook.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "LLVMCodegen.hpp"

#include <iostream>

std::unique_ptr<Module> TheModule;
LLVMContext TheContext;
std::map<std::string, AllocaInst*> NamedValues;
std::map<std::string, GlobalVariable*> GlobalValues;
IRBuilder<> Builder(TheContext);
std::unique_ptr<legacy::FunctionPassManager> TheFPM;

Function* mainFunction = nullptr;

Value* logError(std::string err_msg) {
    std::cerr << err_msg << std::endl;
    return nullptr;
}

void initializeModuleAndPassManager() {
    TheModule = make_unique<Module>("VLANG MODULE", TheContext);
    TheFPM = make_unique<legacy::FunctionPassManager>(TheModule.get());
    TheFPM->doInitialization();
}

AllocaInst* CreateEntryBlockAllocaInt32(Function* TheFunction, const std::string& name) {
    IRBuilder<> b(&TheFunction->getEntryBlock(), TheFunction->getEntryBlock().begin());
    return b.CreateAlloca(Type::getInt32Ty(TheContext), 0, name.c_str());
}

AllocaInst* CreateEntryBlockAllocaInt64(Function* TheFunction, const std::string& name) {
    IRBuilder<> b(&TheFunction->getEntryBlock(), TheFunction->getEntryBlock().begin());
    return b.CreateAlloca(Type::getInt64Ty(TheContext), 0, name.c_str());
}
AllocaInst* CreateEntryBlockAllocaDouble(Function* TheFunction, const std::string& name) {
    IRBuilder<> b(&TheFunction->getEntryBlock(), TheFunction->getEntryBlock().begin());
    return b.CreateAlloca(Type::getDoubleTy(TheContext), 0, name.c_str());
}
