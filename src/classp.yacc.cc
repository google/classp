// A Bison parser, made by GNU Bison 3.0.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2013 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.


// First part of user declarations.

#line 37 "classp.yacc.cc" // lalr1.cc:399

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "classp.yacc.hh"

// User implementation prologue.

#line 51 "classp.yacc.cc" // lalr1.cc:407
// Unqualified %code blocks.
#line 122 "classp.y" // lalr1.cc:408


#include "parser-base.h"
#include "lexer-base.h"

namespace classp {

static inline yyParser::symbol_type yylex(ParserBase* parser) {
    return parser->lexer_->NextToken(parser);
}

}  // namespace classp


#line 68 "classp.yacc.cc" // lalr1.cc:408


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (/*CONSTCOND*/ false)
# endif


// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << std::endl;                  \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE(Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void>(0)
# define YY_STACK_PRINT()                static_cast<void>(0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyempty = true)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 45 "classp.y" // lalr1.cc:474
namespace classp {
#line 154 "classp.yacc.cc" // lalr1.cc:474

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  yyParser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              // Fall through.
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }


  /// Build a parser object.
  yyParser::yyParser (classp::ParserBase* parser_yyarg, classp::ParseTree** result_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      parser (parser_yyarg),
      result (result_yyarg)
  {}

  yyParser::~yyParser ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/



  // by_state.
  inline
  yyParser::by_state::by_state ()
    : state (empty)
  {}

  inline
  yyParser::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
  yyParser::by_state::move (by_state& that)
  {
    state = that.state;
    that.state = empty;
  }

  inline
  yyParser::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
  yyParser::symbol_number_type
  yyParser::by_state::type_get () const
  {
    return state == empty ? 0 : yystos_[state];
  }

  inline
  yyParser::stack_symbol_type::stack_symbol_type ()
  {}


  inline
  yyParser::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s, that.location)
  {
      switch (that.type_get ())
    {
      case 68: // declaration_list
      case 73: // class_body
      case 82: // features
        value.move<  vector<ParseTree*>  > (that.value);
        break;

      case 89: // syntax_case_list
        value.move< ParseTreeAltList* > (that.value);
        break;

      case 76: // opt_optional
      case 77: // opt_array
        value.move< bool > (that.value);
        break;

      case 53: // operand
      case 54: // identifier
      case 55: // numeric_literal
      case 56: // literal
      case 58: // expression
      case 59: // conjunction
      case 60: // comparison
      case 62: // simple_expression
      case 64: // term
      case 66: // factor
      case 70: // declaration
      case 74: // attribute_decl
      case 75: // opt_initializer
      case 79: // opt_syntax_decl
      case 80: // syntax_decl
      case 81: // sample_decl
      case 83: // syntax_alt_list
      case 84: // syntax_spec
      case 85: // opt_syntax_item_list
      case 86: // syntax_item_list
      case 87: // syntax_item
      case 88: // syntax_attribute
        value.move< classp::ParseTree* > (that.value);
        break;

      case 57: // string_literal
        value.move< classp::ParseTreeSymbol* > (that.value);
        break;

      case 45: // "float literal"
        value.move< double > (that.value);
        break;

      case 44: // "integer literal"
      case 61: // relational_operator
      case 63: // additive_operator
      case 65: // multiplicative_operator
      case 67: // unary_operator
        value.move< int64_t > (that.value);
        break;

      case 43: // "identifier"
      case 46: // "single-quote delimited string literal"
      case 47: // "double-quote delimited string literal"
      case 52: // opt_code_literal
      case 78: // opt_identifier
        value.move< string > (that.value);
        break;

      case 71: // parents_list
      case 72: // parents_list2
        value.move< vector<string> > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty;
  }

  inline
  yyParser::stack_symbol_type&
  yyParser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
      switch (that.type_get ())
    {
      case 68: // declaration_list
      case 73: // class_body
      case 82: // features
        value.copy<  vector<ParseTree*>  > (that.value);
        break;

      case 89: // syntax_case_list
        value.copy< ParseTreeAltList* > (that.value);
        break;

      case 76: // opt_optional
      case 77: // opt_array
        value.copy< bool > (that.value);
        break;

      case 53: // operand
      case 54: // identifier
      case 55: // numeric_literal
      case 56: // literal
      case 58: // expression
      case 59: // conjunction
      case 60: // comparison
      case 62: // simple_expression
      case 64: // term
      case 66: // factor
      case 70: // declaration
      case 74: // attribute_decl
      case 75: // opt_initializer
      case 79: // opt_syntax_decl
      case 80: // syntax_decl
      case 81: // sample_decl
      case 83: // syntax_alt_list
      case 84: // syntax_spec
      case 85: // opt_syntax_item_list
      case 86: // syntax_item_list
      case 87: // syntax_item
      case 88: // syntax_attribute
        value.copy< classp::ParseTree* > (that.value);
        break;

      case 57: // string_literal
        value.copy< classp::ParseTreeSymbol* > (that.value);
        break;

      case 45: // "float literal"
        value.copy< double > (that.value);
        break;

      case 44: // "integer literal"
      case 61: // relational_operator
      case 63: // additive_operator
      case 65: // multiplicative_operator
      case 67: // unary_operator
        value.copy< int64_t > (that.value);
        break;

      case 43: // "identifier"
      case 46: // "single-quote delimited string literal"
      case 47: // "double-quote delimited string literal"
      case 52: // opt_code_literal
      case 78: // opt_identifier
        value.copy< string > (that.value);
        break;

      case 71: // parents_list
      case 72: // parents_list2
        value.copy< vector<string> > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }


  template <typename Base>
  inline
  void
  yyParser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  yyParser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  inline
  void
  yyParser::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
  yyParser::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
  yyParser::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  yyParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  yyParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  yyParser::debug_level_type
  yyParser::debug_level () const
  {
    return yydebug_;
  }

  void
  yyParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  inline yyParser::state_type
  yyParser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  inline bool
  yyParser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  yyParser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  yyParser::parse ()
  {
    /// Whether yyla contains a lookahead.
    bool yyempty = true;

    // State.
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    // User initialization code.
    #line 52 "classp.y" // lalr1.cc:725
{
  parser->token_names_ = yytname_;
}

#line 555 "classp.yacc.cc" // lalr1.cc:725

    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, yyla);

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << std::endl;

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    // Backup.
  yybackup:

    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyempty)
      {
        YYCDEBUG << "Reading a token: ";
        try
          {
            symbol_type yylookahead (yylex (parser));
            yyla.move (yylookahead);
          }
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
        yyempty = false;
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Discard the token being shifted.
    yyempty = true;

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", yyn, yyla);
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
        switch (yyr1_[yyn])
    {
      case 68: // declaration_list
      case 73: // class_body
      case 82: // features
        yylhs.value.build<  vector<ParseTree*>  > ();
        break;

      case 89: // syntax_case_list
        yylhs.value.build< ParseTreeAltList* > ();
        break;

      case 76: // opt_optional
      case 77: // opt_array
        yylhs.value.build< bool > ();
        break;

      case 53: // operand
      case 54: // identifier
      case 55: // numeric_literal
      case 56: // literal
      case 58: // expression
      case 59: // conjunction
      case 60: // comparison
      case 62: // simple_expression
      case 64: // term
      case 66: // factor
      case 70: // declaration
      case 74: // attribute_decl
      case 75: // opt_initializer
      case 79: // opt_syntax_decl
      case 80: // syntax_decl
      case 81: // sample_decl
      case 83: // syntax_alt_list
      case 84: // syntax_spec
      case 85: // opt_syntax_item_list
      case 86: // syntax_item_list
      case 87: // syntax_item
      case 88: // syntax_attribute
        yylhs.value.build< classp::ParseTree* > ();
        break;

      case 57: // string_literal
        yylhs.value.build< classp::ParseTreeSymbol* > ();
        break;

      case 45: // "float literal"
        yylhs.value.build< double > ();
        break;

      case 44: // "integer literal"
      case 61: // relational_operator
      case 63: // additive_operator
      case 65: // multiplicative_operator
      case 67: // unary_operator
        yylhs.value.build< int64_t > ();
        break;

      case 43: // "identifier"
      case 46: // "single-quote delimited string literal"
      case 47: // "double-quote delimited string literal"
      case 52: // opt_code_literal
      case 78: // opt_identifier
        yylhs.value.build< string > ();
        break;

      case 71: // parents_list
      case 72: // parents_list2
        yylhs.value.build< vector<string> > ();
        break;

      default:
        break;
    }


      // Compute the default @$.
      {
        slice<stack_symbol_type, stack_type> slice (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, slice, yylen);
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
      try
        {
          switch (yyn)
            {
  case 2:
#line 177 "classp.y" // lalr1.cc:847
    {
      parser->AddCodePrefix(yystack_[2].value.as< string > ());
      *result = new ParseTreeArray(parser, yylhs.location, yystack_[1].value.as<  vector<ParseTree*>  > ()); }
#line 740 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 3:
#line 183 "classp.y" // lalr1.cc:847
    { yylhs.value.as< string > () = string(""); }
#line 746 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 4:
#line 184 "classp.y" // lalr1.cc:847
    { yylhs.value.as< string > () = yystack_[0].value.as< string > (); }
#line 752 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 5:
#line 188 "classp.y" // lalr1.cc:847
    { yylhs.value.as< classp::ParseTree* > () = yystack_[0].value.as< classp::ParseTree* > (); }
#line 758 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 6:
#line 189 "classp.y" // lalr1.cc:847
    { yylhs.value.as< classp::ParseTree* > () = yystack_[0].value.as< classp::ParseTree* > (); }
#line 764 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 7:
#line 190 "classp.y" // lalr1.cc:847
    { yylhs.value.as< classp::ParseTree* > () = nullptr; }
#line 770 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 8:
#line 191 "classp.y" // lalr1.cc:847
    { yylhs.value.as< classp::ParseTree* > () = yystack_[1].value.as< classp::ParseTree* > (); }
#line 776 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 9:
#line 195 "classp.y" // lalr1.cc:847
    { yylhs.value.as< classp::ParseTree* > () = new ParseTreeIdentifier(parser, yylhs.location, yystack_[0].value.as< string > ()); }
#line 782 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 10:
#line 199 "classp.y" // lalr1.cc:847
    { yylhs.value.as< classp::ParseTree* > () = new ParseTreeInteger(parser, yylhs.location, yystack_[0].value.as< int64_t > ()); }
#line 788 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 11:
#line 200 "classp.y" // lalr1.cc:847
    { yylhs.value.as< classp::ParseTree* > () = new ParseTreeFloat(parser, yylhs.location, yystack_[0].value.as< double > ()); }
#line 794 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 12:
#line 204 "classp.y" // lalr1.cc:847
    { yylhs.value.as< classp::ParseTree* > () = yystack_[0].value.as< classp::ParseTree* > (); }
#line 800 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 13:
#line 205 "classp.y" // lalr1.cc:847
    { yylhs.value.as< classp::ParseTree* > () = new ParseTreeBool(parser, yylhs.location, true); }
#line 806 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 14:
#line 206 "classp.y" // lalr1.cc:847
    { yylhs.value.as< classp::ParseTree* > () = new ParseTreeBool(parser, yylhs.location, false); }
#line 812 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 15:
#line 207 "classp.y" // lalr1.cc:847
    { yylhs.value.as< classp::ParseTree* > () = yystack_[0].value.as< classp::ParseTreeSymbol* > (); }
#line 818 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 16:
#line 211 "classp.y" // lalr1.cc:847
    {
        yylhs.value.as< classp::ParseTreeSymbol* > () = new ParseTreeSymbol(parser, yylhs.location, yystack_[0].value.as< string > ());
      }
#line 826 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 17:
#line 214 "classp.y" // lalr1.cc:847
    {
        yylhs.value.as< classp::ParseTreeSymbol* > () = new ParseTreeSymbol(parser, yylhs.location, yystack_[0].value.as< string > ());
      }
#line 834 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 18:
#line 222 "classp.y" // lalr1.cc:847
    { yylhs.value.as< classp::ParseTree* > () = yystack_[0].value.as< classp::ParseTree* > (); }
#line 840 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 19:
#line 223 "classp.y" // lalr1.cc:847
    {
        yylhs.value.as< classp::ParseTree* > () = new ParseTreeBinop(parser, yylhs.location, token::TOK_OR, yystack_[2].value.as< classp::ParseTree* > (), yystack_[0].value.as< classp::ParseTree* > ());
      }
#line 848 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 20:
#line 229 "classp.y" // lalr1.cc:847
    { yylhs.value.as< classp::ParseTree* > () = yystack_[0].value.as< classp::ParseTree* > (); }
#line 854 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 21:
#line 230 "classp.y" // lalr1.cc:847
    {
        yylhs.value.as< classp::ParseTree* > () = new ParseTreeBinop(parser, yylhs.location, token::TOK_AND, yystack_[2].value.as< classp::ParseTree* > (), yystack_[0].value.as< classp::ParseTree* > ());
      }
#line 862 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 22:
#line 236 "classp.y" // lalr1.cc:847
    { yylhs.value.as< classp::ParseTree* > () = yystack_[0].value.as< classp::ParseTree* > (); }
#line 868 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 23:
#line 237 "classp.y" // lalr1.cc:847
    {
        yylhs.value.as< classp::ParseTree* > () = new ParseTreeBinop(parser, yylhs.location, yystack_[1].value.as< int64_t > (), yystack_[2].value.as< classp::ParseTree* > (), yystack_[0].value.as< classp::ParseTree* > ());
      }
#line 876 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 24:
#line 243 "classp.y" // lalr1.cc:847
    { yylhs.value.as< int64_t > () = token::TOK_EQL; }
#line 882 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 25:
#line 244 "classp.y" // lalr1.cc:847
    { yylhs.value.as< int64_t > () = token::TOK_NEQ; }
#line 888 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 26:
#line 245 "classp.y" // lalr1.cc:847
    { yylhs.value.as< int64_t > () = token::TOK_LSS; }
#line 894 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 27:
#line 246 "classp.y" // lalr1.cc:847
    { yylhs.value.as< int64_t > () = token::TOK_LEQ; }
#line 900 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 28:
#line 247 "classp.y" // lalr1.cc:847
    { yylhs.value.as< int64_t > () = token::TOK_GTR; }
#line 906 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 29:
#line 248 "classp.y" // lalr1.cc:847
    { yylhs.value.as< int64_t > () = token::TOK_GEQ; }
#line 912 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 30:
#line 252 "classp.y" // lalr1.cc:847
    { yylhs.value.as< classp::ParseTree* > () = yystack_[0].value.as< classp::ParseTree* > (); }
#line 918 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 31:
#line 253 "classp.y" // lalr1.cc:847
    {
        yylhs.value.as< classp::ParseTree* > () = new ParseTreeBinop(parser, yylhs.location, yystack_[1].value.as< int64_t > (), yystack_[2].value.as< classp::ParseTree* > (), yystack_[0].value.as< classp::ParseTree* > ());
      }
#line 926 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 32:
#line 259 "classp.y" // lalr1.cc:847
    { yylhs.value.as< int64_t > () = token::TOK_PLUS; }
#line 932 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 33:
#line 260 "classp.y" // lalr1.cc:847
    { yylhs.value.as< int64_t > () = token::TOK_MINUS; }
#line 938 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 34:
#line 264 "classp.y" // lalr1.cc:847
    { yylhs.value.as< classp::ParseTree* > () = yystack_[0].value.as< classp::ParseTree* > (); }
#line 944 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 35:
#line 265 "classp.y" // lalr1.cc:847
    {
        yylhs.value.as< classp::ParseTree* > () = new ParseTreeBinop(parser, yylhs.location, yystack_[1].value.as< int64_t > (), yystack_[2].value.as< classp::ParseTree* > (), yystack_[0].value.as< classp::ParseTree* > ());
      }
#line 952 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 36:
#line 271 "classp.y" // lalr1.cc:847
    { yylhs.value.as< int64_t > () = token::TOK_STAR; }
#line 958 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 37:
#line 272 "classp.y" // lalr1.cc:847
    { yylhs.value.as< int64_t > () = token::TOK_SLASH; }
#line 964 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 38:
#line 273 "classp.y" // lalr1.cc:847
    { yylhs.value.as< int64_t > () = token::TOK_PERCENT; }
#line 970 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 39:
#line 274 "classp.y" // lalr1.cc:847
    { yylhs.value.as< int64_t > () = token::TOK_LSHIFT; }
#line 976 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 40:
#line 275 "classp.y" // lalr1.cc:847
    { yylhs.value.as< int64_t > () = token::TOK_RSHIFT; }
#line 982 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 41:
#line 279 "classp.y" // lalr1.cc:847
    { yylhs.value.as< classp::ParseTree* > () = yystack_[0].value.as< classp::ParseTree* > (); }
#line 988 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 42:
#line 280 "classp.y" // lalr1.cc:847
    {
        yylhs.value.as< classp::ParseTree* > () = new ParseTreeUnop(parser, yylhs.location, yystack_[1].value.as< int64_t > (), yystack_[0].value.as< classp::ParseTree* > ());
      }
#line 996 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 43:
#line 286 "classp.y" // lalr1.cc:847
    { yylhs.value.as< int64_t > () = token::TOK_MINUS; }
#line 1002 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 44:
#line 287 "classp.y" // lalr1.cc:847
    { yylhs.value.as< int64_t > () = token::TOK_NOT; }
#line 1008 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 45:
#line 292 "classp.y" // lalr1.cc:847
    { yylhs.value.as<  vector<ParseTree*>  > () = vector<ParseTree*>(); yylhs.value.as<  vector<ParseTree*>  > ().push_back(yystack_[0].value.as< classp::ParseTree* > ()); }
#line 1014 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 46:
#line 293 "classp.y" // lalr1.cc:847
    { yylhs.value.as<  vector<ParseTree*>  > () = yystack_[2].value.as<  vector<ParseTree*>  > (); yylhs.value.as<  vector<ParseTree*>  > ().push_back(yystack_[0].value.as< classp::ParseTree* > ()); }
#line 1020 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 49:
#line 299 "classp.y" // lalr1.cc:847
    {
    yylhs.value.as< classp::ParseTree* > () = new ParseTreeClassDecl(parser, yylhs.location, yystack_[4].value.as< string > (), yystack_[3].value.as< vector<string> > (), yystack_[1].value.as<  vector<ParseTree*>  > ());
    }
#line 1028 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 50:
#line 305 "classp.y" // lalr1.cc:847
    { yylhs.value.as< vector<string> > () = vector<string>(); }
#line 1034 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 51:
#line 306 "classp.y" // lalr1.cc:847
    { yylhs.value.as< vector<string> > () = yystack_[0].value.as< vector<string> > (); }
#line 1040 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 52:
#line 310 "classp.y" // lalr1.cc:847
    { yylhs.value.as< vector<string> > () = vector<string>(); yylhs.value.as< vector<string> > ().push_back(yystack_[0].value.as< string > ()); }
#line 1046 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 53:
#line 311 "classp.y" // lalr1.cc:847
    { yylhs.value.as< vector<string> > () = yystack_[2].value.as< vector<string> > (); yylhs.value.as< vector<string> > ().push_back(yystack_[0].value.as< string > ()); }
#line 1052 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 54:
#line 315 "classp.y" // lalr1.cc:847
    { yylhs.value.as<  vector<ParseTree*>  > () = vector<ParseTree*>(); }
#line 1058 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 55:
#line 316 "classp.y" // lalr1.cc:847
    { yylhs.value.as<  vector<ParseTree*>  > () = yystack_[2].value.as<  vector<ParseTree*>  > (); yylhs.value.as<  vector<ParseTree*>  > ().push_back(yystack_[1].value.as< classp::ParseTree* > ()); }
#line 1064 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 56:
#line 321 "classp.y" // lalr1.cc:847
    {
      yylhs.value.as< classp::ParseTree* > () = new ParseTreeAttribute(parser, yylhs.location, yystack_[3].value.as< string > (), yystack_[4].value.as< string > (), yystack_[5].value.as< bool > (), yystack_[2].value.as< bool > (), yystack_[1].value.as< classp::ParseTree* > (), yystack_[0].value.as< classp::ParseTree* > ()); }
#line 1071 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 57:
#line 323 "classp.y" // lalr1.cc:847
    { yylhs.value.as< classp::ParseTree* > () = yystack_[0].value.as< classp::ParseTree* > (); }
#line 1077 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 58:
#line 324 "classp.y" // lalr1.cc:847
    { yylhs.value.as< classp::ParseTree* > () = yystack_[0].value.as< classp::ParseTree* > (); }
#line 1083 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 59:
#line 325 "classp.y" // lalr1.cc:847
    { yylhs.value.as< classp::ParseTree* > () = yystack_[0].value.as< classp::ParseTree* > (); }
#line 1089 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 60:
#line 329 "classp.y" // lalr1.cc:847
    { yylhs.value.as< classp::ParseTree* > () = nullptr; }
#line 1095 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 61:
#line 330 "classp.y" // lalr1.cc:847
    { yylhs.value.as< classp::ParseTree* > () = yystack_[0].value.as< classp::ParseTree* > (); }
#line 1101 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 62:
#line 333 "classp.y" // lalr1.cc:847
    { yylhs.value.as< bool > () = false; }
#line 1107 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 63:
#line 334 "classp.y" // lalr1.cc:847
    { yylhs.value.as< bool > () = true; }
#line 1113 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 64:
#line 337 "classp.y" // lalr1.cc:847
    { yylhs.value.as< bool > () = false; }
#line 1119 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 65:
#line 338 "classp.y" // lalr1.cc:847
    { yylhs.value.as< bool > () = true; }
#line 1125 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 66:
#line 341 "classp.y" // lalr1.cc:847
    { yylhs.value.as< string > () = ""; }
#line 1131 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 67:
#line 342 "classp.y" // lalr1.cc:847
    { yylhs.value.as< string > () = yystack_[0].value.as< string > (); }
#line 1137 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 68:
#line 346 "classp.y" // lalr1.cc:847
    { yylhs.value.as< classp::ParseTree* > () = nullptr; }
#line 1143 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 69:
#line 347 "classp.y" // lalr1.cc:847
    { yylhs.value.as< classp::ParseTree* > () = yystack_[0].value.as< classp::ParseTree* > (); }
#line 1149 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 70:
#line 350 "classp.y" // lalr1.cc:847
    {
      yylhs.value.as< classp::ParseTree* > () = new ParseTreeSyntaxDecl(parser, yylhs.location, yystack_[4].value.as< string > (), yystack_[0].value.as<  vector<ParseTree*>  > (), yystack_[2].value.as< classp::ParseTree* > ()); }
#line 1156 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 71:
#line 355 "classp.y" // lalr1.cc:847
    {
      yylhs.value.as< classp::ParseTree* > () = new ParseTreeSample(parser, yylhs.location, yystack_[1].value.as< classp::ParseTreeSymbol* > (), nullptr); }
#line 1163 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 72:
#line 357 "classp.y" // lalr1.cc:847
    {
      yylhs.value.as< classp::ParseTree* > () = new ParseTreeSample(parser, yylhs.location, yystack_[3].value.as< classp::ParseTreeSymbol* > (), yystack_[1].value.as< classp::ParseTree* > ()); }
#line 1170 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 73:
#line 359 "classp.y" // lalr1.cc:847
    {
      yylhs.value.as< classp::ParseTree* > () = new ParseTreeSample(parser, yylhs.location, yystack_[3].value.as< classp::ParseTreeSymbol* > (), yystack_[1].value.as< classp::ParseTreeSymbol* > ()); }
#line 1177 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 74:
#line 364 "classp.y" // lalr1.cc:847
    { yylhs.value.as<  vector<ParseTree*>  > () = vector<ParseTree*>(); }
#line 1183 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 75:
#line 365 "classp.y" // lalr1.cc:847
    {
    yylhs.value.as<  vector<ParseTree*>  > () = yystack_[3].value.as<  vector<ParseTree*>  > ();
    yylhs.value.as<  vector<ParseTree*>  > ().push_back(new ParseTreeBinop(parser, yylhs.location, token::TOK_PERCENT, yystack_[1].value.as< classp::ParseTree* > (), yystack_[0].value.as< classp::ParseTree* > ()));
    }
#line 1192 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 76:
#line 372 "classp.y" // lalr1.cc:847
    { yylhs.value.as< classp::ParseTree* > () = yystack_[0].value.as< classp::ParseTree* > (); }
#line 1198 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 77:
#line 373 "classp.y" // lalr1.cc:847
    {
    // push a sequence of nested syntax_alt_lists into a single list
    ParseTreeAltList* alts = yystack_[2].value.as< classp::ParseTree* > ()->AsAltList();
    if (!alts) {
      alts = new ParseTreeAltList(parser, yylhs.location);
      alts->array.push_back(yystack_[2].value.as< classp::ParseTree* > ());
    }
    alts->array.push_back(yystack_[0].value.as< classp::ParseTree* > ());
    yylhs.value.as< classp::ParseTree* > () = alts;
  }
#line 1213 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 78:
#line 386 "classp.y" // lalr1.cc:847
    { yylhs.value.as< classp::ParseTree* > () = yystack_[0].value.as< classp::ParseTree* > (); }
#line 1219 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 79:
#line 387 "classp.y" // lalr1.cc:847
    {
      yylhs.value.as< classp::ParseTree* > () = new ParseTreeBinop(parser, yylhs.location, token::TOK_STAR, yystack_[2].value.as< classp::ParseTree* > (), yystack_[0].value.as< classp::ParseTree* > ()); }
#line 1226 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 80:
#line 389 "classp.y" // lalr1.cc:847
    {
      yylhs.value.as< classp::ParseTree* > () = new ParseTreeBinop(parser, yylhs.location, token::TOK_PLUS, yystack_[2].value.as< classp::ParseTree* > (), yystack_[0].value.as< classp::ParseTree* > ()); }
#line 1233 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 81:
#line 391 "classp.y" // lalr1.cc:847
    {
      yylhs.value.as< classp::ParseTree* > () = new ParseTreeUnop(parser, yylhs.location, token::TOK_STAR, yystack_[1].value.as< classp::ParseTree* > ()); }
#line 1240 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 82:
#line 393 "classp.y" // lalr1.cc:847
    {
      yylhs.value.as< classp::ParseTree* > () = new ParseTreeUnop(parser, yylhs.location, token::TOK_PLUS, yystack_[1].value.as< classp::ParseTree* > ()); }
#line 1247 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 83:
#line 395 "classp.y" // lalr1.cc:847
    {
      yylhs.value.as< classp::ParseTree* > () = new ParseTreeUnop(parser, yylhs.location, token::TOK_QUESTION, yystack_[1].value.as< classp::ParseTree* > ()); }
#line 1254 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 84:
#line 400 "classp.y" // lalr1.cc:847
    { yylhs.value.as< classp::ParseTree* > () = new ParseTreeSymbol(parser, yylhs.location, ""); }
#line 1260 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 85:
#line 401 "classp.y" // lalr1.cc:847
    { yylhs.value.as< classp::ParseTree* > () = yystack_[0].value.as< classp::ParseTree* > (); }
#line 1266 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 86:
#line 404 "classp.y" // lalr1.cc:847
    { yylhs.value.as< classp::ParseTree* > () = yystack_[0].value.as< classp::ParseTree* > (); }
#line 1272 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 87:
#line 405 "classp.y" // lalr1.cc:847
    {
    // push a sequence of nested syntax_item_lists into a single list
    ParseTreeItemList* items = yystack_[1].value.as< classp::ParseTree* > ()->AsItemList();
    if (!items) {
      items = new ParseTreeItemList(parser, yylhs.location);
      items->array.push_back(yystack_[1].value.as< classp::ParseTree* > ());
    }
    items->array.push_back(yystack_[0].value.as< classp::ParseTree* > ());
    yylhs.value.as< classp::ParseTree* > () = items;
  }
#line 1287 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 88:
#line 418 "classp.y" // lalr1.cc:847
    { yylhs.value.as< classp::ParseTree* > () = yystack_[0].value.as< classp::ParseTreeSymbol* > (); }
#line 1293 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 89:
#line 419 "classp.y" // lalr1.cc:847
    { yylhs.value.as< classp::ParseTree* > () = yystack_[0].value.as< classp::ParseTree* > (); }
#line 1299 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 90:
#line 420 "classp.y" // lalr1.cc:847
    {
    yylhs.value.as< classp::ParseTree* > () = new ParseTreeBinop(parser, yylhs.location, token::TOK_LBRACE, yystack_[3].value.as< classp::ParseTree* > (), yystack_[1].value.as< ParseTreeAltList* > ());}
#line 1306 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 91:
#line 422 "classp.y" // lalr1.cc:847
    { yylhs.value.as< classp::ParseTree* > () = yystack_[1].value.as< classp::ParseTree* > (); }
#line 1312 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 92:
#line 426 "classp.y" // lalr1.cc:847
    { yylhs.value.as< classp::ParseTree* > () = yystack_[0].value.as< classp::ParseTree* > (); }
#line 1318 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 93:
#line 430 "classp.y" // lalr1.cc:847
    {
    yylhs.value.as< ParseTreeAltList* > () = new ParseTreeAltList(parser, yylhs.location);
    yylhs.value.as< ParseTreeAltList* > ()->array.push_back(
        new ParseTreeBinop(parser, yylhs.location, token::TOK_RIGHTARROW, yystack_[0].value.as< classp::ParseTree* > (), yystack_[2].value.as< classp::ParseTree* > ())); }
#line 1327 "classp.yacc.cc" // lalr1.cc:847
    break;

  case 94:
#line 434 "classp.y" // lalr1.cc:847
    {
      yylhs.value.as< ParseTreeAltList* > () = yystack_[4].value.as< ParseTreeAltList* > (); 
      yylhs.value.as< ParseTreeAltList* > ()->array.push_back(
          new ParseTreeBinop(parser, yylhs.location, token::TOK_RIGHTARROW, yystack_[0].value.as< classp::ParseTree* > (), yystack_[2].value.as< classp::ParseTree* > ())); }
#line 1336 "classp.yacc.cc" // lalr1.cc:847
    break;


#line 1340 "classp.yacc.cc" // lalr1.cc:847
            default:
              break;
            }
        }
      catch (const syntax_error& yyexc)
        {
          error (yyexc);
          YYERROR;
        }
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, yylhs);
    }
    goto yynewstate;

  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yyla.location, yysyntax_error_ (yystack_[0].state,
                                           yyempty ? yyempty_ : yyla.type_get ()));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyempty)
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyempty = true;
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;
    yyerror_range[1].location = yystack_[yylen - 1].location;
    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yyerror_range[1].location = yystack_[0].location;
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (!yyempty)
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (!yyempty)
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  void
  yyParser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what());
  }

  // Generate an error message.
  std::string
  yyParser::yysyntax_error_ (state_type yystate, symbol_number_type yytoken) const
  {
    std::string yyres;
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    size_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yytoken) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state
         merging (from LALR or IELR) and default reductions corrupt the
         expected token list.  However, the list is correct for
         canonical LR with one exception: it will still contain any
         token that will not be accepted due to an error action in a
         later state.
    */
    if (yytoken != yyempty_)
      {
        yyarg[yycount++] = yytname_[yytoken];
        int yyn = yypact_[yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            // Stay within bounds of both yycheck and yytname.
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
        YYCASE_(0, YY_("syntax error"));
        YYCASE_(1, YY_("syntax error, unexpected %s"));
        YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    // Argument number.
    size_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char yyParser::yypact_ninf_ = -66;

  const signed char yyParser::yytable_ninf_ = -1;

  const signed char
  yyParser::yypact_[] =
  {
     -31,   -66,    29,   -16,   -66,     1,     5,   -66,    37,   -66,
     -66,   -16,     6,    13,   -66,   -66,    48,   -66,    14,    97,
     -66,    21,   -66,   -66,    31,    53,    38,    33,   -66,   -66,
     -66,   -66,   -66,    66,    22,   -66,    41,     0,   -66,   -66,
      -4,    75,     0,   -66,   -66,    -1,   -66,   -17,     0,   -66,
      80,   -23,   -66,    73,    55,     2,     0,   -66,     0,   -66,
       0,   -66,    15,    84,    87,   -66,    15,    70,   -66,   -66,
      95,     0,     0,   -66,    15,   -66,   -66,   -66,   -66,   -66,
     -66,   -66,   -66,   -66,   -66,   -66,    27,   109,   -66,   105,
      78,   -66,    62,     3,   -66,   -66,    99,   -66,   -66,    21,
     -11,    15,     0,    15,   -66,   -66,   -66,   -66,   -66,   -66,
     -66,   -66,    15,    15,   -66,   -66,   -66,   -66,   -66,    15,
     -66,    15,   -66,    57,   -66,   109,   -66,   -66,    -9,    78,
     -66,    39,   -66,     0,   -66
  };

  const unsigned char
  yyParser::yydefact_[] =
  {
       3,     4,     0,     0,     1,     0,    47,    45,    50,     2,
      48,     0,     0,     0,    46,    52,    51,    54,     0,    62,
      53,     0,    49,    63,    66,     0,     0,     0,    57,    58,
       9,    59,    67,     0,     0,    55,     0,    84,    16,    17,
       0,    64,    84,    92,    88,     0,    76,    78,    85,    86,
      89,     0,    71,     0,    60,     0,    84,    74,    82,    83,
      81,    87,     0,     0,     0,    65,     0,    68,    91,    77,
      70,    80,    79,    14,     0,    43,    44,     7,    13,    10,
      11,    41,     6,    12,     5,    15,     0,    18,    20,    22,
      30,    34,     0,     0,    72,    73,    61,    56,    69,     0,
       0,     0,    84,     0,    24,    29,    28,    27,    26,    33,
      25,    32,     0,     0,    39,    38,    40,    37,    36,     0,
      42,     0,    90,     0,     8,    19,    93,    21,    23,    31,
      35,     0,    75,    84,    94
  };

  const signed char
  yyParser::yypgoto_[] =
  {
     -66,   -66,   -66,    36,   -21,     7,   -66,   -26,   -65,    28,
      30,   -66,    19,   -66,    26,   -66,    16,   -66,   -66,   -66,
     123,   -66,   -66,   -66,   -66,   -66,   -66,   -66,   -66,   -66,
      74,   -66,   -66,    98,   -52,   -66,   -44,    17,   -66,   -66
  };

  const signed char
  yyParser::yydefgoto_[] =
  {
      -1,     2,     3,    81,    43,    83,    84,    44,    86,    87,
      88,   112,    89,   113,    90,   119,    91,    92,     6,    11,
       7,    13,    16,    19,    26,    67,    27,    54,    33,    97,
      28,    29,    70,    45,    46,    47,    48,    49,    50,    93
  };

  const unsigned char
  yyParser::yytable_[] =
  {
      31,    96,    51,    56,    69,     9,    56,   121,    40,   100,
      58,   109,    59,   101,    71,     1,    72,    42,   111,    60,
      30,   124,     5,    38,    39,    64,    73,    17,    52,     4,
      63,    57,    74,   122,    68,    75,    85,    76,    77,    10,
      85,    82,    12,    30,     8,    82,    38,    39,    85,    15,
     126,   101,    78,    82,    18,   102,   131,    20,    30,    79,
      80,    38,    39,   101,    30,    61,    85,   133,    38,    39,
      34,    82,    35,    73,    32,    85,    36,    85,   123,    74,
      82,   134,    82,    37,    41,    77,    85,    85,    61,    61,
      53,    82,    82,    85,    62,    85,   114,    66,    82,    78,
      82,    79,    80,   115,    65,    30,    79,    80,    38,    39,
      24,   116,   103,   117,   118,   104,    94,   105,   106,    95,
      99,   107,    21,   101,   108,   109,   110,    22,   120,   125,
     132,   128,   111,   127,    14,   130,    23,    24,    25,   129,
      55,    98
  };

  const unsigned char
  yyParser::yycheck_[] =
  {
      21,    66,     6,     4,    56,     0,     4,     4,    34,    74,
      27,    20,    29,    24,    58,    46,    60,    17,    27,    36,
      43,    32,    38,    46,    47,    51,    11,    14,    32,     0,
      51,    32,    17,    30,    32,    20,    62,    22,    23,    34,
      66,    62,     5,    43,    43,    66,    46,    47,    74,    43,
     102,    24,    37,    74,     6,    28,   121,    43,    43,    44,
      45,    46,    47,    24,    43,    48,    92,    28,    46,    47,
      17,    92,    34,    11,    43,   101,    43,   103,    99,    17,
     101,   133,   103,    17,    43,    23,   112,   113,    71,    72,
      15,   112,   113,   119,    14,   121,    18,    42,   119,    37,
     121,    44,    45,    25,    31,    43,    44,    45,    46,    47,
      40,    33,     3,    35,    36,    10,    32,    12,    13,    32,
      25,    16,    25,    24,    19,    20,    21,    30,    92,   101,
     123,   112,    27,   103,    11,   119,    39,    40,    41,   113,
      42,    67
  };

  const unsigned char
  yyParser::yystos_[] =
  {
       0,    46,    51,    52,     0,    38,    68,    70,    43,     0,
      34,    69,     5,    71,    70,    43,    72,    14,     6,    73,
      43,    25,    30,    39,    40,    41,    74,    76,    80,    81,
      43,    54,    43,    78,    17,    34,    43,    17,    46,    47,
      57,    43,    17,    54,    57,    83,    84,    85,    86,    87,
      88,     6,    32,    15,    77,    83,     4,    32,    27,    29,
      36,    87,    14,    54,    57,    31,    42,    75,    32,    84,
      82,    86,    86,    11,    17,    20,    22,    23,    37,    44,
      45,    53,    54,    55,    56,    57,    58,    59,    60,    62,
      64,    66,    67,    89,    32,    32,    58,    79,    80,    25,
      58,    24,    28,     3,    10,    12,    13,    16,    19,    20,
      21,    27,    61,    63,    18,    25,    33,    35,    36,    65,
      53,     4,    30,    54,    32,    59,    84,    60,    62,    64,
      66,    58,    55,    28,    84
  };

  const unsigned char
  yyParser::yyr1_[] =
  {
       0,    50,    51,    52,    52,    53,    53,    53,    53,    54,
      55,    55,    56,    56,    56,    56,    57,    57,    58,    58,
      59,    59,    60,    60,    61,    61,    61,    61,    61,    61,
      62,    62,    63,    63,    64,    64,    65,    65,    65,    65,
      65,    66,    66,    67,    67,    68,    68,    69,    69,    70,
      71,    71,    72,    72,    73,    73,    74,    74,    74,    74,
      75,    75,    76,    76,    77,    77,    78,    78,    79,    79,
      80,    81,    81,    81,    82,    82,    83,    83,    84,    84,
      84,    84,    84,    84,    85,    85,    86,    86,    87,    87,
      87,    87,    88,    89,    89
  };

  const unsigned char
  yyParser::yyr2_[] =
  {
       0,     2,     3,     0,     1,     1,     1,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       1,     3,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     3,     1,     1,     1,     3,     1,     1,     1,     1,
       1,     1,     2,     1,     1,     1,     3,     0,     1,     6,
       0,     2,     1,     3,     0,     3,     6,     1,     1,     2,
       0,     2,     0,     1,     0,     2,     0,     1,     0,     1,
       6,     4,     6,     6,     0,     4,     1,     3,     1,     3,
       3,     2,     2,     2,     0,     1,     1,     2,     1,     1,
       4,     3,     1,     3,     5
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const yyParser::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "\"`&&`\"", "\"`|`\"",
  "\"`:`\"", "\"`,`\"", "\"`$$`\"", "\"`..`\"", "\"`=`\"", "\"`==`\"",
  "\"`false`\"", "\"`>=`\"", "\"`>`\"", "\"`{`\"", "\"`[`\"", "\"`<=`\"",
  "\"`(`\"", "\"`<<`\"", "\"`<`\"", "\"`-`\"", "\"`!=`\"", "\"`!`\"",
  "\"`null`\"", "\"`||`\"", "\"`%`\"", "\"`.`\"", "\"`+`\"", "\"`->`\"",
  "\"`?`\"", "\"`}`\"", "\"`]`\"", "\"`)`\"", "\"`>>`\"", "\"`;`\"",
  "\"`/`\"", "\"`*`\"", "\"`true`\"", "\"`class`\"", "\"`optional`\"",
  "\"`syntax`\"", "\"`sample`\"", "\"`default`\"", "\"identifier\"",
  "\"integer literal\"", "\"float literal\"",
  "\"single-quote delimited string literal\"",
  "\"double-quote delimited string literal\"", "TOK_START_EXPRESSION",
  "TOK_START_REFERENCE", "$accept", "start", "opt_code_literal", "operand",
  "identifier", "numeric_literal", "literal", "string_literal",
  "expression", "conjunction", "comparison", "relational_operator",
  "simple_expression", "additive_operator", "term",
  "multiplicative_operator", "factor", "unary_operator",
  "declaration_list", "opt_semicolon", "declaration", "parents_list",
  "parents_list2", "class_body", "attribute_decl", "opt_initializer",
  "opt_optional", "opt_array", "opt_identifier", "opt_syntax_decl",
  "syntax_decl", "sample_decl", "features", "syntax_alt_list",
  "syntax_spec", "opt_syntax_item_list", "syntax_item_list", "syntax_item",
  "syntax_attribute", "syntax_case_list", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  yyParser::yyrline_[] =
  {
       0,   177,   177,   183,   184,   188,   189,   190,   191,   195,
     199,   200,   204,   205,   206,   207,   211,   214,   222,   223,
     229,   230,   236,   237,   243,   244,   245,   246,   247,   248,
     252,   253,   259,   260,   264,   265,   271,   272,   273,   274,
     275,   279,   280,   286,   287,   292,   293,   296,   296,   299,
     305,   306,   310,   311,   315,   316,   320,   323,   324,   325,
     329,   330,   333,   334,   337,   338,   341,   342,   346,   347,
     350,   355,   357,   359,   364,   365,   372,   373,   386,   387,
     389,   391,   393,   395,   400,   401,   404,   405,   418,   419,
     420,   422,   426,   430,   434
  };

  // Print the state stack on the debug stream.
  void
  yyParser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  yyParser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):" << std::endl;
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG


#line 45 "classp.y" // lalr1.cc:1155
} // classp
#line 1823 "classp.yacc.cc" // lalr1.cc:1155
#line 439 "classp.y" // lalr1.cc:1156

