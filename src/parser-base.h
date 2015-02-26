/*
 * This file is a part of the Classp parser, formatter, and AST generator.
 * Author: David Gudeman
 * Description: ParserBase is the state object used by the parser and the
 *  various tree traversals. This class also controls the printing of the
 *  output.
 *
 * Copyright 015 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/
#ifndef PARSER_BASE_H_
#define PARSER_BASE_H_

#include "parse-tree.h"
#include "stringprintf.h"

namespace classp {
class ParserBase;
struct ParseTreeSymbol;
struct ParseTreeIdentifier;
struct ParseTreeBinop;
struct ParseTreeArray;
struct ParseTreeClassDecl;
struct ParseTreeAttribute;
struct ParseTreeSyntaxDecl;
struct ParseTreeItemList;
struct ParseTreeAltList;

class ParserBase {
 public:
  ParserBase(FILE* source);
  virtual ~ParserBase();
  ParseTree* Parse();

  // A function used by the lexer to report errors.
  void Error(const yyParser::location_type& loc, const std::string& msg);
  void Error(const std::string& msg);
  void Fatal[[noreturn]](const yyParser::location_type& loc,
                         const std::string& msg);

  // Set the start symbol.
  void AddStart(ParseTreeClassDecl* start_class);

  void AddSample(const string& class_name, ParseTreeSample* sample);

  // Add a class to the list of classes that have productions.
  void AddClassProduction(ParseTreeClassDecl* c);

  void AddCodePrefix(const string& code) { code_prefix_ = code; }

  // These are automatically generated functions to print the output files.
  void PrintLexer(ostream& out);
  void PrintParser(ostream& out);
  void PrintHFile(ostream& out);


  // These are helpers for the automatically generated functions above.
  // Prints the actual lex commands to scan the tokens.
  void PrintTokenLexers(ostream& out);
  // Prints the yacc token declarations.
  void PrintTokenDeclarations(ostream& out);
  // Prints the types for the yacc nonterminals.
  void PrintNonterminalTypes(ostream& out);
  // Prints the yacc productions,
  void PrintProductions(ostream& out);
  // Prints the forward delcarations for the AST node classes.
  void PrintForwardDeclarations(ostream& out);
  // Prints the AST node class definitions.
  void PrintClassDefinitions(ostream& out);
  // Prints the parseable class rules for the start production.
  void PrintParseableClassRules(ostream& out);
  // Prints a parsing expression for the automatically generated tests.
  void PrintParseParseable(ostream& out);
  // Prints the definitions of the class methods.
  void PrintMethodDefinitioms(ostream& out);
  // Prints code to test samples.
  void PrintSampleTests(ostream& out);
  // Prints the code prefix (this goes at the top of the .h file)
  void PrintCodePrefix(ostream& out);

  // The Get...Matcher() functions return a string that can be used in place in
  // a rule to match something. It might be a nonterminal, a token name, or a
  // sequence of token names. These functions also have the side effect of
  // maintaining the various generating tables and sometimes, of actually
  // generating the rule.

  // Add type_name to the various generating tables.
  void AddParsedType(const string& type_name, const string& nonterminal_type);

  void AddPrecedenceProductions(const string& class_name,
                                set<Precedence> precedences);

  // Returns the name of a nonterminal to parse an object of type type_name.
  string GetTypeMatcher(const string& type_name) {
    return GetTypeMatcher(type_name, 0);
  }

  // Returns the name of a nonterminal to parse an object of type type_name
  // with the given precedence.
  string GetTypeMatcher(const string& type_name, int precedence);

  // Returns a token name to match a word token.
  string GetWordMatcher(const string& type_name);

  // Returns a token name to match a non-word token.
  string GetSymbolMatcher(const string& type_name);

  // Returns a sequence of token names to match a sequence of tokens given as
  // a single string.
  string GetStringMatcher(const string& token, int* count);

  // Create a nonterminal and add it to the parsing tables. The nonterminal
  // is formatted like a printf statement.
  string CreateNontermial(const string& declared_type, const char* format, ...);

  string GetTokenName(int64_t token) { return token_names_[token]; }

  string GetNonterminalType(const string& nonterminal) {
    return tag_to_nonterminal_type_[nonterminal_to_tag_[nonterminal]];
  }

  ParseTreeClassDecl* GetClassDecl(const string& name) {
    auto elem = class_decls_.find(name);
    if (elem == class_decls_.end()) return nullptr;
    return elem->second;
  }

  void SetNamespace(const string& name) {
    namespace_tag = name;
  }

  string ModuleName() { return namespace_tag; }
  string ModuleNameUpper() { return namespace_tag; }
  string NamespaceName() { return namespace_tag; }

  FILE* source_;
  LexerBase* lexer_;
  int num_errors_ = 0;
  string code_prefix_;
  const char* const* token_names_ = nullptr;
  const char* directory = "directory";  // NOT IMPLEMENTED
  string namespace_tag = "classp";  // NOT IMPLEMENTED

  ParseTreeArray* decl_list;  // the parsed list of declarations

  // A list of all classes. It is set in the class constructor.
  map<string, ParseTreeClassDecl*> class_decls_;

  // A list of syntax samples for testing.
  std::multimap<string, ParseTreeSample*> samples;

  // The following are the generating tables referred to in other comments. They
  // are used in decorating the tree and generating the output.

  // A map (type_tag->nonterminal_type) for setting up the %union field.
  map<string, string> tag_to_nonterminal_type_;

  // A map (nonterminal->type_tag) for declaring the types of nonterminals.
  map<string, string> nonterminal_to_tag_;

  // A map (nonterminal->number-of-uses) for recording how many times a base
  // nonterminal name was used. Certain kinds of nonterminals are uniquified by
  // adding __n where n is the next larger number.
  map<string, int> uniquifier_;

  // A map (type_name->nonterminal) telling how to parse each base type
  map<string, string> type_to_nonterminal_;

  // A map (type_name->nonterminal_type) giving the actual type returned by a
  // base type.
  map<string, string> type_to_nonterminal_type_;

  // A map (token->token_name) giving the token name to use in a rule for
  // simple tokens representing a single symbol.
  map<string, string> token_to_name_;

  // A map (token_name->definition) for complex tokens defined by the user
  map<string, string> token_name_to_definition_;

  // A set of all class that have special precedence productions. If a class
  // is in this set, then GetTypeMatcher() returns a name specialized for the
  // precedence.
  set<string> precedence_classes;

  // The following are used for output.

  // The actual text of the rules.
  vector<string> rules_;

  // The class entries that go in the .h file.
  vector<string> class_defs_;

  // start_class_ is the single class what we can parse. It would be better to
  // make all classes parseable, which we could do even in bison using this
  // trick:
  // http://www.gnu.org/software/bison/manual/html_node/Multiple-start_002dsymbols.html
  // but when there are precedences, you have to be careful to avoid having
  // the number of productions blow up. When B and C both extend A and B has
  // higher precedence than C, then when parsing A you need a rule for C that
  // mentions B, but if you want to parse B on its own, then you need another
  // rule for B that doesn't mention C.
  ParseTreeClassDecl* start_class_ = nullptr;

  // A list of classes that we need to create productions for.
  vector<ParseTreeClassDecl*> class_productions;
};

string PrecedenceNonterminal(const string& nonterminal, int prec);
inline string ClassDeclareType(const string& class_name) {
  return class_name + "*";
}
string ExtractTag(const string& declared_type);

string QuoteStringForC(const string& s);

}  // namespace classp

#endif  // PARSER_BASE_H_
