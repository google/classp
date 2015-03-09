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

namespace t7 {
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
%lex-param {t7::ParseDriver* parser}
%locations
%define api.namespace {t7}
%parse-param {t7::ParseDriver* parser}
%parse-param {t7::AstNode** result}
%define api.token.constructor
%skeleton "lalr1.cc"

%initial-action {
}

%token TOK_EOF 0              "end of file"

/* BEGIN GENERATED TOKEN DECLARATIONS */
%token TOK_STAR "`*`"
%token TOK_PLUS "`+`"
%token TOK_COMMA "`,`"
%token TOK_LT "`<`"
/* END GENERATED TOKEN DECLARATIONS */

%token TOK_BOOL   "boolean literal"
%token TOK_IDENTIFIER         "identifier"
%token TOK_INT64    "integer literal"
%token TOK_STRING_LITERAL     "string literal"


/* BEGIN NONTERMINAL TYPES */
%type <A*> class_A
%type <B*> class_B
%type <C*> class_C
%type <AttributeMap> alt_A__1
%type <AttributeMap> alt_B__1
%type <AttributeMap> alt_C__1
%type <AttributeMap> altiterPLUS_B
%type <AttributeMap> altiterPLUS_C
%type <AttributeMap> altiterSTAR_A
/* END NONTERMINAL TYPES */

%type <bool> TOK_BOOL
%type <int64_t> TOK_INT64
%type <string>  TOK_IDENTIFIER
%type <string>  TOK_STRING_LITERAL

%code {

#include "t7.yacc.hh"
#include "t7.h"

namespace t7 {

YYParser::symbol_type yylex(ParseDriver* parser);

}  // namespace t7

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
  :  altiterSTAR_A {
      AttributeMap keywords = $1;
      $$ = new A(@$, keywords); }
  ;

alt_A__1
  : TOK_INT64 {
    $$ = AttributeMap();
    $$.Add("x", $1); }
  | TOK_INT64 {
    $$ = AttributeMap();
    $$.Add("y", $1); }
  ;

altiterSTAR_A
  : { $$ = AttributeMap(); }
  | altiterSTAR_A  alt_A__1 { $$ = $1; $$.Merge($2); }
  ;

class_B
  :  altiterPLUS_B {
      AttributeMap keywords = $1;
      $$ = new B(@$, keywords); }
  ;

alt_B__1
  : TOK_INT64 {
    $$ = AttributeMap();
    $$.Push("a1", $1); }
  | TOK_INT64 {
    $$ = AttributeMap();
    $$.Add("x", $1); }
  ;

altiterPLUS_B
  :  alt_B__1 { $$ = $1; }
  | altiterPLUS_B  alt_B__1 { $$ = $1; $$.Merge($2); }
  ;

class_C
  :  altiterPLUS_C {
      AttributeMap keywords = $1;
      $$ = new C(@$, {}, keywords); }
  ;

alt_C__1
  :  class_A TOK_PLUS class_A {
    $$ = AttributeMap();
    $$.Push("a1", $1);
    $$.Push("a2", $3); }
  |  TOK_INT64 TOK_STAR TOK_INT64 {
    $$ = AttributeMap();
    $$.Add("x", $1);
    $$.Add("y", $3); }
  |  class_A TOK_LT TOK_INT64 {
    $$ = AttributeMap();
    $$.Push("a1", $1);
    $$.Add("z", $3); }
  ;

altiterPLUS_C
  :  alt_C__1 { $$ = $1; }
  | altiterPLUS_C TOK_COMMA alt_C__1 { $$ = $1; $$.Merge($3); }
  ;

/* END PRODUCTIONS */

%%

#include <sstream>

namespace t7 {

using std::istream;
using std::ostream;
using std::stringstream;

AstNode* parse(istream& input, ostream& errors);

/* BEGIN METHOD DEFINITIONS */
A::A(ParseState parseState, AttributeMap& keyword_args)
    : AstNode(parseState) {
  has_x = keyword_args.Take("x", x);
  has_y = keyword_args.Take("y", y);
}

void A::printMembers(ostream& out) {
  if (has_x) {
    out << " x:";
    classpPrint(out, x);
  } else {
    out << "x[not defined]";
    }
  if (has_y) {
    out << " y:";
    classpPrint(out, y);
  } else {
    out << "y[not defined]";
    }
}

void A::format(ostream& out, int precedence) {
  if (has_x) {
    classpFormat(out, 0, x);
  }
  if (has_y) {
    classpFormat(out, 0, y);
  }
}
B::B(ParseState parseState, AttributeMap& keyword_args)
    : AstNode(parseState) {
  keyword_args.Take("a1", a1);
  has_x = keyword_args.Take("x", x);
}

void B::printMembers(ostream& out) {
  out << " a1:";
  classpPrint(out, a1);
  if (has_x) {
    out << " x:";
    classpPrint(out, x);
  } else {
    out << "x[not defined]";
    }
}

void B::format(ostream& out, int precedence) {
  if ((!a1.empty())) {
    for (size_t i = 0; i < a1.size(); i++) {
      classpFormat(out, 0, a1[i]);
    }
  }
  if (has_x) {
    classpFormat(out, 0, x);
  }
}
C::C(ParseState parseState, vector<A*> a3, AttributeMap& keyword_args)
    : AstNode(parseState)
    , a3(a3) {
  keyword_args.Take("a1", a1);
  keyword_args.Take("a2", a2);
  has_x = keyword_args.Take("x", x);
  has_y = keyword_args.Take("y", y);
  has_z = keyword_args.Take("z", z);
}

void C::printMembers(ostream& out) {
  out << " a1:";
  classpPrint(out, a1);
  out << " a2:";
  classpPrint(out, a2);
  out << " a3:";
  classpPrint(out, a3);
  if (has_x) {
    out << " x:";
    classpPrint(out, x);
  } else {
    out << "x[not defined]";
    }
  if (has_y) {
    out << " y:";
    classpPrint(out, y);
  } else {
    out << "y[not defined]";
    }
  if (has_z) {
    out << " z:";
    classpPrint(out, z);
  } else {
    out << "z[not defined]";
    }
}

void C::format(ostream& out, int precedence) {
  bool found1_ = false;
  if ((!a1.empty())) {
    for (size_t i = 0; i < a1.size(); i++) {
      if (i > 0) {
        out << ",";
      }
      classpFormat(out, 0, a1[i]);
      out << " ";
      out << "+";
      out << " ";
      classpFormat(out, 0, a2[i]);
    }
    found1_ = true;
  }
  if (has_x) {
    if (found1_) {
      out << ",";
    }
    classpFormat(out, 0, x);
    out << " ";
    out << "*";
    out << " ";
    classpFormat(out, 0, y);
    found1_ = true;
  }
  if ((!a1.empty())) {
    if (found1_) {
      out << ",";
    }
    for (size_t i = 0; i < a1.size(); i++) {
      if (i > 0) {
        out << ",";
      }
      classpFormat(out, 0, a1[i]);
      out << " ";
      out << "<";
      out << " ";
      classpFormat(out, 0, z);
    }
    found1_ = true;
  }
}
/* END METHOD DEFINITIONS */

#ifdef PARSER_TEST

void ParseAndPrint(istream& input, ostream& out) {
/* BEGIN PARSE PARSEABLE */
  AstNode* result = nullptr;
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
/* END SAMPLES */
  std::cout << "Errors: " << num_errors << "\n";
  return num_errors;
}

#endif  // PARSER_TEST


}  // namespace t7

#ifdef PARSER_TEST

#include <fstream>
#include <iostream>

using std::istream;
using std::ifstream;

const char usage[] = "usage: t7 [input-file | --samples]\n";

int main(int argc, char** argv) {
  if (argc > 2) {
    std::cerr << usage;
    exit(1);
  }
  if (argc == 2 && std::string(argv[1]) == "--samples") {
    if (t7::ParseSamples() > 0) exit(1);
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
    t7::ParseAndPrint(input, std::cout);
  }
  return 0;
}

#endif  // PARSER_TEST

/* END PARSER */
