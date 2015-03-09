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
The language is documented in the [language manual](https://docs.google.com/document/d/1hqtbPkKbA5janVVb0xEXg4n1nGKs3tCgijK2PFRCx2k/pub).
The program itself is documented in the [user manual](https://docs.google.com/document/d/1Qq3R87a-_Aru8DXXVpxM35y5rWWwrJ1tpKoWs3y0o1U/pub).

#BUILDING AND TESTING
The source code is in the src subdirectory. It builds with Bison 3.02 or later,
Flex 2.5.39 or later, and g++ 4.8.2.

Cd to the src directory and type

  make classp

This will create all intermediate and object files as well as the executable,
classp, in the srce directory. To run tests of the code generation type

  make tests

To have some tests actually run and execute sample parses type

  make samples

#STATUS
The system does not have any hooks for doing anything with the AST once it
is constructed other than formatting it or printing a tree representation.
Essentially, this is still a demo system and not a real parsing tool.

Avoid using precedence indicators on productions with array attributes. The
grammars generated from this can cause the Bison-generated parsers to go
into an infinite loop.
