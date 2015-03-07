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

namespace t4 {
class ParseDriver;
class AstNode;
/* BEGIN FORWARD DECLARATIONS */
class A;
class B;
class C;
/* END FORWARD DECLARATIONS */
}

}

%require "3.0.2"
%defines
%define api.value.type variant
%define "parser_class_name" {YYParser}
%error-verbose
%lex-param {t4::ParseDriver* parser}
%locations
%define api.namespace {t4}
%parse-param {t4::ParseDriver* parser}
%parse-param {t4::AstNode** result}
%define api.token.constructor
%skeleton "lalr1.cc"

%initial-action {
}

%token TOK_EOF 0              "end of file"

/* BEGIN GENERATED TOKEN DECLARATIONS */
%token WORD_x "`x`"
%token WORD_y "`y`"
%token WORD_z "`z`"
/* END GENERATED TOKEN DECLARATIONS */

%token TOK_BOOL   "boolean literal"
%token TOK_IDENTIFIER         "identifier"
%token TOK_INT64    "integer literal"
%token TOK_STRING_LITERAL     "string literal"


/* BEGIN NONTERMINAL TYPES */
%type <A*> class_A
/* END NONTERMINAL TYPES */

%type <bool> TOK_BOOL
%type <int64_t> TOK_INT64
%type <string>  TOK_IDENTIFIER
%type <string>  TOK_STRING_LITERAL

%code {

#include "t4.yacc.hh"
#include "t4.h"

namespace t4 {

YYParser::symbol_type yylex(ParseDriver* parser);

}  // namespace t4

}


%start start

%%

start
/* BEGIN PARSEABLE */
  : class_A TOK_EOF { *result = $1; }
/* END PARSEABLE */
  ;

/* BEGIN PRODUCTIONS */
class_A
  :  WORD_x TOK_INT64 WORD_y TOK_INT64 {
      AttributeMap keywords = AttributeMap();
      keywords.Add("x", $2);
      keywords.Add("y", $4);
      $$ = new B(@$, keywords); }  // B
  |  WORD_x TOK_INT64 WORD_z TOK_INT64 {
      AttributeMap keywords = AttributeMap();
      keywords.Add("x", $2);
      keywords.Add("z", $4);
      $$ = new C(@$, keywords); }  // C
  ;



/* END PRODUCTIONS */

%%

#include <sstream>

namespace t4 {

using std::istream;
using std::ostream;
using std::stringstream;

AstNode* parse(istream& input, ostream& errors);

/* BEGIN METHOD DEFINITIONS */
A::A(ParseState parseState, AttributeMap& keyword_args)
    : AstNode(parseState) {
  has_x = keyword_args.Take("x", x);
}

A* A::parse(istream& input, ostream& errors) {
  return dynamic_cast<A*>(t4::parse(input, errors));
}
void A::printMembers(ostream& out) {
  if (has_x) {
    out << " x:";
    classpPrint(out, x);
  } else {
    out << "x[not defined]";
    }
}
B::B(ParseState parseState, AttributeMap& keyword_args)
    : A(parseState, keyword_args) {
  has_y = keyword_args.Take("y", y);
}

void B::printMembers(ostream& out) {
  A::printMembers(out);

  if (has_y) {
    out << " y:";
    classpPrint(out, y);
  } else {
    out << "y[not defined]";
    }
}

void B::format(ostream& out, int precedence) {
  out << "x";
  out << " ";
  classpFormat(out, 0, x);
  out << " ";
  out << "y";
  out << " ";
  classpFormat(out, 0, y);
}
C::C(ParseState parseState, AttributeMap& keyword_args)
    : A(parseState, keyword_args) {
  has_z = keyword_args.Take("z", z);
}

void C::printMembers(ostream& out) {
  A::printMembers(out);

  if (has_z) {
    out << " z:";
    classpPrint(out, z);
  } else {
    out << "z[not defined]";
    }
}

void C::format(ostream& out, int precedence) {
  out << "x";
  out << " ";
  classpFormat(out, 0, x);
  out << " ";
  out << "z";
  out << " ";
  classpFormat(out, 0, z);
}
/* END METHOD DEFINITIONS */

#ifdef PARSER_TEST

void ParseAndPrint(istream& input, ostream& out) {
/* BEGIN PARSE PARSEABLE */
  AstNode* result = A::parse(input, out);
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
  num_errors += ParseSample<A>(R"#A#(x 1 y 2)#A#", R"#A#((B x:1 y:2))#A#");
/* END SAMPLES */
  std::cout << "Errors: " << num_errors << "\n";
  return num_errors;
}

#endif  // PARSER_TEST


}  // namespace t4

#ifdef PARSER_TEST

#include <fstream>
#include <iostream>
#include <memory>

using std::istream;
using std::ifstream;
using std::unique_ptr;

const char usage[] = "usage: t4 [input-file | --samples]\n";

int main(int argc, char** argv) {
  if (argc > 2) {
    std::cerr << usage;
    exit(1);
  }
  if (argc == 2 && std::string(argv[1]) == "--samples") {
    if (t4::ParseSamples() > 0) exit(1);
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
    t4::ParseAndPrint(input, std::cout);
  }
  return 0;
}

#endif  // PARSER_TEST

/* END PARSER */
