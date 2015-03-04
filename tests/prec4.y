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

namespace prec4 {
class ParseDriver;
class AstNode;
/* BEGIN FORWARD DECLARATIONS */
class Expression;
class IntegerLiteral;
class Product;
class Sum;
/* END FORWARD DECLARATIONS */
}

}

%require "3.0.2"
%defines
%define api.value.type variant
%define "parser_class_name" {YYParser}
%error-verbose
%lex-param {prec4::ParseDriver* parser}
%locations
%define api.namespace {prec4}
%parse-param {prec4::ParseDriver* parser}
%parse-param {prec4::AstNode** result}
%define api.token.constructor
%skeleton "lalr1.cc"

%initial-action {
}

%token TOK_EOF 0              "end of file"

/* BEGIN GENERATED TOKEN DECLARATIONS */
%token TOK_STAR "`*`"
%token TOK_PLUS "`+`"
%token WORD_a "`a`"
/* END GENERATED TOKEN DECLARATIONS */

%token TOK_BOOL   "boolean literal"
%token TOK_IDENTIFIER         "identifier"
%token TOK_INT64    "integer literal"
%token TOK_STRING_LITERAL     "string literal"


/* BEGIN NONTERMINAL TYPES */
%type <Expression*> class_Expression
%type <Expression*> class_Expression_p2
%type <Expression*> class_Expression_p3
/* END NONTERMINAL TYPES */

%type <bool> TOK_BOOL
%type <int64_t> TOK_INT64
%type <string>  TOK_IDENTIFIER
%type <string>  TOK_STRING_LITERAL

%code {

#include "prec4.yacc.hh"
#include "prec4.h"

namespace prec4 {

YYParser::symbol_type yylex(ParseDriver* parser);

}  // namespace prec4

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
  :  class_Expression TOK_PLUS class_Expression_p2 {
      $$ = new Sum(@$, $1, $3); }  // Sum
  | class_Expression_p2 { $$ = $1; }
  ;
class_Expression_p2
  :  class_Expression_p2 TOK_STAR class_Expression_p3 {
      $$ = new Product(@$, $1, $3); }  // Product
  | class_Expression_p3 { $$ = $1; }
  ;
class_Expression_p3
  : WORD_a {
      $$ = new IntegerLiteral(@$); }  // IntegerLiteral
  ;




/* END PRODUCTIONS */

%%

#include <sstream>

namespace prec4 {

using std::istream;
using std::ostream;
using std::stringstream;

AstNode* parse(istream& input, ostream& errors);

/* BEGIN METHOD DEFINITIONS */
Expression::Expression(ParseState parseState)
    : AstNode(parseState) {
}

Expression* Expression::parse(istream& input, ostream& errors) {
  return dynamic_cast<Expression*>(prec4::parse(input, errors));
}
void Expression::printMembers(ostream& out) {
}

void Expression::bracketFormat(ostream& out, AstNode* self) {
    self->format(out, 0);
}
Sum::Sum(ParseState parseState, Expression* val1, Expression* val2)
    : Expression(parseState)
    , val1(val1)
    , val2(val2) {
}

void Sum::printMembers(ostream& out) {
  Expression::printMembers(out);

  out << " val1:";
  classpPrint(out, val1);
  out << " val2:";
  classpPrint(out, val2);
}

void Sum::format(ostream& out, int precedence) {
  if (precedence <= 1) {
    classpFormat(out, 1, val1);
    out << " ";
    out << "+";
    out << " ";
    classpFormat(out, 2, val2);
  } else {
    bracketFormat(out, this);
  }
}
Product::Product(ParseState parseState, Expression* val1, Expression* val2)
    : Expression(parseState)
    , val1(val1)
    , val2(val2) {
}

void Product::printMembers(ostream& out) {
  Expression::printMembers(out);

  out << " val1:";
  classpPrint(out, val1);
  out << " val2:";
  classpPrint(out, val2);
}

void Product::format(ostream& out, int precedence) {
  if (precedence <= 2) {
    classpFormat(out, 2, val1);
    out << " ";
    out << "*";
    out << " ";
    classpFormat(out, 3, val2);
  } else {
    bracketFormat(out, this);
  }
}
IntegerLiteral::IntegerLiteral(ParseState parseState)
    : Expression(parseState) {
}

void IntegerLiteral::printMembers(ostream& out) {
  Expression::printMembers(out);

}

void IntegerLiteral::format(ostream& out, int precedence) {
  if (precedence <= 3) {
    out << "a";
  } else {
    bracketFormat(out, this);
  }
}
/* END METHOD DEFINITIONS */

#ifdef PARSER_TEST

void ParseAndPrint(istream& input, ostream& out) {
/* BEGIN PARSE PARSEABLE */
  AstNode* result = Expression::parse(input, out);
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
  num_errors += ParseSample<Expression>(R"#A#(a)#A#", R"#A#((IntegerLiteral))#A#");
  num_errors += ParseSample<Expression>(R"#A#(a+a*a)#A#", R"#A#((Sum val1:(IntegerLiteral) val2:(Product val1:(IntegerLiteral) val2:(IntegerLiteral))))#A#");
/* END SAMPLES */
  std::cout << "Errors: " << num_errors << "\n";
  return num_errors;
}

#endif  // PARSER_TEST


}  // namespace prec4

#ifdef PARSER_TEST

#include <fstream>
#include <iostream>
#include <memory>

using std::istream;
using std::ifstream;
using std::unique_ptr;

const char usage[] = "usage: prec4 [input-file | --samples]\n";

int main(int argc, char** argv) {
  if (argc > 2) {
    std::cerr << usage;
    exit(1);
  }
  if (argc == 2 && std::string(argv[1]) == "--samples") {
    if (prec4::ParseSamples() > 0) exit(1);
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
    prec4::ParseAndPrint(input, std::cout);
  }
  return 0;
}

#endif  // PARSER_TEST

/* END PARSER */
