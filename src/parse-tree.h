/*
 * This file is a part of the Classp parser, formatter, and AST generator.
 * Description: These structs are used for both the parse tree and AST.
 *
 * Copyright 2015 Google Inc.
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
#ifndef PARSE_TREE_H_
#define PARSE_TREE_H_

// Generated include files
#include "classp.yacc.hh"
#include "location.hh"
#include "position.hh"

#include <assert.h>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

namespace classp {
using std::vector;
using std::map;
using std::multimap;
using std::pair;
using std::ostream;
using std::set;
using std::stringstream;
using std::string;

class FormatInfo;
struct ParseTreeSymbol;
struct ParseTreeIdentifier;
struct ParseTreeBinop;
struct ParseTreeArray;
struct ParseTreeClassDecl;
struct ParseTreeAttribute;
struct ParseTreeSyntaxDecl;
struct ParseTreeItemList;
struct ParseTreeAltList;
struct ParseTreeSample;
class ParserBase;
class LexerBase;

typedef yyParser::token token;

#define SELF_ATTR_NAME "self"

enum Associativity { AssocNone, AssocLeft, AssocRight, AssocNassoc };
// Precedence is a typedef because it may prove convenient to make it a float
// instead of an int so that users can modify a grammar by putting a new
// precedence in between existing precedences.
typedef int Precedence;

// Positional matches are values that are matched by position in a rule. For
// example: A: B C { foo($1, $2); }. Here, B and C are positional matches
// represented by $1 and $2. PositionalMatches records lists of such matches
// in the form <position, attribute>. If second is nullptr then first
// is the position of an AttributeMap, which is a collecion of
// keyword parameters.
typedef vector<pair<int, ParseTreeAttribute*> > PositionalMatches;

// Keyword matches are values that are matched by name. These are generated
// for example by alternations like "(x | y)" where x and y are attributes.
// These matches are bubbled up the parse tree in a generated map and
// extracted in the generated constructor. KeywordMatches is a set of such
// attributes.
typedef set<ParseTreeAttribute*> KeywordMatches;

struct RuleInfo;

// When formating an alternation there are three cases for each alternate:
// 1. Fully determining such as an optional attribute or an array which can
//    have zero elements. The alternate must be printed iff it is present.
// 2. Semi-determining which is a required attribute with a default. These are
//    only printed if they have a non-default value or if we have to print
//    the default because there is nothing else to print.
// 3. Non-determining such as a literal or a required non-array attribute with
//    no default. Every alternate must contain at least one thing which is not
//    in this category.
enum AlternateDetermination {
  FULLY_DETERMINING, SEMI_DETERMINING, NOT_DETERMINING};

struct ParseTree {
  ParseTree(ParserBase* parser, const yyParser::location_type& location)
      : parser(parser), location(location) {}
  virtual ~ParseTree() {}

  void Error(const std::string& msg);
  void Fatal[[noreturn]](const std::string& msg);

  // Prints a representation of the parse node
  virtual void Print(ostream& stream_out) const = 0;

  // Run-time type functions return this if the type is correct, otherwise
  // nullptr.
  virtual ParseTreeSymbol* AsSymbol() { return nullptr; }
  virtual ParseTreeIdentifier* AsIdentifier() { return nullptr; }
  virtual ParseTreeBinop* AsBinop() { return nullptr; }
  virtual ParseTreeArray* AsArray() { return nullptr; }
  virtual ParseTreeClassDecl* AsClass() { return nullptr; }
  virtual ParseTreeAttribute* AsAttribute() { return nullptr; }
  virtual ParseTreeSyntaxDecl* AsSyntax() { return nullptr; }
  virtual ParseTreeItemList* AsItemList() { return nullptr; }
  virtual ParseTreeAltList* AsAltList() { return nullptr; }

  // If this is an identifier and it names an associativity then return the
  // named assicativity. Otherwise it is an error.
  virtual Associativity GetAssoc() {
    Error("Not an associativity.");
    return AssocNone;
  }

  // If this is a number, return it as a Precedence. Otherwise it is an error.
  virtual Precedence GetFloat() {
    Error("Expected a number.");
    return 0.0;
  }

  // Pass 1 of a series of passes designed to convert the parse tree into
  // something more like an abstract syntax tree and then generate the output.
  virtual void Decorate1() { Fatal("illegal Decorate1"); }

  virtual bool HasOptionalAttribute() { return false; }

  // Resolve names in syntax declarations to attributes. This is part of pass
  // Decorate2.
  virtual ParseTree* ResolveSyntax(ParseTreeSyntaxDecl* syntax_def,
                                   bool nested_alternate) {
    return this;
  }

  // Does some error checking on the declarations.
  virtual void Validate() { Fatal("illegal Validate"); }

  // Prints to  the sequence of terminals and nonterminals to match this.
  // class_def is the class being parsed. positition is the index that the
  // generated pieces start with (used for match_list). All attributes matched
  // are appened to match_list along with their index. Returns the
  // index of the next piece that would be generated after this one is done.
  virtual int GenerateMatcher(RuleInfo* rule_info, int position, bool is_last) {
    Fatal("illegal GenerateMatcher");
  }

  // Generates an action assigning results for a rule.
  void GenerateAction(RuleInfo* rule_info);

  // Generates a single rule for recognizing this pattern, including the
  // action to return the matched values.
  virtual void GenerateRuleForAlternate(RuleInfo* rule_info);

  // Generates an expression to stream.
  virtual void GenerateExpression(ostream& stream) { Print(stream); }

  virtual int GenerateAssignmentListMatcher(RuleInfo* rule_info, ParseTree* t1,
                                            ParseTree* t2, int position);

  virtual bool IsAssignment() { return false; }

  // Generates to stream the sequence needed to match an array.
  int GenerateArrayMatcher(RuleInfo* rule_info, int min_size, ParseTree* t1,
                           ParseTree* t2, int position);

  int GenerateSingleArrayMatcher(RuleInfo* rule_info, int min_size,
                                 ParseTreeAttribute* attribute,
                                 ParseTree* separator, int position);

  int GenerateMultiArrayMatcher(RuleInfo* rule_info, int min_size,
                                ParseTreeItemList* item_list,
                                ParseTree* separator, int position);

  int GenerateAltIteratorMatcher(RuleInfo* rule_info, int min_size,
                                 ParseTreeAltList* alt_list,
                                 ParseTree* separator_pattern, int position);

  virtual bool IsLiteral() const { return true; }

  virtual bool HasArrayAttribute() { return false; }

  // Generates code to format the node.
  virtual void GenerateFormatter(const FormatInfo& format_info,
                                 const string& separator, bool is_first,
                                 bool is_last);

  void GenerateAltIterFormatter(const FormatInfo& format_info,
                                const string& separator, bool is_first,
                                bool is_last, ParseTreeAltList* alt_list,
                                ParseTree* t2);

  void GenerateArrayFormatter(const FormatInfo& format_info,
                              const string& separator, bool is_first,
                              bool is_last, ParseTree* t1, ParseTree* t2);

  void GenerateCaseFormatter(const FormatInfo& format_info,
                             const string& separator, ParseTree* t1,
                             ParseTree* t2);

  // Write the condition used to decide whether to print this alternate and
  // return its category.
  virtual AlternateDetermination GenerateAlternateCondition(ostream& stream) {
    return NOT_DETERMINING;
  }

  ParserBase* parser;
  const yyParser::location_type location;
};

struct ParseTreeBool : public ParseTree {
  ParseTreeBool(ParserBase* parser, const yyParser::location_type& location,
                const bool value)
      : ParseTree(parser, location), value(value) {}
  virtual ~ParseTreeBool() {}
  void Print(ostream& stream_out) const override {
    stream_out << (value ? "true" : "false");
  }

  const bool value;
};

struct ParseTreeInteger : public ParseTree {
  ParseTreeInteger(ParserBase* parser, const yyParser::location_type& location,
                   const int64_t value)
      : ParseTree(parser, location), value(value) {}
  virtual ~ParseTreeInteger() {}
  void Print(ostream& stream_out) const override { stream_out << value; }

  Precedence GetFloat() override { return value; }

  const int64_t value;
};

struct ParseTreeFloat : public ParseTree {
  ParseTreeFloat(ParserBase* parser, const yyParser::location_type& location,
                 const double value)
      : ParseTree(parser, location), value(value) {}
  virtual ~ParseTreeFloat() {}
  void Print(ostream& stream_out) const override { stream_out << value; }

  const double value;
};

struct ParseTreeSymbol : public ParseTree {
  ParseTreeSymbol(ParserBase* parser, const yyParser::location_type& location,
                  const string& value)
      : ParseTree(parser, location), value(value) {}
  virtual ~ParseTreeSymbol() {}

  ParseTreeSymbol* AsSymbol() override { return this; }

  void Print(ostream& stream_out) const override {
    stream_out << "\"" << value << "\"";
  }

  int GenerateMatcher(RuleInfo* rule_info, int position, bool is_last) override;

  void GenerateFormatter(const FormatInfo& format_info, const string& separator,
                         bool is_first, bool is_last) override;
  const string value;
};

struct ParseTreeIdentifier : public ParseTree {
  ParseTreeIdentifier(ParserBase* parser,
                      const yyParser::location_type& location,
                      const string& value)
      : ParseTree(parser, location), value(value) {}
  virtual ~ParseTreeIdentifier() {}

  ParseTreeIdentifier* AsIdentifier() override { return this; }

  void Print(ostream& stream_out) const override { stream_out << value; }
  ParseTree* ResolveSyntax(ParseTreeSyntaxDecl* syntax_def,
                           bool nested_alternate) override;
  Associativity GetAssoc() override;
  void GenerateExpression(ostream& stream) override;

  const string value;
};

struct ParseTreeMulti : public ParseTree {
  ParseTreeMulti(ParserBase* parser, const yyParser::location_type& location)
      : ParseTree(parser, location) {}
  ParseTreeMulti(ParserBase* parser, const yyParser::location_type& location,
                 const vector<ParseTree*>& array)
      : ParseTree(parser, location), array(array) {}
  virtual ~ParseTreeMulti() {}

  ParseTree* ResolveSyntax(ParseTreeSyntaxDecl* syntax_def,
                           bool nested_alternate) override;

  bool HasOptionalAttribute() override;

  AlternateDetermination GenerateAlternateCondition(ostream& stream) override;

  bool IsLiteral() const override {
    for (auto elem : array) {
      if (!elem->IsLiteral()) return false;
    }
    return true;
  }

  bool HasArrayAttribute() override {
    for (auto elem : array) {
      if (elem->HasArrayAttribute()) return true;
    }
    return false;
  }
  vector<ParseTree*> array;
};

struct ParseTreeUnop : public ParseTreeMulti {
  ParseTreeUnop(ParserBase* parser, const yyParser::location_type& location,
                const int64_t op, ParseTree* op1)
      : ParseTreeMulti(parser, location), op(op) {
    array.push_back(op1);
  }
  virtual ~ParseTreeUnop() {}
  const ParseTree* operand1() const { return array[0]; }
  ParseTree*& operand1() { return array[0]; }
  void Print(ostream& stream_out) const override;
  ParseTree* ResolveSyntax(ParseTreeSyntaxDecl* syntax_def,
                           bool nested_alternate) override;
  int GenerateMatcher(RuleInfo* rule_info, int position, bool is_last) override;
  void GenerateFormatter(const FormatInfo& format_info, const string& separator,
                         bool is_first, bool is_last) override;

  const int64_t op;
};

struct ParseTreeBinop : public ParseTreeMulti {
  ParseTreeBinop(ParserBase* parser, const yyParser::location_type& location,
                 const int64_t op, ParseTree* op1, ParseTree* op2)
      : ParseTreeMulti(parser, location), op(op) {
    array.push_back(op1);
    array.push_back(op2);
  }
  virtual ~ParseTreeBinop() {}
  ParseTreeBinop* AsBinop() override { return this; }
  const ParseTree* operand1() const { return array[0]; }
  const ParseTree* operand2() const { return array[1]; }
  ParseTree*& operand1() { return array[0]; }
  ParseTree*& operand2() { return array[1]; }
  void Print(ostream& stream_out) const override;
  ParseTree* ResolveSyntax(ParseTreeSyntaxDecl* syntax_def,
                           bool nested_alternate) override;
  int GenerateMatcher(RuleInfo* rule_info, int position, bool is_last) override;
  void GenerateRuleForAlternate(RuleInfo* rule_info) override;
  bool IsAssignment() override { return op == token::TOK_RIGHTARROW; }
  void GenerateFormatter(const FormatInfo& format_info, const string& separator,
                         bool is_first, bool is_last) override;

  const int64_t op;
};

struct ParseTreeArray : public ParseTreeMulti {
  ParseTreeArray(ParserBase* parser, const yyParser::location_type& location,
                 const vector<ParseTree*> array)
      : ParseTreeMulti(parser, location, array) {}
  virtual ~ParseTreeArray() {}

  ParseTreeArray* AsArray() override { return this; }

  void Print(ostream& stream_out) const override;
  void Decorate1() override;
  // Compacts all of the user-provided precedence numbers so that they are all
  // in a compact sequence 1..n with no gaps.
  //  void ConvertPrecedences();

  // Pass 2 of the conversion to an AST. This pass calls ResolveSyntax().
  void Decorate2();

  // Pass 3 of the conversion to AST. This one deals with precedence.
  void Decorate3();
};

struct ParseTreeClassDecl : public ParseTree {
  ParseTreeClassDecl(ParserBase* parser,
                     const yyParser::location_type& location,
                     const string& class_name, const vector<string>& parent_classes,
                     const vector<ParseTree*>& class_body);
  virtual ~ParseTreeClassDecl() {}

  void Print(ostream& stream_out) const override;

  ParseTreeClassDecl* AsClass() override { return this; }

  ParseTreeClassDecl* LookupSuperclass(const string& name);

  void Decorate1() override;

  void Validate() override;

  ParseTree* ResolveSyntax(ParseTreeSyntaxDecl* syntax_def,
                           bool nested_alternate) override {
    assert(false);
  }
  void ResolveSyntax();

  // Go through the class definition and assign all non-optional attributes as
  // constructor arguments.
  void SetConstructorArguments();

  void GenerateClassDefinition(ostream& out);

  void GenerateMethodDefinitions(ostream& out);

  void GenerateProductions();

  // Add to precedences all precedences of all syntaxes and all syntaxes of all
  // descendents.
  void GetPrecedences(set<Precedence>* precedences);

  void AddPrecedenceParsed(Precedence prec);

  // Set up the precedence table and notify the parser about the extra
  // productions needed. Called in Decorate3.
  void SetupPrecedences();

  // Generate a constructor for the class.
  void GenerateConstructor(ostream& constructor_stream);

  void GenerateConstructorDeclaration(ostream& out);

  // Generate all of the constructor args except the parser state.
  void GenerateConstructorArgs(ostream& stream);

  // Generate a call to the constructor.
  void GenerateConstructorCall(ostream& stream,
                               const PositionalMatches& positional_matches);

  void GenerateAction(RuleInfo* rule_info);

  // Generate the single productions for a class that does not deal with
  // precedence.
  void GenerateSingleProduction(ostream& rule_stream);

  // Generate the productions for a class that has children with precedence.
  void GeneratePrecedenceProductions(ostream& rule_stream);

  // Adds to syntaxes all of the syntax elements of this and all descendents.
  void AddAllSyntaxes(
    multimap<Precedence, ParseTreeSyntaxDecl*>* syntaxes);

  void GenerateClassDeclarations(ostream& out);

  ParseTreeAttribute* LookupAttribute(string attribute_name);
  void AddParsingSubclass(ParseTreeClassDecl* subclass);

  ParseTreeAttribute* GetAttribute(const string& attr_name) {
    auto attr = attr_list.find(attr_name);
    if (attr != attr_list.end()) return attr->second;
    return nullptr;
  }
  void AddAttributes(const map<string, ParseTreeAttribute*>& attrs);

  // Copies to the syntaxe of source to the parsed_syntax member of all ancestor
  // classes (including this as an ancestor) that have the is_parsed flag set.
  void CopySyntaxToParsedAncestors(ParseTreeClassDecl* source);

  // Set the has_precedence flag for this and all subclasses.
  void SetHasPrecedence();

  void GenerateClassFormatter(ostream& out);
  void GenerateClassFormatterDeclarations(ostream& out);

  const string class_name;
  vector<string> parent_classes;
  vector<ParseTree*> class_body;

  // Used by parser-base to avoid adding this class to a list multiple times.
  bool production_required = false;

  // The following are set in Decorate1
  vector<ParseTreeSyntaxDecl*> syntax_list;
  vector<ParseTreeClassDecl*> parsing_subclasses;
  // maps attribut_name -> ParseTreeAttribute for each attribute in class
  map<string, ParseTreeAttribute*> attr_list;
  bool parseable = false;  // true if the %parseable indicator is present

  // The following are set in Decorate2
  bool is_parsed = false;  // true if parseable or appears in a syntax.
  // A list of the fixed position parameters for the constructor in the order
  // they occur as formal parameters.
  vector<ParseTreeAttribute*> required_params;
  // How many of the required_params are inherited.
  int num_inherited_required_params;
  // Optional parameters that are represented as "keyword" parameters.
  KeywordMatches optional_params;
  // True if a keyword argument is needed in the constructor.
  bool has_keyword_arg = false;

  // The following are set in Decorate3
  // True if this or any subclass has a syntax with a precedence declaration.
  bool has_precedence = false;
  // A set of all precedences used by syntaxes of this class and subclasses.
  // This is only set if the class is parseable.
  set<Precedence> precedences;
  // The list of syntaxes that are used to create parsing rules for this class.
  vector<ParseTreeSyntaxDecl*> parsing_syntax;
};

struct ParseTreeAttribute : public ParseTree {
  ParseTreeAttribute(ParserBase* parser,
                     const yyParser::location_type& location,
                     const string& attribute_name, const string& source_type,
                     const bool is_optional, const bool is_array,
                     ParseTree* default_value, ParseTree* syntax_decl);
  ParseTreeAttribute(ParserBase* parser,
                     const yyParser::location_type& location,
                     const ParseTreeClassDecl* class_def)
      : ParseTreeAttribute(parser, location, SELF_ATTR_NAME,
                           class_def->class_name, false, false, nullptr,
                           nullptr) {
    is_self = true;
  }
  virtual ~ParseTreeAttribute() {}

  void Print(ostream& stream_out) const override;

  ParseTreeAttribute* AsAttribute() override { return this; }

  void Decorate1() override { assert(false); }
  void Decorate1(ParseTreeClassDecl* class_def);

  bool HasOptionalAttribute() override {
    return is_optional || is_array || default_value;
  }

  bool IsLiteral() const override { return false; }

  bool HasArrayAttribute() override { return is_array; }

  // Sets state of this attribute to show that it is actually formatted
  // and parsed.
  ParseTreeAttribute* SetFormatted(bool nested_alternate);

  // Returns the syntax element needed to parse this attribute. Also set up the
  // generating tables in parser.
  ParseTree* GetSyntax(ParseTreeSyntaxDecl* syntax_def, bool nested_alternate);

  int GenerateMatcher(RuleInfo* rule_info, int position, bool is_last) override;

  // These functions generate various parts of the code needed to implement
  // the attribute in the class.
  void GeneratePrinter(ostream& stream);

  void GenerateFormatter(const FormatInfo& format_info, const string& separator,
                         bool is_first, bool is_last) override;
  AlternateDetermination GenerateAlternateCondition(ostream& stream) override;

  const string attribute_name;
  const string source_type;
  const bool is_optional;
  const bool is_array;
  ParseTree* default_value;
  ParseTreeSyntaxDecl* syntax_decl;
  bool is_self = false;

  // The following are set in Decorate1
  ParseTreeClassDecl* containing_class = nullptr;
  ParseTreeClassDecl* class_val = nullptr;  // If this is of class type then
                                            // the class is class_val.
  bool is_ptr;       // true for arrays and classes, otherwise false.
  string base_type;  // Translation of the source_type to output language.
  string declare_base_type;  // base_type* if is_ptr, else just base_type.
  string full_type;          // a vector if is_array, else just base_type.
  string declare_full_type;  // full_type* if is_ptr, else just full_type.

  // The following are set in Decorate2
  bool is_nested = false;  // appears in a nested position requiring keyword arg
  bool is_required = false; // one of the params in the constructor
  bool is_case_assigned = false;  // the target in a case pattern
  bool in_syntax_decl = false;  // detects infinite recursion on syntax_decl
};

struct ParseTreeSyntaxDecl : public ParseTree {
  ParseTreeSyntaxDecl(ParserBase* parser,
                      const yyParser::location_type& location,
                      const string& syntax_type, const vector<ParseTree*>& features,
                      ParseTree* syntax)
      : ParseTree(parser, location),
        syntax_type(syntax_type),
        features(features),
        syntax(syntax) {}
  virtual ~ParseTreeSyntaxDecl() {}

  void Print(ostream& stream_out) const override;

  ParseTreeSyntaxDecl* AsSyntax() override { return this; }

  void Decorate1() override { assert(false); }
  void Decorate1(ParseTreeClassDecl* class_def, ParseTreeAttribute* attr_def);

  ParseTree* ResolveSyntax(ParseTreeSyntaxDecl* syntax_def,
                           bool nested_alternate) override {
    assert(false);
  }
  void ResolveSyntax(bool nested_alternate);

  // Returns true if this is a bracketing syntax, meaning that it is of the form
  // "<string>+ self <string>+"
  bool IsBracketSyntax();
  // Generates a rule for this syntax.
  void GenerateSyntaxRule(ostream& rule_stream,
                          const set<Precedence>& precedences);

  void GenerateClassFormatter(ostream& out,
                              const string& separator);

  // During name resolution, returns the attribute that id refers to, even if
  // it is the name "self" inside a local syntax.
  ParseTreeAttribute* GetAttributeBeingFormatted(
    ParseTreeIdentifier* id);

  const string syntax_type;
  vector<ParseTree*> features;
  ParseTree* syntax;
  bool is_self = false;

  // The following are set in Decorate1
  Associativity assoc = AssocNone;
  ParseTreeClassDecl* class_def = nullptr;  // the class this sytnax is for
  ParseTreeAttribute* attr_def = nullptr;   // the attribute for local syntaxes

  // Set to the initial value in Decorate1 but possibly updated in Decorate3
  Precedence precedence = -1;
};

struct ParseTreeItemList : public ParseTreeMulti {
  ParseTreeItemList(ParserBase* parser, const yyParser::location_type& location)
      : ParseTreeMulti(parser, location) {}
  virtual ~ParseTreeItemList() {}

  void Print(ostream& stream_out) const override;

  ParseTreeItemList* AsItemList() override { return this; }

  ParseTree* ResolveSyntax(ParseTreeSyntaxDecl* syntax_def,
                           bool nested_alternate) override;

  int GenerateMatcher(RuleInfo* rule_info, int position, bool is_last) override;

  void GenerateFormatter(const FormatInfo& format_info, const string& separator,
                         bool is_first, bool is_last) override;
};

struct ParseTreeAltList : public ParseTreeMulti {
  ParseTreeAltList(ParserBase* parser, const yyParser::location_type& location)
      : ParseTreeMulti(parser, location) {}
  virtual ~ParseTreeAltList() {}

  void Print(ostream& stream_out) const override;

  ParseTreeAltList* AsAltList() override { return this; }

  ParseTree* ResolveSyntax(ParseTreeSyntaxDecl* syntax_def,
                           bool nested_alternate) override;

  int GenerateMatcher(RuleInfo* rule_info, int position, bool is_last) override;

  void GenerateProduction(RuleInfo* rule_info, const string& nonterminal);

  void GenerateFormatter(const FormatInfo& format_info, const string& separator,
                         bool is_first, bool is_last) override;
};

struct ParseTreeSample : public ParseTree {
  ParseTreeSample(ParserBase* parser,
                      const yyParser::location_type& location,
                      ParseTreeSymbol* sample, ParseTree* result);
  virtual ~ParseTreeSample() {}

  void Print(ostream& stream_out) const override { stream_out << "sample()"; }

  void PrintExpectedResult(ostream& out);

  enum {EXPECT_SUCCEED, EXPECT_FAIL, PRINT_RESULT, COMPARE_RESULT} type;
  string sample;
  string expected_result;
};

}  // namespace classp

#endif  // PARSE_TREE_H_
