# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#                                   /   \
#  _                        )      ((   ))     (
# (@)                      /|\      ))_((     /|\
# |-|                     / | \    (/\|/\)   / | \                      (@)
# | | -------------------/--|-voV---\`|'/--Vov-|--\---------------------|-|
# |-|                         '^`   (o o)  '^`                          | |
# | |                               `\Y/'                               |-|
# |-|                                                                   | |
# | | 				2016 Nemanja Micovic <nmicovic@outlook.com>          |-|
# |-|                                                                   | |
# | |                                                                   |-|
# |_|___________________________________________________________________| |
# (@)                 l   /\ /      ( (       \ /\   l                `\|-|
#                     l /   V        \ \       V   \ l                  (@)
#                     l/             _) )_          \I
#                                    `\ /'
# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
PROGRAM = vlang
CXX = clang++
CXXFLAGS = $(shell llvm-config --cxxflags)
LDFLAGS = $(shell --ldflags --system-libs --libs core native mcjit)
FILES = Makefile parser.ypp lexer.lex
CLOC = $(shell type -p cloc || echo wc -l)
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
$(PROGRAM): lex.yy.o parser.tab.o llvm_codegen.o
	$(CXX) -o $@ $^ $(LDFLAGS)
parser.tab.o: parser.tab.cpp parser.tab.hpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)
parser.tab.cpp parser.tab.hpp: parser.ypp
	bison -d -v $<
lex.yy.o: lex.yy.c parser.tab.hpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)
lex.yy.c: lexer.lex
	flex $<
llvm_codegen.o: llvm_codegen.cpp llvm_codegen.hpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
.PHONY: clean dist author

clean:
	@rm -rf *.o *~ $(PROGRAM) *tab* lex.yy.* *.output
	@clear
	@echo "Workspace cleared!"

dist:
	@clear
	@tar -cvf $(PROGRAM).tar $(FILES)
	@echo "Project packed up"

author:
	@clear
	@echo "Created by: Nemanja Micovic"

lines:
	@$(CLOC) *
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
