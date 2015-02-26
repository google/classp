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

namespace t9 {
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
%lex-param {t9::ParseDriver* parser}
%locations
%define api.namespace {t9}
%parse-param {t9::ParseDriver* parser}
%parse-param {t9::AstNode** result}
%define api.token.constructor
%skeleton "lalr1.cc"

%initial-action {
}

%token TOK_EOF 0              "end of file"

/* BEGIN GENERATED TOKEN DECLARATIONS */
/* END GENERATED TOKEN DECLARATIONS */

%token TOK_BOOL   "boolean literal"
%token TOK_IDENTIFIER         "identifier"
%token TOK_INT64    "integer literal"
%token TOK_STRING_LITERAL     "string literal"


/* BEGIN NONTERMINAL TYPES */
%type <AttributeMap> alt_C__1
%type <AttributeMap> alt_C__2
%type <AttributeMap> alt_C__3
%type <AttributeMap> altiterSTAR_C
%type <A*> class_A
%type <B*> class_B
%type <C*> class_C
/* END NONTERMINAL TYPES */

%type <bool> TOK_BOOL
%type <int64_t> TOK_INT64
%type <string>  TOK_IDENTIFIER
%type <string>  TOK_STRING_LITERAL

%code {

#include "t9.yacc.hh"
#include "t9.h"

namespace t9 {

YYParser::symbol_type yylex(ParseDriver* parser);

}  // namespace t9

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
  : class_B { $$ = $1; }
  ;

class_B
  : class_C { $$ = $1; }
  ;

class_C
  :  TOK_INT64 TOK_INT64 TOK_INT64 TOK_INT64 TOK_INT64 TOK_INT64 TOK_INT64 TOK_INT64 TOK_INT64 TOK_INT64 TOK_INT64 TOK_INT64  alt_C__1  alt_C__2  altiterSTAR_C {
      AttributeMap keywords = $13;
      keywords.Merge($14);
      keywords.Merge($15);
      keywords.Add("a2", $2);
      keywords.Add("a3", $3);
      keywords.Add("b2", $6);
      keywords.Add("b3", $7);
      keywords.Add("c2", $10);
      keywords.Add("c3", $11);
      $$ = new C(@$, $1, $4, $5, $8, $9, $12, keywords); }
  ;

alt_C__1
  : TOK_INT64 {
    $$ = AttributeMap();
    $$.Add("c5", $1); }
  |  {
    $$ = AttributeMap(); }
  ;

alt_C__2
  : TOK_INT64 {
    $$ = AttributeMap();
    $$.Add("c6", $1); }
  | TOK_INT64 {
    $$ = AttributeMap();
    $$.Add("c7", $1); }
  ;

alt_C__3
  : TOK_INT64 {
    $$ = AttributeMap();
    $$.Add("c8", $1); }
  | TOK_INT64 {
    $$ = AttributeMap();
    $$.Push("c9", $1); }
  ;

altiterSTAR_C
  : { $$ = AttributeMap(); }
  | altiterSTAR_C  alt_C__3 { $$ = $1; $$->Merge($2); }
  ;

/* END PRODUCTIONS */

%%

#include <sstream>

namespace t9 {

using std::istream;
using std::ostream;
using std::stringstream;

AstNode* parse(istream& input, ostream& errors);

/* BEGIN METHOD DEFINITIONS */
A::A(ParseState parseState, int a1, vector<int> a4, AttributeMap& keyword_args)
    : AstNode(parseState)
    , a1(a1)
    , a4(a4) {
  has_a2 = keyword_args.Take("a2", a2);
  if (!(keyword_args.Take("a3", a3))) {
    a3 = 0;
  }
}

A* A::parse(istream& input, ostream& errors) {
  return dynamic_cast<A*>(t9::parse(input, errors));
}
void A::printMembers(ostream& out) {
  out << " a1:";
  classpPrint(out, a1);
  if (has_a2) {
    out << " a2:";
    classpPrint(out, a2);
  } else {
    out << "a2[not defined]";
    }
  out << " a3:";
  classpPrint(out, a3);
  out << " a4:";
  classpPrint(out, a4);
}
B::B(ParseState parseState, int a1, vector<int> a4, int b1, vector<int> b4, AttributeMap& keyword_args)
    : A(parseState, a1, a4, keyword_args)
    , b1(b1)
    , b4(b4) {
  has_b2 = keyword_args.Take("b2", b2);
  if (!(keyword_args.Take("b3", b3))) {
    b3 = 0;
  }
}

void B::printMembers(ostream& out) {
  A::printMembers(out);

  out << " b1:";
  classpPrint(out, b1);
  if (has_b2) {
    out << " b2:";
    classpPrint(out, b2);
  } else {
    out << "b2[not defined]";
    }
  out << " b3:";
  classpPrint(out, b3);
  out << " b4:";
  classpPrint(out, b4);
}
C::C(ParseState parseState, int a1, vector<int> a4, int b1, vector<int> b4, int c1, vector<int> c4, AttributeMap& keyword_args)
    : B(parseState, a1, a4, b1, b4, keyword_args)
    , c1(c1)
    , c4(c4) {
  has_c2 = keyword_args.Take("c2", c2);
  if (!(keyword_args.Take("c3", c3))) {
    c3 = 0;
  }
  has_c5 = keyword_args.Take("c5", c5);
  if (!(keyword_args.Take("c6", c6))) {
    c6 = 0;
  }
  keyword_args.Take("c7", c7);
  if (!(keyword_args.Take("c8", c8))) {
    c8 = 0;
  }
  keyword_args.Take("c9", c9);
}

void C::printMembers(ostream& out) {
  B::printMembers(out);

  out << " c1:";
  classpPrint(out, c1);
  if (has_c2) {
    out << " c2:";
    classpPrint(out, c2);
  } else {
    out << "c2[not defined]";
    }
  out << " c3:";
  classpPrint(out, c3);
  out << " c4:";
  classpPrint(out, c4);
  if (has_c5) {
    out << " c5:";
    classpPrint(out, c5);
  } else {
    out << "c5[not defined]";
    }
  out << " c6:";
  classpPrint(out, c6);
  out << " c7:";
  classpPrint(out, c7);
  out << " c8:";
  classpPrint(out, c8);
  out << " c9:";
  classpPrint(out, c9);
}

void C::format(ostream& out, int precedence) {
  classpFormat(out, 0, a1);
  classpFormat(out, 0, a2);
  classpFormat(out, 0, a3);
  classpFormat(out, 0, a4[i]);
  classpFormat(out, 0, b1);
  classpFormat(out, 0, b2);
  classpFormat(out, 0, b3);
  classpFormat(out, 0, b4[i]);
  classpFormat(out, 0, c1);
  classpFormat(out, 0, c2);
  classpFormat(out, 0, c3);
  classpFormat(out, 0, c4[i]);
  if (has_c5) {
    classpFormat(out, 0, c5);
  }
  if (c6 != 0) {
    classpFormat(out, 0, c6);
  } else if ((!c7.empty())) {
    classpFormat(out, 0, c7[i]);
  } else {
    classpFormat(out, 0, c6);
  }
  if (c8 != 0) {
    classpFormat(out, 0, c8);
  }
  if ((!c9.empty())) {
    for (size_t i = 0; i < c9.size(); i++) {
      classpFormat(out, 0, c9[i]);
    }
  }
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
  std::cout << "parsing '" << sample << "':\n";
  AstNode* result = T::parse(input, errors);
  if (result) {
    std::cout << "SUCCEEDS";
    if (expected_result == kPrint) {
      std::cout << ": ";
      result->print(std::cout);
    } else if (expected_result == kFail) {
      std::cout << ": ERROR[expected fail]\n";
      return 1;
    } else if (expected_result != kSucceed) {
      stringstream actual_result;
      result->print(actual_result);
      if (actual_result.str() != expected_result) {
        std::cout << ": ERROR[no match:\n  expected-> " << expected_result
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


}  // namespace t9

#ifdef PARSER_TEST

#include <fstream>
#include <iostream>
#include <memory>

using std::istream;
using std::ifstream;
using std::unique_ptr;

const char usage[] = "usage: t9 [input-file | --samples]\n";

int main(int argc, char** argv) {
  if (argc > 2) {
    std::cerr << usage;
    exit(1);
  }
  if (std::string(argv[1]) == "--samples") {
    if (t9::ParseSamples() > 0) exit(1);
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
    t9::ParseAndPrint(input, std::cout);
  }
  return 0;
}

#endif  // PARSER_TEST

/* END PARSER */
