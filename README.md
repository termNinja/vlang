# vlang: LLVM compiler for Vala

## About Vala
Vala is an object oriented programming language created for developing Gnome based applications.
Syntax of Vala is similar to C#.
Vala supports:
* Interfaces
* Properties
* Signals
* Foreach
* Lambda expressions
* Type inference for local variables
* Generics
* Non-null types
* Assisted memory management
* Exception handling
* Type modules (Plugins)

Here's a Vala code for hello world:
```vala
int main() {
    stdout.printf("Hello, World\n");
    return 0;
}
```

Or similar to Java and C# with a static main function:
```vala
class Demo.HelloWorld : GLib.Object {

    public static int main(string[] args) {
        stdout.printf("Hello, World\n");
        return 0;
    }
}
```

## About vlang
Vlang is inspired by [clang](http://clang.llvm.org/) and it has been created as a
course project for "Compiler Construction" at my faculty. Creating a compiler by
yourself is extremely difficult, so I've limited myself to only a
subset of Vala without support for Gnome libraries, inheritance, generics, lambdas, exceptions etc. I've selected most of the features
I've had some experience during course and added classes on top of that.

I would love people to join the project and continue working on the compiler. If you're insterested, don't hesitate to contact me.

## Building the project
In order to build the project, make sure to have:
* flex
* bison
* llvm
* boost program options
* boost lexical cast
* clang++ compiler (std=c++11)

## TODO (planned)
- [x] simple project structure (lexical and syntaxical analysis, Makefile)
- [x] basic ASTs
- [x] support function prototypes, definitions and function calls
- [x] formatted output of parsed code
- [x] support basic arithmetic/relational operations
- [x] support simple control structures (if-else, while)
- [ ] support advanced control structures (if-elseif-...-else, for, switch) (maybe)
- [x] support functions
- [ ] support classes
- [ ] basics of semantic analysis
- [x] include llvm
- [ ] generate basic LLVM IR (constants, variables, functions)
- [ ] generated additional LLVM IR (pointers, strings, arrays)
- [x] add compiler options (for example ```vlang main.vala point.vala -o geometry```)

## Running vlang
```bash
$ ./vlang --help
All options:
    --help                          produce help message
    -i [ --input-file ] arg         input .vala file
    -o [ --output ] arg (=a.out)    executable output path and name
    -s [ --emit-source ] arg (=0)   shows the parsed source code
```

For developing/debugging purposes, source will always be emited at the moment.
You can run vlang without input files and it will start in interpret mode.

```bash
********************************************************
Input: 
* tests/01_simple.vala
Output: a.out
********************************************************


*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*
parsed source:
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*
int main() {
    return 0;
}

```
