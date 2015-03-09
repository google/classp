/* BEGIN PARSER */
%code requires {

#include <string>
using std::string;
#include <vector>
using std::vector;

/* BEGIN CODE PREFIX */
/* END CODE PREFIX */

#include "classp.h"
using classp::AttributeMap;
typedef string identifier;

namespace t3 {
class ParseDriver;
class AstNode;
/* BEGIN FORWARD DECLARATIONS */
class Declaration;
class ProcedureDecl;
class Statement;
class Unparsed;
/* END FORWARD DECLARATIONS */
}

}

%require "3.0.2"
%defines
%define api.value.type variant
%define "parser_class_name" {YYParser}
%error-verbose
%lex-param {t3::ParseDriver* parser}
%locations
%define api.namespace {t3}
%parse-param {t3::ParseDriver* parser}
%parse-param {t3::AstNode** result}
%define api.token.constructor
%skeleton "lalr1.cc"

%initial-action {
}

%token TOK_EOF 0              "end of file"

/* BEGIN GENERATED TOKEN DECLARATIONS */
%token TOK_LPAREN "`(`"
%token TOK_RPAREN "`)`"
%token TOK_COMMA "`,`"
%token TOK_SEMICOLON "`;`"
%token WORD_begin "`begin`"
%token WORD_declare "`declare`"
%token WORD_end "`end`"
%token WORD_procedure "`procedure`"
/* END GENERATED TOKEN DECLARATIONS */

%token TOK_BOOL   "boolean literal"
%token TOK_IDENTIFIER         "identifier"
%token TOK_INT64    "integer literal"
%token TOK_STRING_LITERAL     "string literal"


/* BEGIN NONTERMINAL TYPES */
%type <ProcedureDecl*> class_ProcedureDecl
%type <Declaration*> class_Declaration
%type <Statement*> class_Statement
%type <vector<Declaration*>> array0_Declaration__1
%type <vector<Declaration*>> array0_Declaration__2
%type <vector<Statement*>> array0_Statement__1
%type <vector<identifier>> array0_identifier__1
/* END NONTERMINAL TYPES */

%type <bool> TOK_BOOL
%type <int64_t> TOK_INT64
%type <string>  TOK_IDENTIFIER
%type <string>  TOK_STRING_LITERAL

%code {

#include "t3.yacc.hh"
#include "t3.h"

namespace t3 {

YYParser::symbol_type yylex(ParseDriver* parser);

}  // namespace t3

}


%start start

%%

start
/* BEGIN PARSEABLE */
  : class_ProcedureDecl TOK_EOF { *result = $1; }
/* END PARSEABLE */
  ;

/* BEGIN PRODUCTIONS */
class_ProcedureDecl
  :  WORD_procedure TOK_IDENTIFIER TOK_LPAREN  array0_Declaration__1 TOK_RPAREN WORD_declare  array0_Declaration__2 WORD_begin  array0_Statement__1 WORD_end {
      $$ = new ProcedureDecl(@$, $2, $4, $7, $9); }
  ;

array0_Declaration__1
  : { $$ = vector<Declaration*>(); }
  | class_Declaration { $$ = vector<Declaration*>(); $$.emplace_back($1); }
  | array0_Declaration__1 TOK_COMMA class_Declaration { $$ = $1; $$.emplace_back($3); }
  ;

array0_Declaration__2
  : { $$ = vector<Declaration*>(); }
  | class_Declaration { $$ = vector<Declaration*>(); $$.emplace_back($1); }
  | array0_Declaration__2 TOK_SEMICOLON class_Declaration { $$ = $1; $$.emplace_back($3); }
  ;

array0_Statement__1
  : { $$ = vector<Statement*>(); }
  | array0_Statement__1  class_Statement { $$ = $1; $$.emplace_back($2); }
  ;

class_Declaration
  :  TOK_IDENTIFIER TOK_IDENTIFIER {
      $$ = new Declaration(@$, $1, $2); }
  ;

class_Statement
  :  TOK_IDENTIFIER TOK_LPAREN  array0_identifier__1 TOK_RPAREN {
      $$ = new Statement(@$, $1, $3); }
  ;

array0_identifier__1
  : { $$ = vector<identifier>(); }
  | TOK_IDENTIFIER { $$ = vector<identifier>(); $$.emplace_back($1); }
  | array0_identifier__1 TOK_COMMA TOK_IDENTIFIER { $$ = $1; $$.emplace_back($3); }
  ;


/* END PRODUCTIONS */

%%

#include <sstream>

namespace t3 {

using std::istream;
using std::ostream;
using std::stringstream;

AstNode* parse(istream& input, ostream& errors);

/* BEGIN METHOD DEFINITIONS */
ProcedureDecl::ProcedureDecl(ParseState parseState, identifier name, vector<Declaration*> arguments, vector<Declaration*> locals, vector<Statement*> statements)
    : AstNode(parseState)
    , name(name)
    , arguments(arguments)
    , locals(locals)
    , statements(statements) {
}

ProcedureDecl* ProcedureDecl::parse(istream& input, ostream& errors) {
  return dynamic_cast<ProcedureDecl*>(t3::parse(input, errors));
}
void ProcedureDecl::printMembers(ostream& out) {
  out << " name:";
  classpPrint(out, name);
  out << " arguments:";
  classpPrint(out, arguments);
  out << " locals:";
  classpPrint(out, locals);
  out << " statements:";
  classpPrint(out, statements);
}

void ProcedureDecl::format(ostream& out, int precedence) {
  out << "procedure";
  out << " ";
  classpFormat(out, 0, name);
  out << " ";
  out << "(";
  out << " ";
  for (size_t i = 0; i < arguments.size(); i++) {
    if (i > 0) {
      out << ",";
    }
    classpFormat(out, 0, arguments[i]);
  }
  out << " ";
  out << ")";
  out << " ";
  out << "declare";
  out << " ";
  for (size_t i = 0; i < locals.size(); i++) {
    if (i > 0) {
      out << ";";
    }
    classpFormat(out, 0, locals[i]);
  }
  out << " ";
  out << "begin";
  out << " ";
  for (size_t i = 0; i < statements.size(); i++) {
    classpFormat(out, 0, statements[i]);
  }
  out << " ";
  out << "end";
}
Declaration::Declaration(ParseState parseState, identifier type_name, identifier variable_name)
    : AstNode(parseState)
    , type_name(type_name)
    , variable_name(variable_name) {
}

void Declaration::printMembers(ostream& out) {
  out << " type_name:";
  classpPrint(out, type_name);
  out << " variable_name:";
  classpPrint(out, variable_name);
}

void Declaration::format(ostream& out, int precedence) {
  classpFormat(out, 0, type_name);
  out << " ";
  classpFormat(out, 0, variable_name);
}
Statement::Statement(ParseState parseState, identifier procedure_name, vector<identifier> arguments)
    : AstNode(parseState)
    , procedure_name(procedure_name)
    , arguments(arguments) {
}

void Statement::printMembers(ostream& out) {
  out << " procedure_name:";
  classpPrint(out, procedure_name);
  out << " arguments:";
  classpPrint(out, arguments);
}

void Statement::format(ostream& out, int precedence) {
  classpFormat(out, 0, procedure_name);
  out << " ";
  out << "(";
  out << " ";
  for (size_t i = 0; i < arguments.size(); i++) {
    if (i > 0) {
      out << ",";
    }
    classpFormat(out, 0, arguments[i]);
  }
  out << " ";
  out << ")";
}
Unparsed::Unparsed(ParseState parseState, int foo)
    : AstNode(parseState)
    , foo(foo) {
}

void Unparsed::printMembers(ostream& out) {
  out << " foo:";
  classpPrint(out, foo);
}

void Unparsed::format(ostream& out, int precedence) {
  classpFormat(out, 0, foo);
}
/* END METHOD DEFINITIONS */

#ifdef PARSER_TEST

void ParseAndPrint(istream& input, ostream& out) {
/* BEGIN PARSE PARSEABLE */
  AstNode* result = ProcedureDecl::parse(input, out);
/* END PARSE PARSEABLE */
  if (result) {
    out << "Succeeded:\n";
    result->format(out);
    out << "\n";
  } else {
    out << "Parse failed.\n";
  }
}

// These constants never a legal output from AstParser::print()
const char kSucceed[] = "!succeed!";
const char kFail[] = "!fail!";
const char kPrint[] = "!print!";

// Runs the parser in the given sample and compares the result to
// expected_result which can be nullptr to indicate that the parse should fail.
// With no second argument, this just prints the result. Returns 1 if the parse
// failed and was not expected to, or if the result is different from the
// expected result, otherwise returns 0.
template<class T>
int ParseSample(const char* sample, const char* expected_result = kPrint) {
  stringstream input(sample);
  stringstream errors;
  std::cout << "parsing sample '" << sample << "':\n";
  AstNode* result = T::parse(input, errors);
  if (result) {
    stringstream actual_result;
    result->print(actual_result);
    if (expected_result == kFail) {
      std::cout << "ERROR[succeeds but expected fail:\n"
          << "  result->" << actual_result.str() << "]\n";
      return 1;
    }

    // Now format the output and try parsing it again.
    stringstream formatted;
    result->format(formatted);
    std::cout << "parsing formatted result '" << formatted.str() << "'\n";
    AstNode* result2 = T::parse(formatted, errors);
    if (!result2) {
      std::cout << "\nERROR[parsing the formatted string failed." 
          << "\n  original parse->" << actual_result.str() << "]\n";
      return 1;
    }
    stringstream actual_result2;
    result2->print(actual_result2);
    if (actual_result.str() != actual_result2.str()) {
      std::cout << "ERROR[parsed formatted string does not match:"
          << "\n  original->" << actual_result.str()
          << "\n  parsed->  " << actual_result2.str() << "\n  ]\n";
      return 1;
    }

    std::cout<< "SUCCEEDS";
    if (expected_result == kPrint) {
      std::cout << ": ";
      result->print(std::cout);
    } else if (expected_result != kSucceed) {
      if (actual_result.str() != expected_result) {
        std::cout << "\nERROR[expected and actual result do not match:"
            << "\n  expected-> " << expected_result
            << "\n  actual->   " << actual_result.str() << "\n  ]\n";
        return 1;
      }
    }
    std::cout << "\n";
    return 0;
  } else {
    std::cout << "FAILS";
    if (expected_result == kFail) {
      std::cout << " [as expected]\n";
      return 0;
    } else if (expected_result == kPrint || expected_result == kSucceed) {
      std::cout << ": ERROR[expected success]";
    } else {
      std::cout << ": ERROR[expected " << expected_result << "]";
    }
    std::cout << "\n  " << errors.str();
    return 1;
  }
}

int ParseSamples() {
  int num_errors = 0;
/* BEGIN SAMPLES */
  num_errors += ParseSample<ProcedureDecl>(R"#A#(procedure foo (int a) declare int b begin foo(a) end)#A#", R"#A#((ProcedureDecl name:foo arguments:[(Declaration type_name:int variable_name:a)] locals:[(Declaration type_name:int variable_name:b)] statements:[(Statement procedure_name:foo arguments:[a])]))#A#");
/* END SAMPLES */
  std::cout << "Errors: " << num_errors << "\n";
  return num_errors;
}

#endif  // PARSER_TEST


}  // namespace t3

#ifdef PARSER_TEST

#include <fstream>
#include <iostream>

using std::istream;
using std::ifstream;

const char usage[] = "usage: t3 [input-file | --samples]\n";

int main(int argc, char** argv) {
  if (argc > 2) {
    std::cerr << usage;
    exit(1);
  }
  if (argc == 2 && std::string(argv[1]) == "--samples") {
    if (t3::ParseSamples() > 0) exit(1);
  } else {
    ifstream file;
    if (argc == 2) {
      file.open(argv[1]);
      if (file.fail()) {
        std::cerr << "failed to open '" << argv[1] << "' for reading\n";
        exit(1);
      }
    }
    istream& input = argc == 2 ? file : std::cin;
    t3::ParseAndPrint(input, std::cout);
  }
  return 0;
}

#endif  // PARSER_TEST

/* END PARSER */
