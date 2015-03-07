/*
 * This file is a part of the Classp parser, formatter, and AST generator.
 * Description: Lexical analyzer for the Classp language.
 *
 * Copyright 2015 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/
%{

#define CLASSP_LEX_CC_

#include <string>

#include "stringprintf.h"
#include "lexer-base.h"
#include "parser-base.h"

#include "classp.yacc.hh"

using classp::yyParser;

#undef YY_DECL
#define YY_DECL \
  yyParser::symbol_type classp::LexerBase::NextToken(classp::ParserBase* parser)

typedef yyParser::token token;

#define yylloc location_
#define yyterminate() return yyParser::make_TOK_EOF(yylloc)
#define YY_USER_ACTION yylloc.columns(yyleng);

%}

/*
   Lexer options.  These have been carefully tuned, don't change without
   careful analysis.  batch and full are for speed. noyywrap is because we only
   use one source file (and also for speed).  c++ and yyclass are because the
   generated C++ code is much easier to use and adapt than C code.  8bit is to
   be able to analyze UTF-8 strings.
*/
%option 8bit
%option batch
%option c++
%option full
%option noyywrap
%option yyclass="classp::LexerBase"

/*
   Start conditions used for parsing strings and units.  Note that they are all
   exclusive.
*/
%x dquote_string
%x squote_multiline
%x squote_singleline
%x unit
%x blank
%x comment
%x comment2

EOL    \r\n|\n

EXP    [eE]
SIGN    [+-]

BDIGIT    [01]
ODIGIT    [0-7]
DDIGIT    [[:digit:]]
XDIGIT    [[:xdigit:]]

ALNUM    [[:alnum:]_]
ALPHA    [[:alpha:]_]
BLANK    [[:blank:]\t]

DDIGITS  {DDIGIT}(_?{DDIGIT})*
EXPONENT {EXP}{SIGN}?{DDIGITS}

%%

%{

  // Used to factor out the conditions that deal with escape sequences in
  // double-quote delimited strings.
  bool multiline = false;
  yylloc.step();
%}

<INITIAL><<EOF>>  {
    return yyParser::make_TOK_EOF(yylloc);
  }

  /* All the keywords and operators below */
&&  {
    return yyParser::make_TOK_AND(yylloc);
  }

\|  {
    return yyParser::make_TOK_BAR(yylloc);
}

:  {
    return yyParser::make_TOK_COLON(yylloc);
  }

,  {
    return yyParser::make_TOK_COMMA(yylloc);
  }
\$\$ {
    return yyParser::make_TOK_DOLLARDOLLAR(yylloc);
  }
\.\.  {
    return yyParser::make_TOK_DOTDOT(yylloc);
  }

==  {
    return yyParser::make_TOK_EQL(yylloc);
  }

=  {
    return yyParser::make_TOK_EQUAL(yylloc);
  }

false  {
    return yyParser::make_TOK_FALSE(yylloc);
  }

>=  {
    return yyParser::make_TOK_GEQ(yylloc);
  }

>  {
    return yyParser::make_TOK_GTR(yylloc);
  }

\{  {
    return yyParser::make_TOK_LBRACE(yylloc);
  }

\[  {
    return yyParser::make_TOK_LBRACK(yylloc);
  }

\<=  {
    return yyParser::make_TOK_LEQ(yylloc);
  }

\(  {
    return yyParser::make_TOK_LPAREN(yylloc);
  }

\<\<  {
    return yyParser::make_TOK_LSHIFT(yylloc);
  }

\<  {
    return yyParser::make_TOK_LSS(yylloc);
  }

-  {
    return yyParser::make_TOK_MINUS(yylloc);
  }

!=  {
    return yyParser::make_TOK_NEQ(yylloc);
  }

!  {
    return yyParser::make_TOK_NOT(yylloc);
  }

null  {
    return yyParser::make_TOK_NULL(yylloc);
  }

\|\|  {
    return yyParser::make_TOK_OR(yylloc);
  }

%  {
    return yyParser::make_TOK_PERCENT(yylloc);
  }

\.  {
    return yyParser::make_TOK_PERIOD(yylloc);
  }

\+  {
    return yyParser::make_TOK_PLUS(yylloc);
  }

\}  {
    return yyParser::make_TOK_RBRACE(yylloc);
  }

\]  {
    return yyParser::make_TOK_RBRACK(yylloc);
  }

\-\> {
    return yyParser::make_TOK_RIGHTARROW(yylloc);
  }
\)  {
    return yyParser::make_TOK_RPAREN(yylloc);
  }

\?  {
    return yyParser::make_TOK_QUESTION(yylloc);
}

>>  {
    return yyParser::make_TOK_RSHIFT(yylloc);
  }

\;  {
    return yyParser::make_TOK_SEMICOLON(yylloc);
  }

\/  {
    return yyParser::make_TOK_SLASH(yylloc);
  }

\*  {
    return yyParser::make_TOK_STAR(yylloc);
  }

true  {
    return yyParser::make_TOK_TRUE(yylloc);
  }

class  {
    return yyParser::make_TOK_CLASS(yylloc);
  }

optional  {
    return yyParser::make_TOK_OPTIONAL(yylloc);
  }

sample  {
    return yyParser::make_TOK_SAMPLE(yylloc);
  }

syntax  {
    return yyParser::make_TOK_SYNTAX(yylloc);
  }

default  {
    return yyParser::make_TOK_DEFAULT(yylloc);
  }

<comment2>{
  \*\/  {
    BEGIN(INITIAL);
    //  delete // yylval->string_value;
  }

  <<EOF>>  {
    BEGIN(INITIAL);
    //  delete yylval->string_value;
    // ProcessEOF(yylloc);
  }

  {EOL}  {
    yy_push_state(blank);
    yylloc.step();
    // yylval->string_value->append(YYText());
  }

  {BLANK}+  {
    yy_push_state(blank);
    yylloc.step();
    // yylval->string_value->append(YYText());
  }

  .  {
    yylloc.step();
    // yylval->string_value->append(YYText());
  }

 }

  /* Comment. */
\/\*  {
    BEGIN(comment2);
    yylloc.step();
    // yylval->string_value = new string(YYText());
  }

<comment>{
  {EOL}  {
    BEGIN(INITIAL);
    UnputAndResetLocation('\n');
    //  delete // yylval->string_value;
  }

  <<EOF>>  {
    BEGIN(INITIAL);
    //  delete // yylval->string_value;
    // ProcessEOF(yylloc);
  }

  {BLANK}+  {
    yy_push_state(blank);
    yylloc.step();
    // yylval->string_value->append(YYText());
  }

  .  {
    yylloc.step();
    // yylval->string_value->append(YYText());
  }

 }

  /* Comment. */
\/\/  {
    BEGIN(comment);
    yylloc.step();
    // yylval->string_value = new string(YYText());
  }

<blank>{
  {EOL}  {
    yy_pop_state();
    // ProcessBlankEOL(yylloc);
    UnputAndResetLocation('\n');
  }

  <<EOF>>  {
    yy_pop_state();
    // ProcessBlankEOF(yylloc);
  }

  .  {
    yy_pop_state();
    UnputAndResetLocation(YYText()[0]);
  }
}
  /* Whitespace. */
{BLANK}+  {
    yy_push_state(blank);
    yylloc.step();
  }

  /* End of line. */
{EOL}  {
    // ProcessEOL(yylloc);
    yylloc.lines(1);
    yylloc.step();
}

  /* Float literal, e.g., 3.14e-12 */
{DDIGITS}\.{DDIGITS}{EXPONENT}  {
    return yyParser::make_TOK_FLOAT_LITERAL(atof(YYText()), yylloc);
  }

  /* Float literal, e.g., 3.14 */
{DDIGITS}\.{DDIGITS}  {
    return yyParser::make_TOK_FLOAT_LITERAL(atof(YYText()), yylloc);
  }

  /* Float literal, e.g., 314.e-12
     This rule has precedence over the next one. */
{DDIGITS}\.{EXPONENT}  {
    return yyParser::make_TOK_FLOAT_LITERAL(atof(YYText()), yylloc);
  }

  /* Float literal, e.g., 314.
     Note that we need a trailing context to deal with ranges. */
{DDIGITS}\./[^.] {
    return yyParser::make_TOK_FLOAT_LITERAL(atof(YYText()), yylloc);
  }

  /* Float literal, e.g., 314e-12 */
{DDIGITS}{EXPONENT}  {
    return yyParser::make_TOK_FLOAT_LITERAL(atof(YYText()), yylloc);
  }

  /* Float literal, e.g., .314e-12 */
\.{DDIGITS}{EXPONENT}  {
    return yyParser::make_TOK_FLOAT_LITERAL(atof(YYText()), yylloc);
  }

  /* Float literal, e.g., .314 or .314e-12 */
\.{DDIGITS}  {
    return yyParser::make_TOK_FLOAT_LITERAL(atof(YYText()), yylloc);
  }

  /* Decimal integer literal */
{DDIGITS}  {
    return yyParser::make_TOK_INTEGER_LITERAL(strtol(YYText(), nullptr, 10), yylloc);
  }

  /* Identifier. */
{ALPHA}(-*{ALNUM})*  {
    return yyParser::make_TOK_IDENTIFIER(YYText(), yylloc);
  }

  /* Backquoted identifier (may be empty). */
`[^`[:cntrl:]]*`  {
    return yyParser::make_TOK_IDENTIFIER(string(YYText() + 1, yyleng - 2), yylloc);
  }


  /* Multi-line quote. */
@@'  {
    current_token_.str("");
    BEGIN(squote_multiline);
}

  /* Single-quote delimited string. */
'  {
    current_token_.str("");
    BEGIN(squote_singleline);
}

  /* Double-quote delimited string, may have escape sequences. */
\"  {
    current_token_.str("");
    multiline = false;
    BEGIN(dquote_string);
  }

  /* Any other character. */
.  {
    LexerError(StringPrintf("unexpected character: 0x%02x", YYText()[0]).c_str());
    return yyParser::make_TOK_IDENTIFIER("***ERROR***", yylloc);
  }

<squote_multiline>{

  '@@ {
      BEGIN(INITIAL);
      return yyParser::make_TOK_SINGLE_QUOTED_STRING_LITERAL(current_token_.str(), yylloc);
    }

  ''  {
       current_token_ << "'";
    }

  {EOL}  {
      yylloc.lines(1);
      current_token_ << "\n";
    }

  [[:cntrl:]]  {
      LexerError(StringPrintf("unexpected character: 0x%02x",
                              YYText()[0]).c_str());
    }

  <<EOF>>  {
      BEGIN(INITIAL);
      LexerError(StringPrintf("string not terminated: '%s'",
                              current_token_.str().c_str()).c_str());
      return yyParser::make_TOK_SINGLE_QUOTED_STRING_LITERAL(current_token_.str(), yylloc);
    }

  .  {
      current_token_ << YYText();
    }

  }

<squote_singleline>{

  ''  {
       current_token_ << '\'';
    }

  \'  {
      BEGIN(INITIAL);
      return yyParser::make_TOK_SINGLE_QUOTED_STRING_LITERAL(current_token_.str(), yylloc);
    }

  {EOL}  {
      BEGIN(INITIAL);
      // ProcessEOL(yylloc);
      yylloc.lines(1);
      LexerError(StringPrintf("string not terminated: '%s'",
                              current_token_.str().c_str()).c_str());
      return yyParser::make_TOK_SINGLE_QUOTED_STRING_LITERAL(current_token_.str(), yylloc);
    }

  [[:cntrl:]]  {
      LexerError(StringPrintf("unexpected character: 0x%02x",
                              YYText()[0]).c_str());
    }

  <<EOF>>  {
      BEGIN(INITIAL);
      LexerError(StringPrintf("string not terminated: '%s'",
                              current_token_.str().c_str()).c_str());
      return yyParser::make_TOK_SINGLE_QUOTED_STRING_LITERAL(current_token_.str(), yylloc);
    }

  .  {
      current_token_ << YYText();
    }
  }

  /* Any double-quote delimited string may contain escape sequences.  The
     boolean multiline specifies whether this is a multiline or a single-line
     string. */
<dquote_string>{

  \"  {
      if (multiline) {
        current_token_ << '\"';
      } else {
        BEGIN(INITIAL);
        return yyParser::make_TOK_DOUBLE_QUOTED_STRING_LITERAL(current_token_.str(), yylloc);
      }
    }

  \\a  {
      current_token_ << '\a';
    }

  \\b  {
      current_token_ << '\b';
    }

  \\f  {
      current_token_ << '\f';
    }

  \\[n\n]  {
      current_token_ << '\n';
    }

  \\r  {
      current_token_ << '\r';
    }

  \\t  {
      current_token_ << '\t';
    }

  \\v  {
      current_token_ << '\v';
    }

  \\{ODIGIT}{1,3}|\\x{XDIGIT}{1,2}  {
      const bool is_hex = YYText()[1] == 'x';
      const int base = is_hex ? 16 : 8;
      const char* start = is_hex ? YYText() + 2 : YYText() + 1;
      const int64_t b = strtoll(start, NULL, base);
      if (b < 0 || b > 0xFF) {
        LexerError(StringPrintf("byte value 0x%lx out of range", b).c_str());
      }
      current_token_ << b;
    }

  \\.  {
      current_token_ << &YYText()[yyleng - 1];
    }

  {EOL}  {
      // ProcessEOL(yylloc);
      yylloc.lines(1);
      if (multiline) {
        // yylval->string_value->append("\n");
      } else {
        BEGIN(INITIAL);
        LexerError(StringPrintf("string not terminated: \"%s\"",
                                current_token_.str().c_str()).c_str());
        return yyParser::make_TOK_DOUBLE_QUOTED_STRING_LITERAL(current_token_.str(), yylloc);
      }
    }

  [[:cntrl:]]  {
      LexerError(StringPrintf("unexpected character: 0x%02x",
                              YYText()[0]).c_str());
    }

  <<EOF>>  {
      BEGIN(INITIAL);
      LexerError(StringPrintf("string not terminated: \"%s\"",
                              current_token_.str().c_str()).c_str());
      return yyParser::make_TOK_DOUBLE_QUOTED_STRING_LITERAL(current_token_.str(), yylloc);
    }

  .  {
      current_token_ << YYText();
    }

  }

%%
