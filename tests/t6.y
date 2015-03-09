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

namespace t6 {
class ParseDriver;
class AstNode;
/* BEGIN FORWARD DECLARATIONS */
class A;
class B;
class C;
class D;
class E;
/* END FORWARD DECLARATIONS */
}

}

%require "3.0.2"
%defines
%define api.value.type variant
%define "parser_class_name" {YYParser}
%error-verbose
%lex-param {t6::ParseDriver* parser}
%locations
%define api.namespace {t6}
%parse-param {t6::ParseDriver* parser}
%parse-param {t6::AstNode** result}
%define api.token.constructor
%skeleton "lalr1.cc"

%initial-action {
}

%token TOK_EOF 0              "end of file"

/* BEGIN GENERATED TOKEN DECLARATIONS */
%token TOK_HASH "`#`"
%token TOK_COMMA "`,`"
%token TOK_LT "`<`"
%token TOK_GT "`>`"
%token TOK_BAR "`|`"
/* END GENERATED TOKEN DECLARATIONS */

%token TOK_BOOL   "boolean literal"
%token TOK_IDENTIFIER         "identifier"
%token TOK_INT64    "integer literal"
%token TOK_STRING_LITERAL     "string literal"


/* BEGIN NONTERMINAL TYPES */
%type <A*> class_A
%type <B*> class_B
%type <C*> class_C
%type <D*> class_D
%type <E*> class_E
%type <AttributeMap> multiarrayPLUS_D
%type <AttributeMap> multiarrayPLUS_D__2
%type <AttributeMap> multiarrayPLUS_E
%type <AttributeMap> multiarraySTAR_A
%type <AttributeMap> multiarraySTAR_B
%type <AttributeMap> multiarraySTAR_C
%type <AttributeMap> multiarraySTAR_E
/* END NONTERMINAL TYPES */

%type <bool> TOK_BOOL
%type <int64_t> TOK_INT64
%type <string>  TOK_IDENTIFIER
%type <string>  TOK_STRING_LITERAL

%code {

#include "t6.yacc.hh"
#include "t6.h"

namespace t6 {

YYParser::symbol_type yylex(ParseDriver* parser);

}  // namespace t6

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
  :  multiarraySTAR_A {
      AttributeMap keywords = $1;
      $$ = new A(@$, keywords); }
  ;

multiarraySTAR_A
  : { $$ = AttributeMap(); }
  |  TOK_LT TOK_INT64 TOK_GT {
      $$ = AttributeMap();
      $$.Push<int>("a1", $2); }
  | multiarraySTAR_A  TOK_BAR TOK_HASH TOK_LT TOK_INT64 TOK_GT {
      $$ = $1;
      $$.Push("a1", $5); }
  ;

class_B
  :  multiarraySTAR_B {
      AttributeMap keywords = $1;
      $$ = new B(@$, keywords); }
  ;

multiarraySTAR_B
  : { $$ = AttributeMap(); }
  |  TOK_LT TOK_INT64 TOK_COMMA TOK_INT64 TOK_GT {
      $$ = AttributeMap();
      $$.Push<int>("a1", $2);
      $$.Push<int>("a2", $4); }
  | multiarraySTAR_B TOK_BAR TOK_LT TOK_INT64 TOK_COMMA TOK_INT64 TOK_GT {
      $$ = $1;
      $$.Push("a1", $4);
      $$.Push("a2", $6); }
  ;

class_C
  :  multiarraySTAR_C {
      AttributeMap keywords = $1;
      $$ = new C(@$, keywords); }
  ;

multiarraySTAR_C
  : { $$ = AttributeMap(); }
  | multiarraySTAR_C  TOK_LT TOK_INT64 TOK_COMMA TOK_INT64 TOK_GT {
      $$ = $1;
      $$.Push("a1", $3);
      $$.Push("a2", $5); }
  ;

class_D
  :   multiarrayPLUS_D  multiarrayPLUS_D__2 {
      AttributeMap keywords = $1;
      keywords.Merge($2);
      $$ = new D(@$, keywords); }
  ;

multiarrayPLUS_D
  :  TOK_LT TOK_INT64 TOK_COMMA TOK_INT64 TOK_GT {
      $$ = AttributeMap();
      $$.Push<int>("a1", $2);
      $$.Push<int>("a2", $4); }
  | multiarrayPLUS_D  TOK_LT TOK_INT64 TOK_COMMA TOK_INT64 TOK_GT {
      $$ = $1;
      $$.Push("a1", $3);
      $$.Push("a2", $5); }
  ;

multiarrayPLUS_D__2
  :  TOK_LT TOK_INT64 TOK_COMMA TOK_INT64 TOK_GT {
      $$ = AttributeMap();
      $$.Push<int>("a3", $2);
      $$.Push<int>("a4", $4); }
  | multiarrayPLUS_D__2 TOK_BAR TOK_LT TOK_INT64 TOK_COMMA TOK_INT64 TOK_GT {
      $$ = $1;
      $$.Push("a3", $4);
      $$.Push("a4", $6); }
  ;

class_E
  :   multiarraySTAR_E  multiarrayPLUS_E {
      $$ = new E(@$); }
  ;

multiarraySTAR_E
  : { $$ = AttributeMap(); }
  | multiarraySTAR_E  TOK_LT TOK_GT {
      $$ = $1; }
  ;

multiarrayPLUS_E
  :  TOK_LT TOK_GT {
      $$ = AttributeMap(); }
  | multiarrayPLUS_E TOK_COMMA TOK_LT TOK_GT {
      $$ = $1; }
  ;

/* END PRODUCTIONS */

%%

#include <sstream>

namespace t6 {

using std::istream;
using std::ostream;
using std::stringstream;

AstNode* parse(istream& input, ostream& errors);

/* BEGIN METHOD DEFINITIONS */
A::A(ParseState parseState, AttributeMap& keyword_args)
    : AstNode(parseState) {
  keyword_args.Take("a1", a1);
}

void A::printMembers(ostream& out) {
  out << " a1:";
  classpPrint(out, a1);
}

void A::format(ostream& out, int precedence) {
  for (size_t i = 0; i < a1.size(); i++) {
    if (i > 0) {
      out << "|";
      out << " ";
      out << "#";
    }
    out << "<";
    out << " ";
    classpFormat(out, 0, a1[i]);
    out << " ";
    out << ">";
  }
}
B::B(ParseState parseState, AttributeMap& keyword_args)
    : AstNode(parseState) {
  keyword_args.Take("a1", a1);
  keyword_args.Take("a2", a2);
}

void B::printMembers(ostream& out) {
  out << " a1:";
  classpPrint(out, a1);
  out << " a2:";
  classpPrint(out, a2);
}

void B::format(ostream& out, int precedence) {
  for (size_t i = 0; i < a1.size(); i++) {
    if (i > 0) {
      out << "|";
    }
    out << "<";
    out << " ";
    classpFormat(out, 0, a1[i]);
    out << " ";
    out << ",";
    out << " ";
    classpFormat(out, 0, a2[i]);
    out << " ";
    out << ">";
  }
}
C::C(ParseState parseState, AttributeMap& keyword_args)
    : AstNode(parseState) {
  keyword_args.Take("a1", a1);
  keyword_args.Take("a2", a2);
}

void C::printMembers(ostream& out) {
  out << " a1:";
  classpPrint(out, a1);
  out << " a2:";
  classpPrint(out, a2);
}

void C::format(ostream& out, int precedence) {
  for (size_t i = 0; i < a1.size(); i++) {
    out << "<";
    out << " ";
    classpFormat(out, 0, a1[i]);
    out << " ";
    out << ",";
    out << " ";
    classpFormat(out, 0, a2[i]);
    out << " ";
    out << ">";
  }
}
D::D(ParseState parseState, AttributeMap& keyword_args)
    : AstNode(parseState) {
  keyword_args.Take("a1", a1);
  keyword_args.Take("a2", a2);
  keyword_args.Take("a3", a3);
  keyword_args.Take("a4", a4);
}

void D::printMembers(ostream& out) {
  out << " a1:";
  classpPrint(out, a1);
  out << " a2:";
  classpPrint(out, a2);
  out << " a3:";
  classpPrint(out, a3);
  out << " a4:";
  classpPrint(out, a4);
}

void D::format(ostream& out, int precedence) {
  for (size_t i = 0; i < a1.size(); i++) {
    out << "<";
    out << " ";
    classpFormat(out, 0, a1[i]);
    out << " ";
    out << ",";
    out << " ";
    classpFormat(out, 0, a2[i]);
    out << " ";
    out << ">";
  }
  out << " ";
  for (size_t i = 0; i < a3.size(); i++) {
    if (i > 0) {
      out << "|";
    }
    out << "<";
    out << " ";
    classpFormat(out, 0, a3[i]);
    out << " ";
    out << ",";
    out << " ";
    classpFormat(out, 0, a4[i]);
    out << " ";
    out << ">";
  }
}
E::E(ParseState parseState)
    : AstNode(parseState) {
}

void E::printMembers(ostream& out) {
}

void E::format(ostream& out, int precedence) {
  out << "<";
  out << " ";
  out << ">";
  out << " ";
  out << "<";
  out << " ";
  out << ">";
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


}  // namespace t6

#ifdef PARSER_TEST

#include <fstream>
#include <iostream>

using std::istream;
using std::ifstream;

const char usage[] = "usage: t6 [input-file | --samples]\n";

int main(int argc, char** argv) {
  if (argc > 2) {
    std::cerr << usage;
    exit(1);
  }
  if (argc == 2 && std::string(argv[1]) == "--samples") {
    if (t6::ParseSamples() > 0) exit(1);
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
    t6::ParseAndPrint(input, std::cout);
  }
  return 0;
}

#endif  // PARSER_TEST

/* END PARSER */
