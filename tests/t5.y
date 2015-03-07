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

namespace t5 {
class ParseDriver;
class AstNode;
/* BEGIN FORWARD DECLARATIONS */
class Expression;
class Query;
/* END FORWARD DECLARATIONS */
}

}

%require "3.0.2"
%defines
%define api.value.type variant
%define "parser_class_name" {YYParser}
%error-verbose
%lex-param {t5::ParseDriver* parser}
%locations
%define api.namespace {t5}
%parse-param {t5::ParseDriver* parser}
%parse-param {t5::AstNode** result}
%define api.token.constructor
%skeleton "lalr1.cc"

%initial-action {
}

%token TOK_EOF 0              "end of file"

/* BEGIN GENERATED TOKEN DECLARATIONS */
%token TOK_COMMA "`,`"
%token WORD_BY "`BY`"
%token WORD_FROM "`FROM`"
%token WORD_GROUP "`GROUP`"
%token WORD_SELECT "`SELECT`"
%token WORD_WHERE "`WHERE`"
/* END GENERATED TOKEN DECLARATIONS */

%token TOK_BOOL   "boolean literal"
%token TOK_IDENTIFIER         "identifier"
%token TOK_INT64    "integer literal"
%token TOK_STRING_LITERAL     "string literal"


/* BEGIN NONTERMINAL TYPES */
%type <Query*> class_Query
%type <Expression*> class_Expression
%type <AttributeMap> alt_Query__1
%type <AttributeMap> alt_Query__2
%type <AttributeMap> alt_Query__3
%type <vector<Expression*>> array1_Expression__1
%type <vector<Expression*>> array1_Expression__2
%type <vector<Expression*>> array1_Expression__3
/* END NONTERMINAL TYPES */

%type <bool> TOK_BOOL
%type <int64_t> TOK_INT64
%type <string>  TOK_IDENTIFIER
%type <string>  TOK_STRING_LITERAL

%code {

#include "t5.yacc.hh"
#include "t5.h"

namespace t5 {

YYParser::symbol_type yylex(ParseDriver* parser);

}  // namespace t5

}


%start start

%%

start
/* BEGIN PARSEABLE */
  : class_Query TOK_EOF { *result = $1; }
/* END PARSEABLE */
  ;

/* BEGIN PRODUCTIONS */
class_Query
  :  WORD_SELECT  array1_Expression__1  alt_Query__1  alt_Query__2  alt_Query__3 {
      AttributeMap keywords = $3;
      keywords.Merge($4);
      keywords.Merge($5);
      $$ = new Query(@$, $2, keywords); }
  ;

array1_Expression__1
  : class_Expression { $$ = vector<Expression*>(); $$.emplace_back($1); }
  | array1_Expression__1 TOK_COMMA class_Expression { $$ = $1; $$.emplace_back($3); }
  ;

array1_Expression__2
  : class_Expression { $$ = vector<Expression*>(); $$.emplace_back($1); }
  | array1_Expression__2 TOK_COMMA class_Expression { $$ = $1; $$.emplace_back($3); }
  ;

alt_Query__1
  :  WORD_FROM  array1_Expression__2 {
    $$ = AttributeMap();
    $$.Add("from_list", $2); }
  |  {
    $$ = AttributeMap(); }
  ;

alt_Query__2
  :  WORD_WHERE class_Expression {
    $$ = AttributeMap();
    $$.Add("where_clause", $2); }
  |  {
    $$ = AttributeMap(); }
  ;

array1_Expression__3
  : class_Expression { $$ = vector<Expression*>(); $$.emplace_back($1); }
  | array1_Expression__3 TOK_COMMA class_Expression { $$ = $1; $$.emplace_back($3); }
  ;

alt_Query__3
  :  WORD_GROUP WORD_BY  array1_Expression__3 {
    $$ = AttributeMap();
    $$.Add("group_by", $3); }
  |  {
    $$ = AttributeMap(); }
  ;

class_Expression
  : TOK_IDENTIFIER {
      $$ = new Expression(@$, $1); }
  ;

/* END PRODUCTIONS */

%%

#include <sstream>

namespace t5 {

using std::istream;
using std::ostream;
using std::stringstream;

AstNode* parse(istream& input, ostream& errors);

/* BEGIN METHOD DEFINITIONS */
Query::Query(ParseState parseState, vector<Expression*> select_list, AttributeMap& keyword_args)
    : AstNode(parseState)
    , select_list(select_list) {
  keyword_args.Take("from_list", from_list);
  has_where_clause = keyword_args.Take("where_clause", where_clause);
  keyword_args.Take("group_by", group_by);
}

Query* Query::parse(istream& input, ostream& errors) {
  return dynamic_cast<Query*>(t5::parse(input, errors));
}
void Query::printMembers(ostream& out) {
  out << " select_list:";
  classpPrint(out, select_list);
  out << " from_list:";
  classpPrint(out, from_list);
  if (has_where_clause) {
    out << " where_clause:";
    classpPrint(out, where_clause);
  } else {
    out << "where_clause[not defined]";
    }
  out << " group_by:";
  classpPrint(out, group_by);
}

void Query::format(ostream& out, int precedence) {
  out << "SELECT";
  out << " ";
  for (size_t i = 0; i < select_list.size(); i++) {
    if (i > 0) {
      out << ",";
    }
    classpFormat(out, 0, select_list[i]);
  }
  out << " ";
  if ((!from_list.empty())) {
    out << "FROM";
    out << " ";
    for (size_t i = 0; i < from_list.size(); i++) {
      if (i > 0) {
        out << ",";
      }
      classpFormat(out, 0, from_list[i]);
    }
  }
  out << " ";
  if (has_where_clause) {
    out << "WHERE";
    out << " ";
    classpFormat(out, 0, where_clause);
  }
  out << " ";
  if ((!group_by.empty())) {
    out << "GROUP";
    out << " ";
    out << "BY";
    out << " ";
    for (size_t i = 0; i < group_by.size(); i++) {
      if (i > 0) {
        out << ",";
      }
      classpFormat(out, 0, group_by[i]);
    }
  }
}
Expression::Expression(ParseState parseState, identifier id)
    : AstNode(parseState)
    , id(id) {
}

void Expression::printMembers(ostream& out) {
  out << " id:";
  classpPrint(out, id);
}

void Expression::format(ostream& out, int precedence) {
  classpFormat(out, 0, id);
}
/* END METHOD DEFINITIONS */

#ifdef PARSER_TEST

void ParseAndPrint(istream& input, ostream& out) {
/* BEGIN PARSE PARSEABLE */
  AstNode* result = Query::parse(input, out);
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
  num_errors += ParseSample<Query>(R"#A#(SELECT a, b FROM t1, t2)#A#", R"#A#((Query select_list:[(Expression id:a), (Expression id:b)] from_list:[(Expression id:t1), (Expression id:t2)]where_clause[not defined] group_by:[]))#A#");
/* END SAMPLES */
  std::cout << "Errors: " << num_errors << "\n";
  return num_errors;
}

#endif  // PARSER_TEST


}  // namespace t5

#ifdef PARSER_TEST

#include <fstream>
#include <iostream>
#include <memory>

using std::istream;
using std::ifstream;
using std::unique_ptr;

const char usage[] = "usage: t5 [input-file | --samples]\n";

int main(int argc, char** argv) {
  if (argc > 2) {
    std::cerr << usage;
    exit(1);
  }
  if (argc == 2 && std::string(argv[1]) == "--samples") {
    if (t5::ParseSamples() > 0) exit(1);
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
    t5::ParseAndPrint(input, std::cout);
  }
  return 0;
}

#endif  // PARSER_TEST

/* END PARSER */
