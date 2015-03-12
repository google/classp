This directory contains an implementation of Classp written by David Gudeman
(dgudeman@google.com or dave.gudeman@gmail.com).

#INTRODUCTION
The Classp language is a system for writing parsers without a writing a grammar.
Grammars are designed to describe the surface structure of a language rather
than the logical structure that the programmer really wants to work with.
Grammars also make use of concepts that are not easily compatible with normal
programming languages, making them an awkward fit for parsing.

Classp takes a radically different approach; it starts with the abstract
syntax tree (AST) which represents the logical structure of the language.
These ASTs are defined with classes and with inheritance, just as you would
normally define an AST in a modern object-oriented language. Each class
has a class pattern (hence, CLASSPattern) which says what that class looks
like in language form. Classp turns these class patterns into a parser and
formatter for the AST.

A Classp AST description is compiled by classp into C++, Bison, and Flex.
The language is documented in the [language manual](https://docs.google.com/document/d/1hqtbPkKbA5janVVb0xEXg4n1nGKs3tCgijK2PFRCx2k/edit?usp=sharing).
The program itself is documented in the [user manual](https://docs.google.com/document/d/1Qq3R87a-_Aru8DXXVpxM35y5rWWwrJ1tpKoWs3y0o1U/edit?usp=sharing).

#BUILDING AND TESTING
The source code is in the src subdirectory. It runs on Linux and builds with Bison 3.02
or later, Flex 2.5.39 or later, and g++ 4.8.2.

Cd to the src directory and type

  make classp

This will create all intermediate and object files as well as the executable,
classp, in the srce directory. To run tests of the code generation type

  make tests

To have some tests actually run and execute sample parses type

  make samples

#CLASSP FOR THE IMPATIENT
After building Classp as described above, here is a quick example of using it. Suppose
the main directory is DIR, then enter the following lines in the shell:
```
export CLASSP_TOP_DIR=DIR
alias classp=$CLASSP_TOP_DIR/src/classp
export CLASSP_INCLUDE=$CLASSP_TOP_DIR/include
```
Create a classp file:
```
cat >test.classp <<EOF
class Expression {
  %parseable;
  sample('1+2+3', succeed);
}
class Sum: Expression {
  Expression arg1;
  Expression arg2;
  syntax(arg1 '+' arg2);
}
class Int: Expression {
  int n;
  syntax(n);
}
EOF
```
then build and execute
```
classp -s test
```
This command builds and runs a test program that parses the sample expression
and then formats it. The output looks like this:
```
executing: bison -o test.yacc.cc test.y
test.y: warning: 1 shift/reduce conflict [-Wconflicts-sr]
executing: flex -o test.lex.cc test.l
executing: g++ -g -o test.exe -std=c++11 -I/usr/local/google/home/dgudeman/classp/include -Wall -DPARSER_TEST test.yacc.cc test.lex.cc
executing: ./test.exe --samples
parsing sample '1+2+3':
parsing formatted result '1 + 2 + 3'
SUCCEEDS
Errors: 0
```
It tells you what commands it is executing so that you can try them
individually if you want. It prints the string that it is about to
parse: '1+2+3' and then formats the parsed AST back out: '1 + 2 + 3'.

To run your own tests, build a statically linked library:
```
classp -La test
```
Create a main:
```
cat >main.cc <<EOF
#include "test.h"
#include <iostream>
int main(int, char**) {
  test::AstNode* expr = test::Expression::parse(std::cin, std::cout);
  if (expr) { expr->format(std::cout); std::cout << "\n"; }
  else std::cout << "parse failed\n";
  return 0;
}
EOF
```
Build it
```
g++ -std=c++11 -I$CLASSP_INCLUDE main.cc test.a
```
Run the program
```
./a.out <<EOF
1+2+3
EOF
```

#STATUS
The system does not have any hooks for doing anything with the AST once it
is constructed other than formatting it or printing a tree representation.
Essentially, this is still a demo system and not a real parsing tool.

Avoid using precedence indicators on productions with array attributes. The
grammars generated from this can cause the Bison-generated parsers to go
into an infinite loop.
