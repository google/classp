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

namespace assign1 {
class ParseDriver;
class AstNode;
/* BEGIN FORWARD DECLARATIONS */
class B;
class B2;
class D;
class I;
class S;
/* END FORWARD DECLARATIONS */
}

}

%require "3.0.2"
%defines
%define api.value.type variant
%define "parser_class_name" {YYParser}
%error-verbose
%lex-param {assign1::ParseDriver* parser}
%locations
%define api.namespace {assign1}
%parse-param {assign1::ParseDriver* parser}
%parse-param {assign1::AstNode** result}
%define api.token.constructor
%skeleton "lalr1.cc"

%initial-action {
}

%token TOK_EOF 0              "end of file"

/* BEGIN GENERATED TOKEN DECLARATIONS */
%token WORD_B "`B`"
%token WORD_B2 "`B2`"
%token WORD_I "`I`"
%token WORD_S "`S`"
%token WORD_done "`done`"
%token WORD_f "`f`"
%token WORD_one "`one`"
%token WORD_t "`t`"
%token WORD_two "`two`"
/* END GENERATED TOKEN DECLARATIONS */

%token TOK_BOOL   "boolean literal"
%token TOK_IDENTIFIER         "identifier"
%token TOK_INT64    "integer literal"
%token TOK_STRING_LITERAL     "string literal"


/* BEGIN NONTERMINAL TYPES */
%type <D*> class_D
%type <B*> class_B
%type <I*> class_I
%type <S*> class_S
%type <B2*> class_B2
%type <bool> typed_bool__1
%type <bool> typed_bool__2
%type <int> typed_int__1
%type <string> typed_string__1
/* END NONTERMINAL TYPES */

%type <bool> TOK_BOOL
%type <int64_t> TOK_INT64
%type <string>  TOK_IDENTIFIER
%type <string>  TOK_STRING_LITERAL

%code {

#include "assign1.yacc.hh"
#include "assign1.h"

namespace assign1 {

YYParser::symbol_type yylex(ParseDriver* parser);

}  // namespace assign1

}


%start start

%%

start
/* BEGIN PARSEABLE */
  : class_D TOK_EOF { *result = $1; }
/* END PARSEABLE */
  ;

/* BEGIN PRODUCTIONS */
class_B
  : typed_bool__1 {
      $$ = new B(@$, $1); }
  ;

typed_bool__1
  : WORD_t { $$ = true; }
  | WORD_f { $$ = false; }
  ;

class_I
  : typed_int__1 {
      $$ = new I(@$, $1); }
  ;

typed_int__1
  : WORD_one { $$ = 1; }
  | WORD_two { $$ = 2; }
  ;

class_S
  : typed_string__1 {
      $$ = new S(@$, $1); }
  ;

typed_string__1
  : WORD_one { $$ = "1"; }
  | WORD_two { $$ = "2"; }
  ;

class_B2
  : typed_bool__2 {
      $$ = new B2(@$, $1); }
  ;

typed_bool__2
  : WORD_t { $$ = true; }
  |  { $$ = false; }
  ;

class_D
  :  WORD_B class_B WORD_I class_I WORD_S class_S WORD_B2 class_B2 WORD_done {
      $$ = new D(@$, $2, $4, $6, $8); }
  ;

/* END PRODUCTIONS */

%%

#include <sstream>

namespace assign1 {

using std::istream;
using std::ostream;
using std::stringstream;

AstNode* parse(istream& input, ostream& errors);

/* BEGIN METHOD DEFINITIONS */
B::B(ParseState parseState, bool b)
    : AstNode(parseState)
    , b(b) {
}

void B::printMembers(ostream& out) {
  out << " b:";
  classpPrint(out, b);
}

void B::format(ostream& out, int precedence) {
  if (b == true) {
    out << "t";
  } else if (b == false) {
    out << "f";
  }
}
I::I(ParseState parseState, int i)
    : AstNode(parseState)
    , i(i) {
}

void I::printMembers(ostream& out) {
  out << " i:";
  classpPrint(out, i);
}

void I::format(ostream& out, int precedence) {
  if (i == 1) {
    out << "one";
  } else if (i == 2) {
    out << "two";
  }
}
S::S(ParseState parseState, string s)
    : AstNode(parseState)
    , s(s) {
}

void S::printMembers(ostream& out) {
  out << " s:";
  classpPrint(out, s);
}

void S::format(ostream& out, int precedence) {
  if (s == "1") {
    out << "one";
  } else if (s == "2") {
    out << "two";
  }
}
B2::B2(ParseState parseState, bool b)
    : AstNode(parseState)
    , b(b) {
}

void B2::printMembers(ostream& out) {
  out << " b:";
  classpPrint(out, b);
}

void B2::format(ostream& out, int precedence) {
  if (b == true) {
    out << "t";
  } else if (b == false) {
    out << "";
  }
}
D::D(ParseState parseState, B* b, I* i, S* s, B2* b2)
    : AstNode(parseState)
    , b(b)
    , i(i)
    , s(s)
    , b2(b2) {
}

D* D::parse(istream& input, ostream& errors) {
  return dynamic_cast<D*>(assign1::parse(input, errors));
}
void D::printMembers(ostream& out) {
  out << " b:";
  classpPrint(out, b);
  out << " i:";
  classpPrint(out, i);
  out << " s:";
  classpPrint(out, s);
  out << " b2:";
  classpPrint(out, b2);
}

void D::format(ostream& out, int precedence) {
  out << "B";
  out << " ";
  classpFormat(out, 0, b);
  out << " ";
  out << "I";
  out << " ";
  classpFormat(out, 0, i);
  out << " ";
  out << "S";
  out << " ";
  classpFormat(out, 0, s);
  out << " ";
  out << "B2";
  out << " ";
  classpFormat(out, 0, b2);
  out << " ";
  out << "done";
}
/* END METHOD DEFINITIONS */

#ifdef PARSER_TEST

void ParseAndPrint(istream& input, ostream& out) {
/* BEGIN PARSE PARSEABLE */
  AstNode* result = D::parse(input, out);
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
  num_errors += ParseSample<D>(R"#A#(B t I two S one B2 done)#A#", R"#A#((D b:(B b:true) i:(I i:2) s:(S s:1) b2:(B2 b:false)))#A#");
  num_errors += ParseSample<D>(R"#A#(B f I one S two B2 t done)#A#", R"#A#((D b:(B b:false) i:(I i:1) s:(S s:2) b2:(B2 b:true)))#A#");
/* END SAMPLES */
  std::cout << "Errors: " << num_errors << "\n";
  return num_errors;
}

#endif  // PARSER_TEST


}  // namespace assign1

#ifdef PARSER_TEST

#include <fstream>
#include <iostream>

using std::istream;
using std::ifstream;

const char usage[] = "usage: assign1 [input-file | --samples]\n";

int main(int argc, char** argv) {
  if (argc > 2) {
    std::cerr << usage;
    exit(1);
  }
  if (argc == 2 && std::string(argv[1]) == "--samples") {
    if (assign1::ParseSamples() > 0) exit(1);
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
    assign1::ParseAndPrint(input, std::cout);
  }
  return 0;
}

#endif  // PARSER_TEST

/* END PARSER */
