/*
 * This file is a part of the Classp parser, formatter, and AST generator.
 * Author: David Gudeman
 * Description: Parser for the Classp language.
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
%code requires {

#include <string>
using std::string;
#include <vector>
using std::vector;

namespace classp {
struct ParseTree;
struct ParseTreeList;
struct ParseTreeSymbol;
class ParserBase;
class ParseTreeAltList;
}

}

%require "3.0.2"
%defines                                   // Create a .h file.
%define api.value.type variant             // Use variants instead of %union
%define "parser_class_name" {yyParser}     // Name of the generated class.
%error-verbose                             // Produce comprehensible errors.
%lex-param {classp::ParserBase* parser}    // Passed to classp::classplex()
%locations                                 // Generate source locations.
%define api.namespace {classp}             // Namespace for parser.
%parse-param {classp::ParserBase* parser}  // The parser to use for reductions.
%parse-param {classp::ParseTree** result}  // The result of the parsing.
%define api.token.constructor              // constructors for tokens
%skeleton "lalr1.cc"                       // Generate C++.

/* The code that initializes the source location at the beginning of a file. */
%initial-action {
  parser->token_names_ = yytname_;
}

/*
   The declaration of tokens.  The word that follows most of the token names is
   used for emitting error messages.  We use backquotes to defeat the broken
   heuristics of Bison.
*/
%token TOK_EOF 0              "end of file"

/* Simple tokens. */
%token TOK_AND                "`&&`"
%token TOK_BAR                "`|`"
%token TOK_COLON              "`:`"
%token TOK_COMMA              "`,`"
%token TOK_DOLLARDOLLAR       "`$$`"
%token TOK_DOTDOT             "`..`"
%token TOK_EQUAL              "`=`"
%token TOK_EQL                "`==`"
%token TOK_FALSE              "`false`"
%token TOK_GEQ                "`>=`"
%token TOK_GTR                "`>`"
%token TOK_LBRACE             "`{`"
%token TOK_LBRACK             "`[`"
%token TOK_LEQ                "`<=`"
%token TOK_LPAREN             "`(`"
%token TOK_LSHIFT             "`<<`"
%token TOK_LSS                "`<`"
%token TOK_MINUS              "`-`"
%token TOK_NEQ                "`!=`"
%token TOK_NOT                "`!`"
%token TOK_NULL               "`null`"
%token TOK_OR                 "`||`"
%token TOK_PERCENT            "`%`"
%token TOK_PERIOD             "`.`"
%token TOK_PLUS               "`+`"
%token TOK_RIGHTARROW         "`->`"
%token TOK_QUESTION           "`?`"
%token TOK_RBRACE             "`}`"
%token TOK_RBRACK             "`]`"
%token TOK_RPAREN             "`)`"
%token TOK_RSHIFT             "`>>`"
%token TOK_SEMICOLON          "`;`"
%token TOK_SLASH              "`/`"
%token TOK_STAR               "`*`"
%token TOK_TRUE               "`true`"

/* Keywords */
%token TOK_CLASS              "`class`"
%token TOK_OPTIONAL           "`optional`"
%token TOK_SYNTAX             "`syntax`"
%token TOK_SAMPLE             "`sample`"
%token TOK_DEFAULT            "`default`"

/* Complex tokens. */
%token TOK_IDENTIFIER         "identifier"
%token TOK_INTEGER_LITERAL    "integer literal"
%token TOK_FLOAT_LITERAL      "float literal"
%token TOK_SINGLE_QUOTED_STRING_LITERAL "single-quote delimited string literal"
%token TOK_DOUBLE_QUOTED_STRING_LITERAL "double-quote delimited string literal"

/* These tokens are never extracted from the source, they are used to cause
   Bison to parse a particular nonterminal. */
%token TOK_START_EXPRESSION
%token TOK_START_REFERENCE

/*
   The includes that go at the beginning of classp.yacc.cc.
*/
%code {

#include "parser-base.h"
#include "lexer-base.h"

namespace classp {

static inline yyParser::symbol_type yylex(ParserBase* parser) {
    return parser->lexer_->NextToken(parser);
}

}  // namespace classp

}

%type <classp::ParseTree*> declaration attribute_decl syntax_decl opt_syntax_decl
%type <classp::ParseTree*> syntax_item syntax_spec syntax_alt_list syntax_item_list
%type <classp::ParseTree*> opt_syntax_item_list syntax_attribute identifier sample_decl
%type < vector<ParseTree*> > declaration_list class_body features
%type <ParseTreeAltList*> syntax_case_list
%type <vector<string>> parents_list parents_list2
%type <bool> opt_optional opt_array
%type <classp::ParseTree*> conjunction
%type <classp::ParseTree*> expression opt_initializer
%type <classp::ParseTree*> comparison
%type <classp::ParseTree*> factor
%type <classp::ParseTree*> literal
%type <classp::ParseTree*> operand
%type <classp::ParseTree*> simple_expression
%type <classp::ParseTree*> term
%type <classp::ParseTree*> numeric_literal
%type <classp::ParseTreeSymbol*> string_literal
%type <int64_t> TOK_INTEGER_LITERAL
%type <double> TOK_FLOAT_LITERAL
%type <string>  TOK_IDENTIFIER opt_identifier
%type <string>  TOK_SINGLE_QUOTED_STRING_LITERAL
%type <string>  TOK_DOUBLE_QUOTED_STRING_LITERAL
%type <int64_t> additive_operator
%type <int64_t> multiplicative_operator
%type <int64_t> relational_operator
%type <int64_t> unary_operator
%type <string> opt_code_literal

%start start

/*
   And now the rules of the grammar.  As much as possible the actions of the
   grammar dispatch to some virtual operation of the object parser, so that it
   is possible to use different subclasses of classp::ParserBase for different
   purposes.  The result of the parsing is stored in *result.  As much as
   possible the names of the nonterminals follow those of the 'old' parser.
*/
%%

start:
    opt_code_literal declaration_list TOK_EOF {
      parser->AddCodePrefix($1);
      *result = new ParseTreeArray(parser, @$, $2); }
  ;

opt_code_literal:
    { $$ = string(""); }
  | TOK_SINGLE_QUOTED_STRING_LITERAL { $$ = $1; }
  ;

operand:
    literal        { $$ = $1; }
  | identifier     { $$ = $1; }
  | TOK_NULL       { $$ = nullptr; }
  | TOK_LPAREN expression TOK_RPAREN  { $$ = $2; }
  ;

identifier:
    TOK_IDENTIFIER { $$ = new ParseTreeIdentifier(parser, @$, $1); }
  ;

numeric_literal:
    TOK_INTEGER_LITERAL  { $$ = new ParseTreeInteger(parser, @$, $1); }
  | TOK_FLOAT_LITERAL    { $$ = new ParseTreeFloat(parser, @$, $1); }
  ;

literal:
    numeric_literal         { $$ = $1; }
  | TOK_TRUE                { $$ = new ParseTreeBool(parser, @$, true); }
  | TOK_FALSE               { $$ = new ParseTreeBool(parser, @$, false); }
  | string_literal          { $$ = $1; }
  ;

string_literal:
    TOK_SINGLE_QUOTED_STRING_LITERAL  {
        $$ = new ParseTreeSymbol(parser, @$, $1);
      }
  | TOK_DOUBLE_QUOTED_STRING_LITERAL  {
        $$ = new ParseTreeSymbol(parser, @$, $1);
      }
  ;

/* Expressions. */

expression:
    conjunction  { $$ = $1; }
  | expression TOK_OR conjunction  {
        $$ = new ParseTreeBinop(parser, @$, token::TOK_OR, $1, $3);
      }
  ;

conjunction:
    comparison  { $$ = $1; }
  | conjunction TOK_AND comparison  {
        $$ = new ParseTreeBinop(parser, @$, token::TOK_AND, $1, $3);
      }
  ;

comparison:
    simple_expression  { $$ = $1; }
  | simple_expression relational_operator simple_expression  {
        $$ = new ParseTreeBinop(parser, @$, $2, $1, $3);
      }
  ;

relational_operator:
    TOK_EQL  { $$ = token::TOK_EQL; }
  | TOK_NEQ  { $$ = token::TOK_NEQ; }
  | TOK_LSS  { $$ = token::TOK_LSS; }
  | TOK_LEQ  { $$ = token::TOK_LEQ; }
  | TOK_GTR  { $$ = token::TOK_GTR; }
  | TOK_GEQ  { $$ = token::TOK_GEQ; }
  ;

simple_expression:
    term  { $$ = $1; }
  | simple_expression additive_operator term  {
        $$ = new ParseTreeBinop(parser, @$, $2, $1, $3);
      }
  ;

additive_operator:
    TOK_PLUS   { $$ = token::TOK_PLUS; }
  | TOK_MINUS  { $$ = token::TOK_MINUS; }
  ;

term:
    factor  { $$ = $1; }
  | term multiplicative_operator factor  {
        $$ = new ParseTreeBinop(parser, @$, $2, $1, $3);
      }
  ;

multiplicative_operator:
    TOK_STAR     { $$ = token::TOK_STAR; }
  | TOK_SLASH    { $$ = token::TOK_SLASH; }
  | TOK_PERCENT  { $$ = token::TOK_PERCENT; }
  | TOK_LSHIFT   { $$ = token::TOK_LSHIFT; }
  | TOK_RSHIFT   { $$ = token::TOK_RSHIFT; }
  ;

factor:
    operand  { $$ = $1; }
  | unary_operator operand  {
        $$ = new ParseTreeUnop(parser, @$, $1, $2);
      }
  ;

unary_operator:
    TOK_MINUS  { $$ = token::TOK_MINUS; }
  | TOK_NOT    { $$ = token::TOK_NOT; }
  ;

declaration_list:
    /* for now we don't accept empty files */
    declaration { $$ = vector<ParseTree*>(); $$.push_back($1); }
  | declaration_list opt_semicolon declaration { $$ = $1; $$.push_back($3); }
  ;

opt_semicolon: | TOK_SEMICOLON;

declaration:
    TOK_CLASS TOK_IDENTIFIER parents_list TOK_LBRACE class_body TOK_RBRACE {
    $$ = new ParseTreeClassDecl(parser, @$, $2, $3, $5);
    }
  ;

parents_list:
    { $$ = vector<string>(); }
  | TOK_COLON parents_list2 { $$ = $2; }
  ;

parents_list2:
    TOK_IDENTIFIER { $$ = vector<string>(); $$.push_back($1); }
  | parents_list2 TOK_COMMA TOK_IDENTIFIER { $$ = $1; $$.push_back($3); }
  ;

class_body:
    { $$ = vector<ParseTree*>(); }
  | class_body attribute_decl TOK_SEMICOLON { $$ = $1; $$.push_back($2); }
  ;

attribute_decl:
    opt_optional TOK_IDENTIFIER TOK_IDENTIFIER opt_array opt_initializer
    opt_syntax_decl {
      $$ = new ParseTreeAttribute(parser, @$, $3, $2, $1, $4, $5, $6); }
  | syntax_decl { $$ = $1; }
  | sample_decl { $$ = $1; }
  | TOK_PERCENT identifier { $$ = $2; }
  ;

opt_initializer:
    { $$ = nullptr; }
  | TOK_DEFAULT expression { $$ = $2; }

opt_optional:
    { $$ = false; }
  | TOK_OPTIONAL { $$ = true; }

opt_array:
    { $$ = false; }
  | TOK_LBRACK TOK_RBRACK  { $$ = true; }

opt_identifier:
    { $$ = ""; }
  | TOK_IDENTIFIER { $$ = $1; }
  ;

opt_syntax_decl:
    { $$ = nullptr; }
  | syntax_decl { $$ = $1; }

syntax_decl:
    TOK_SYNTAX opt_identifier TOK_LPAREN syntax_alt_list TOK_RPAREN features {
      $$ = new ParseTreeSyntaxDecl(parser, @$, $2, $6, $4); }
  ;

sample_decl:
    TOK_SAMPLE TOK_LPAREN string_literal TOK_RPAREN {
      $$ = new ParseTreeSample(parser, @$, $3, nullptr); }
  | TOK_SAMPLE TOK_LPAREN string_literal TOK_COMMA identifier TOK_RPAREN {
      $$ = new ParseTreeSample(parser, @$, $3, $5); }
  | TOK_SAMPLE TOK_LPAREN string_literal TOK_COMMA string_literal TOK_RPAREN {
      $$ = new ParseTreeSample(parser, @$, $3, $5); }
  ;

features:
    { $$ = vector<ParseTree*>(); }
  | features TOK_PERCENT identifier numeric_literal {
    $$ = $1;
    $$.push_back(new ParseTreeBinop(parser, @$, token::TOK_PERCENT, $3, $4));
    }
  ;

syntax_alt_list:
    syntax_spec { $$ = $1; }
  | syntax_alt_list TOK_BAR syntax_spec {
    // push a sequence of nested syntax_alt_lists into a single list
    ParseTreeAltList* alts = $1->AsAltList();
    if (!alts) {
      alts = new ParseTreeAltList(parser, @$);
      alts->array.push_back($1);
    }
    alts->array.push_back($3);
    $$ = alts;
  }
  ;

syntax_spec:
    opt_syntax_item_list { $$ = $1; }
  | opt_syntax_item_list TOK_STAR syntax_item_list {
      $$ = new ParseTreeBinop(parser, @$, token::TOK_STAR, $1, $3); }
  | opt_syntax_item_list TOK_PLUS syntax_item_list {
      $$ = new ParseTreeBinop(parser, @$, token::TOK_PLUS, $1, $3); }
  | opt_syntax_item_list TOK_STAR {
      $$ = new ParseTreeUnop(parser, @$, token::TOK_STAR, $1); }
  | opt_syntax_item_list TOK_PLUS {
      $$ = new ParseTreeUnop(parser, @$, token::TOK_PLUS, $1); }
  | opt_syntax_item_list TOK_QUESTION {
      $$ = new ParseTreeUnop(parser, @$, token::TOK_QUESTION, $1); }
  ;

opt_syntax_item_list:
    { $$ = new ParseTreeSymbol(parser, @$, ""); }
  | syntax_item_list { $$ = $1; }

syntax_item_list:
    syntax_item { $$ = $1; }
  | syntax_item_list syntax_item {
    // push a sequence of nested syntax_item_lists into a single list
    ParseTreeItemList* items = $1->AsItemList();
    if (!items) {
      items = new ParseTreeItemList(parser, @$);
      items->array.push_back($1);
    }
    items->array.push_back($2);
    $$ = items;
  }
  ;

syntax_item:
    string_literal { $$ = $1; }
  | syntax_attribute { $$ = $1; }
  | syntax_attribute TOK_LBRACE syntax_case_list TOK_RBRACE {
    $$ = new ParseTreeBinop(parser, @$, token::TOK_LBRACE, $1, $3);}
  | TOK_LPAREN syntax_alt_list TOK_RPAREN  { $$ = $2; }
  ;

  syntax_attribute:
    identifier { $$ = $1; }
  ;

syntax_case_list:
    expression TOK_RIGHTARROW syntax_spec {
    $$ = new ParseTreeAltList(parser, @$);
    $$->array.push_back(
        new ParseTreeBinop(parser, @$, token::TOK_RIGHTARROW, $3, $1)); }
  | syntax_case_list TOK_BAR expression TOK_RIGHTARROW syntax_spec {
      $$ = $1; 
      $$->array.push_back(
          new ParseTreeBinop(parser, @$, token::TOK_RIGHTARROW, $5, $3)); }
  ;
%%
