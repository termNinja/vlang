%option noyywrap
%option nounput
%option noinput
%{
/*
 * lexer.lex
 * Copyright (C) 2016 Nemanja Mićović <nmicovic@outlook.com>
 *
 * Distributed under terms of the MIT license.
 */

#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>

#include "LLVMCodegen.hpp"
#include "Types.hpp"
#include "Expression.hpp"
#include "Statement.hpp"

#include "GlobalContainers.hpp"

#include "parser.tab.hpp"

unsigned long long int ProgramLineCounter = 1;
%}

%x C_COMMENT

%%
int                 return int_ty_tok;
double              return double_ty_tok;
string              return string_ty_tok;
bool                return bool_ty_tok;
void                return void_ty_tok;
if                  return if_tok;
else                return else_tok;
while               return while_tok;
for                 return for_tok;
true {
    yylval.bool_val = true;
    return bool_val_tok;
}
false {
    yylval.bool_val = false;
    return bool_val_tok;
}

stdout[.]printf     return stdout_printf_tok;

return              return return_tok;

["][^"]*["] {
    std::string tmp(yytext);
    yylval.str_val = new std::string(tmp.substr(1, tmp.size() -2));
    return str_val_tok;
}

"//".* {}

"/*"                BEGIN(C_COMMENT);
<C_COMMENT>"*/"     BEGIN(INITIAL);
<C_COMMENT>.        { }

"{" {
    vlang::BeginScope();
    return *yytext;
}

"}" {
    vlang::EndScope();
    return *yytext;
}

">="                return GTE_tok;
"<="                return LTE_tok;
"=="                return EQ_tok;
"!="                return NEQ_tok;

[,.<>;=!&|:?()\[\]+*/%-] {
    return *yytext;
}

[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.str_val = new std::string(yytext);
    return id_tok;
}

[+-]?[0-9]+ {
    yylval.int_val = atoi(yytext);
    return int_val_tok;
}
[-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)? {
    yylval.double_val = atof(yytext);
    return double_val_tok;
}

[\n] {
    ++ProgramLineCounter;
}

[\t ] {}
. {
    std::cerr << "Lexical error. Unrecognized character: '" << *yytext << "'" << std::endl;
    exit(EXIT_FAILURE);
}
%%
