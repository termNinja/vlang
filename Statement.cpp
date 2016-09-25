/*
 * Statement.cpp
 * Copyright (C) 2016 Nemanja Mićović
 *
 * Distributed under terms of the MIT license.
 */

#include "Statement.hpp"
#include "SemanticAnalyzer.hpp"
#include "color.h"
#include "ProgramOptions.hpp"

#include <map>

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
namespace vlang {
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

std::map<std::string, PrototypeAST> FunctionProtos;
std::string indent_style = "    ";

std::string getStrWithIndent(int level = 0) {
    if (level < 0) return "";
    std::string res = "";
    while (level > 0) {
        res += indent_style;
        --level;
    }
    return res;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// LLVM CODEGEN
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
Value* ReturnStmtAST::codegen() const {
    Value* retVal = m_retVal->codegen();
    if (retVal == nullptr)
        return logError("Failed m_retVal->codegen() in ReturnStmtAST::codegen()");
    return Builder.CreateRet(retVal);
}
Value* BlockStmtAST::codegen() const {
    for (auto & cmd : m_cmds) {
        Value* val = cmd->codegen();
        if (val == nullptr) return logError("Failed m_cmds[i]->codegen() in BlockStmtAST::codegen()");
    }
    return LLVM_BOOL(true);
}
//Value* AssignmentStmtAST::codegen() const {
    //Function* TheFunction = Builder.GetInsertBlock()->getParent();
    //if (m_type != VLANG_TYPE::NO_VAR_DECL) {
         //We allocate memory for variable
        //std::unique_ptr<VlangType> t(make_from_enum(m_type));
        //AllocaInst* addr = GetEntryBlockAllocaForType(TheFunction, t->llvm_type(), m_varName);

         // And put it in named values
        //NamedValues[m_varName] = addr;
    //}
    //AllocaInst* addr = NamedValues[m_varName];
    //if (addr == nullptr) return logError("Failed getting addres for variable " + m_varName);

    //Value* assignMe = m_expr->codegen();
    //if (assignMe == nullptr) return logError("Failed m_expr->codegen() in AssignmentStmtAST::codegen()");

    //Builder.CreateStore(addr, assignMe);
    //return LLVM_BOOL(true);
//}

Value* handleAssignment(std::string varName, ExprAST* expr, VLANG_TYPE type) {
    Function* TheFunction = Builder.GetInsertBlock()->getParent();
    if (type != VLANG_TYPE::NO_VAR_DECL) {
        // We allocate memory for variable
        std::unique_ptr<VlangType> t(make_from_enum(type));
        AllocaInst* addr = GetEntryBlockAllocaForType(TheFunction, t->llvm_type(), varName);

        // And put it in named values
        NamedValues[varName] = addr;
    }
    AllocaInst* addr = NamedValues[varName];
    if (addr == nullptr) return logError("Failed getting addres for variable " + varName);

    Value* assignMe = expr->codegen();
    if (assignMe == nullptr) return logError("Failed m_expr->codegen() in AssignmentStmtAST::codegen()");

    Builder.CreateStore(addr, assignMe);
    return LLVM_BOOL(true);
}

Value* AssignmentStmtAST::codegen() const {
    return handleAssignment(m_varName, m_expr, m_type);
}

Value* AssignmentListStmtAST::codegen() const {
    for (auto & p : m_list) {
        handleAssignment(p.first, p.second, m_type);
    }

    return LLVM_BOOL(true);
}
Value* ExpressionStmtAST::codegen() const {
    Value* val = m_expr->codegen();
    if (val == nullptr) return logError("Failed m_expr->codegen() in ExpressionStmtAST::codegen()");
    return val;
}
Value* EmptyStmtAST::codegen() const {
    // TODO: How to compile this? Maybe remove it...
    return LLVM_BOOL(true);
}

Value* handleIf(ExprAST* condExpr, StmtAST* thenStmt) {
    Value* cond = condExpr->codegen();
    if (cond == nullptr) return logError("Failed m_condExpr->codegen() in IfStmtAST::codegen()");

    if (cond->getType() == LLVM_DOUBLETY())
        cond = Builder.CreateFCmpONE(cond, LLVM_DOUBLE(0.0), "ifcond");

    Function* TheFunction = Builder.GetInsertBlock()->getParent();

    BasicBlock* thenBB = BasicBlock::Create(TheContext, "ifthen", TheFunction);
    BasicBlock* mergeBB = BasicBlock::Create(TheContext, "ifmerge");

    Builder.CreateCondBr(cond, thenBB, mergeBB);

    // Handling then
    Builder.SetInsertPoint(thenBB);
    Value* thenVal = thenStmt->codegen();
    if (! thenVal) return logError("Failed m_thenStmt->codegen() in IfStmtAST::codegen()");
    Builder.CreateBr(mergeBB);
    thenBB = Builder.GetInsertBlock();

    // Handling merge
    TheFunction->getBasicBlockList().push_back(mergeBB);
    Builder.SetInsertPoint(mergeBB);
    //PHINode* thePhi = Builder.CreatePHI(thenVal->getType(), 1, "iftmp");
    //thePhi->addIncoming(thenVal, thenBB);

    return LLVM_BOOL(true);
}

Value* IfStmtAST::codegen() const {
    return handleIf(m_condExpr, m_thenStmt);
}

//Value* IfStmtAST::codegen() const {
//    Value* cond = m_condExpr->codegen();
//    if (cond == nullptr) return logError("Failed m_condExpr->codegen() in IfStmtAST::codegen()");
//
//    if (cond->getType() == LLVM_DOUBLETY())
//        cond = Builder.CreateFCmpONE(cond, LLVM_DOUBLE(0.0), "ifcond");
//
//    Function* TheFunction = Builder.GetInsertBlock()->getParent();
//
//    BasicBlock* thenBB = BasicBlock::Create(TheContext, "ifthen", TheFunction);
//    BasicBlock* mergeBB = BasicBlock::Create(TheContext, "ifmerge");
//
//    Builder.CreateCondBr(cond, thenBB, mergeBB);
//
//    // Handling then
//    Builder.SetInsertPoint(thenBB);
//    Value* thenVal = m_thenStmt->codegen();
//    if (! thenVal) return logError("Failed m_thenStmt->codegen() in IfStmtAST::codegen()");
//    Builder.CreateBr(mergeBB);
//    thenBB = Builder.GetInsertBlock();
//
//    // Handling merge
//    TheFunction->getBasicBlockList().push_back(mergeBB);
//    Builder.SetInsertPoint(mergeBB);
//    //PHINode* thePhi = Builder.CreatePHI(thenVal->getType(), 1, "iftmp");
//    //thePhi->addIncoming(thenVal, thenBB);
//
//    return LLVM_BOOL(true);
//}
Value* IfElseStmtAST::codegen() const {
    Value* cond = m_condExpr->codegen();
    if (cond == nullptr) return logError("Failed m_condExpr->codegen() in IfStmtAST::codegen()");

    if (cond->getType() == LLVM_DOUBLETY())
        cond = Builder.CreateFCmpONE(cond, LLVM_DOUBLE(0.0), "ifcond");

    Function* TheFunction = Builder.GetInsertBlock()->getParent();

    BasicBlock* thenBB = BasicBlock::Create(TheContext, "ifthen", TheFunction);
    BasicBlock* elseBB = BasicBlock::Create(TheContext, "ifelse");
    BasicBlock* mergeBB = BasicBlock::Create(TheContext, "ifmerge");

    Builder.CreateCondBr(cond, thenBB, elseBB);

    // Handling then
    Builder.SetInsertPoint(thenBB);
    Value* thenVal = m_thenStmt->codegen();
    if (! thenVal) return logError("Failed m_thenStmt->codegen() in IfStmtAST::codegen()");
    Builder.CreateBr(mergeBB);
    thenBB = Builder.GetInsertBlock();

    // Handling else
    TheFunction->getBasicBlockList().push_back(elseBB);
    Builder.SetInsertPoint(elseBB);
    Value* elseVal = m_elseStmt->codegen();
    if (! elseVal) return logError("Failed m_thenStmt->codegen() in IfStmtAST::codegen()");
    Builder.CreateBr(mergeBB);
    elseBB = Builder.GetInsertBlock();

    // Handling merge
    TheFunction->getBasicBlockList().push_back(mergeBB);
    Builder.SetInsertPoint(mergeBB);

    return LLVM_BOOL(true);
}
Value* WhileStmtAST::codegen() const {
    Function* TheFunction = Builder.GetInsertBlock()->getParent();

	BasicBlock* entryBB = BasicBlock::Create(TheContext, "entry_while", TheFunction);
	BasicBlock* loopBB = BasicBlock::Create(TheContext, "loop_while");
	BasicBlock* endBB = BasicBlock::Create(TheContext, "end_while");

	// Jump into entry
	Builder.CreateBr(entryBB);

	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	// HANDLE LOOP ENTRY
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	Builder.SetInsertPoint(entryBB);
	Value* condVal = m_condExpr->codegen();
	if (! condVal) return logError("Failed m_cond->codegen() in WhileExprAST::codegen()");
    if (condVal->getType() == LLVM_DOUBLETY())
        condVal = Builder.CreateFCmpONE(condVal, LLVM_DOUBLE(0.0), "while_cmp");
	Builder.CreateCondBr(condVal, loopBB, endBB);
	entryBB = Builder.GetInsertBlock();

	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	// HANDLE LOOP BODY
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	TheFunction->getBasicBlockList().push_back(loopBB);
	Builder.SetInsertPoint(loopBB);
	Value* bodyVal = m_bodyStmt->codegen();
	if (! bodyVal) return logError("Failed m_body->codegen() in WhileExprAST::codegen()");
	Builder.CreateBr(entryBB);

	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	// HANDLE LOOP END
	// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	TheFunction->getBasicBlockList().push_back(endBB);
	Builder.SetInsertPoint(endBB);

    return LLVM_BOOL(true);
}
Value* PrototypeAST::codegen() const {
    std::vector<Type*> protoParameters;
    for (auto & param : m_args) {
        Type* t = make_from_enum(param.first)->llvm_type();
        if (t == nullptr) return logError("Failed making llvm_type");
        protoParameters.push_back(t);
    }
    Type* t = make_from_enum(m_retVal)->llvm_type();
    FunctionType* ftype = FunctionType::get(t, protoParameters, false);
    Function* theFunction = Function::Create(ftype, Function::ExternalLinkage, m_name, TheModule.get());

    // Set argument names
    unsigned i = 0;
    for (auto &arg : theFunction->args())
        arg.setName(m_args[i++].second);

    return theFunction;
}
Value* FunctionAST::codegen() const {
    FunctionProtos.insert(std::pair<std::string, PrototypeAST>(m_proto.name(), m_proto));
    Function* theFunction = GetFunction(m_proto.name());

    // If function doesn't exist, we generate a declaration for it.
    if (theFunction == nullptr) theFunction = static_cast<Function*>(m_proto.codegen());

    // If this failed, we report error
    if (theFunction == nullptr) return logError("Failed m_proto.codegen() in FunctionAST::codegen()");

    // What if function exists and has a body?
    if (! theFunction->empty())
        return logError("Function '" + m_proto.name() + "' can't be redefined.");

    // We give our function a basic block
    BasicBlock* bodyBB = BasicBlock::Create(TheContext, "entry", theFunction);
    Builder.SetInsertPoint(bodyBB);

    // We add arguments as local variables
    NamedValues.clear();
    for (auto & arg : theFunction->args()) {
        // TODO: Make different allocas for different types!
        AllocaInst* argAddr = GetEntryBlockAllocaForType(theFunction, arg.getType(), arg.getName());
        NamedValues[arg.getName()] = argAddr;
        Builder.CreateStore(&arg, argAddr);
    }

    // Now we can generate function body
    Value* fBody = m_definition->codegen();
    if (fBody == nullptr) {
        theFunction->eraseFromParent();
        return logError("Failed m_definition->codegen() in FunctionAST::codegen()");
    } else {
        // TODO:
        if (m_proto.ret_val_type() == VLANG_TYPE::VOID) {
            return Builder.CreateRetVoid();
        } else {
            //Builder.CreateRet(fBody);
            verifyFunction(*theFunction);
            TheFPM->run(*theFunction);
            return theFunction;
        }
    }
}


// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Checking if assignment is valid
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
bool AssignmentStmtAST::isAllowed() const {
    return semant::SemanticAnalyzer::isAllowedAssignment(m_type, m_expr->type()->vlang_type());
}

std::unique_ptr<std::vector<bool>> AssignmentListStmtAST::isAllowed() const {
    std::unique_ptr<std::vector<bool>> result(new std::vector<bool>());

    for (auto &ass : m_list) {
        const VlangType* exprType = ass.second->type();
        if (exprType == nullptr) result->push_back(false);
        else if (semant::SemanticAnalyzer::isAllowedAssignment(m_type, exprType->vlang_type()))
            result->push_back(true);
        else
            result->push_back(false);
    }

    return result;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Dump functions
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
std::string ReturnStmtAST::dump(int level) const {
    std::string res = getStrWithIndent(level);
    if (util::ProgramOptions::get().syntax_highlight())
        res += std::string(KEYWORD_C) + "return " + std::string(RESET);
    else res += "return ";
    res += m_retVal->dump() + ";";
    return res;
}

std::string PrototypeAST::dump(int level) const {
    std::string res = getStrWithIndent(level);
    res += to_str(m_retVal) + " ";
    if (util::ProgramOptions::get().syntax_highlight())
        res += std::string(FUNNAME_C) + m_name + std::string(RESET) + "(";
    else res += m_name + "(";

    if (! m_args.empty()) {
        unsigned i = 0;
        for (; i < m_args.size()-1; ++i) {
            res += to_str(m_args[i].first) + " ";
            if (util::ProgramOptions::get().syntax_highlight())
                res += std::string(VARIABLE_C) + m_args[i].second + std::string(RESET) + ",";
            else  res += m_args[i].second + ", ";

        }
        res += to_str(m_args[i].first) + " ";
        if (util::ProgramOptions::get().syntax_highlight())
            res += std::string(VARIABLE_C) + m_args[i].second + std::string(RESET);
        else  res += m_args[i].second;
    }
    res += ");";
    return res;
}

std::string FunctionAST::dump(int level) const {
    std::string res = m_proto.dump(level);
    res.erase(res.size()-1, res.size());        // remove ';' from the end of proto
    res += " " + m_definition->dump(level+1);
    return res;
}

// TODO: Multiple nested blocks are shown badly (because of indenting
// as I wanted to avoid newline with '{' symbol.
std::string BlockStmtAST::dump(int level) const {
    std::string res = "{\n";
    for (auto &cmd : m_cmds) {
        res += cmd->dump(level) + "\n";
    }
    res += getStrWithIndent(level-1) + "}";
    return res;
}

std::string ExpressionStmtAST::dump(int level) const {
    std::string res = getStrWithIndent(level);
    res += m_expr->dump() + ";";
    return res;
}

std::string EmptyStmtAST::dump(int) const {
    std::cerr << "EMPTY STATEMENT!" << std::endl;
    return ";";
}

std::string IfStmtAST::dump(int level) const {
    std::string res = getStrWithIndent(level);
    if (util::ProgramOptions::get().syntax_highlight())
        res += std::string(KEYWORD_C) + "if" + std::string(RESET) + " (";
    else
        res += "if (";
    res += m_condExpr->dump() + ") ";
    if (m_thenStmt->stmt_type() != STMT_TYPE::BLOCK)
        res += "\n" + m_thenStmt->dump(level+1);
    else res += m_thenStmt->dump(level+1);
    return res;
}

std::string IfElseStmtAST::dump(int level) const {
    std::string res = getStrWithIndent(level);

    if (util::ProgramOptions::get().syntax_highlight())
        res += std::string(KEYWORD_C) + "if" + std::string(RESET) + " (";
    else
        res += "if (";
    res += m_condExpr->dump() + ") ";

    // if part
    if (m_thenStmt->stmt_type() != STMT_TYPE::BLOCK)
        res += "\n" + m_thenStmt->dump(level+1);
    else
        res += m_thenStmt->dump(level+1);

    // else part
    std::string else_thingie = "else";
    if (util::ProgramOptions::get().syntax_highlight())
        else_thingie = std::string(KEYWORD_C) + else_thingie + std::string(RESET);
    if (m_thenStmt->stmt_type() != STMT_TYPE::BLOCK)
        res += "\n" + getStrWithIndent(level) + else_thingie;
    else
        res += else_thingie;
    if (m_elseStmt->stmt_type() != STMT_TYPE::BLOCK)
        res += "\n" + m_elseStmt->dump(level+1);
    else
        res += m_elseStmt->dump(level+1);
    return res;
}

std::string WhileStmtAST::dump(int level) const {
    std::string res = getStrWithIndent(level);
    if (util::ProgramOptions::get().syntax_highlight())
        res += std::string(KEYWORD_C) + "while " + std::string(RESET);
    else
        res += "while ";
    res += "(" + m_condExpr->dump() + ")";
    if (m_bodyStmt->stmt_type() == STMT_TYPE::BLOCK)
        res += " " + m_bodyStmt->dump(level+1);
    else
        res += "\n" + getStrWithIndent(level+1) + m_bodyStmt->dump();
    return res;
}

std::string AssignmentStmtAST::dump(int level) const {
    std::string res = getStrWithIndent(level);
    std::string eq = (util::ProgramOptions::get().syntax_highlight() ?
            std::string(OPERATOR_C) + " = " + std::string(RESET) : " = ");
    if (m_type != VLANG_TYPE::NO_VAR_DECL) res += to_str(m_type) + " ";
    res += m_varName + eq + m_expr->dump() + ";";
    return res;
}

std::string AssignmentListStmtAST::dump(int level) const {
    std::string res = getStrWithIndent(level);
    std::string eq = (util::ProgramOptions::get().syntax_highlight() ?
            std::string(OPERATOR_C) + " = " + std::string(RESET) : " = ");
    res += to_str(m_type) + " ";
    if (m_list.empty())
        return "Error, assignment list is empty!";
    unsigned i = 0;
    for (; i < m_list.size()-1; ++i) {
        if (m_list[i].second == nullptr)
            res += m_list[i].first + ", ";
        else
            res += m_list[i].first + eq + m_list[i].second->dump() + ", ";
    }
    if (m_list[i].second == nullptr) res += m_list[i].first + ";";
    else res += m_list[i].first + eq + m_list[i].second->dump() + ";";
    return res;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
} // ;vlang
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
