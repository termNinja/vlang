%option noyywrap
%option nounput
%option noinput

%{
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include "parser.tab.hpp"
%}

%%
int 								{ return int_ty_tok; 		}
double 								{ return double_ty_tok; 	}
string 								{ return string_ty_tok; 	}

stdout[.]printf 					{ return stdout_printf_tok; }

main 								{ return main_tok; }
return 								{ return return_tok; }

["][^"]*["] {
	std::string tmp(yytext);
	yylval.str_val = new std::string(tmp.substr(1, tmp.size() -2));
	return str_val_tok;
}

[+-]?[0-9]+ { 
	yylval.int_val = atoi(yytext);
	return int_val_tok;
}
[-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)? {
	yylval.double_val = atof(yytext);
	return double_val_tok;
}

[<>;=!&|:?(){}\[\]+*/%-] {
	return *yytext;
}

[\t\n ] {}
. {
	std::cerr << "Lexical error. Unrecognized character: '" << *yytext << "'" << std::endl;
	exit(EXIT_FAILURE);
}
%%
