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

namespace calculator1 {
class ParseDriver;
class AstNode;
/* BEGIN FORWARD DECLARATIONS */
class Binop;
class Diff;
class Div;
class Expression;
class IntegerLiteral;
class Negate;
class Prod;
class Sum;
/* END FORWARD DECLARATIONS */
}

}

%require "3.0.2"
%defines
%define api.value.type variant
%define "parser_class_name" {YYParser}
%error-verbose
%lex-param {calculator1::ParseDriver* parser}
%locations
%define api.namespace {calculator1}
%parse-param {calculator1::ParseDriver* parser}
%parse-param {calculator1::AstNode** result}
%define api.token.constructor
%skeleton "lalr1.cc"

%initial-action {
}

%token TOK_EOF 0              "end of file"

/* BEGIN GENERATED TOKEN DECLARATIONS */
%token TOK_LPAREN "`(`"
%token TOK_RPAREN "`)`"
%token TOK_STAR "`*`"
%token TOK_PLUS "`+`"
%token TOK_MINUS "`-`"
%token TOK_SLASH "`/`"
/* END GENERATED TOKEN DECLARATIONS */

%token TOK_BOOL   "boolean literal"
%token TOK_IDENTIFIER         "identifier"
%token TOK_INT64    "integer literal"
%token TOK_STRING_LITERAL     "string literal"


/* BEGIN NONTERMINAL TYPES */
%type <Expression*> class_Expression
%type <Expression*> class_Expression_p2
%type <Expression*> class_Expression_p3
%type <Expression*> class_Expression_p4
/* END NONTERMINAL TYPES */

%type <bool> TOK_BOOL
%type <int64_t> TOK_INT64
%type <string>  TOK_IDENTIFIER
%type <string>  TOK_STRING_LITERAL

%code {

#include "calculator1.yacc.hh"
#include "calculator1.h"

namespace calculator1 {

YYParser::symbol_type yylex(ParseDriver* parser);

}  // namespace calculator1

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
  :  class_Expression TOK_MINUS class_Expression_p2 {
      $$ = new Diff(@$, $1, $3); }  // Diff
  |  class_Expression TOK_PLUS class_Expression_p2 {
      $$ = new Sum(@$, $1, $3); }  // Sum
  | class_Expression_p2 { $$ = $1; }
  ;
class_Expression_p2
  :  class_Expression_p2 TOK_SLASH class_Expression_p3 {
      $$ = new Div(@$, $1, $3); }  // Div
  |  class_Expression_p2 TOK_STAR class_Expression_p3 {
      $$ = new Prod(@$, $1, $3); }  // Prod
  |  class_Expression_p2 class_Expression_p3 {
      $$ = new Prod(@$, $1, $2); }  // Prod
  | class_Expression_p3 { $$ = $1; }
  ;
class_Expression_p3
  :  TOK_MINUS class_Expression_p4 {
      $$ = new Negate(@$, $2); }  // Negate
  | class_Expression_p4 { $$ = $1; }
  ;
class_Expression_p4
  :  TOK_LPAREN class_Expression TOK_RPAREN { $$ = $2; }
  | TOK_INT64 {
      $$ = new IntegerLiteral(@$, $1); }  // IntegerLiteral
  ;








/* END PRODUCTIONS */

%%

#include <sstream>

namespace calculator1 {

using std::istream;
using std::ostream;
using std::stringstream;

AstNode* parse(istream& input, ostream& errors);

/* BEGIN METHOD DEFINITIONS */
Expression::Expression(ParseState parseState)
    : AstNode(parseState) {
}

Expression* Expression::parse(istream& input, ostream& errors) {
  return dynamic_cast<Expression*>(calculator1::parse(input, errors));
}
void Expression::printMembers(ostream& out) {
}

void Expression::bracketFormat(ostream& out, AstNode* self) {
  out << "(";
  out << " ";
  classpFormat(out, 0, self);
  out << " ";
  out << ")";
}
Negate::Negate(ParseState parseState, Expression* arg)
    : Expression(parseState)
    , arg(arg) {
}

void Negate::printMembers(ostream& out) {
  Expression::printMembers(out);

  out << " arg:";
  classpPrint(out, arg);
}

void Negate::format(ostream& out, int precedence) {
  if (precedence <= 3) {
    out << "-";
    out << " ";
    classpFormat(out, 4, arg);
  } else {
    bracketFormat(out, this);
  }
}
Binop::Binop(ParseState parseState, Expression* arg1, Expression* arg2)
    : Expression(parseState)
    , arg1(arg1)
    , arg2(arg2) {
}

void Binop::printMembers(ostream& out) {
  Expression::printMembers(out);

  out << " arg1:";
  classpPrint(out, arg1);
  out << " arg2:";
  classpPrint(out, arg2);
}
Sum::Sum(ParseState parseState, Expression* arg1, Expression* arg2)
    : Binop(parseState, arg1, arg2) {
}

void Sum::printMembers(ostream& out) {
  Binop::printMembers(out);

}

void Sum::format(ostream& out, int precedence) {
  if (precedence <= 1) {
    classpFormat(out, 1, arg1);
    out << " ";
    out << "+";
    out << " ";
    classpFormat(out, 2, arg2);
  } else {
    bracketFormat(out, this);
  }
}
Diff::Diff(ParseState parseState, Expression* arg1, Expression* arg2)
    : Binop(parseState, arg1, arg2) {
}

void Diff::printMembers(ostream& out) {
  Binop::printMembers(out);

}

void Diff::format(ostream& out, int precedence) {
  if (precedence <= 1) {
    classpFormat(out, 1, arg1);
    out << " ";
    out << "-";
    out << " ";
    classpFormat(out, 2, arg2);
  } else {
    bracketFormat(out, this);
  }
}
Prod::Prod(ParseState parseState, Expression* arg1, Expression* arg2)
    : Binop(parseState, arg1, arg2) {
}

void Prod::printMembers(ostream& out) {
  Binop::printMembers(out);

}

void Prod::format(ostream& out, int precedence) {
  if (precedence <= 2) {
    classpFormat(out, 2, arg1);
    out << " ";
    out << "*";
    out << " ";
    classpFormat(out, 3, arg2);
  } else {
    bracketFormat(out, this);
  }
}
Div::Div(ParseState parseState, Expression* arg1, Expression* arg2)
    : Binop(parseState, arg1, arg2) {
}

void Div::printMembers(ostream& out) {
  Binop::printMembers(out);

}

void Div::format(ostream& out, int precedence) {
  if (precedence <= 2) {
    classpFormat(out, 2, arg1);
    out << " ";
    out << "/";
    out << " ";
    classpFormat(out, 3, arg2);
  } else {
    bracketFormat(out, this);
  }
}
IntegerLiteral::IntegerLiteral(ParseState parseState, int n)
    : Expression(parseState)
    , n(n) {
}

void IntegerLiteral::printMembers(ostream& out) {
  Expression::printMembers(out);

  out << " n:";
  classpPrint(out, n);
}

void IntegerLiteral::format(ostream& out, int precedence) {
  classpFormat(out, 0, n);
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
  num_errors += ParseSample<Expression>(R"#A#(4-1+-2*5)#A#", kPrint);
  num_errors += ParseSample<Expression>(R"#A#(-(5*6))#A#", kPrint);
  num_errors += ParseSample<Expression>(R"#A#(4-1+-2 5)#A#", kPrint);
  num_errors += ParseSample<Expression>(R"#A#(-(5 6))#A#", kPrint);
/* END SAMPLES */
  std::cout << "Errors: " << num_errors << "\n";
  return num_errors;
}

#endif  // PARSER_TEST


}  // namespace calculator1

#ifdef PARSER_TEST

#include <fstream>
#include <iostream>

using std::istream;
using std::ifstream;

const char usage[] = "usage: calculator1 [input-file | --samples]\n";

int main(int argc, char** argv) {
  if (argc > 2) {
    std::cerr << usage;
    exit(1);
  }
  if (argc == 2 && std::string(argv[1]) == "--samples") {
    if (calculator1::ParseSamples() > 0) exit(1);
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
    calculator1::ParseAndPrint(input, std::cout);
  }
  return 0;
}

#endif  // PARSER_TEST

/* END PARSER */
