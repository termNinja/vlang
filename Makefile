# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#					   Copyright (C) 2016 Nemanja Mićović <nmicovic@outlook.com>
#							   Distributed under terms of the MIT license.
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#                                             /   \
#            _                        )      ((   ))     (
#           (@)                      /|\      ))_((     /|\
#           |-|\                    / | \    (/\|/\)   / | \                      (@)
#           | | -------------------/--|-voV---\`|'/--Vov-|--\---------------------|-|
#           |-|                         '^`   (o o)  '^`                          | |
#           | |                               `\Y/'                               |-|
#           |-|                                                                   | |
#           | |              2016 Nemanja Micovic <nmicovic@outlook.com>          |-|
#           |-|                                                                   | |
#           | |                                                                   |-|
#           |_|___________________________________________________________________| |
#           (@)                 l   /\ /      ( (       \ /\   l                `\|-|
#                               l /   V        \ \       V   \ l                  (@)
#                               l/             _) )_          \I
#                                              `\ /'
#                                                |
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
PROGRAM = vlang
CXX = clang++
CXXFLAGS = -g $(shell llvm-config --cxxflags) -fexceptions
LDFLAGS = $(shell llvm-config --ldflags --system-libs --libs core native mcjit)
BOOST = -DBOOST_NO_EXCEPTIONS -DBOOST_NO_EXCEPTION_STD_NAMESPACE -L /usr/lib/ -lm -lboost_program_options -fexceptions
FILES =						\
	Makefile				\
	parser.ypp				\
	lexer.lex				\
	color.h					\
	Expression.cpp			\
	Expression.hpp			\
	GlobalContainers.cpp	\
	GlobalContainers.hpp	\
	LLVMCodegen.cpp			\
	LLVMCodegen.hpp			\
	ProgramOptions.cpp		\
	ProgramOptions.hpp		\
	SemanticAnalyzer.cpp	\
	SemanticAnalyzer.hpp	\
	Statement.cpp			\
	Statement.hpp			\
	Types.cpp				\
	Types.hpp

CLOC = $(shell type -p cloc || echo wc -l)
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
$(PROGRAM): lex.yy.o parser.tab.o LLVMCodegen.o Expression.o Types.o Statement.o \
			ProgramOptions.o GlobalContainers.o SemanticAnalyzer.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(BOOST)
	@echo
parser.tab.o:	parser.tab.cpp parser.tab.hpp LLVMCodegen.hpp Types.hpp Expression.hpp \
				Statement.hpp ProgramOptions.hpp GlobalContainers.hpp \
				SemanticAnalyzer.hpp color.h
	$(CXX) -c -o $@ $< $(CXXFLAGS)
	@echo
parser.tab.cpp parser.tab.hpp: parser.ypp
	bison -d -v $<
	@echo
lex.yy.o: lex.yy.c parser.tab.hpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)
	@echo
lex.yy.c: lexer.lex
	flex $<
	@echo
LLVMCodegen.o: LLVMCodegen.cpp LLVMCodegen.hpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)
	@echo
Expression.o: Expression.cpp Expression.hpp LLVMCodegen.hpp Types.hpp SemanticAnalyzer.hpp ProgramOptions.hpp color.h
	$(CXX) -c -o $@ $< $(CXXFLAGS)
	@echo
Types.o: Types.cpp Types.hpp LLVMCodegen.hpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)
	@echo
Statement.o: Statement.cpp Statement.hpp Expression.hpp LLVMCodegen.hpp SemanticAnalyzer.hpp ProgramOptions.hpp color.h
	$(CXX) -c -o $@ $< $(CXXFLAGS)
	@echo
ProgramOptions.o: ProgramOptions.cpp ProgramOptions.hpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)
	@echo
GlobalContainers.o: GlobalContainers.cpp GlobalContainers.hpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)
	@echo
SemanticAnalyzer.o: SemanticAnalyzer.cpp SemanticAnalyzer.hpp Statement.hpp Expression.hpp \
	GlobalContainers.hpp color.h
	$(CXX) -c -o $@ $< $(CXXFLAGS)
	@echo
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
.PHONY: clean dist author

clean:
	@rm -rf *.o *~ $(PROGRAM) *tab* lex.yy.* *.output
	@clear
	@echo "Workspace cleared!"

dist:
	@clear
	@tar -pczf $(PROGRAM).tar.gz $(FILES)
	@echo "Project packed up"

author:
	@clear
	@echo "Created by: Nemanja Micovic"

lines:
	@$(CLOC) $(FILES)
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
