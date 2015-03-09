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

namespace expr1 {
class ParseDriver;
class AstNode;
/* BEGIN FORWARD DECLARATIONS */
class AndOp;
class CaseExpression;
class DivideOp;
class EqOp;
class Expression;
class FunctionCall;
class GeOp;
class GtOp;
class IntegerLiteral;
class Invocation;
class LeOp;
class LtOp;
class MinusOp;
class NeOp;
class NegateOp;
class NotOp;
class OrOp;
class PlusOp;
class TimesOp;
class Variable;
/* END FORWARD DECLARATIONS */
}

}

%require "3.0.2"
%defines
%define api.value.type variant
%define "parser_class_name" {YYParser}
%error-verbose
%lex-param {expr1::ParseDriver* parser}
%locations
%define api.namespace {expr1}
%parse-param {expr1::ParseDriver* parser}
%parse-param {expr1::AstNode** result}
%define api.token.constructor
%skeleton "lalr1.cc"

%initial-action {
}

%token TOK_EOF 0              "end of file"

/* BEGIN GENERATED TOKEN DECLARATIONS */
%token SYM__2 "`!=`"
%token TOK_LPAREN "`(`"
%token TOK_RPAREN "`)`"
%token TOK_STAR "`*`"
%token TOK_PLUS "`+`"
%token TOK_COMMA "`,`"
%token TOK_MINUS "`-`"
%token TOK_SLASH "`/`"
%token TOK_LT "`<`"
%token SYM__1 "`<=`"
%token TOK_EQ "`=`"
%token TOK_GT "`>`"
%token SYM__0 "`>=`"
%token WORD_and "`and`"
%token WORD_case "`case`"
%token WORD_else "`else`"
%token WORD_end "`end`"
%token WORD_not "`not`"
%token WORD_or "`or`"
%token WORD_then "`then`"
%token WORD_when "`when`"
/* END GENERATED TOKEN DECLARATIONS */

%token TOK_BOOL   "boolean literal"
%token TOK_IDENTIFIER         "identifier"
%token TOK_INT64    "integer literal"
%token TOK_STRING_LITERAL     "string literal"


/* BEGIN NONTERMINAL TYPES */
%type <Expression*> class_Expression
%type <vector<Expression*>> array0_Expression__1
%type <Expression*> class_Expression_p2
%type <Expression*> class_Expression_p3
%type <Expression*> class_Expression_p4
%type <Expression*> class_Expression_p5
%type <Expression*> class_Expression_p6
%type <Expression*> class_Expression_p7
%type <Expression*> class_Expression_p8
%type <AttributeMap> multiarraySTAR_CaseExpression
/* END NONTERMINAL TYPES */

%type <bool> TOK_BOOL
%type <int64_t> TOK_INT64
%type <string>  TOK_IDENTIFIER
%type <string>  TOK_STRING_LITERAL

%code {

#include "expr1.yacc.hh"
#include "expr1.h"

namespace expr1 {

YYParser::symbol_type yylex(ParseDriver* parser);

}  // namespace expr1

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
  :  class_Expression WORD_or class_Expression_p2 {
      AttributeMap keywords = AttributeMap();
      keywords.Add("arg2", $3);
      $$ = new OrOp(@$, $1, keywords); }  // OrOp
  | class_Expression_p2 { $$ = $1; }
  ;
class_Expression_p2
  :  class_Expression_p2 WORD_and class_Expression_p3 {
      AttributeMap keywords = AttributeMap();
      keywords.Add("arg2", $3);
      $$ = new AndOp(@$, $1, keywords); }  // AndOp
  | class_Expression_p3 { $$ = $1; }
  ;
class_Expression_p3
  :  WORD_not class_Expression_p4 {
      AttributeMap keywords = AttributeMap();
      $$ = new NotOp(@$, $2, keywords); }  // NotOp
  | class_Expression_p4 { $$ = $1; }
  ;
class_Expression_p4
  :  class_Expression_p5 TOK_EQ class_Expression_p5 {
      AttributeMap keywords = AttributeMap();
      keywords.Add("arg2", $3);
      $$ = new EqOp(@$, $1, keywords); }  // EqOp
  |  class_Expression_p5 SYM__0 class_Expression_p5 {
      AttributeMap keywords = AttributeMap();
      keywords.Add("arg2", $3);
      $$ = new GeOp(@$, $1, keywords); }  // GeOp
  |  class_Expression_p5 TOK_GT class_Expression_p5 {
      AttributeMap keywords = AttributeMap();
      keywords.Add("arg2", $3);
      $$ = new GtOp(@$, $1, keywords); }  // GtOp
  |  class_Expression_p5 SYM__1 class_Expression_p5 {
      AttributeMap keywords = AttributeMap();
      keywords.Add("arg2", $3);
      $$ = new LeOp(@$, $1, keywords); }  // LeOp
  |  class_Expression_p5 TOK_LT class_Expression_p5 {
      AttributeMap keywords = AttributeMap();
      keywords.Add("arg2", $3);
      $$ = new LtOp(@$, $1, keywords); }  // LtOp
  |  class_Expression_p5 SYM__2 class_Expression_p5 {
      AttributeMap keywords = AttributeMap();
      keywords.Add("arg2", $3);
      $$ = new NeOp(@$, $1, keywords); }  // NeOp
  | class_Expression_p5 { $$ = $1; }
  ;
class_Expression_p5
  :  class_Expression_p5 TOK_MINUS class_Expression_p6 {
      AttributeMap keywords = AttributeMap();
      keywords.Add("arg2", $3);
      $$ = new MinusOp(@$, $1, keywords); }  // MinusOp
  |  class_Expression_p5 TOK_PLUS class_Expression_p6 {
      AttributeMap keywords = AttributeMap();
      keywords.Add("arg2", $3);
      $$ = new PlusOp(@$, $1, keywords); }  // PlusOp
  | class_Expression_p6 { $$ = $1; }
  ;
class_Expression_p6
  :  class_Expression_p6 TOK_SLASH class_Expression_p7 {
      AttributeMap keywords = AttributeMap();
      keywords.Add("arg2", $3);
      $$ = new DivideOp(@$, $1, keywords); }  // DivideOp
  |  class_Expression_p6 TOK_STAR class_Expression_p7 {
      AttributeMap keywords = AttributeMap();
      keywords.Add("arg2", $3);
      $$ = new TimesOp(@$, $1, keywords); }  // TimesOp
  | class_Expression_p7 { $$ = $1; }
  ;
class_Expression_p7
  :  TOK_MINUS class_Expression_p8 {
      AttributeMap keywords = AttributeMap();
      $$ = new NegateOp(@$, $2, keywords); }  // NegateOp
  | class_Expression_p8 { $$ = $1; }
  ;
class_Expression_p8
  :  WORD_case class_Expression  multiarraySTAR_CaseExpression  WORD_else class_Expression WORD_end {
      AttributeMap keywords = $3;
      $$ = new CaseExpression(@$, $2, $5, keywords); }  // CaseExpression
  |  TOK_LPAREN class_Expression TOK_RPAREN { $$ = $2; }
  |  TOK_IDENTIFIER TOK_LPAREN  array0_Expression__1 TOK_RPAREN {
      $$ = new FunctionCall(@$, $1, $3); }  // FunctionCall
  | TOK_INT64 {
      $$ = new IntegerLiteral(@$, $1); }  // IntegerLiteral
  | TOK_IDENTIFIER {
      $$ = new Variable(@$, $1); }  // Variable
  ;

multiarraySTAR_CaseExpression
  : { $$ = AttributeMap(); }
  | multiarraySTAR_CaseExpression  WORD_when class_Expression WORD_then class_Expression {
      $$ = $1;
      $$.Push("conditions", $3);
      $$.Push("results", $5); }
  ;

array0_Expression__1
  : { $$ = vector<Expression*>(); }
  | class_Expression { $$ = vector<Expression*>(); $$.emplace_back($1); }
  | array0_Expression__1 TOK_COMMA class_Expression { $$ = $1; $$.emplace_back($3); }
  ;




















/* END PRODUCTIONS */

%%

#include <sstream>

namespace expr1 {

using std::istream;
using std::ostream;
using std::stringstream;

AstNode* parse(istream& input, ostream& errors);

/* BEGIN METHOD DEFINITIONS */
Expression::Expression(ParseState parseState)
    : AstNode(parseState) {
}

Expression* Expression::parse(istream& input, ostream& errors) {
  return dynamic_cast<Expression*>(expr1::parse(input, errors));
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
Variable::Variable(ParseState parseState, identifier id)
    : Expression(parseState)
    , id(id) {
}

void Variable::printMembers(ostream& out) {
  Expression::printMembers(out);

  out << " id:";
  classpPrint(out, id);
}

void Variable::format(ostream& out, int precedence) {
  classpFormat(out, 0, id);
}
IntegerLiteral::IntegerLiteral(ParseState parseState, int val)
    : Expression(parseState)
    , val(val) {
}

void IntegerLiteral::printMembers(ostream& out) {
  Expression::printMembers(out);

  out << " val:";
  classpPrint(out, val);
}

void IntegerLiteral::format(ostream& out, int precedence) {
  classpFormat(out, 0, val);
}
CaseExpression::CaseExpression(ParseState parseState, Expression* testExpr, Expression* elseResult, AttributeMap& keyword_args)
    : Expression(parseState)
    , testExpr(testExpr)
    , elseResult(elseResult) {
  keyword_args.Take("conditions", conditions);
  keyword_args.Take("results", results);
}

void CaseExpression::printMembers(ostream& out) {
  Expression::printMembers(out);

  out << " testExpr:";
  classpPrint(out, testExpr);
  out << " conditions:";
  classpPrint(out, conditions);
  out << " results:";
  classpPrint(out, results);
  out << " elseResult:";
  classpPrint(out, elseResult);
}

void CaseExpression::format(ostream& out, int precedence) {
  out << "case";
  out << " ";
  classpFormat(out, 0, testExpr);
  out << " ";
  for (size_t i = 0; i < conditions.size(); i++) {
    out << "when";
    out << " ";
    classpFormat(out, 0, conditions[i]);
    out << " ";
    out << "then";
    out << " ";
    classpFormat(out, 0, results[i]);
  }
  out << " ";
  out << "else";
  out << " ";
  classpFormat(out, 0, elseResult);
  out << " ";
  out << "end";
}
Invocation::Invocation(ParseState parseState, Expression* arg1, AttributeMap& keyword_args)
    : Expression(parseState)
    , arg1(arg1) {
  has_arg2 = keyword_args.Take("arg2", arg2);
  has_arg3 = keyword_args.Take("arg3", arg3);
}

void Invocation::printMembers(ostream& out) {
  Expression::printMembers(out);

  out << " arg1:";
  classpPrint(out, arg1);
  if (has_arg2) {
    out << " arg2:";
    classpPrint(out, arg2);
  } else {
    out << "arg2[not defined]";
    }
  if (has_arg3) {
    out << " arg3:";
    classpPrint(out, arg3);
  } else {
    out << "arg3[not defined]";
    }
}
OrOp::OrOp(ParseState parseState, Expression* arg1, AttributeMap& keyword_args)
    : Invocation(parseState, arg1, keyword_args) {
}

void OrOp::printMembers(ostream& out) {
  Invocation::printMembers(out);

}

void OrOp::format(ostream& out, int precedence) {
  if (precedence <= 1) {
    classpFormat(out, 1, arg1);
    out << " ";
    out << "or";
    out << " ";
    classpFormat(out, 2, arg2);
  } else {
    bracketFormat(out, this);
  }
}
AndOp::AndOp(ParseState parseState, Expression* arg1, AttributeMap& keyword_args)
    : Invocation(parseState, arg1, keyword_args) {
}

void AndOp::printMembers(ostream& out) {
  Invocation::printMembers(out);

}

void AndOp::format(ostream& out, int precedence) {
  if (precedence <= 2) {
    classpFormat(out, 2, arg1);
    out << " ";
    out << "and";
    out << " ";
    classpFormat(out, 3, arg2);
  } else {
    bracketFormat(out, this);
  }
}
NotOp::NotOp(ParseState parseState, Expression* arg1, AttributeMap& keyword_args)
    : Invocation(parseState, arg1, keyword_args) {
}

void NotOp::printMembers(ostream& out) {
  Invocation::printMembers(out);

}

void NotOp::format(ostream& out, int precedence) {
  if (precedence <= 3) {
    out << "not";
    out << " ";
    classpFormat(out, 4, arg1);
  } else {
    bracketFormat(out, this);
  }
}
EqOp::EqOp(ParseState parseState, Expression* arg1, AttributeMap& keyword_args)
    : Invocation(parseState, arg1, keyword_args) {
}

void EqOp::printMembers(ostream& out) {
  Invocation::printMembers(out);

}

void EqOp::format(ostream& out, int precedence) {
  if (precedence <= 4) {
    classpFormat(out, 5, arg1);
    out << " ";
    out << "=";
    out << " ";
    classpFormat(out, 5, arg2);
  } else {
    bracketFormat(out, this);
  }
}
NeOp::NeOp(ParseState parseState, Expression* arg1, AttributeMap& keyword_args)
    : Invocation(parseState, arg1, keyword_args) {
}

void NeOp::printMembers(ostream& out) {
  Invocation::printMembers(out);

}

void NeOp::format(ostream& out, int precedence) {
  if (precedence <= 4) {
    classpFormat(out, 5, arg1);
    out << " ";
    out << "!=";
    out << " ";
    classpFormat(out, 5, arg2);
  } else {
    bracketFormat(out, this);
  }
}
LtOp::LtOp(ParseState parseState, Expression* arg1, AttributeMap& keyword_args)
    : Invocation(parseState, arg1, keyword_args) {
}

void LtOp::printMembers(ostream& out) {
  Invocation::printMembers(out);

}

void LtOp::format(ostream& out, int precedence) {
  if (precedence <= 4) {
    classpFormat(out, 5, arg1);
    out << " ";
    out << "<";
    out << " ";
    classpFormat(out, 5, arg2);
  } else {
    bracketFormat(out, this);
  }
}
LeOp::LeOp(ParseState parseState, Expression* arg1, AttributeMap& keyword_args)
    : Invocation(parseState, arg1, keyword_args) {
}

void LeOp::printMembers(ostream& out) {
  Invocation::printMembers(out);

}

void LeOp::format(ostream& out, int precedence) {
  if (precedence <= 4) {
    classpFormat(out, 5, arg1);
    out << " ";
    out << "<=";
    out << " ";
    classpFormat(out, 5, arg2);
  } else {
    bracketFormat(out, this);
  }
}
GtOp::GtOp(ParseState parseState, Expression* arg1, AttributeMap& keyword_args)
    : Invocation(parseState, arg1, keyword_args) {
}

void GtOp::printMembers(ostream& out) {
  Invocation::printMembers(out);

}

void GtOp::format(ostream& out, int precedence) {
  if (precedence <= 4) {
    classpFormat(out, 5, arg1);
    out << " ";
    out << ">";
    out << " ";
    classpFormat(out, 5, arg2);
  } else {
    bracketFormat(out, this);
  }
}
GeOp::GeOp(ParseState parseState, Expression* arg1, AttributeMap& keyword_args)
    : Invocation(parseState, arg1, keyword_args) {
}

void GeOp::printMembers(ostream& out) {
  Invocation::printMembers(out);

}

void GeOp::format(ostream& out, int precedence) {
  if (precedence <= 4) {
    classpFormat(out, 5, arg1);
    out << " ";
    out << ">=";
    out << " ";
    classpFormat(out, 5, arg2);
  } else {
    bracketFormat(out, this);
  }
}
PlusOp::PlusOp(ParseState parseState, Expression* arg1, AttributeMap& keyword_args)
    : Invocation(parseState, arg1, keyword_args) {
}

void PlusOp::printMembers(ostream& out) {
  Invocation::printMembers(out);

}

void PlusOp::format(ostream& out, int precedence) {
  if (precedence <= 5) {
    classpFormat(out, 5, arg1);
    out << " ";
    out << "+";
    out << " ";
    classpFormat(out, 6, arg2);
  } else {
    bracketFormat(out, this);
  }
}
MinusOp::MinusOp(ParseState parseState, Expression* arg1, AttributeMap& keyword_args)
    : Invocation(parseState, arg1, keyword_args) {
}

void MinusOp::printMembers(ostream& out) {
  Invocation::printMembers(out);

}

void MinusOp::format(ostream& out, int precedence) {
  if (precedence <= 5) {
    classpFormat(out, 5, arg1);
    out << " ";
    out << "-";
    out << " ";
    classpFormat(out, 6, arg2);
  } else {
    bracketFormat(out, this);
  }
}
TimesOp::TimesOp(ParseState parseState, Expression* arg1, AttributeMap& keyword_args)
    : Invocation(parseState, arg1, keyword_args) {
}

void TimesOp::printMembers(ostream& out) {
  Invocation::printMembers(out);

}

void TimesOp::format(ostream& out, int precedence) {
  if (precedence <= 6) {
    classpFormat(out, 6, arg1);
    out << " ";
    out << "*";
    out << " ";
    classpFormat(out, 7, arg2);
  } else {
    bracketFormat(out, this);
  }
}
DivideOp::DivideOp(ParseState parseState, Expression* arg1, AttributeMap& keyword_args)
    : Invocation(parseState, arg1, keyword_args) {
}

void DivideOp::printMembers(ostream& out) {
  Invocation::printMembers(out);

}

void DivideOp::format(ostream& out, int precedence) {
  if (precedence <= 6) {
    classpFormat(out, 6, arg1);
    out << " ";
    out << "/";
    out << " ";
    classpFormat(out, 7, arg2);
  } else {
    bracketFormat(out, this);
  }
}
NegateOp::NegateOp(ParseState parseState, Expression* arg1, AttributeMap& keyword_args)
    : Invocation(parseState, arg1, keyword_args) {
}

void NegateOp::printMembers(ostream& out) {
  Invocation::printMembers(out);

}

void NegateOp::format(ostream& out, int precedence) {
  if (precedence <= 7) {
    out << "-";
    out << " ";
    classpFormat(out, 8, arg1);
  } else {
    bracketFormat(out, this);
  }
}
FunctionCall::FunctionCall(ParseState parseState, identifier functionName, vector<Expression*> args)
    : Expression(parseState)
    , functionName(functionName)
    , args(args) {
}

void FunctionCall::printMembers(ostream& out) {
  Expression::printMembers(out);

  out << " functionName:";
  classpPrint(out, functionName);
  out << " args:";
  classpPrint(out, args);
}

void FunctionCall::format(ostream& out, int precedence) {
  classpFormat(out, 0, functionName);
  out << " ";
  out << "(";
  out << " ";
  for (size_t i = 0; i < args.size(); i++) {
    if (i > 0) {
      out << ",";
    }
    classpFormat(out, 0, args[i]);
  }
  out << " ";
  out << ")";
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
  num_errors += ParseSample<Expression>(R"#A#(a + 2)#A#", R"#A#((PlusOp arg1:(Variable id:a) arg2:(IntegerLiteral val:2)arg3[not defined]))#A#");
  num_errors += ParseSample<Expression>(R"#A#(a*(2+3=4) < 5)#A#", R"#A#((LtOp arg1:(TimesOp arg1:(Variable id:a) arg2:(EqOp arg1:(PlusOp arg1:(IntegerLiteral val:2) arg2:(IntegerLiteral val:3)arg3[not defined]) arg2:(IntegerLiteral val:4)arg3[not defined])arg3[not defined]) arg2:(IntegerLiteral val:5)arg3[not defined]))#A#");
  num_errors += ParseSample<Expression>(R"#A#(1 + case 1 when 1 then 1 else 2 + a end)#A#", R"#A#((PlusOp arg1:(IntegerLiteral val:1) arg2:(CaseExpression testExpr:(IntegerLiteral val:1) conditions:[(IntegerLiteral val:1)] results:[(IntegerLiteral val:1)] elseResult:(PlusOp arg1:(IntegerLiteral val:2) arg2:(Variable id:a)arg3[not defined]))arg3[not defined]))#A#");
/* END SAMPLES */
  std::cout << "Errors: " << num_errors << "\n";
  return num_errors;
}

#endif  // PARSER_TEST


}  // namespace expr1

#ifdef PARSER_TEST

#include <fstream>
#include <iostream>

using std::istream;
using std::ifstream;

const char usage[] = "usage: expr1 [input-file | --samples]\n";

int main(int argc, char** argv) {
  if (argc > 2) {
    std::cerr << usage;
    exit(1);
  }
  if (argc == 2 && std::string(argv[1]) == "--samples") {
    if (expr1::ParseSamples() > 0) exit(1);
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
    expr1::ParseAndPrint(input, std::cout);
  }
  return 0;
}

#endif  // PARSER_TEST

/* END PARSER */
