// A Bison parser, made by GNU Bison 3.0.2.

// Skeleton interface for Bison LALR(1) parsers in C++

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

/**
 ** \file classp.yacc.hh
 ** Define the classp::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

#ifndef YY_YY_CLASSP_YACC_HH_INCLUDED
# define YY_YY_CLASSP_YACC_HH_INCLUDED
// //                    "%code requires" blocks.
#line 21 "classp.y" // lalr1.cc:372


#include <string>
using std::string;
#include <vector>
using std::vector;

namespace classp {
struct ParseTree;
struct ParseTreeList;
struct ParseTreeSymbol;
class ParserBase;
class ParseTreeAltList;
}


#line 61 "classp.yacc.hh" // lalr1.cc:372


# include <vector>
# include <iostream>
# include <stdexcept>
# include <string>
# include "stack.hh"
# include "location.hh"

#ifndef YYASSERT
# include <cassert>
# define YYASSERT assert
#endif


#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

#line 45 "classp.y" // lalr1.cc:372
namespace classp {
#line 137 "classp.yacc.hh" // lalr1.cc:372



  /// A char[S] buffer to store and retrieve objects.
  ///
  /// Sort of a variant, but does not keep track of the nature
  /// of the stored data, since that knowledge is available
  /// via the current state.
  template <size_t S>
  struct variant
  {
    /// Type of *this.
    typedef variant<S> self_type;

    /// Empty construction.
    variant ()
    {}

    /// Construct and fill.
    template <typename T>
    variant (const T& t)
    {
      YYASSERT (sizeof (T) <= S);
      new (yyas_<T> ()) T (t);
    }

    /// Destruction, allowed only if empty.
    ~variant ()
    {}

    /// Instantiate an empty \a T in here.
    template <typename T>
    T&
    build ()
    {
      return *new (yyas_<T> ()) T;
    }

    /// Instantiate a \a T in here from \a t.
    template <typename T>
    T&
    build (const T& t)
    {
      return *new (yyas_<T> ()) T (t);
    }

    /// Accessor to a built \a T.
    template <typename T>
    T&
    as ()
    {
      return *yyas_<T> ();
    }

    /// Const accessor to a built \a T (for %printer).
    template <typename T>
    const T&
    as () const
    {
      return *yyas_<T> ();
    }

    /// Swap the content with \a other, of same type.
    ///
    /// Both variants must be built beforehand, because swapping the actual
    /// data requires reading it (with as()), and this is not possible on
    /// unconstructed variants: it would require some dynamic testing, which
    /// should not be the variant's responsability.
    /// Swapping between built and (possibly) non-built is done with
    /// variant::move ().
    template <typename T>
    void
    swap (self_type& other)
    {
      std::swap (as<T> (), other.as<T> ());
    }

    /// Move the content of \a other to this.
    ///
    /// Destroys \a other.
    template <typename T>
    void
    move (self_type& other)
    {
      build<T> ();
      swap<T> (other);
      other.destroy<T> ();
    }

    /// Copy the content of \a other to this.
    template <typename T>
    void
    copy (const self_type& other)
    {
      build<T> (other.as<T> ());
    }

    /// Destroy the stored \a T.
    template <typename T>
    void
    destroy ()
    {
      as<T> ().~T ();
    }

  private:
    /// Prohibit blind copies.
    self_type& operator=(const self_type&);
    variant (const self_type&);

    /// Accessor to raw memory as \a T.
    template <typename T>
    T*
    yyas_ ()
    {
      void *yyp = yybuffer_.yyraw;
      return static_cast<T*> (yyp);
     }

    /// Const accessor to raw memory as \a T.
    template <typename T>
    const T*
    yyas_ () const
    {
      const void *yyp = yybuffer_.yyraw;
      return static_cast<const T*> (yyp);
     }

    union
    {
      /// Strongest alignment constraints.
      long double yyalign_me;
      /// A buffer large enough to store any of the semantic values.
      char yyraw[S];
    } yybuffer_;
  };


  /// A Bison parser.
  class yyParser
  {
  public:
#ifndef YYSTYPE
    /// An auxiliary type to compute the largest semantic type.
    union union_type
    {
      // declaration_list
      // class_body
      // features
      char dummy1[sizeof( vector<ParseTree*> )];

      // syntax_case_list
      char dummy2[sizeof(ParseTreeAltList*)];

      // opt_optional
      // opt_array
      char dummy3[sizeof(bool)];

      // operand
      // identifier
      // numeric_literal
      // literal
      // expression
      // conjunction
      // comparison
      // simple_expression
      // term
      // factor
      // declaration
      // attribute_decl
      // opt_initializer
      // opt_syntax_decl
      // syntax_decl
      // sample_decl
      // syntax_alt_list
      // syntax_spec
      // opt_syntax_item_list
      // syntax_item_list
      // syntax_item
      // syntax_attribute
      char dummy4[sizeof(classp::ParseTree*)];

      // string_literal
      char dummy5[sizeof(classp::ParseTreeSymbol*)];

      // "float literal"
      char dummy6[sizeof(double)];

      // "integer literal"
      // relational_operator
      // additive_operator
      // multiplicative_operator
      // unary_operator
      char dummy7[sizeof(int64_t)];

      // "identifier"
      // "single-quote delimited string literal"
      // "double-quote delimited string literal"
      // opt_code_literal
      // opt_identifier
      char dummy8[sizeof(string)];

      // parents_list
      // parents_list2
      char dummy9[sizeof(vector<string>)];
};

    /// Symbol semantic values.
    typedef variant<sizeof(union_type)> semantic_type;
#else
    typedef YYSTYPE semantic_type;
#endif
    /// Symbol locations.
    typedef location location_type;

    /// Syntax errors thrown from user actions.
    struct syntax_error : std::runtime_error
    {
      syntax_error (const location_type& l, const std::string& m);
      location_type location;
    };

    /// Tokens.
    struct token
    {
      enum yytokentype
      {
        TOK_EOF = 0,
        TOK_AND = 258,
        TOK_BAR = 259,
        TOK_COLON = 260,
        TOK_COMMA = 261,
        TOK_DOLLARDOLLAR = 262,
        TOK_DOTDOT = 263,
        TOK_EQUAL = 264,
        TOK_EQL = 265,
        TOK_FALSE = 266,
        TOK_GEQ = 267,
        TOK_GTR = 268,
        TOK_LBRACE = 269,
        TOK_LBRACK = 270,
        TOK_LEQ = 271,
        TOK_LPAREN = 272,
        TOK_LSHIFT = 273,
        TOK_LSS = 274,
        TOK_MINUS = 275,
        TOK_NEQ = 276,
        TOK_NOT = 277,
        TOK_NULL = 278,
        TOK_OR = 279,
        TOK_PERCENT = 280,
        TOK_PERIOD = 281,
        TOK_PLUS = 282,
        TOK_RIGHTARROW = 283,
        TOK_QUESTION = 284,
        TOK_RBRACE = 285,
        TOK_RBRACK = 286,
        TOK_RPAREN = 287,
        TOK_RSHIFT = 288,
        TOK_SEMICOLON = 289,
        TOK_SLASH = 290,
        TOK_STAR = 291,
        TOK_TRUE = 292,
        TOK_CLASS = 293,
        TOK_OPTIONAL = 294,
        TOK_SYNTAX = 295,
        TOK_SAMPLE = 296,
        TOK_DEFAULT = 297,
        TOK_IDENTIFIER = 298,
        TOK_INTEGER_LITERAL = 299,
        TOK_FLOAT_LITERAL = 300,
        TOK_SINGLE_QUOTED_STRING_LITERAL = 301,
        TOK_DOUBLE_QUOTED_STRING_LITERAL = 302,
        TOK_START_EXPRESSION = 303,
        TOK_START_REFERENCE = 304
      };
    };

    /// (External) token type, as returned by yylex.
    typedef token::yytokentype token_type;

    /// Internal symbol number.
    typedef int symbol_number_type;

    /// Internal symbol number for tokens (subsumed by symbol_number_type).
    typedef unsigned char token_number_type;

    /// A complete symbol.
    ///
    /// Expects its Base type to provide access to the symbol type
    /// via type_get().
    ///
    /// Provide access to semantic value and location.
    template <typename Base>
    struct basic_symbol : Base
    {
      /// Alias to Base.
      typedef Base super_type;

      /// Default constructor.
      basic_symbol ();

      /// Copy constructor.
      basic_symbol (const basic_symbol& other);

      /// Constructor for valueless symbols, and symbols from each type.

  basic_symbol (typename Base::kind_type t, const location_type& l);

  basic_symbol (typename Base::kind_type t, const  vector<ParseTree*>  v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const ParseTreeAltList* v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const bool v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const classp::ParseTree* v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const classp::ParseTreeSymbol* v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const double v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const int64_t v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const string v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const vector<string> v, const location_type& l);


      /// Constructor for symbols with semantic value.
      basic_symbol (typename Base::kind_type t,
                    const semantic_type& v,
                    const location_type& l);

      ~basic_symbol ();

      /// Destructive move, \a s is emptied into this.
      void move (basic_symbol& s);

      /// The semantic value.
      semantic_type value;

      /// The location.
      location_type location;

    private:
      /// Assignment operator.
      basic_symbol& operator= (const basic_symbol& other);
    };

    /// Type access provider for token (enum) based symbols.
    struct by_type
    {
      /// Default constructor.
      by_type ();

      /// Copy constructor.
      by_type (const by_type& other);

      /// The symbol type as needed by the constructor.
      typedef token_type kind_type;

      /// Constructor from (external) token numbers.
      by_type (kind_type t);

      /// Steal the symbol type from \a that.
      void move (by_type& that);

      /// The (internal) type number (corresponding to \a type).
      /// -1 when this symbol is empty.
      symbol_number_type type_get () const;

      /// The token.
      token_type token () const;

      enum { empty = 0 };

      /// The symbol type.
      /// -1 when this symbol is empty.
      token_number_type type;
    };

    /// "External" symbols: returned by the scanner.
    typedef basic_symbol<by_type> symbol_type;

    // Symbol constructors declarations.
    static inline
    symbol_type
    make_TOK_EOF (const location_type& l);

    static inline
    symbol_type
    make_TOK_AND (const location_type& l);

    static inline
    symbol_type
    make_TOK_BAR (const location_type& l);

    static inline
    symbol_type
    make_TOK_COLON (const location_type& l);

    static inline
    symbol_type
    make_TOK_COMMA (const location_type& l);

    static inline
    symbol_type
    make_TOK_DOLLARDOLLAR (const location_type& l);

    static inline
    symbol_type
    make_TOK_DOTDOT (const location_type& l);

    static inline
    symbol_type
    make_TOK_EQUAL (const location_type& l);

    static inline
    symbol_type
    make_TOK_EQL (const location_type& l);

    static inline
    symbol_type
    make_TOK_FALSE (const location_type& l);

    static inline
    symbol_type
    make_TOK_GEQ (const location_type& l);

    static inline
    symbol_type
    make_TOK_GTR (const location_type& l);

    static inline
    symbol_type
    make_TOK_LBRACE (const location_type& l);

    static inline
    symbol_type
    make_TOK_LBRACK (const location_type& l);

    static inline
    symbol_type
    make_TOK_LEQ (const location_type& l);

    static inline
    symbol_type
    make_TOK_LPAREN (const location_type& l);

    static inline
    symbol_type
    make_TOK_LSHIFT (const location_type& l);

    static inline
    symbol_type
    make_TOK_LSS (const location_type& l);

    static inline
    symbol_type
    make_TOK_MINUS (const location_type& l);

    static inline
    symbol_type
    make_TOK_NEQ (const location_type& l);

    static inline
    symbol_type
    make_TOK_NOT (const location_type& l);

    static inline
    symbol_type
    make_TOK_NULL (const location_type& l);

    static inline
    symbol_type
    make_TOK_OR (const location_type& l);

    static inline
    symbol_type
    make_TOK_PERCENT (const location_type& l);

    static inline
    symbol_type
    make_TOK_PERIOD (const location_type& l);

    static inline
    symbol_type
    make_TOK_PLUS (const location_type& l);

    static inline
    symbol_type
    make_TOK_RIGHTARROW (const location_type& l);

    static inline
    symbol_type
    make_TOK_QUESTION (const location_type& l);

    static inline
    symbol_type
    make_TOK_RBRACE (const location_type& l);

    static inline
    symbol_type
    make_TOK_RBRACK (const location_type& l);

    static inline
    symbol_type
    make_TOK_RPAREN (const location_type& l);

    static inline
    symbol_type
    make_TOK_RSHIFT (const location_type& l);

    static inline
    symbol_type
    make_TOK_SEMICOLON (const location_type& l);

    static inline
    symbol_type
    make_TOK_SLASH (const location_type& l);

    static inline
    symbol_type
    make_TOK_STAR (const location_type& l);

    static inline
    symbol_type
    make_TOK_TRUE (const location_type& l);

    static inline
    symbol_type
    make_TOK_CLASS (const location_type& l);

    static inline
    symbol_type
    make_TOK_OPTIONAL (const location_type& l);

    static inline
    symbol_type
    make_TOK_SYNTAX (const location_type& l);

    static inline
    symbol_type
    make_TOK_SAMPLE (const location_type& l);

    static inline
    symbol_type
    make_TOK_DEFAULT (const location_type& l);

    static inline
    symbol_type
    make_TOK_IDENTIFIER (const string& v, const location_type& l);

    static inline
    symbol_type
    make_TOK_INTEGER_LITERAL (const int64_t& v, const location_type& l);

    static inline
    symbol_type
    make_TOK_FLOAT_LITERAL (const double& v, const location_type& l);

    static inline
    symbol_type
    make_TOK_SINGLE_QUOTED_STRING_LITERAL (const string& v, const location_type& l);

    static inline
    symbol_type
    make_TOK_DOUBLE_QUOTED_STRING_LITERAL (const string& v, const location_type& l);

    static inline
    symbol_type
    make_TOK_START_EXPRESSION (const location_type& l);

    static inline
    symbol_type
    make_TOK_START_REFERENCE (const location_type& l);


    /// Build a parser object.
    yyParser (classp::ParserBase* parser_yyarg, classp::ParseTree** result_yyarg);
    virtual ~yyParser ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

    /// Report a syntax error.
    /// \param loc    where the syntax error is found.
    /// \param msg    a description of the syntax error.
    virtual void error (const location_type& loc, const std::string& msg);

    /// Report a syntax error.
    void error (const syntax_error& err);

  private:
    /// This class is not copyable.
    yyParser (const yyParser&);
    yyParser& operator= (const yyParser&);

    /// State numbers.
    typedef int state_type;

    /// Generate an error message.
    /// \param yystate   the state where the error occurred.
    /// \param yytoken   the lookahead token type, or yyempty_.
    virtual std::string yysyntax_error_ (state_type yystate,
                                         symbol_number_type yytoken) const;

    /// Compute post-reduction state.
    /// \param yystate   the current state
    /// \param yysym     the nonterminal to push on the stack
    state_type yy_lr_goto_state_ (state_type yystate, int yysym);

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue);

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue);

    static const signed char yypact_ninf_;
    static const signed char yytable_ninf_;

    /// Convert a scanner token number \a t to a symbol number.
    static token_number_type yytranslate_ (token_type t);

    // Tables.
  // YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
  // STATE-NUM.
  static const signed char yypact_[];

  // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
  // Performed when YYTABLE does not specify something else to do.  Zero
  // means the default is an error.
  static const unsigned char yydefact_[];

  // YYPGOTO[NTERM-NUM].
  static const signed char yypgoto_[];

  // YYDEFGOTO[NTERM-NUM].
  static const signed char yydefgoto_[];

  // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
  // positive, shift that token.  If negative, reduce the rule whose
  // number is the opposite.  If YYTABLE_NINF, syntax error.
  static const unsigned char yytable_[];

  static const unsigned char yycheck_[];

  // YYSTOS[STATE-NUM] -- The (internal number of the) accessing
  // symbol of state STATE-NUM.
  static const unsigned char yystos_[];

  // YYR1[YYN] -- Symbol number of symbol that rule YYN derives.
  static const unsigned char yyr1_[];

  // YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.
  static const unsigned char yyr2_[];


    /// Convert the symbol name \a n to a form suitable for a diagnostic.
    static std::string yytnamerr_ (const char *n);


    /// For a symbol, its name in clear.
    static const char* const yytname_[];
#if YYDEBUG
  // YYRLINE[YYN] -- Source line where rule number YYN was defined.
  static const unsigned short int yyrline_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r);
    /// Print the state stack on the debug stream.
    virtual void yystack_print_ ();

    // Debugging.
    int yydebug_;
    std::ostream* yycdebug_;

    /// \brief Display a symbol type, value and location.
    /// \param yyo    The output stream.
    /// \param yysym  The symbol.
    template <typename Base>
    void yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const;
#endif

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg     Why this token is reclaimed.
    ///                  If null, print nothing.
    /// \param yysym     The symbol.
    template <typename Base>
    void yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const;

  private:
    /// Type access provider for state based symbols.
    struct by_state
    {
      /// Default constructor.
      by_state ();

      /// The symbol type as needed by the constructor.
      typedef state_type kind_type;

      /// Constructor.
      by_state (kind_type s);

      /// Copy constructor.
      by_state (const by_state& other);

      /// Steal the symbol type from \a that.
      void move (by_state& that);

      /// The (internal) type number (corresponding to \a state).
      /// "empty" when empty.
      symbol_number_type type_get () const;

      enum { empty = 0 };

      /// The state.
      state_type state;
    };

    /// "Internal" symbol: element of the stack.
    struct stack_symbol_type : basic_symbol<by_state>
    {
      /// Superclass.
      typedef basic_symbol<by_state> super_type;
      /// Construct an empty symbol.
      stack_symbol_type ();
      /// Steal the contents from \a sym to build this.
      stack_symbol_type (state_type s, symbol_type& sym);
      /// Assignment, needed by push_back.
      stack_symbol_type& operator= (const stack_symbol_type& that);
    };

    /// Stack type.
    typedef stack<stack_symbol_type> stack_type;

    /// The stack.
    stack_type yystack_;

    /// Push a new state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the symbol
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, stack_symbol_type& s);

    /// Push a new look ahead token on the state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the state
    /// \param sym  the symbol (for its value and location).
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, state_type s, symbol_type& sym);

    /// Pop \a n symbols the three stacks.
    void yypop_ (unsigned int n = 1);

    // Constants.
    enum
    {
      yyeof_ = 0,
      yylast_ = 141,     ///< Last index in yytable_.
      yynnts_ = 40,  ///< Number of nonterminal symbols.
      yyempty_ = -2,
      yyfinal_ = 4, ///< Termination state number.
      yyterror_ = 1,
      yyerrcode_ = 256,
      yyntokens_ = 50  ///< Number of tokens.
    };


    // User arguments.
    classp::ParserBase* parser;
    classp::ParseTree** result;
  };

  // Symbol number corresponding to token number t.
  inline
  yyParser::token_number_type
  yyParser::yytranslate_ (token_type t)
  {
    static
    const token_number_type
    translate_table[] =
    {
     0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49
    };
    const unsigned int user_token_number_max_ = 304;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }

  inline
  yyParser::syntax_error::syntax_error (const location_type& l, const std::string& m)
    : std::runtime_error (m)
    , location (l)
  {}

  // basic_symbol.
  template <typename Base>
  inline
  yyParser::basic_symbol<Base>::basic_symbol ()
    : value ()
  {}

  template <typename Base>
  inline
  yyParser::basic_symbol<Base>::basic_symbol (const basic_symbol& other)
    : Base (other)
    , value ()
    , location (other.location)
  {
      switch (other.type_get ())
    {
      case 68: // declaration_list
      case 73: // class_body
      case 82: // features
        value.copy<  vector<ParseTree*>  > (other.value);
        break;

      case 89: // syntax_case_list
        value.copy< ParseTreeAltList* > (other.value);
        break;

      case 76: // opt_optional
      case 77: // opt_array
        value.copy< bool > (other.value);
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
        value.copy< classp::ParseTree* > (other.value);
        break;

      case 57: // string_literal
        value.copy< classp::ParseTreeSymbol* > (other.value);
        break;

      case 45: // "float literal"
        value.copy< double > (other.value);
        break;

      case 44: // "integer literal"
      case 61: // relational_operator
      case 63: // additive_operator
      case 65: // multiplicative_operator
      case 67: // unary_operator
        value.copy< int64_t > (other.value);
        break;

      case 43: // "identifier"
      case 46: // "single-quote delimited string literal"
      case 47: // "double-quote delimited string literal"
      case 52: // opt_code_literal
      case 78: // opt_identifier
        value.copy< string > (other.value);
        break;

      case 71: // parents_list
      case 72: // parents_list2
        value.copy< vector<string> > (other.value);
        break;

      default:
        break;
    }

  }


  template <typename Base>
  inline
  yyParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const semantic_type& v, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {
    (void) v;
      switch (this->type_get ())
    {
      case 68: // declaration_list
      case 73: // class_body
      case 82: // features
        value.copy<  vector<ParseTree*>  > (v);
        break;

      case 89: // syntax_case_list
        value.copy< ParseTreeAltList* > (v);
        break;

      case 76: // opt_optional
      case 77: // opt_array
        value.copy< bool > (v);
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
        value.copy< classp::ParseTree* > (v);
        break;

      case 57: // string_literal
        value.copy< classp::ParseTreeSymbol* > (v);
        break;

      case 45: // "float literal"
        value.copy< double > (v);
        break;

      case 44: // "integer literal"
      case 61: // relational_operator
      case 63: // additive_operator
      case 65: // multiplicative_operator
      case 67: // unary_operator
        value.copy< int64_t > (v);
        break;

      case 43: // "identifier"
      case 46: // "single-quote delimited string literal"
      case 47: // "double-quote delimited string literal"
      case 52: // opt_code_literal
      case 78: // opt_identifier
        value.copy< string > (v);
        break;

      case 71: // parents_list
      case 72: // parents_list2
        value.copy< vector<string> > (v);
        break;

      default:
        break;
    }
}


  // Implementation of basic_symbol constructor for each type.

  template <typename Base>
  yyParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {}

  template <typename Base>
  yyParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const  vector<ParseTree*>  v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  yyParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const ParseTreeAltList* v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  yyParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const bool v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  yyParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const classp::ParseTree* v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  yyParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const classp::ParseTreeSymbol* v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  yyParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const double v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  yyParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const int64_t v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  yyParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const string v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  yyParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const vector<string> v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}


  template <typename Base>
  inline
  yyParser::basic_symbol<Base>::~basic_symbol ()
  {
    // User destructor.
    symbol_number_type yytype = this->type_get ();
    switch (yytype)
    {
   default:
      break;
    }

    // Type destructor.
    switch (yytype)
    {
      case 68: // declaration_list
      case 73: // class_body
      case 82: // features
        value.template destroy<  vector<ParseTree*>  > ();
        break;

      case 89: // syntax_case_list
        value.template destroy< ParseTreeAltList* > ();
        break;

      case 76: // opt_optional
      case 77: // opt_array
        value.template destroy< bool > ();
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
        value.template destroy< classp::ParseTree* > ();
        break;

      case 57: // string_literal
        value.template destroy< classp::ParseTreeSymbol* > ();
        break;

      case 45: // "float literal"
        value.template destroy< double > ();
        break;

      case 44: // "integer literal"
      case 61: // relational_operator
      case 63: // additive_operator
      case 65: // multiplicative_operator
      case 67: // unary_operator
        value.template destroy< int64_t > ();
        break;

      case 43: // "identifier"
      case 46: // "single-quote delimited string literal"
      case 47: // "double-quote delimited string literal"
      case 52: // opt_code_literal
      case 78: // opt_identifier
        value.template destroy< string > ();
        break;

      case 71: // parents_list
      case 72: // parents_list2
        value.template destroy< vector<string> > ();
        break;

      default:
        break;
    }

  }

  template <typename Base>
  inline
  void
  yyParser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move(s);
      switch (this->type_get ())
    {
      case 68: // declaration_list
      case 73: // class_body
      case 82: // features
        value.move<  vector<ParseTree*>  > (s.value);
        break;

      case 89: // syntax_case_list
        value.move< ParseTreeAltList* > (s.value);
        break;

      case 76: // opt_optional
      case 77: // opt_array
        value.move< bool > (s.value);
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
        value.move< classp::ParseTree* > (s.value);
        break;

      case 57: // string_literal
        value.move< classp::ParseTreeSymbol* > (s.value);
        break;

      case 45: // "float literal"
        value.move< double > (s.value);
        break;

      case 44: // "integer literal"
      case 61: // relational_operator
      case 63: // additive_operator
      case 65: // multiplicative_operator
      case 67: // unary_operator
        value.move< int64_t > (s.value);
        break;

      case 43: // "identifier"
      case 46: // "single-quote delimited string literal"
      case 47: // "double-quote delimited string literal"
      case 52: // opt_code_literal
      case 78: // opt_identifier
        value.move< string > (s.value);
        break;

      case 71: // parents_list
      case 72: // parents_list2
        value.move< vector<string> > (s.value);
        break;

      default:
        break;
    }

    location = s.location;
  }

  // by_type.
  inline
  yyParser::by_type::by_type ()
     : type (empty)
  {}

  inline
  yyParser::by_type::by_type (const by_type& other)
    : type (other.type)
  {}

  inline
  yyParser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  inline
  void
  yyParser::by_type::move (by_type& that)
  {
    type = that.type;
    that.type = empty;
  }

  inline
  int
  yyParser::by_type::type_get () const
  {
    return type;
  }

  inline
  yyParser::token_type
  yyParser::by_type::token () const
  {
    // YYTOKNUM[NUM] -- (External) token number corresponding to the
    // (internal) symbol number NUM (which must be that of a token).  */
    static
    const unsigned short int
    yytoken_number_[] =
    {
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304
    };
    return static_cast<token_type> (yytoken_number_[type]);
  }
  // Implementation of make_symbol for each symbol type.
  yyParser::symbol_type
  yyParser::make_TOK_EOF (const location_type& l)
  {
    return symbol_type (token::TOK_EOF, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_AND (const location_type& l)
  {
    return symbol_type (token::TOK_AND, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_BAR (const location_type& l)
  {
    return symbol_type (token::TOK_BAR, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_COLON (const location_type& l)
  {
    return symbol_type (token::TOK_COLON, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_COMMA (const location_type& l)
  {
    return symbol_type (token::TOK_COMMA, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_DOLLARDOLLAR (const location_type& l)
  {
    return symbol_type (token::TOK_DOLLARDOLLAR, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_DOTDOT (const location_type& l)
  {
    return symbol_type (token::TOK_DOTDOT, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_EQUAL (const location_type& l)
  {
    return symbol_type (token::TOK_EQUAL, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_EQL (const location_type& l)
  {
    return symbol_type (token::TOK_EQL, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_FALSE (const location_type& l)
  {
    return symbol_type (token::TOK_FALSE, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_GEQ (const location_type& l)
  {
    return symbol_type (token::TOK_GEQ, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_GTR (const location_type& l)
  {
    return symbol_type (token::TOK_GTR, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_LBRACE (const location_type& l)
  {
    return symbol_type (token::TOK_LBRACE, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_LBRACK (const location_type& l)
  {
    return symbol_type (token::TOK_LBRACK, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_LEQ (const location_type& l)
  {
    return symbol_type (token::TOK_LEQ, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_LPAREN (const location_type& l)
  {
    return symbol_type (token::TOK_LPAREN, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_LSHIFT (const location_type& l)
  {
    return symbol_type (token::TOK_LSHIFT, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_LSS (const location_type& l)
  {
    return symbol_type (token::TOK_LSS, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_MINUS (const location_type& l)
  {
    return symbol_type (token::TOK_MINUS, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_NEQ (const location_type& l)
  {
    return symbol_type (token::TOK_NEQ, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_NOT (const location_type& l)
  {
    return symbol_type (token::TOK_NOT, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_NULL (const location_type& l)
  {
    return symbol_type (token::TOK_NULL, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_OR (const location_type& l)
  {
    return symbol_type (token::TOK_OR, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_PERCENT (const location_type& l)
  {
    return symbol_type (token::TOK_PERCENT, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_PERIOD (const location_type& l)
  {
    return symbol_type (token::TOK_PERIOD, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_PLUS (const location_type& l)
  {
    return symbol_type (token::TOK_PLUS, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_RIGHTARROW (const location_type& l)
  {
    return symbol_type (token::TOK_RIGHTARROW, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_QUESTION (const location_type& l)
  {
    return symbol_type (token::TOK_QUESTION, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_RBRACE (const location_type& l)
  {
    return symbol_type (token::TOK_RBRACE, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_RBRACK (const location_type& l)
  {
    return symbol_type (token::TOK_RBRACK, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_RPAREN (const location_type& l)
  {
    return symbol_type (token::TOK_RPAREN, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_RSHIFT (const location_type& l)
  {
    return symbol_type (token::TOK_RSHIFT, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_SEMICOLON (const location_type& l)
  {
    return symbol_type (token::TOK_SEMICOLON, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_SLASH (const location_type& l)
  {
    return symbol_type (token::TOK_SLASH, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_STAR (const location_type& l)
  {
    return symbol_type (token::TOK_STAR, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_TRUE (const location_type& l)
  {
    return symbol_type (token::TOK_TRUE, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_CLASS (const location_type& l)
  {
    return symbol_type (token::TOK_CLASS, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_OPTIONAL (const location_type& l)
  {
    return symbol_type (token::TOK_OPTIONAL, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_SYNTAX (const location_type& l)
  {
    return symbol_type (token::TOK_SYNTAX, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_SAMPLE (const location_type& l)
  {
    return symbol_type (token::TOK_SAMPLE, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_DEFAULT (const location_type& l)
  {
    return symbol_type (token::TOK_DEFAULT, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_IDENTIFIER (const string& v, const location_type& l)
  {
    return symbol_type (token::TOK_IDENTIFIER, v, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_INTEGER_LITERAL (const int64_t& v, const location_type& l)
  {
    return symbol_type (token::TOK_INTEGER_LITERAL, v, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_FLOAT_LITERAL (const double& v, const location_type& l)
  {
    return symbol_type (token::TOK_FLOAT_LITERAL, v, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_SINGLE_QUOTED_STRING_LITERAL (const string& v, const location_type& l)
  {
    return symbol_type (token::TOK_SINGLE_QUOTED_STRING_LITERAL, v, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_DOUBLE_QUOTED_STRING_LITERAL (const string& v, const location_type& l)
  {
    return symbol_type (token::TOK_DOUBLE_QUOTED_STRING_LITERAL, v, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_START_EXPRESSION (const location_type& l)
  {
    return symbol_type (token::TOK_START_EXPRESSION, l);
  }

  yyParser::symbol_type
  yyParser::make_TOK_START_REFERENCE (const location_type& l)
  {
    return symbol_type (token::TOK_START_REFERENCE, l);
  }


#line 45 "classp.y" // lalr1.cc:372
} // classp
#line 1753 "classp.yacc.hh" // lalr1.cc:372




#endif // !YY_YY_CLASSP_YACC_HH_INCLUDED
