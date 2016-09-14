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
subset of Vala without support for Gnome. Once that is complete, I shall make a decision
for the future of the project.

## Building the project
In order to build the project, make sure to have:
* flex
* bison
* llvm (along with clang++ compiler)

## TODO (planned)
- [x] simple project structure (lexical and syntaxical analysis)
- [x] include llvm
- [ ] support basic arithmetic/relational operations
- [ ] support control structures (branching, loops)
- [ ] support functions
- [ ] support classes

