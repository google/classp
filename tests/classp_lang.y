/* BEGIN PARSER */
%code requires {

#include <string>
using std::string;
#include <vector>
using std::vector;

/* BEGIN CODE PREFIX */
/* begin user-included code prefix */
enum Associativity{AssocLeft, AssocRight, AssocNassoc};
/* end user-included code prefix */
/* END CODE PREFIX */

#include "classp.h"
using classp::AttributeMap;
typedef string identifier;

namespace classp_lang {
class ParseDriver;
class AstNode;
/* BEGIN FORWARD DECLARATIONS */
class Alternation;
class AttributeDeclaration;
class CasePattern;
class ClassDeclaration;
class Declarations;
class Expression;
class Iterator;
class Iterator0OrMore;
class Iterator1OrMore;
class Pattern;
class PatternSequence;
class SyntaxDeclaration;
/* END FORWARD DECLARATIONS */
}

}

%require "3.0.2"
%defines
%define api.value.type variant
%define "parser_class_name" {YYParser}
%error-verbose
%lex-param {classp_lang::ParseDriver* parser}
%locations
%define api.namespace {classp_lang}
%parse-param {classp_lang::ParseDriver* parser}
%parse-param {classp_lang::AstNode** result}
%define api.token.constructor
%skeleton "lalr1.cc"

%initial-action {
}

%token TOK_EOF 0              "end of file"

/* BEGIN GENERATED TOKEN DECLARATIONS */
%token TOK_PERCENT "`%`"
%token SYM__1 "`%parseable`"
%token TOK_LPAREN "`(`"
%token TOK_RPAREN "`)`"
%token TOK_STAR "`*`"
%token TOK_PLUS "`+`"
%token SYM__2 "`->`"
%token SYM__0 "`:`"
%token TOK_SEMICOLON "`;`"
%token WORD_assoc "`assoc`"
%token WORD_class "`class`"
%token WORD_default "`default`"
%token WORD_left "`left`"
%token WORD_nassoc "`nassoc`"
%token WORD_optional "`optional`"
%token WORD_right "`right`"
%token WORD_syntax "`syntax`"
%token TOK_LBRACE "`{`"
%token TOK_BAR "`|`"
%token TOK_RBRACE "`}`"
/* END GENERATED TOKEN DECLARATIONS */

%token TOK_BOOL   "boolean literal"
%token TOK_IDENTIFIER         "identifier"
%token TOK_INT64    "integer literal"
%token TOK_STRING_LITERAL     "string literal"


/* BEGIN NONTERMINAL TYPES */
%type <AttributeMap> alt_AttributeDeclaration__1
%type <AttributeMap> alt_AttributeDeclaration__2
%type <AttributeMap> alt_ClassDeclaration__1
%type <AttributeMap> alt_ClassDeclaration__2
%type <AttributeMap> alt_SyntaxDeclaration__1
%type <AttributeMap> altiterSTAR_ClassDeclaration
%type <vector<ClassDeclaration*>> array0_ClassDeclaration__1
%type <vector<Pattern*>> array0_Pattern__1
%type <vector<Pattern*>> array0_Pattern__2
%type <AttributeDeclaration*> class_AttributeDeclaration
%type <ClassDeclaration*> class_ClassDeclaration
%type <Declarations*> class_Declarations
%type <Expression*> class_Expression
%type <Pattern*> class_Pattern
%type <Pattern*> class_Pattern_p2
%type <Pattern*> class_Pattern_p3
%type <Pattern*> class_Pattern_p4
%type <SyntaxDeclaration*> class_SyntaxDeclaration
%type <AttributeMap> multiarrayPLUS_CasePattern
%type <Associativity> typed_Associativity__1
%type <bool> typed_bool__1
%type <bool> typed_bool__2
/* END NONTERMINAL TYPES */

%type <bool> TOK_BOOL
%type <int64_t> TOK_INT64
%type <string>  TOK_IDENTIFIER
%type <string>  TOK_STRING_LITERAL

%code {

#include "classp_lang.yacc.hh"
#include "classp_lang.h"

namespace classp_lang {

YYParser::symbol_type yylex(ParseDriver* parser);

}  // namespace classp_lang

}


%start start

%%

start
/* BEGIN PARSEABLE */
  : class_Declarations TOK_EOF { *result = $1; }
/* END PARSEABLE */
  ;

/* BEGIN PRODUCTIONS */
class_Declarations
  :  array0_ClassDeclaration__1 {
      $$ = new Declarations(@$, $1); }
  ;

array0_ClassDeclaration__1
  : { $$ = vector<ClassDeclaration*>(); }
  | array0_ClassDeclaration__1  class_ClassDeclaration { $$ = $1; $$.emplace_back($2); }

class_ClassDeclaration
  :  WORD_class TOK_IDENTIFIER  alt_ClassDeclaration__1 TOK_LBRACE  altiterSTAR_ClassDeclaration TOK_RBRACE {
      AttributeMap keywords = $3;
      keywords.Merge($5);
      $$ = new ClassDeclaration(@$, $2, keywords); }
  ;

alt_ClassDeclaration__1
  :  SYM__0 TOK_IDENTIFIER {
    $$ = AttributeMap();
    $$.Add("parent_name", $2); }
  |  {
    $$ = AttributeMap(); }
  ;

typed_bool__1
  :  SYM__1 TOK_SEMICOLON { $$ = true; }
  ;

alt_ClassDeclaration__2
  : class_AttributeDeclaration {
    $$ = AttributeMap();
    $$.Push("attributes", $1); }
  | class_SyntaxDeclaration {
    $$ = AttributeMap();
    $$.Add("syntax_decl", $1); }
  | typed_bool__1 {
    $$ = AttributeMap();
    $$.Add("parseable", $1); }
  ;

altiterSTAR_ClassDeclaration
  : { $$ = AttributeMap(); }
  | altiterSTAR_ClassDeclaration  alt_ClassDeclaration__2 { $$ = $1; $$->Merge($2); }
  ;

class_AttributeDeclaration
  :  typed_bool__2 TOK_IDENTIFIER TOK_IDENTIFIER  alt_AttributeDeclaration__1  alt_AttributeDeclaration__2 TOK_SEMICOLON {
      AttributeMap keywords = $4;
      keywords.Merge($5);
      keywords.Add("is_optional", $1);
      $$ = new AttributeDeclaration(@$, $3, $2, keywords); }
  ;

typed_bool__2
  : WORD_optional { $$ = true; }
  ;

alt_AttributeDeclaration__1
  :  WORD_default class_Expression {
    $$ = AttributeMap();
    $$.Add("default_value", $2); }
  |  {
    $$ = AttributeMap(); }
  ;

alt_AttributeDeclaration__2
  :  WORD_syntax TOK_LPAREN class_SyntaxDeclaration TOK_RPAREN {
    $$ = AttributeMap();
    $$.Add("syntax_decl", $3); }
  |  {
    $$ = AttributeMap(); }
  ;

class_SyntaxDeclaration
  :  WORD_syntax TOK_LPAREN class_Pattern TOK_RPAREN  alt_SyntaxDeclaration__1 {
      AttributeMap keywords = $5;
      $$ = new SyntaxDeclaration(@$, $3, keywords); }
  ;

typed_Associativity__1
  : WORD_left { $$ = AssocLeft; }
  | WORD_right { $$ = AssocRight; }
  | WORD_nassoc { $$ = AssocNassoc; }
  | WORD_assoc { $$ = AssocLeft; }
  ;

alt_SyntaxDeclaration__1
  :  TOK_PERCENT typed_Associativity__1 TOK_INT64 {
    $$ = AttributeMap();
    $$.Add("assoc", $2);
    $$.Add("precedence", $3); }
  |  {
    $$ = AttributeMap(); }
  ;

class_Pattern
  :  array0_Pattern__1 {
      $$ = new Alternation(@$, $1); }  // Alternation
  | class_Pattern_p2 { $$ = $1; }
  ;
class_Pattern_p2
  :  class_Pattern_p3 TOK_STAR class_Pattern_p3 {
      $$ = new Iterator0OrMore(@$, $1, $3); }  // Iterator0OrMore
  |  class_Pattern_p3 TOK_PLUS class_Pattern_p3 {
      $$ = new Iterator1OrMore(@$, $1, $3); }  // Iterator1OrMore
  | class_Pattern_p3 { $$ = $1; }
  ;
class_Pattern_p3
  :  array0_Pattern__2 {
      $$ = new PatternSequence(@$, $1); }  // PatternSequence
  | class_Pattern_p4 { $$ = $1; }
  ;
class_Pattern_p4
  :  TOK_LPAREN class_Pattern TOK_RPAREN { $$ = $2; }
  |  TOK_IDENTIFIER TOK_LBRACE  multiarrayPLUS_CasePattern TOK_RBRACE {
      AttributeMap keywords = $3;
      $$ = new CasePattern(@$, $1, keywords); }  // CasePattern
  ;

array0_Pattern__1
  : { $$ = vector<Pattern*>(); }
  | class_Pattern{ $$ = vector<Pattern*>(); $$.emplace_back($1); }
  | array0_Pattern__1 TOK_BAR class_Pattern { $$ = $1; $$.emplace_back($3); }

array0_Pattern__2
  : { $$ = vector<Pattern*>(); }
  | array0_Pattern__2  class_Pattern { $$ = $1; $$.emplace_back($2); }

multiarrayPLUS_CasePattern
  :  class_Expression SYM__2 class_Pattern {
      $$ = AttributeMap();
      $$.Push<Expression*>("exprs", 1);
      $$.Push<Pattern*>("patterns", 3); }
  | multiarrayPLUS_CasePattern TOK_BAR class_Expression SYM__2 class_Pattern {
      $$ = $1;
      $$.Push("exprs", $3);
      $$.Push("patterns", $5); }
  ;







class_Expression
  : TOK_IDENTIFIER {
      $$ = new Expression(@$, $1); }
  ;

/* END PRODUCTIONS */

%%

#include <sstream>

namespace classp_lang {

using std::istream;
using std::ostream;
using std::stringstream;

AstNode* parse(istream& input, ostream& errors);

/* BEGIN METHOD DEFINITIONS */
Declarations::Declarations(ParseState parseState, vector<ClassDeclaration*> decl)
    : AstNode(parseState)
    , decl(decl) {
}

Declarations* Declarations::parse(istream& input, ostream& errors) {
  return dynamic_cast<Declarations*>(classp_lang::parse(input, errors));
}
void Declarations::printMembers(ostream& out) {
  out << " decl:";
  classpPrint(out, decl);
}

void Declarations::format(ostream& out, int precedence) {
  for (size_t i = 0; i < decl.size(); i++) {
    classpFormat(out, 0, decl[i]);
  }
}
ClassDeclaration::ClassDeclaration(ParseState parseState, identifier class_name, AttributeMap& keyword_args)
    : AstNode(parseState)
    , class_name(class_name) {
  if (!(keyword_args.Take("parseable", parseable))) {
    parseable = false;
  }
  has_parent_name = keyword_args.Take("parent_name", parent_name);
  keyword_args.Take("attributes", attributes);
  has_syntax_decl = keyword_args.Take("syntax_decl", syntax_decl);
}

void ClassDeclaration::printMembers(ostream& out) {
  out << " class_name:";
  classpPrint(out, class_name);
  if (has_parent_name) {
    out << " parent_name:";
    classpPrint(out, parent_name);
  } else {
    out << "parent_name[not defined]";
    }
  out << " attributes:";
  classpPrint(out, attributes);
  if (has_syntax_decl) {
    out << " syntax_decl:";
    classpPrint(out, syntax_decl);
  } else {
    out << "syntax_decl[not defined]";
    }
  out << " parseable:";
  classpPrint(out, parseable);
}

void ClassDeclaration::format(ostream& out, int precedence) {
  out << " class ";
  classpFormat(out, 0, class_name);
  if (has_parent_name) {
    out << " : ";
    classpFormat(out, 0, parent_name);
  }
  out << " { ";
  if ((!attributes.empty())) {
    for (size_t i = 0; i < attributes.size(); i++) {
      classpFormat(out, 0, attributes[i]);
    }
  }
  if (has_syntax_decl) {
    classpFormat(out, 0, syntax_decl);
  }
  if (parseable != false) {
    if (parseable == true) {
      out << " %parseable " << "; ";
    }
  }
  out << " } ";
}
AttributeDeclaration::AttributeDeclaration(ParseState parseState, identifier attribute_name, identifier type_name, AttributeMap& keyword_args)
    : AstNode(parseState)
    , attribute_name(attribute_name)
    , type_name(type_name) {
  if (!(keyword_args.Take("is_optional", is_optional))) {
    is_optional = false;
  }
  has_default_value = keyword_args.Take("default_value", default_value);
  has_syntax_decl = keyword_args.Take("syntax_decl", syntax_decl);
}

void AttributeDeclaration::printMembers(ostream& out) {
  out << " is_optional:";
  classpPrint(out, is_optional);
  out << " attribute_name:";
  classpPrint(out, attribute_name);
  out << " type_name:";
  classpPrint(out, type_name);
  if (has_default_value) {
    out << " default_value:";
    classpPrint(out, default_value);
  } else {
    out << "default_value[not defined]";
    }
  if (has_syntax_decl) {
    out << " syntax_decl:";
    classpPrint(out, syntax_decl);
  } else {
    out << "syntax_decl[not defined]";
    }
}

void AttributeDeclaration::format(ostream& out, int precedence) {
  if (is_optional == true) {
    out << " optional ";
  }
  classpFormat(out, 0, type_name);
  classpFormat(out, 0, attribute_name);
  if (has_default_value) {
    out << " default ";
    classpFormat(out, 0, default_value);
  }
  if (has_syntax_decl) {
    out << " syntax " << "( ";
    classpFormat(out, 0, syntax_decl);
    out << " ) ";
  }
  out << " ; ";
}
SyntaxDeclaration::SyntaxDeclaration(ParseState parseState, Pattern* pattern, AttributeMap& keyword_args)
    : AstNode(parseState)
    , pattern(pattern) {
  has_assoc = keyword_args.Take("assoc", assoc);
  has_precedence = keyword_args.Take("precedence", precedence);
}

void SyntaxDeclaration::printMembers(ostream& out) {
  out << " pattern:";
  classpPrint(out, pattern);
  if (has_assoc) {
    out << " assoc:";
    classpPrint(out, assoc);
  } else {
    out << "assoc[not defined]";
    }
  if (has_precedence) {
    out << " precedence:";
    classpPrint(out, precedence);
  } else {
    out << "precedence[not defined]";
    }
}

void SyntaxDeclaration::format(ostream& out, int precedence) {
  out << " syntax " << "( ";
  classpFormat(out, 0, pattern);
  out << " ) ";
  if (has_assoc) {
    out << " % ";
    if (!has_assoc) {
    }
    } else if (assoc == AssocLeft) {
      out << " left ";
    } else if (assoc == AssocRight) {
      out << " right ";
    } else if (assoc == AssocNassoc) {
      out << " nassoc ";
    } else if (assoc == AssocLeft) {
      out << " assoc ";
    }
    classpFormat(out, 0, precedence);
  }
}
Pattern::Pattern(ParseState parseState)
    : AstNode(parseState) {
}

void Pattern::printMembers(ostream& out) {
}

void Pattern::bracketFormat(ostream& out, AstNode* self) {
  out << " ( ";
  classpFormat(out, 0, self);
  out << " ) ";
  }
Alternation::Alternation(ParseState parseState, vector<Pattern*> alternates)
    : Pattern(parseState)
    , alternates(alternates) {
}

void Alternation::printMembers(ostream& out) {
  Pattern::printMembers(out);

  out << " alternates:";
  classpPrint(out, alternates);
}

void Alternation::format(ostream& out, int precedence) {
  if (precedence <= 1) {
    for (size_t i = 0; i < alternates.size(); i++) {
      if (i > 0) {
        out << " | ";
      }
      classpFormat(out, 0, alternates[i]);
    }
  } else {
    bracketFormat(out, this);
  }
}
Iterator::Iterator(ParseState parseState, Pattern* pattern1, Pattern* pattern2)
    : Pattern(parseState)
    , pattern1(pattern1)
    , pattern2(pattern2) {
}

void Iterator::printMembers(ostream& out) {
  Pattern::printMembers(out);

  out << " pattern1:";
  classpPrint(out, pattern1);
  out << " pattern2:";
  classpPrint(out, pattern2);
}
Iterator0OrMore::Iterator0OrMore(ParseState parseState, Pattern* pattern1, Pattern* pattern2)
    : Iterator(parseState, pattern1, pattern2) {
}

void Iterator0OrMore::printMembers(ostream& out) {
  Iterator::printMembers(out);

}

void Iterator0OrMore::format(ostream& out, int precedence) {
  if (precedence <= 2) {
    classpFormat(out, 1, pattern1);
    out << " * ";
    classpFormat(out, 1, pattern2);
  } else {
    bracketFormat(out, this);
  }
}
Iterator1OrMore::Iterator1OrMore(ParseState parseState, Pattern* pattern1, Pattern* pattern2)
    : Iterator(parseState, pattern1, pattern2) {
}

void Iterator1OrMore::printMembers(ostream& out) {
  Iterator::printMembers(out);

}

void Iterator1OrMore::format(ostream& out, int precedence) {
  if (precedence <= 2) {
    classpFormat(out, 1, pattern1);
    out << " + ";
    classpFormat(out, 1, pattern2);
  } else {
    bracketFormat(out, this);
  }
}
PatternSequence::PatternSequence(ParseState parseState, vector<Pattern*> list)
    : Pattern(parseState)
    , list(list) {
}

void PatternSequence::printMembers(ostream& out) {
  Pattern::printMembers(out);

  out << " list:";
  classpPrint(out, list);
}

void PatternSequence::format(ostream& out, int precedence) {
  if (precedence <= 3) {
    for (size_t i = 0; i < list.size(); i++) {
      classpFormat(out, 2, list[i]);
    }
  } else {
    bracketFormat(out, this);
  }
}
CasePattern::CasePattern(ParseState parseState, identifier attribute, AttributeMap& keyword_args)
    : Pattern(parseState)
    , attribute(attribute) {
  keyword_args.Take("patterns", patterns);
  keyword_args.Take("exprs", exprs);
}

void CasePattern::printMembers(ostream& out) {
  Pattern::printMembers(out);

  out << " attribute:";
  classpPrint(out, attribute);
  out << " patterns:";
  classpPrint(out, patterns);
  out << " exprs:";
  classpPrint(out, exprs);
}

void CasePattern::format(ostream& out, int precedence) {
  if (precedence <= 4) {
    classpFormat(out, 0, attribute);
    out << " { ";
    for (size_t i = 0; i < exprs.size(); i++) {
      if (i > 0) {
        out << " | ";
      }
      classpFormat(out, 0, exprs[i]);
      out << " -> ";
      classpFormat(out, 0, patterns[i]);
    }
    out << " } ";
  } else {
    bracketFormat(out, this);
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
  AstNode* result = Declarations::parse(input, out);
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
  num_errors += ParseSample<Declarations>(R"#A#(class A {%parseable;int n;syntax ('+' n);sample ('+ 3');})#A#", kPrint);
/* END SAMPLES */
  std::cout << "Errors: " << num_errors << "\n";
  return num_errors;
}

#endif  // PARSER_TEST


}  // namespace classp_lang

#ifdef PARSER_TEST

#include <fstream>
#include <iostream>
#include <memory>

using std::istream;
using std::ifstream;
using std::unique_ptr;

const char usage[] = "usage: classp_lang [input-file | --samples]\n";

int main(int argc, char** argv) {
  if (argc > 2) {
    std::cerr << usage;
    exit(1);
  }
  if (std::string(argv[1]) == "--samples") {
    if (classp_lang::ParseSamples() > 0) exit(1);
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
    classp_lang::ParseAndPrint(input, std::cout);
  }
  return 0;
}

#endif  // PARSER_TEST

/* END PARSER */
