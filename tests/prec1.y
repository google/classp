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

namespace prec1 {
class ParseDriver;
class AstNode;
/* BEGIN FORWARD DECLARATIONS */
class A;
class Bleft;
class Bnassoc;
class Bright;
class Cleft;
class Cnassoc;
class Cright;
class Dleft;
class Dnassoc;
class Dright;
class Literal;
/* END FORWARD DECLARATIONS */
}

}

%require "3.0.2"
%defines
%define api.value.type variant
%define "parser_class_name" {YYParser}
%error-verbose
%lex-param {prec1::ParseDriver* parser}
%locations
%define api.namespace {prec1}
%parse-param {prec1::ParseDriver* parser}
%parse-param {prec1::AstNode** result}
%define api.token.constructor
%skeleton "lalr1.cc"

%initial-action {
}

%token TOK_EOF 0              "end of file"

/* BEGIN GENERATED TOKEN DECLARATIONS */
%token TOK_LPAREN "`(`"
%token TOK_RPAREN "`)`"
%token WORD_b "`b`"
%token WORD_c "`c`"
%token WORD_d "`d`"
/* END GENERATED TOKEN DECLARATIONS */

%token TOK_BOOL   "boolean literal"
%token TOK_IDENTIFIER         "identifier"
%token TOK_INT64    "integer literal"
%token TOK_STRING_LITERAL     "string literal"


/* BEGIN NONTERMINAL TYPES */
%type <A*> class_A
%type <A*> class_A_p3
%type <A*> class_A_p5
%type <A*> class_A_p6
/* END NONTERMINAL TYPES */

%type <bool> TOK_BOOL
%type <int64_t> TOK_INT64
%type <string>  TOK_IDENTIFIER
%type <string>  TOK_STRING_LITERAL

%code {

#include "prec1.yacc.hh"
#include "prec1.h"

namespace prec1 {

YYParser::symbol_type yylex(ParseDriver* parser);

}  // namespace prec1

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
  :  WORD_b class_A_p3 {
      $$ = new Bnassoc(@$, $2); }  // Bnassoc
  |  class_A_p3 WORD_c {
      $$ = new Cnassoc(@$, $1); }  // Cnassoc
  |  class_A_p3 WORD_d class_A_p3 {
      $$ = new Dnassoc(@$, $1, $3); }  // Dnassoc
  | class_A_p3 { $$ = $1; }
  ;
class_A_p3
  :  WORD_b class_A_p3 {
      $$ = new Bright(@$, $2); }  // Bright
  |  class_A_p5 WORD_c {
      $$ = new Cright(@$, $1); }  // Cright
  |  class_A_p5 WORD_d class_A_p3 {
      $$ = new Dright(@$, $1, $3); }  // Dright
  | class_A_p5 { $$ = $1; }
  ;
class_A_p5
  :  WORD_b class_A_p6 {
      $$ = new Bleft(@$, $2); }  // Bleft
  |  class_A_p5 WORD_c {
      $$ = new Cleft(@$, $1); }  // Cleft
  |  class_A_p5 WORD_d class_A_p6 {
      $$ = new Dleft(@$, $1, $3); }  // Dleft
  | class_A_p6 { $$ = $1; }
  ;
class_A_p6
  :  TOK_LPAREN class_A TOK_RPAREN { $$ = $2; }
  | TOK_INT64 {
      $$ = new Literal(@$, $1); }  // Literal
  ;











/* END PRODUCTIONS */

%%

#include <sstream>

namespace prec1 {

using std::istream;
using std::ostream;
using std::stringstream;

AstNode* parse(istream& input, ostream& errors);

/* BEGIN METHOD DEFINITIONS */
A::A(ParseState parseState)
    : AstNode(parseState) {
}

void A::printMembers(ostream& out) {
}

void A::bracketFormat(ostream& out, AstNode* self) {
  out << " ( ";
  classpFormat(out, 0, self);
  out << " ) ";
  }
Bleft::Bleft(ParseState parseState, A* a)
    : A(parseState)
    , a(a) {
}

void Bleft::printMembers(ostream& out) {
  A::printMembers(out);

  out << " a:";
  classpPrint(out, a);
}

void Bleft::format(ostream& out, int precedence) {
  if (precedence <= 5) {
    out << " b ";
    classpFormat(out, 4, a);
  } else {
    bracketFormat(out, this);
  }
}
Cleft::Cleft(ParseState parseState, A* a)
    : A(parseState)
    , a(a) {
}

void Cleft::printMembers(ostream& out) {
  A::printMembers(out);

  out << " a:";
  classpPrint(out, a);
}

void Cleft::format(ostream& out, int precedence) {
  if (precedence <= 5) {
    classpFormat(out, 5, a);
    out << " c ";
  } else {
    bracketFormat(out, this);
  }
}
Dleft::Dleft(ParseState parseState, A* a1, A* a2)
    : A(parseState)
    , a1(a1)
    , a2(a2) {
}

void Dleft::printMembers(ostream& out) {
  A::printMembers(out);

  out << " a1:";
  classpPrint(out, a1);
  out << " a2:";
  classpPrint(out, a2);
}

void Dleft::format(ostream& out, int precedence) {
  if (precedence <= 5) {
    classpFormat(out, 5, a1);
    out << " d ";
    classpFormat(out, 4, a2);
  } else {
    bracketFormat(out, this);
  }
}
Bright::Bright(ParseState parseState, A* a)
    : A(parseState)
    , a(a) {
}

void Bright::printMembers(ostream& out) {
  A::printMembers(out);

  out << " a:";
  classpPrint(out, a);
}

void Bright::format(ostream& out, int precedence) {
  if (precedence <= 3) {
    out << " b ";
    classpFormat(out, 3, a);
  } else {
    bracketFormat(out, this);
  }
}
Cright::Cright(ParseState parseState, A* a)
    : A(parseState)
    , a(a) {
}

void Cright::printMembers(ostream& out) {
  A::printMembers(out);

  out << " a:";
  classpPrint(out, a);
}

void Cright::format(ostream& out, int precedence) {
  if (precedence <= 3) {
    classpFormat(out, 2, a);
    out << " c ";
  } else {
    bracketFormat(out, this);
  }
}
Dright::Dright(ParseState parseState, A* a1, A* a2)
    : A(parseState)
    , a1(a1)
    , a2(a2) {
}

void Dright::printMembers(ostream& out) {
  A::printMembers(out);

  out << " a1:";
  classpPrint(out, a1);
  out << " a2:";
  classpPrint(out, a2);
}

void Dright::format(ostream& out, int precedence) {
  if (precedence <= 3) {
    classpFormat(out, 2, a1);
    out << " d ";
    classpFormat(out, 3, a2);
  } else {
    bracketFormat(out, this);
  }
}
Bnassoc::Bnassoc(ParseState parseState, A* a)
    : A(parseState)
    , a(a) {
}

void Bnassoc::printMembers(ostream& out) {
  A::printMembers(out);

  out << " a:";
  classpPrint(out, a);
}

void Bnassoc::format(ostream& out, int precedence) {
  if (precedence <= 2) {
    out << " b ";
    classpFormat(out, 1, a);
  } else {
    bracketFormat(out, this);
  }
}
Cnassoc::Cnassoc(ParseState parseState, A* a)
    : A(parseState)
    , a(a) {
}

void Cnassoc::printMembers(ostream& out) {
  A::printMembers(out);

  out << " a:";
  classpPrint(out, a);
}

void Cnassoc::format(ostream& out, int precedence) {
  if (precedence <= 2) {
    classpFormat(out, 1, a);
    out << " c ";
  } else {
    bracketFormat(out, this);
  }
}
Dnassoc::Dnassoc(ParseState parseState, A* a1, A* a2)
    : A(parseState)
    , a1(a1)
    , a2(a2) {
}

void Dnassoc::printMembers(ostream& out) {
  A::printMembers(out);

  out << " a1:";
  classpPrint(out, a1);
  out << " a2:";
  classpPrint(out, a2);
}

void Dnassoc::format(ostream& out, int precedence) {
  if (precedence <= 2) {
    classpFormat(out, 1, a1);
    out << " d ";
    classpFormat(out, 1, a2);
  } else {
    bracketFormat(out, this);
  }
}
Literal::Literal(ParseState parseState, int val)
    : A(parseState)
    , val(val) {
}

void Literal::printMembers(ostream& out) {
  A::printMembers(out);

  out << " val:";
  classpPrint(out, val);
}

void Literal::format(ostream& out, int precedence) {
  if (precedence <= 6) {
    classpFormat(out, 0, val);
  } else {
    bracketFormat(out, this);
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


}  // namespace prec1

#ifdef PARSER_TEST

#include <fstream>
#include <iostream>
#include <memory>

using std::istream;
using std::ifstream;
using std::unique_ptr;

const char usage[] = "usage: prec1 [input-file | --samples]\n";

int main(int argc, char** argv) {
  if (argc > 2) {
    std::cerr << usage;
    exit(1);
  }
  if (std::string(argv[1]) == "--samples") {
    if (prec1::ParseSamples() > 0) exit(1);
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
    prec1::ParseAndPrint(input, std::cout);
  }
  return 0;
}

#endif  // PARSER_TEST

/* END PARSER */
