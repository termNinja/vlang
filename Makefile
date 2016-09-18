# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#                                   /   \
#  _                        )      ((   ))     (
# (@)                      /|\      ))_((     /|\
# |-|                     / | \    (/\|/\)   / | \                      (@)
# | | -------------------/--|-voV---\`|'/--Vov-|--\---------------------|-|
# |-|                         '^`   (o o)  '^`                          | |
# | |                               `\Y/'                               |-|
# |-|                                                                   | |
# | |              2016 Nemanja Micovic <nmicovic@outlook.com>          |-|
# |-|                                                                   | |
# | |                                                                   |-|
# |_|___________________________________________________________________| |
# (@)                 l   /\ /      ( (       \ /\   l                `\|-|
#                     l /   V        \ \       V   \ l                  (@)
#                     l/             _) )_          \I
#                                    `\ /'
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
PROGRAM = vlang
CXX = clang++
CXXFLAGS = -g $(shell llvm-config --cxxflags)
LDFLAGS = $(shell llvm-config --ldflags --system-libs --libs core native mcjit)
BOOST = -DBOOST_NO_EXCEPTIONS -DBOOST_NO_EXCEPTION_STD_NAMESPACE
FILES =  Makefile parser.ypp lexer.lex Statement.hpp Statement.cpp \
	Expression.hpp Expression.cpp LLVMCodegen.hpp LLVMCodegen.cpp TypeChecker.hpp \
	Types.hpp Types.cpp
CLOC = $(shell type -p cloc || echo wc -l)
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
$(PROGRAM): lex.yy.o parser.tab.o LLVMCodegen.o Expression.o Types.o Statement.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(BOOST)
parser.tab.o: parser.tab.cpp parser.tab.hpp LLVMCodegen.hpp Types.hpp Expression.hpp TypeChecker.hpp Statement.hpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)
parser.tab.cpp parser.tab.hpp: parser.ypp
	bison -d -v $<
lex.yy.o: lex.yy.c parser.tab.hpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)
lex.yy.c: lexer.lex
	flex $<
LLVMCodegen.o: LLVMCodegen.cpp LLVMCodegen.hpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)
Expression.o: Expression.cpp Expression.hpp LLVMCodegen.hpp Types.hpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)
Types.o: Types.cpp Types.hpp LLVMCodegen.hpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)
Statement.o: Statement.cpp Statement.hpp Expression.hpp LLVMCodegen.hpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
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
	@$(CLOC) *
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
