@# This file is a part of the Classp parser, formatter, and AST generator.
@# Description: Skeleton flex file used to generate lexers.
@#
@# Copyright 2015 Google Inc.
@#
@# Licensed under the Apache License, Version 2.0 (the "License");
@# you may not use this file except in compliance with the License.
@# You may obtain a copy of the License at
@#
@#     http://www.apache.org/licenses/LICENSE-2.0
@#
@# Unless required by applicable law or agreed to in writing, software
@# distributed under the License is distributed on an "AS IS" BASIS,
@# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
@# See the License for the specific language governing permissions and
@# limitations under the License.
@#
/* BEGIN LEXER */
%{

#define @ModuleNameUpper()@_LEX_CC_

#include <cctype>
#include <memory>
#include <string>
#include <iostream>
#include <sstream>

#include "@ModuleName()@.yacc.hh"

namespace @NamespaceName()@ {

class AstNode;

// ParseDriver initiates a parse and contains the state data used during a
// single parsing session (that is, during the parse of one file or string).
class ParseDriver : public yyFlexLexer {
 public:
  ParseDriver(std::istream& source, std::ostream& errors);
  ~ParseDriver() override;

  // A function used by the lexer to report errors.
  void Error(const YYParser::location_type& loc, const std::string& msg);
  void Error(const std::string& msg);
  void Fatal(const YYParser::location_type& loc, const std::string& msg);
  void LexerError(const char* msg) override;
  void LexerError(const string& msg) { LexerError(msg.c_str()); }

  // Flex-generated function to get the next token from input
  virtual YYParser::symbol_type NextToken(ParseDriver* parser);

  // Unput a char and reset location.
  virtual void Unput(char c);

  void set_initial_token(YYParser::token_type token) { initial_token_ = token; }

  void consume_initial_token(YYParser::token_type* token) {
    *token = initial_token_;
    initial_token_ = YYParser::token::TOK_EOF;
  }

  int numErrors() { return num_errors_; }

 protected:
  const YYParser::location_type* location() { return &location_; }

 private:
  std::istream* source_;  // Not owned.
  std::ostream& errors_;
  int num_errors_ = 0;
  YYParser::location_type location_;
  std::unique_ptr<std::string> file_name_;
  YYParser::token_type initial_token_;
  std::stringstream current_token_;  // a string token being built up

  // Disallow copy and assign.
  ParseDriver(const ParseDriver&) = delete;
  void operator=(const ParseDriver&) = delete;
};

}  // namespace @NamespaceName()@

using @NamespaceName()@::YYParser;

#undef YY_DECL
#define YY_DECL \
  YYParser::symbol_type @NamespaceName()@::ParseDriver::NextToken(@NamespaceName()@::ParseDriver* parser)

typedef YYParser::token token;

#define yylloc location_
#define yyterminate() return YYParser::make_TOK_EOF(yylloc)
#define YY_USER_ACTION yylloc.columns(yyleng);

%}

%option 8bit
%option batch
%option c++
%option full
%option noyywrap
%option yyclass="@NamespaceName()@::ParseDriver"

%x string_literal
%x blank
%x comment

EOL    \r\n|\n
DDIGIT [[:digit:]]
ALNUM  [[:alnum:]_]
ALPHA  [[:alpha:]_]
BLANK  [[:blank:]\t]

DDIGITS  {DDIGIT}(_?{DDIGIT})*
EXPONENT {EXP}{SIGN}?{DDIGITS}

%%

%{
  yylloc.step();
%}

false {
    return YYParser::make_TOK_BOOL(false, yylloc);
}

true {
    return YYParser::make_TOK_BOOL(true, yylloc);
}

  /* BEGIN GENERATED TOKENS */
  @@PrintTokenLexers(out);@@
  /* END GENERATED TOKENS */

<INITIAL><<EOF>>  {
    return YYParser::make_TOK_EOF(yylloc);
  }

'  {
    current_token_.str("");
    BEGIN(string_literal);
}

\/\/ {
    BEGIN(comment);
}

<string_literal>{

  ''  {
       current_token_ << '\'';
    }

  \'  {
      BEGIN(INITIAL);
      return YYParser::make_TOK_STRING_LITERAL(current_token_.str(), yylloc);
    }

  {EOL}  {
      BEGIN(INITIAL);
      yylloc.lines(1);
      LexerError("string not terminated: '" + current_token_.str() + "'");
    }

  [[:cntrl:]]  {
      LexerError("unexpected character: " + std::to_string(YYText()[0]));
    }

  <<EOF>>  {
      BEGIN(INITIAL);
      LexerError("string not terminated: '" + current_token_.str() + "'");
    }

  .  {
      current_token_ << YYText();
    }
  }

<comment>{
  {EOL}  {
    BEGIN(INITIAL);
    Unput('\n');
  }
}

{BLANK}+  {
  }

  /* integer literal */
{DDIGITS}  {
    return YYParser::make_TOK_INT64(strtol(YYText(), nullptr, 10), yylloc);
  }

  /* identifier */
{ALPHA}(-*{ALNUM})*  {
    return YYParser::make_TOK_IDENTIFIER(YYText(), yylloc);
  }

  /* Any other character. */
.  {
    if (isprint(YYText()[0])) {
      LexerError(string("unexpected character: '") + YYText() + "'");
    } else {
      LexerError("unexpected character: " + std::to_string(YYText()[0]));
    }
  }

%%

namespace @NamespaceName()@ {

using std::istream;
using std::ostream;

class FatalParseError {};

AstNode* parse(istream& input, ostream& errors) {
  ParseDriver parser(input, errors);
  AstNode* result = nullptr;
  YYParser yyparser(&parser, &result);
  int ret_val = 1;
  try { ret_val = yyparser.parse(); }
  catch (FatalParseError) {}
  if (ret_val || parser.numErrors() > 0) {
    errors << parser.numErrors() << " errors.\n";
    return nullptr;
  }
  return result;
}

YYParser::symbol_type yylex(ParseDriver* parser) {
  return parser->NextToken(parser);
}

void YYParser::error(const location_type& loc, const std::string& msg) {
  parser->Error(loc, msg);
}

ParseDriver::ParseDriver(std::istream& source, std::ostream& errors)
    : yyFlexLexer(&source)
    , source_(&source)
    , errors_(errors)
    , initial_token_(YYParser::token::TOK_EOF) {}

ParseDriver::~ParseDriver() {
  // This should be done by the parent, but it uses delete, not delete[]...
  delete[] yy_state_buf;
  yy_state_buf = NULL;

  if (yy_start_stack != NULL) {
    free(yy_start_stack);
    yy_start_stack = NULL;
  }
}

void ParseDriver::Error(
    const YYParser::location_type& loc, const std::string& msg) {
  errors_ << "ERROR: " << loc << ":" << msg << std::endl;
  num_errors_++;
}

void ParseDriver::Error(const std::string& msg) {
  errors_ << "ERROR: " << msg << std::endl;
  num_errors_++;
}

void ParseDriver::Fatal(
    const YYParser::location_type& loc, const std::string& msg) {
  Error(loc, msg);
  throw FatalParseError();
}

void ParseDriver::LexerError(const char* msg) {
  Fatal(location_, msg);
}

void ParseDriver::Unput(char c) {
  assert(location_.end.column > 0);
  location_.end.column--;
  yyunput(c, yytext);
}

}  // namespace @NamespaceName()@
/* END LEXER */
