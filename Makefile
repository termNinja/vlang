# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# 				       Copyright (C) 2016 Nemanja Mićović <nmicovic@outlook.com>
# 				       		   Distributed under terms of the MIT license.
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#                                             /   \
#            _                        )      ((   ))     (
#           (@)                      /|\      ))_((     /|\
#           |-|                     / | \    (/\|/\)   / | \                      (@)
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
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
PROGRAM = vlang
CXX = clang++
CXXFLAGS = -g $(shell llvm-config --cxxflags) -fexceptions
LDFLAGS = $(shell llvm-config --ldflags --system-libs --libs core native mcjit)
BOOST = -DBOOST_NO_EXCEPTIONS -DBOOST_NO_EXCEPTION_STD_NAMESPACE -L /usr/lib/ -lm -lboost_program_options -fexceptions
FILES = Makefile parser.ypp lexer.lex Statement.hpp Statement.cpp \
	Expression.hpp Expression.cpp LLVMCodegen.hpp LLVMCodegen.cpp TypeChecker.hpp \
	Types.hpp Types.cpp
CLOC = $(shell type -p cloc || echo wc -l)
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
$(PROGRAM): lex.yy.o parser.tab.o LLVMCodegen.o Expression.o Types.o Statement.o \
			ProgramOptions.o GlobalContainers.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(BOOST)
	@echo
parser.tab.o: 	parser.tab.cpp parser.tab.hpp LLVMCodegen.hpp Types.hpp Expression.hpp \
				TypeChecker.hpp Statement.hpp ProgramOptions.hpp GlobalContainers.hpp
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
Expression.o: Expression.cpp Expression.hpp LLVMCodegen.hpp Types.hpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)
	@echo
Types.o: Types.cpp Types.hpp LLVMCodegen.hpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)
	@echo
Statement.o: Statement.cpp Statement.hpp Expression.hpp LLVMCodegen.hpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)
	@echo
ProgramOptions.o: ProgramOptions.cpp ProgramOptions.hpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)
	@echo
GlobalContainers.o: GlobalContainers.cpp GlobalContainers.hpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)
	@echo
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

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
