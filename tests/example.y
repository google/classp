/* BEGIN PARSER */
%code requires {

#include <string>
using std::string;
#include <vector>
using std::vector;

#include "classp.h"

namespace example {
class ParseDriver;
class AstNode;
/* BEGIN FORWARD DECLARATIONS */
class Expression;
class IntegerLiteral;
class Variable;
/* END FORWARD DECLARATIONS */
}

}

%require "3.0.2"
%defines
%define api.value.type variant
%define "parser_class_name" {YYParser}
%error-verbose
%lex-param {example::ParseDriver* parser}
%locations
%define api.namespace {example}
%parse-param {example::ParseDriver* parser}
%parse-param {example::AstNode** result}
%define api.token.constructor
%skeleton "lalr1.cc"

%initial-action {
}

%token TOK_EOF 0              "end of file"

/* BEGIN GENERATED TOKEN DECLARATIONS */
/* END GENERATED TOKEN DECLARATIONS */

%token TOK_IDENTIFIER         "identifier"
%token TOK_INT64    "integer literal"
%token TOK_STRING_LITERAL     "string literal"


/* BEGIN NONTERMINAL TYPES */
%type <Expression*> class_Expression
%type <IntegerLiteral*> class_IntegerLiteral
%type <Variable*> class_Variable
/* END NONTERMINAL TYPES */

%type <int64_t> TOK_INT64
%type <string>  TOK_IDENTIFIER
%type <string>  TOK_STRING_LITERAL

%code {

#include "example.yacc.hh"
#include "example-driver.h"
#include "example.h"

namespace example {

static inline YYParser::symbol_type yylex(ParseDriver* parser) {
    return parser->lexer_->NextToken(parser);
}

}  // namespace example

}


%start start

%%

start
/* BEGIN PARSEABLE */
  : class_Expression TOK_EOF { *result = $1; }
/* END PARSEABLE */
  ;

/* BEGIN PRODUCTIONS */
class_Expression
  : class_Variable { $$ = $1; }
  | class_IntegerLiteral { $$ = $1; }
  ;

class_Variable
  : TOK_IDENTIFIER {
      $$ = new Variable(@$, $1); }
  ;

class_IntegerLiteral
  : TOK_INT64 {
      $$ = new IntegerLiteral(@$, $1); }
  ;

/* END PRODUCTIONS */

%%

/* END PARSER */
