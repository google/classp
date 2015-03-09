/* BEGIN PARSER */
%code requires {

#include <string>
using std::string;
#include <vector>
using std::vector;

/* BEGIN CODE PREFIX */
/* begin user-included code prefix */

  enum Associativity{AssocLeft, AssocRight, AssocNassoc};
  enum SampleCheck {ExpectSucceed, ExpectFailure, PrintResult, CompareResult};

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
class ClassDefinition;
class Declarations;
class Expression;
class Iterator;
class Iterator0OrMore;
class Iterator1OrMore;
class Pattern;
class PatternSequence;
class SampleDeclaration;
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
%token TOK_COMMA "`,`"
%token SYM__4 "`->`"
%token SYM__0 "`:`"
%token TOK_SEMICOLON "`;`"
%token SYM__2 "`[`"
%token SYM__3 "`]`"
%token WORD_assoc "`assoc`"
%token WORD_class "`class`"
%token WORD_default "`default`"
%token WORD_fail "`fail`"
%token WORD_left "`left`"
%token WORD_nassoc "`nassoc`"
%token WORD_optional "`optional`"
%token WORD_print "`print`"
%token WORD_right "`right`"
%token WORD_sample "`sample`"
%token WORD_succeed "`succeed`"
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
%type <Declarations*> class_Declarations
%type <ClassDefinition*> class_ClassDefinition
%type <AttributeDeclaration*> class_AttributeDeclaration
%type <SyntaxDeclaration*> class_SyntaxDeclaration
%type <SampleDeclaration*> class_SampleDeclaration
%type <Expression*> class_Expression
%type <Pattern*> class_Pattern
%type <AttributeMap> alt_AttributeDeclaration__1
%type <AttributeMap> alt_AttributeDeclaration__2
%type <AttributeMap> alt_AttributeDeclaration__3
%type <AttributeMap> alt_AttributeDeclaration__4
%type <AttributeMap> alt_ClassDefinition__1
%type <AttributeMap> alt_ClassDefinition__2
%type <AttributeMap> alt_SampleDeclaration__1
%type <AttributeMap> alt_SyntaxDeclaration__1
%type <AttributeMap> altiterSTAR_ClassDefinition
%type <vector<ClassDefinition*>> array0_ClassDefinition__1
%type <vector<Pattern*>> array0_Pattern__1
%type <vector<Pattern*>> array0_Pattern__2
%type <Pattern*> class_Pattern_p2
%type <Pattern*> class_Pattern_p3
%type <Pattern*> class_Pattern_p4
%type <AttributeMap> multiarrayPLUS_CasePattern
%type <Associativity> typed_Associativity__1
%type <SampleCheck> typed_SampleCheck__1
%type <bool> typed_bool__1
%type <bool> typed_bool__2
%type <bool> typed_bool__3
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
  :  array0_ClassDefinition__1 {
      $$ = new Declarations(@$, $1); }
  ;

array0_ClassDefinition__1
  : { $$ = vector<ClassDefinition*>(); }
  | array0_ClassDefinition__1  class_ClassDefinition { $$ = $1; $$.emplace_back($2); }
  ;

class_ClassDefinition
  :  WORD_class TOK_IDENTIFIER  alt_ClassDefinition__1 TOK_LBRACE  altiterSTAR_ClassDefinition TOK_RBRACE {
      AttributeMap keywords = $3;
      keywords.Merge($5);
      $$ = new ClassDefinition(@$, $2, keywords); }
  ;

alt_ClassDefinition__1
  :  SYM__0 TOK_IDENTIFIER {
    $$ = AttributeMap();
    $$.Add("parent_name", $2); }
  |  {
    $$ = AttributeMap(); }
  ;

typed_bool__1
  :  SYM__1 TOK_SEMICOLON { $$ = true; }
  ;

alt_ClassDefinition__2
  : class_AttributeDeclaration {
    $$ = AttributeMap();
    $$.Push("attributes", $1); }
  | class_SyntaxDeclaration {
    $$ = AttributeMap();
    $$.Push("syntax_decl", $1); }
  | class_SampleDeclaration {
    $$ = AttributeMap();
    $$.Push("sample_decl", $1); }
  | typed_bool__1 {
    $$ = AttributeMap();
    $$.Add("parseable", $1); }
  ;

altiterSTAR_ClassDefinition
  : { $$ = AttributeMap(); }
  | altiterSTAR_ClassDefinition  alt_ClassDefinition__2 { $$ = $1; $$.Merge($2); }
  ;

class_AttributeDeclaration
  :   alt_AttributeDeclaration__1 TOK_IDENTIFIER TOK_IDENTIFIER  alt_AttributeDeclaration__2  alt_AttributeDeclaration__4 TOK_SEMICOLON {
      AttributeMap keywords = $1;
      keywords.Merge($4);
      keywords.Merge($5);
      $$ = new AttributeDeclaration(@$, $3, $2, keywords); }
  ;

typed_bool__2
  : WORD_optional { $$ = true; }
  ;

alt_AttributeDeclaration__1
  : typed_bool__2 {
    $$ = AttributeMap();
    $$.Add("is_optional", $1); }
  |  {
    $$ = AttributeMap(); }
  ;

alt_AttributeDeclaration__3
  :  WORD_default class_Expression {
    $$ = AttributeMap();
    $$.Add("default_value", $2); }
  |  {
    $$ = AttributeMap(); }
  ;

typed_bool__3
  :  SYM__2 SYM__3 { $$ = true; }
  ;

alt_AttributeDeclaration__2
  :  alt_AttributeDeclaration__3 {
    $$ = $1; }
  | typed_bool__3 {
    $$ = AttributeMap();
    $$.Add("is_array", $1); }
  |  {
    $$ = AttributeMap(); }
  ;

alt_AttributeDeclaration__4
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
  :  TOK_IDENTIFIER TOK_LBRACE  multiarrayPLUS_CasePattern TOK_RBRACE {
      AttributeMap keywords = $3;
      $$ = new CasePattern(@$, $1, keywords); }  // CasePattern
  |  TOK_LPAREN class_Pattern TOK_RPAREN { $$ = $2; }
  ;

array0_Pattern__1
  : { $$ = vector<Pattern*>(); }
  | class_Pattern { $$ = vector<Pattern*>(); $$.emplace_back($1); }
  | array0_Pattern__1 TOK_BAR class_Pattern_p2 { $$ = $1; $$.emplace_back($3); }
  ;

array0_Pattern__2
  : { $$ = vector<Pattern*>(); }
  | array0_Pattern__2  class_Pattern_p4 { $$ = $1; $$.emplace_back($2); }
  ;

multiarrayPLUS_CasePattern
  :  class_Expression SYM__4 class_Pattern {
      $$ = AttributeMap();
      $$.Push<Expression*>("exprs", $1);
      $$.Push<Pattern*>("patterns", $3); }
  | multiarrayPLUS_CasePattern TOK_BAR class_Expression SYM__4 class_Pattern {
      $$ = $1;
      $$.Push("exprs", $3);
      $$.Push("patterns", $5); }
  ;







class_Expression
  : TOK_IDENTIFIER {
      $$ = new Expression(@$, $1); }
  ;

class_SampleDeclaration
  :  WORD_sample TOK_LPAREN TOK_STRING TOK_COMMA  alt_SampleDeclaration__1 TOK_RPAREN {
      AttributeMap keywords = $5;
      $$ = new SampleDeclaration(@$, $3, keywords); }
  ;

typed_SampleCheck__1
  : WORD_succeed { $$ = ExpectSucceed; }
  | WORD_fail { $$ = ExpectFailure; }
  | WORD_print { $$ = PrintResult; }
  |  { $$ = CompareResult; }
  ;

alt_SampleDeclaration__1
  : TOK_STRING {
    $$ = AttributeMap();
    $$.Add("expected", $1); }
  | typed_SampleCheck__1 {
    $$ = AttributeMap();
    $$.Add("check", $1); }
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
Declarations::Declarations(ParseState parseState, vector<ClassDefinition*> decl)
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
ClassDefinition::ClassDefinition(ParseState parseState, identifier class_name, AttributeMap& keyword_args)
    : AstNode(parseState)
    , class_name(class_name) {
  if (!(keyword_args.Take("parseable", parseable))) {
    parseable = false;
  }
  has_parent_name = keyword_args.Take("parent_name", parent_name);
  keyword_args.Take("attributes", attributes);
  keyword_args.Take("syntax_decl", syntax_decl);
  keyword_args.Take("sample_decl", sample_decl);
}

void ClassDefinition::printMembers(ostream& out) {
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
  out << " syntax_decl:";
  classpPrint(out, syntax_decl);
  out << " sample_decl:";
  classpPrint(out, sample_decl);
  out << " parseable:";
  classpPrint(out, parseable);
}

void ClassDefinition::format(ostream& out, int precedence) {
  out << "class";
  out << " ";
  classpFormat(out, 0, class_name);
  out << " ";
  if (has_parent_name) {
    out << ":";
    out << " ";
    classpFormat(out, 0, parent_name);
  }
  out << " ";
  out << "{";
  out << " ";
  if ((!attributes.empty())) {
    for (size_t i = 0; i < attributes.size(); i++) {
      classpFormat(out, 0, attributes[i]);
    }
  }
  if ((!syntax_decl.empty())) {
    for (size_t i = 0; i < syntax_decl.size(); i++) {
      classpFormat(out, 0, syntax_decl[i]);
    }
  }
  if ((!sample_decl.empty())) {
    for (size_t i = 0; i < sample_decl.size(); i++) {
      classpFormat(out, 0, sample_decl[i]);
    }
  }
  if (parseable != false) {
    if (parseable == true) {
      out << "%parseable";
      out << " ";
      out << ";";
    }
  }
  out << " ";
  out << "}";
}
AttributeDeclaration::AttributeDeclaration(ParseState parseState, identifier attribute_name, identifier type_name, AttributeMap& keyword_args)
    : AstNode(parseState)
    , attribute_name(attribute_name)
    , type_name(type_name) {
  if (!(keyword_args.Take("is_optional", is_optional))) {
    is_optional = false;
  }
  has_default_value = keyword_args.Take("default_value", default_value);
  if (!(keyword_args.Take("is_array", is_array))) {
    is_array = false;
  }
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
  out << " is_array:";
  classpPrint(out, is_array);
  if (has_syntax_decl) {
    out << " syntax_decl:";
    classpPrint(out, syntax_decl);
  } else {
    out << "syntax_decl[not defined]";
    }
}

void AttributeDeclaration::format(ostream& out, int precedence) {
  if (is_optional != false) {
    if (is_optional == true) {
      out << "optional";
    }
  }
  out << " ";
  classpFormat(out, 0, type_name);
  out << " ";
  classpFormat(out, 0, attribute_name);
  out << " ";
  if (has_default_value) {
    if (has_default_value) {
      out << "default";
      out << " ";
      classpFormat(out, 0, default_value);
    }
  } else if (is_array != false) {
    if (is_array == true) {
      out << "[";
      out << " ";
      out << "]";
    }
  }
  out << " ";
  if (has_syntax_decl) {
    out << "syntax";
    out << " ";
    out << "(";
    out << " ";
    classpFormat(out, 0, syntax_decl);
    out << " ";
    out << ")";
  }
  out << " ";
  out << ";";
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
  out << "syntax";
  out << " ";
  out << "(";
  out << " ";
  classpFormat(out, 0, pattern);
  out << " ";
  out << ")";
  out << " ";
  if (has_assoc) {
    out << "%";
    out << " ";
    if (!has_assoc) {
    } else if (assoc == AssocLeft) {
      out << "left";
    } else if (assoc == AssocRight) {
      out << "right";
    } else if (assoc == AssocNassoc) {
      out << "nassoc";
    } else if (assoc == AssocLeft) {
      out << "assoc";
    }
    out << " ";
    classpFormat(out, 0, precedence);
  }
}
Pattern::Pattern(ParseState parseState)
    : AstNode(parseState) {
}

void Pattern::printMembers(ostream& out) {
}

void Pattern::bracketFormat(ostream& out, AstNode* self) {
  out << "(";
  out << " ";
  classpFormat(out, 0, self);
  out << " ";
  out << ")";
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
        out << "|";
      }
      classpFormat(out, 2, alternates[i]);
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
    classpFormat(out, 3, pattern1);
    out << " ";
    out << "*";
    out << " ";
    classpFormat(out, 3, pattern2);
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
    classpFormat(out, 3, pattern1);
    out << " ";
    out << "+";
    out << " ";
    classpFormat(out, 3, pattern2);
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
      classpFormat(out, 4, list[i]);
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
  classpFormat(out, 0, attribute);
  out << " ";
  out << "{";
  out << " ";
  for (size_t i = 0; i < exprs.size(); i++) {
    if (i > 0) {
      out << "|";
    }
    classpFormat(out, 0, exprs[i]);
    out << " ";
    out << "->";
    out << " ";
    classpFormat(out, 0, patterns[i]);
  }
  out << " ";
  out << "}";
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
SampleDeclaration::SampleDeclaration(ParseState parseState, string sample_decl, AttributeMap& keyword_args)
    : AstNode(parseState)
    , sample_decl(sample_decl) {
  has_expected = keyword_args.Take("expected", expected);
  if (!(keyword_args.Take("check", check))) {
    check = ExpectSucceed;
  }
}

void SampleDeclaration::printMembers(ostream& out) {
  out << " sample_decl:";
  classpPrint(out, sample_decl);
  if (has_expected) {
    out << " expected:";
    classpPrint(out, expected);
  } else {
    out << "expected[not defined]";
    }
  out << " check:";
  classpPrint(out, check);
}

void SampleDeclaration::format(ostream& out, int precedence) {
  out << "sample";
  out << " ";
  out << "(";
  out << " ";
  classpFormat(out, 0, sample_decl);
  out << " ";
  out << ",";
  out << " ";
  if (has_expected) {
    classpFormat(out, 0, expected);
  } else if (check != ExpectSucceed) {
    if (check == ExpectSucceed) {
      out << "succeed";
    } else if (check == ExpectFailure) {
      out << "fail";
    } else if (check == PrintResult) {
      out << "print";
    } else if (check == CompareResult) {
      out << "";
    }
  } else {
    if (check == ExpectSucceed) {
      out << "succeed";
    } else if (check == ExpectFailure) {
      out << "fail";
    } else if (check == PrintResult) {
      out << "print";
    } else if (check == CompareResult) {
      out << "";
    }
  }
  out << " ";
  out << ")";
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

using std::istream;
using std::ifstream;

const char usage[] = "usage: classp_lang [input-file | --samples]\n";

int main(int argc, char** argv) {
  if (argc > 2) {
    std::cerr << usage;
    exit(1);
  }
  if (argc == 2 && std::string(argv[1]) == "--samples") {
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
