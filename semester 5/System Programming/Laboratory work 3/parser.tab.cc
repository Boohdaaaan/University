// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

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

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.



// First part of user prologue.
#line 19 "parser.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

extern int yylineno;
extern FILE* yyin;

static std::string take_owned_text(char* text) {
    if (!text) {
        return std::string();
    }
    std::string result(text);
    free(text);
    return result;
}


#line 61 "parser.tab.cc"


#include "parser.tab.hh"




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


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
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
    while (false)
# endif


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
      *yycdebug_ << '\n';                       \
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
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace yy {
#line 158 "parser.tab.cc"

  /// Build a parser object.
  parser::parser ()
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr)
#else

#endif
  {}

  parser::~parser ()
  {}

  parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/

  // basic_symbol.
  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value ()
    , location (that.location)
  {
    switch (this->kind ())
    {
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_STRING_LITERAL: // STRING_LITERAL
      case symbol_kind::S_CHAR_LITERAL: // CHAR_LITERAL
      case symbol_kind::S_FLOAT_LITERAL: // FLOAT_LITERAL
        value.copy< char* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER_LITERAL: // INTEGER_LITERAL
        value.copy< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_parameter: // parameter
        value.copy< std::pair<std::string, std::string> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_type_specifier: // type_specifier
        value.copy< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_block: // block
        value.copy< std::unique_ptr<BlockNode> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_expression: // expression
        value.copy< std::unique_ptr<ExpressionNode> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement: // statement
        value.copy< std::unique_ptr<StatementNode> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_parameter_list: // parameter_list
        value.copy< std::vector<std::pair<std::string, std::string>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_expression_list: // expression_list
        value.copy< std::vector<std::unique_ptr<ExpressionNode>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement_list: // statement_list
        value.copy< std::vector<std::unique_ptr<StatementNode>> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

  }




  template <typename Base>
  parser::symbol_kind_type
  parser::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


  template <typename Base>
  bool
  parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
  parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->kind ())
    {
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_STRING_LITERAL: // STRING_LITERAL
      case symbol_kind::S_CHAR_LITERAL: // CHAR_LITERAL
      case symbol_kind::S_FLOAT_LITERAL: // FLOAT_LITERAL
        value.move< char* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_INTEGER_LITERAL: // INTEGER_LITERAL
        value.move< int > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_parameter: // parameter
        value.move< std::pair<std::string, std::string> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_type_specifier: // type_specifier
        value.move< std::string > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_block: // block
        value.move< std::unique_ptr<BlockNode> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_expression: // expression
        value.move< std::unique_ptr<ExpressionNode> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_statement: // statement
        value.move< std::unique_ptr<StatementNode> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_parameter_list: // parameter_list
        value.move< std::vector<std::pair<std::string, std::string>> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_expression_list: // expression_list
        value.move< std::vector<std::unique_ptr<ExpressionNode>> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_statement_list: // statement_list
        value.move< std::vector<std::unique_ptr<StatementNode>> > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

    location = YY_MOVE (s.location);
  }

  // by_kind.
  parser::by_kind::by_kind () YY_NOEXCEPT
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  parser::by_kind::by_kind (by_kind&& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  parser::by_kind::by_kind (const by_kind& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {}

  parser::by_kind::by_kind (token_kind_type t) YY_NOEXCEPT
    : kind_ (yytranslate_ (t))
  {}



  void
  parser::by_kind::clear () YY_NOEXCEPT
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  void
  parser::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  parser::symbol_kind_type
  parser::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }


  parser::symbol_kind_type
  parser::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }



  // by_state.
  parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  parser::symbol_kind_type
  parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  parser::stack_symbol_type::stack_symbol_type ()
  {}

  parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_STRING_LITERAL: // STRING_LITERAL
      case symbol_kind::S_CHAR_LITERAL: // CHAR_LITERAL
      case symbol_kind::S_FLOAT_LITERAL: // FLOAT_LITERAL
        value.YY_MOVE_OR_COPY< char* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER_LITERAL: // INTEGER_LITERAL
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_parameter: // parameter
        value.YY_MOVE_OR_COPY< std::pair<std::string, std::string> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_type_specifier: // type_specifier
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_block: // block
        value.YY_MOVE_OR_COPY< std::unique_ptr<BlockNode> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_expression: // expression
        value.YY_MOVE_OR_COPY< std::unique_ptr<ExpressionNode> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement: // statement
        value.YY_MOVE_OR_COPY< std::unique_ptr<StatementNode> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_parameter_list: // parameter_list
        value.YY_MOVE_OR_COPY< std::vector<std::pair<std::string, std::string>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_expression_list: // expression_list
        value.YY_MOVE_OR_COPY< std::vector<std::unique_ptr<ExpressionNode>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement_list: // statement_list
        value.YY_MOVE_OR_COPY< std::vector<std::unique_ptr<StatementNode>> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_STRING_LITERAL: // STRING_LITERAL
      case symbol_kind::S_CHAR_LITERAL: // CHAR_LITERAL
      case symbol_kind::S_FLOAT_LITERAL: // FLOAT_LITERAL
        value.move< char* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_INTEGER_LITERAL: // INTEGER_LITERAL
        value.move< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_parameter: // parameter
        value.move< std::pair<std::string, std::string> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_type_specifier: // type_specifier
        value.move< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_block: // block
        value.move< std::unique_ptr<BlockNode> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_expression: // expression
        value.move< std::unique_ptr<ExpressionNode> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement: // statement
        value.move< std::unique_ptr<StatementNode> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_parameter_list: // parameter_list
        value.move< std::vector<std::pair<std::string, std::string>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_expression_list: // expression_list
        value.move< std::vector<std::unique_ptr<ExpressionNode>> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_statement_list: // statement_list
        value.move< std::vector<std::unique_ptr<StatementNode>> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_STRING_LITERAL: // STRING_LITERAL
      case symbol_kind::S_CHAR_LITERAL: // CHAR_LITERAL
      case symbol_kind::S_FLOAT_LITERAL: // FLOAT_LITERAL
        value.copy< char* > (that.value);
        break;

      case symbol_kind::S_INTEGER_LITERAL: // INTEGER_LITERAL
        value.copy< int > (that.value);
        break;

      case symbol_kind::S_parameter: // parameter
        value.copy< std::pair<std::string, std::string> > (that.value);
        break;

      case symbol_kind::S_type_specifier: // type_specifier
        value.copy< std::string > (that.value);
        break;

      case symbol_kind::S_block: // block
        value.copy< std::unique_ptr<BlockNode> > (that.value);
        break;

      case symbol_kind::S_expression: // expression
        value.copy< std::unique_ptr<ExpressionNode> > (that.value);
        break;

      case symbol_kind::S_statement: // statement
        value.copy< std::unique_ptr<StatementNode> > (that.value);
        break;

      case symbol_kind::S_parameter_list: // parameter_list
        value.copy< std::vector<std::pair<std::string, std::string>> > (that.value);
        break;

      case symbol_kind::S_expression_list: // expression_list
        value.copy< std::vector<std::unique_ptr<ExpressionNode>> > (that.value);
        break;

      case symbol_kind::S_statement_list: // statement_list
        value.copy< std::vector<std::unique_ptr<StatementNode>> > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_STRING_LITERAL: // STRING_LITERAL
      case symbol_kind::S_CHAR_LITERAL: // CHAR_LITERAL
      case symbol_kind::S_FLOAT_LITERAL: // FLOAT_LITERAL
        value.move< char* > (that.value);
        break;

      case symbol_kind::S_INTEGER_LITERAL: // INTEGER_LITERAL
        value.move< int > (that.value);
        break;

      case symbol_kind::S_parameter: // parameter
        value.move< std::pair<std::string, std::string> > (that.value);
        break;

      case symbol_kind::S_type_specifier: // type_specifier
        value.move< std::string > (that.value);
        break;

      case symbol_kind::S_block: // block
        value.move< std::unique_ptr<BlockNode> > (that.value);
        break;

      case symbol_kind::S_expression: // expression
        value.move< std::unique_ptr<ExpressionNode> > (that.value);
        break;

      case symbol_kind::S_statement: // statement
        value.move< std::unique_ptr<StatementNode> > (that.value);
        break;

      case symbol_kind::S_parameter_list: // parameter_list
        value.move< std::vector<std::pair<std::string, std::string>> > (that.value);
        break;

      case symbol_kind::S_expression_list: // expression_list
        value.move< std::vector<std::unique_ptr<ExpressionNode>> > (that.value);
        break;

      case symbol_kind::S_statement_list: // statement_list
        value.move< std::vector<std::unique_ptr<StatementNode>> > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " ("
            << yysym.location << ": ";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  parser::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  parser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  parser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::operator() ()
  {
    return parse ();
  }

  int
  parser::parse ()
  {
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

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            yyla.kind_ = yytranslate_ (yylex (&yyla.value, &yyla.location));
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_STRING_LITERAL: // STRING_LITERAL
      case symbol_kind::S_CHAR_LITERAL: // CHAR_LITERAL
      case symbol_kind::S_FLOAT_LITERAL: // FLOAT_LITERAL
        yylhs.value.emplace< char* > ();
        break;

      case symbol_kind::S_INTEGER_LITERAL: // INTEGER_LITERAL
        yylhs.value.emplace< int > ();
        break;

      case symbol_kind::S_parameter: // parameter
        yylhs.value.emplace< std::pair<std::string, std::string> > ();
        break;

      case symbol_kind::S_type_specifier: // type_specifier
        yylhs.value.emplace< std::string > ();
        break;

      case symbol_kind::S_block: // block
        yylhs.value.emplace< std::unique_ptr<BlockNode> > ();
        break;

      case symbol_kind::S_expression: // expression
        yylhs.value.emplace< std::unique_ptr<ExpressionNode> > ();
        break;

      case symbol_kind::S_statement: // statement
        yylhs.value.emplace< std::unique_ptr<StatementNode> > ();
        break;

      case symbol_kind::S_parameter_list: // parameter_list
        yylhs.value.emplace< std::vector<std::pair<std::string, std::string>> > ();
        break;

      case symbol_kind::S_expression_list: // expression_list
        yylhs.value.emplace< std::vector<std::unique_ptr<ExpressionNode>> > ();
        break;

      case symbol_kind::S_statement_list: // statement_list
        yylhs.value.emplace< std::vector<std::unique_ptr<StatementNode>> > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // program: translation_unit
#line 72 "parser.y"
                     { }
#line 947 "parser.tab.cc"
    break;

  case 3: // translation_unit: %empty
#line 76 "parser.y"
                { }
#line 953 "parser.tab.cc"
    break;

  case 4: // translation_unit: translation_unit function_definition
#line 77 "parser.y"
                                           { }
#line 959 "parser.tab.cc"
    break;

  case 5: // translation_unit: translation_unit declaration
#line 78 "parser.y"
                                   { }
#line 965 "parser.tab.cc"
    break;

  case 6: // function_definition: type_specifier IDENTIFIER '(' parameter_list ')' block
#line 82 "parser.y"
                                                           {
        g_ast.push_back(std::make_unique<FunctionNode>(take_owned_text(yystack_[4].value.as < char* > ()), yystack_[5].value.as < std::string > (), std::move(yystack_[2].value.as < std::vector<std::pair<std::string, std::string>> > ()), std::move(yystack_[0].value.as < std::unique_ptr<BlockNode> > ()), yylineno));
    }
#line 973 "parser.tab.cc"
    break;

  case 7: // function_definition: type_specifier IDENTIFIER '(' ')' block
#line 85 "parser.y"
                                              {
        std::vector<std::pair<std::string, std::string>> params;
        g_ast.push_back(std::make_unique<FunctionNode>(take_owned_text(yystack_[3].value.as < char* > ()), yystack_[4].value.as < std::string > (), std::move(params), std::move(yystack_[0].value.as < std::unique_ptr<BlockNode> > ()), yylineno));
    }
#line 982 "parser.tab.cc"
    break;

  case 8: // parameter_list: parameter
#line 92 "parser.y"
              {
        yylhs.value.as < std::vector<std::pair<std::string, std::string>> > () = std::vector<std::pair<std::string, std::string>>();
        yylhs.value.as < std::vector<std::pair<std::string, std::string>> > ().push_back(yystack_[0].value.as < std::pair<std::string, std::string> > ());
    }
#line 991 "parser.tab.cc"
    break;

  case 9: // parameter_list: parameter_list ',' parameter
#line 96 "parser.y"
                                   {
        yylhs.value.as < std::vector<std::pair<std::string, std::string>> > () = std::move(yystack_[2].value.as < std::vector<std::pair<std::string, std::string>> > ());
        yylhs.value.as < std::vector<std::pair<std::string, std::string>> > ().push_back(yystack_[0].value.as < std::pair<std::string, std::string> > ());
    }
#line 1000 "parser.tab.cc"
    break;

  case 10: // parameter: type_specifier IDENTIFIER
#line 103 "parser.y"
                              {
        yylhs.value.as < std::pair<std::string, std::string> > () = std::make_pair(yystack_[1].value.as < std::string > (), take_owned_text(yystack_[0].value.as < char* > ()));
    }
#line 1008 "parser.tab.cc"
    break;

  case 11: // declaration: type_specifier IDENTIFIER ';'
#line 109 "parser.y"
                                  {
        g_ast.push_back(std::make_unique<VarDeclNode>(yystack_[2].value.as < std::string > (), take_owned_text(yystack_[1].value.as < char* > ()), nullptr, yylineno));
    }
#line 1016 "parser.tab.cc"
    break;

  case 12: // declaration: type_specifier IDENTIFIER '=' expression ';'
#line 112 "parser.y"
                                                   {
        g_ast.push_back(std::make_unique<VarDeclNode>(yystack_[4].value.as < std::string > (), take_owned_text(yystack_[3].value.as < char* > ()), std::move(yystack_[1].value.as < std::unique_ptr<ExpressionNode> > ()), yylineno));
    }
#line 1024 "parser.tab.cc"
    break;

  case 13: // type_specifier: INT
#line 118 "parser.y"
        { yylhs.value.as < std::string > () = "int"; }
#line 1030 "parser.tab.cc"
    break;

  case 14: // type_specifier: CHAR
#line 119 "parser.y"
           { yylhs.value.as < std::string > () = "char"; }
#line 1036 "parser.tab.cc"
    break;

  case 15: // type_specifier: FLOAT_TYPE
#line 120 "parser.y"
                 { yylhs.value.as < std::string > () = "float"; }
#line 1042 "parser.tab.cc"
    break;

  case 16: // type_specifier: DOUBLE
#line 121 "parser.y"
             { yylhs.value.as < std::string > () = "double"; }
#line 1048 "parser.tab.cc"
    break;

  case 17: // type_specifier: VOID
#line 122 "parser.y"
           { yylhs.value.as < std::string > () = "void"; }
#line 1054 "parser.tab.cc"
    break;

  case 18: // statement: expression ';'
#line 126 "parser.y"
                   {
        // Expression statement - ignore result
        yylhs.value.as < std::unique_ptr<StatementNode> > () = nullptr;
    }
#line 1063 "parser.tab.cc"
    break;

  case 19: // statement: block
#line 130 "parser.y"
            {
        yylhs.value.as < std::unique_ptr<StatementNode> > () = std::move(yystack_[0].value.as < std::unique_ptr<BlockNode> > ());
    }
#line 1071 "parser.tab.cc"
    break;

  case 20: // statement: IF '(' expression ')' statement
#line 133 "parser.y"
                                      {
        std::unique_ptr<BlockNode> thenBlock = std::make_unique<BlockNode>(std::vector<std::unique_ptr<StatementNode>>());
        if (yystack_[0].value.as < std::unique_ptr<StatementNode> > ()) {
            thenBlock->getStatements().push_back(std::move(yystack_[0].value.as < std::unique_ptr<StatementNode> > ()));
        }
        yylhs.value.as < std::unique_ptr<StatementNode> > () = std::make_unique<IfNode>(std::move(yystack_[2].value.as < std::unique_ptr<ExpressionNode> > ()), std::move(thenBlock), nullptr, yylineno);
    }
#line 1083 "parser.tab.cc"
    break;

  case 21: // statement: IF '(' expression ')' statement ELSE statement
#line 140 "parser.y"
                                                     {
        std::unique_ptr<BlockNode> thenBlock = std::make_unique<BlockNode>(std::vector<std::unique_ptr<StatementNode>>());
        std::unique_ptr<BlockNode> elseBlock = std::make_unique<BlockNode>(std::vector<std::unique_ptr<StatementNode>>());
        if (yystack_[2].value.as < std::unique_ptr<StatementNode> > ()) {
            thenBlock->getStatements().push_back(std::move(yystack_[2].value.as < std::unique_ptr<StatementNode> > ()));
        }
        if (yystack_[0].value.as < std::unique_ptr<StatementNode> > ()) {
            elseBlock->getStatements().push_back(std::move(yystack_[0].value.as < std::unique_ptr<StatementNode> > ()));
        }
        yylhs.value.as < std::unique_ptr<StatementNode> > () = std::make_unique<IfNode>(std::move(yystack_[4].value.as < std::unique_ptr<ExpressionNode> > ()), std::move(thenBlock), std::move(elseBlock), yylineno);
    }
#line 1099 "parser.tab.cc"
    break;

  case 22: // statement: WHILE '(' expression ')' statement
#line 151 "parser.y"
                                         {
        std::unique_ptr<BlockNode> body = std::make_unique<BlockNode>(std::vector<std::unique_ptr<StatementNode>>());
        if (yystack_[0].value.as < std::unique_ptr<StatementNode> > ()) {
            body->getStatements().push_back(std::move(yystack_[0].value.as < std::unique_ptr<StatementNode> > ()));
        }
        yylhs.value.as < std::unique_ptr<StatementNode> > () = std::make_unique<WhileNode>(std::move(yystack_[2].value.as < std::unique_ptr<ExpressionNode> > ()), std::move(body), yylineno);
    }
#line 1111 "parser.tab.cc"
    break;

  case 23: // statement: FOR '(' expression ';' expression ';' expression ')' statement
#line 158 "parser.y"
                                                                     {
        std::unique_ptr<BlockNode> body = std::make_unique<BlockNode>(std::vector<std::unique_ptr<StatementNode>>());
        if (yystack_[0].value.as < std::unique_ptr<StatementNode> > ()) {
            body->getStatements().push_back(std::move(yystack_[0].value.as < std::unique_ptr<StatementNode> > ()));
        }
        yylhs.value.as < std::unique_ptr<StatementNode> > () = std::make_unique<ForNode>(nullptr, std::move(yystack_[4].value.as < std::unique_ptr<ExpressionNode> > ()), std::move(yystack_[2].value.as < std::unique_ptr<ExpressionNode> > ()), std::move(body), yylineno);
    }
#line 1123 "parser.tab.cc"
    break;

  case 24: // statement: RETURN ';'
#line 165 "parser.y"
                 {
        yylhs.value.as < std::unique_ptr<StatementNode> > () = std::make_unique<ReturnNode>(nullptr, yylineno);
    }
#line 1131 "parser.tab.cc"
    break;

  case 25: // statement: RETURN expression ';'
#line 168 "parser.y"
                            {
        yylhs.value.as < std::unique_ptr<StatementNode> > () = std::make_unique<ReturnNode>(std::move(yystack_[1].value.as < std::unique_ptr<ExpressionNode> > ()), yylineno);
    }
#line 1139 "parser.tab.cc"
    break;

  case 26: // statement: BREAK ';'
#line 171 "parser.y"
                {
        yylhs.value.as < std::unique_ptr<StatementNode> > () = std::make_unique<BreakNode>(yylineno);
    }
#line 1147 "parser.tab.cc"
    break;

  case 27: // statement: CONTINUE ';'
#line 174 "parser.y"
                   {
        yylhs.value.as < std::unique_ptr<StatementNode> > () = std::make_unique<ContinueNode>(yylineno);
    }
#line 1155 "parser.tab.cc"
    break;

  case 28: // statement: type_specifier IDENTIFIER ';'
#line 177 "parser.y"
                                    {
        yylhs.value.as < std::unique_ptr<StatementNode> > () = std::make_unique<VarDeclNode>(yystack_[2].value.as < std::string > (), take_owned_text(yystack_[1].value.as < char* > ()), nullptr, yylineno);
    }
#line 1163 "parser.tab.cc"
    break;

  case 29: // statement: type_specifier IDENTIFIER '=' expression ';'
#line 180 "parser.y"
                                                   {
        yylhs.value.as < std::unique_ptr<StatementNode> > () = std::make_unique<VarDeclNode>(yystack_[4].value.as < std::string > (), take_owned_text(yystack_[3].value.as < char* > ()), std::move(yystack_[1].value.as < std::unique_ptr<ExpressionNode> > ()), yylineno);
    }
#line 1171 "parser.tab.cc"
    break;

  case 30: // statement: IDENTIFIER '=' expression ';'
#line 183 "parser.y"
                                    {
        yylhs.value.as < std::unique_ptr<StatementNode> > () = std::make_unique<AssignNode>(take_owned_text(yystack_[3].value.as < char* > ()), std::move(yystack_[1].value.as < std::unique_ptr<ExpressionNode> > ()), yylineno);
    }
#line 1179 "parser.tab.cc"
    break;

  case 31: // block: '{' statement_list '}'
#line 189 "parser.y"
                           {
        yylhs.value.as < std::unique_ptr<BlockNode> > () = std::make_unique<BlockNode>(std::move(yystack_[1].value.as < std::vector<std::unique_ptr<StatementNode>> > ()), yylineno);
    }
#line 1187 "parser.tab.cc"
    break;

  case 32: // block: '{' '}'
#line 192 "parser.y"
              {
        yylhs.value.as < std::unique_ptr<BlockNode> > () = std::make_unique<BlockNode>(std::vector<std::unique_ptr<StatementNode>>(), yylineno);
    }
#line 1195 "parser.tab.cc"
    break;

  case 33: // statement_list: statement
#line 198 "parser.y"
              {
        yylhs.value.as < std::vector<std::unique_ptr<StatementNode>> > () = std::vector<std::unique_ptr<StatementNode>>();
        if (yystack_[0].value.as < std::unique_ptr<StatementNode> > ()) {
            yylhs.value.as < std::vector<std::unique_ptr<StatementNode>> > ().push_back(std::move(yystack_[0].value.as < std::unique_ptr<StatementNode> > ()));
        }
    }
#line 1206 "parser.tab.cc"
    break;

  case 34: // statement_list: statement_list statement
#line 204 "parser.y"
                               {
        yylhs.value.as < std::vector<std::unique_ptr<StatementNode>> > () = std::move(yystack_[1].value.as < std::vector<std::unique_ptr<StatementNode>> > ());
        if (yystack_[0].value.as < std::unique_ptr<StatementNode> > ()) {
            yylhs.value.as < std::vector<std::unique_ptr<StatementNode>> > ().push_back(std::move(yystack_[0].value.as < std::unique_ptr<StatementNode> > ()));
        }
    }
#line 1217 "parser.tab.cc"
    break;

  case 35: // expression: INTEGER_LITERAL
#line 213 "parser.y"
                    {
        yylhs.value.as < std::unique_ptr<ExpressionNode> > () = std::make_unique<LiteralNode>(std::to_string(yystack_[0].value.as < int > ()), "int", yylineno);
    }
#line 1225 "parser.tab.cc"
    break;

  case 36: // expression: FLOAT_LITERAL
#line 216 "parser.y"
                    {
        yylhs.value.as < std::unique_ptr<ExpressionNode> > () = std::make_unique<LiteralNode>(take_owned_text(yystack_[0].value.as < char* > ()), "float", yylineno);
    }
#line 1233 "parser.tab.cc"
    break;

  case 37: // expression: STRING_LITERAL
#line 219 "parser.y"
                     {
        yylhs.value.as < std::unique_ptr<ExpressionNode> > () = std::make_unique<LiteralNode>(take_owned_text(yystack_[0].value.as < char* > ()), "string", yylineno);
    }
#line 1241 "parser.tab.cc"
    break;

  case 38: // expression: CHAR_LITERAL
#line 222 "parser.y"
                   {
        yylhs.value.as < std::unique_ptr<ExpressionNode> > () = std::make_unique<LiteralNode>(take_owned_text(yystack_[0].value.as < char* > ()), "char", yylineno);
    }
#line 1249 "parser.tab.cc"
    break;

  case 39: // expression: IDENTIFIER
#line 225 "parser.y"
                 {
        yylhs.value.as < std::unique_ptr<ExpressionNode> > () = std::make_unique<IdentifierNode>(take_owned_text(yystack_[0].value.as < char* > ()), yylineno);
    }
#line 1257 "parser.tab.cc"
    break;

  case 40: // expression: IDENTIFIER '(' expression_list ')'
#line 228 "parser.y"
                                         {
        yylhs.value.as < std::unique_ptr<ExpressionNode> > () = std::make_unique<CallNode>(take_owned_text(yystack_[3].value.as < char* > ()), std::move(yystack_[1].value.as < std::vector<std::unique_ptr<ExpressionNode>> > ()), yylineno);
    }
#line 1265 "parser.tab.cc"
    break;

  case 41: // expression: IDENTIFIER '(' ')'
#line 231 "parser.y"
                         {
        std::vector<std::unique_ptr<ExpressionNode>> args;
        yylhs.value.as < std::unique_ptr<ExpressionNode> > () = std::make_unique<CallNode>(take_owned_text(yystack_[2].value.as < char* > ()), std::move(args), yylineno);
    }
#line 1274 "parser.tab.cc"
    break;

  case 42: // expression: '(' expression ')'
#line 235 "parser.y"
                         {
        yylhs.value.as < std::unique_ptr<ExpressionNode> > () = std::move(yystack_[1].value.as < std::unique_ptr<ExpressionNode> > ());
    }
#line 1282 "parser.tab.cc"
    break;

  case 43: // expression: expression '+' expression
#line 238 "parser.y"
                                {
        yylhs.value.as < std::unique_ptr<ExpressionNode> > () = std::make_unique<BinaryOpNode>("+", std::move(yystack_[2].value.as < std::unique_ptr<ExpressionNode> > ()), std::move(yystack_[0].value.as < std::unique_ptr<ExpressionNode> > ()), yylineno);
    }
#line 1290 "parser.tab.cc"
    break;

  case 44: // expression: expression '-' expression
#line 241 "parser.y"
                                {
        yylhs.value.as < std::unique_ptr<ExpressionNode> > () = std::make_unique<BinaryOpNode>("-", std::move(yystack_[2].value.as < std::unique_ptr<ExpressionNode> > ()), std::move(yystack_[0].value.as < std::unique_ptr<ExpressionNode> > ()), yylineno);
    }
#line 1298 "parser.tab.cc"
    break;

  case 45: // expression: expression '*' expression
#line 244 "parser.y"
                                {
        yylhs.value.as < std::unique_ptr<ExpressionNode> > () = std::make_unique<BinaryOpNode>("*", std::move(yystack_[2].value.as < std::unique_ptr<ExpressionNode> > ()), std::move(yystack_[0].value.as < std::unique_ptr<ExpressionNode> > ()), yylineno);
    }
#line 1306 "parser.tab.cc"
    break;

  case 46: // expression: expression '/' expression
#line 247 "parser.y"
                                {
        yylhs.value.as < std::unique_ptr<ExpressionNode> > () = std::make_unique<BinaryOpNode>("/", std::move(yystack_[2].value.as < std::unique_ptr<ExpressionNode> > ()), std::move(yystack_[0].value.as < std::unique_ptr<ExpressionNode> > ()), yylineno);
    }
#line 1314 "parser.tab.cc"
    break;

  case 47: // expression: expression '%' expression
#line 250 "parser.y"
                                {
        yylhs.value.as < std::unique_ptr<ExpressionNode> > () = std::make_unique<BinaryOpNode>("%", std::move(yystack_[2].value.as < std::unique_ptr<ExpressionNode> > ()), std::move(yystack_[0].value.as < std::unique_ptr<ExpressionNode> > ()), yylineno);
    }
#line 1322 "parser.tab.cc"
    break;

  case 48: // expression: expression EQ expression
#line 253 "parser.y"
                               {
        yylhs.value.as < std::unique_ptr<ExpressionNode> > () = std::make_unique<BinaryOpNode>("==", std::move(yystack_[2].value.as < std::unique_ptr<ExpressionNode> > ()), std::move(yystack_[0].value.as < std::unique_ptr<ExpressionNode> > ()), yylineno);
    }
#line 1330 "parser.tab.cc"
    break;

  case 49: // expression: expression NE expression
#line 256 "parser.y"
                               {
        yylhs.value.as < std::unique_ptr<ExpressionNode> > () = std::make_unique<BinaryOpNode>("!=", std::move(yystack_[2].value.as < std::unique_ptr<ExpressionNode> > ()), std::move(yystack_[0].value.as < std::unique_ptr<ExpressionNode> > ()), yylineno);
    }
#line 1338 "parser.tab.cc"
    break;

  case 50: // expression: expression '<' expression
#line 259 "parser.y"
                                {
        yylhs.value.as < std::unique_ptr<ExpressionNode> > () = std::make_unique<BinaryOpNode>("<", std::move(yystack_[2].value.as < std::unique_ptr<ExpressionNode> > ()), std::move(yystack_[0].value.as < std::unique_ptr<ExpressionNode> > ()), yylineno);
    }
#line 1346 "parser.tab.cc"
    break;

  case 51: // expression: expression '>' expression
#line 262 "parser.y"
                                {
        yylhs.value.as < std::unique_ptr<ExpressionNode> > () = std::make_unique<BinaryOpNode>(">", std::move(yystack_[2].value.as < std::unique_ptr<ExpressionNode> > ()), std::move(yystack_[0].value.as < std::unique_ptr<ExpressionNode> > ()), yylineno);
    }
#line 1354 "parser.tab.cc"
    break;

  case 52: // expression: expression LE expression
#line 265 "parser.y"
                               {
        yylhs.value.as < std::unique_ptr<ExpressionNode> > () = std::make_unique<BinaryOpNode>("<=", std::move(yystack_[2].value.as < std::unique_ptr<ExpressionNode> > ()), std::move(yystack_[0].value.as < std::unique_ptr<ExpressionNode> > ()), yylineno);
    }
#line 1362 "parser.tab.cc"
    break;

  case 53: // expression: expression GE expression
#line 268 "parser.y"
                               {
        yylhs.value.as < std::unique_ptr<ExpressionNode> > () = std::make_unique<BinaryOpNode>(">=", std::move(yystack_[2].value.as < std::unique_ptr<ExpressionNode> > ()), std::move(yystack_[0].value.as < std::unique_ptr<ExpressionNode> > ()), yylineno);
    }
#line 1370 "parser.tab.cc"
    break;

  case 54: // expression: expression AND expression
#line 271 "parser.y"
                                {
        yylhs.value.as < std::unique_ptr<ExpressionNode> > () = std::make_unique<BinaryOpNode>("&&", std::move(yystack_[2].value.as < std::unique_ptr<ExpressionNode> > ()), std::move(yystack_[0].value.as < std::unique_ptr<ExpressionNode> > ()), yylineno);
    }
#line 1378 "parser.tab.cc"
    break;

  case 55: // expression: expression OR expression
#line 274 "parser.y"
                               {
        yylhs.value.as < std::unique_ptr<ExpressionNode> > () = std::make_unique<BinaryOpNode>("||", std::move(yystack_[2].value.as < std::unique_ptr<ExpressionNode> > ()), std::move(yystack_[0].value.as < std::unique_ptr<ExpressionNode> > ()), yylineno);
    }
#line 1386 "parser.tab.cc"
    break;

  case 56: // expression: '!' expression
#line 277 "parser.y"
                     {
        yylhs.value.as < std::unique_ptr<ExpressionNode> > () = std::make_unique<UnaryOpNode>("!", std::move(yystack_[0].value.as < std::unique_ptr<ExpressionNode> > ()), yylineno);
    }
#line 1394 "parser.tab.cc"
    break;

  case 57: // expression: '-' expression
#line 280 "parser.y"
                     {
        yylhs.value.as < std::unique_ptr<ExpressionNode> > () = std::make_unique<UnaryOpNode>("-", std::move(yystack_[0].value.as < std::unique_ptr<ExpressionNode> > ()), yylineno);
    }
#line 1402 "parser.tab.cc"
    break;

  case 58: // expression: '+' expression
#line 283 "parser.y"
                     {
        yylhs.value.as < std::unique_ptr<ExpressionNode> > () = std::make_unique<UnaryOpNode>("+", std::move(yystack_[0].value.as < std::unique_ptr<ExpressionNode> > ()), yylineno);
    }
#line 1410 "parser.tab.cc"
    break;

  case 59: // expression: expression '=' expression
#line 286 "parser.y"
                                {
        yylhs.value.as < std::unique_ptr<ExpressionNode> > () = std::make_unique<BinaryOpNode>("=", std::move(yystack_[2].value.as < std::unique_ptr<ExpressionNode> > ()), std::move(yystack_[0].value.as < std::unique_ptr<ExpressionNode> > ()), yylineno);
    }
#line 1418 "parser.tab.cc"
    break;

  case 60: // expression_list: expression
#line 292 "parser.y"
               {
        yylhs.value.as < std::vector<std::unique_ptr<ExpressionNode>> > () = std::vector<std::unique_ptr<ExpressionNode>>();
        yylhs.value.as < std::vector<std::unique_ptr<ExpressionNode>> > ().push_back(std::move(yystack_[0].value.as < std::unique_ptr<ExpressionNode> > ()));
    }
#line 1427 "parser.tab.cc"
    break;

  case 61: // expression_list: expression_list ',' expression
#line 296 "parser.y"
                                     {
        yylhs.value.as < std::vector<std::unique_ptr<ExpressionNode>> > () = std::move(yystack_[2].value.as < std::vector<std::unique_ptr<ExpressionNode>> > ());
        yylhs.value.as < std::vector<std::unique_ptr<ExpressionNode>> > ().push_back(std::move(yystack_[0].value.as < std::unique_ptr<ExpressionNode> > ()));
    }
#line 1436 "parser.tab.cc"
    break;


#line 1440 "parser.tab.cc"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
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
        context yyctx (*this, yyla);
        std::string msg = yysyntax_error_ (yyctx);
        error (yyla.location, YY_MOVE (msg));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
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
    {
      stack_symbol_type error_token;

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr;
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
              else
                goto append;

            append:
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

  std::string
  parser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytnamerr_ (yytname_[yysymbol]);
  }



  // parser::context.
  parser::context::context (const parser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  parser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
  {
    // Actual number of expected tokens
    int yycount = 0;

    const int yyn = yypact_[+yyparser_.yystack_[0].state];
    if (!yy_pact_value_is_default_ (yyn))
      {
        /* Start YYX at -YYN if negative to avoid negative indexes in
           YYCHECK.  In other words, skip the first -YYN actions for
           this state because they are default actions.  */
        const int yyxbegin = yyn < 0 ? -yyn : 0;
        // Stay within bounds of both yycheck and yytname.
        const int yychecklim = yylast_ - yyn + 1;
        const int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
        for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
          if (yycheck_[yyx + yyn] == yyx && yyx != symbol_kind::S_YYerror
              && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
            {
              if (!yyarg)
                ++yycount;
              else if (yycount == yyargn)
                return 0;
              else
                yyarg[yycount++] = YY_CAST (symbol_kind_type, yyx);
            }
      }

    if (yyarg && yycount == 0 && 0 < yyargn)
      yyarg[0] = symbol_kind::S_YYEMPTY;
    return yycount;
  }






  int
  parser::yy_syntax_error_arguments_ (const context& yyctx,
                                                 symbol_kind_type yyarg[], int yyargn) const
  {
    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
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
         scanner and before detecting a syntax error.  Thus, state merging
         (from LALR or IELR) and default reductions corrupt the expected
         token list.  However, the list is correct for canonical LR with
         one exception: it will still contain any token that will not be
         accepted due to an error action in a later state.
    */

    if (!yyctx.lookahead ().empty ())
      {
        if (yyarg)
          yyarg[0] = yyctx.token ();
        int yyn = yyctx.expected_tokens (yyarg ? yyarg + 1 : yyarg, yyargn - 1);
        return yyn + 1;
      }
    return 0;
  }

  // Generate an error message.
  std::string
  parser::yysyntax_error_ (const context& yyctx) const
  {
    // Its maximum.
    enum { YYARGS_MAX = 5 };
    // Arguments of yyformat.
    symbol_kind_type yyarg[YYARGS_MAX];
    int yycount = yy_syntax_error_arguments_ (yyctx, yyarg, YYARGS_MAX);

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += symbol_name (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char parser::yypact_ninf_ = -43;

  const signed char parser::yytable_ninf_ = -1;

  const short
  parser::yypact_[] =
  {
     -43,     6,   215,   -43,   -43,   -43,   -43,   -43,   -43,   -43,
     -43,    17,    -1,   146,    -7,   -43,   -43,   -26,   -43,   -43,
     -43,   146,   146,   146,   146,   179,    18,    -9,   -43,    68,
     139,    13,    13,    26,   438,   146,   146,   146,   146,   146,
     146,   146,   146,   146,   146,   146,   146,   146,   146,   -43,
      34,   -43,    18,   215,   -43,   -43,   536,    19,   -43,   162,
     162,   162,   162,   577,   546,   162,   162,    13,    13,    26,
      26,    26,    26,   -42,    28,    29,    32,     9,    31,    33,
     -43,    94,   -43,   -43,   105,   216,   -43,   -43,   -43,   146,
     146,   146,   146,   146,   -43,   253,   -43,   -43,     0,   -43,
     -43,   -43,   536,   290,   452,   487,   327,   -43,   146,   -43,
     -43,   122,   122,   146,   364,    89,   -43,   401,   -43,   122,
     146,   -43,   501,   122,   -43
  };

  const signed char
  parser::yydefact_[] =
  {
       3,     0,     2,     1,    13,    14,    15,    16,    17,     4,
       5,     0,     0,     0,     0,    11,    35,    39,    37,    38,
      36,     0,     0,     0,     0,     0,     0,     0,     8,     0,
       0,    58,    57,    56,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    12,
       0,     7,     0,     0,    10,    41,    60,     0,    42,    48,
      49,    52,    53,    54,    55,    50,    51,    43,    44,    45,
      46,    47,    59,    39,     0,     0,     0,     0,     0,     0,
      32,     0,    33,    19,     0,     0,     6,     9,    40,     0,
       0,     0,     0,     0,    24,     0,    26,    27,     0,    31,
      34,    18,    61,     0,     0,     0,     0,    25,     0,    28,
      30,     0,     0,     0,     0,    20,    22,     0,    29,     0,
       0,    21,     0,     0,    23
  };

  const signed char
  parser::yypgoto_[] =
  {
     -43,   -43,   -43,   -43,   -43,    51,   -43,     5,   -30,    22,
     -43,   -13,   -43
  };

  const signed char
  parser::yydefgoto_[] =
  {
       0,     1,     2,     9,    27,    28,    10,    81,    82,    83,
      84,    85,    57
  };

  const signed char
  parser::yytable_[] =
  {
      25,     4,     5,     6,     7,     8,     3,    11,    31,    32,
      33,    34,    16,    17,    18,    19,    20,    56,    90,    29,
      30,    12,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    30,    16,    73,    18,
      19,    20,     4,     5,     6,     7,     8,    74,    51,    75,
      76,    77,    78,    79,   100,    52,    53,    26,    29,    13,
     108,    14,    21,    22,    95,    15,   109,    23,    45,    46,
      47,    24,    54,    48,    86,    94,   102,   103,   104,   105,
     106,   115,   116,    88,    89,    50,    48,    21,    22,   121,
      91,    92,    23,   124,    93,   114,    24,    96,    98,    97,
     117,    50,    80,   119,    87,     0,     0,   122,    16,    73,
      18,    19,    20,     4,     5,     6,     7,     8,    74,     0,
      75,    76,    77,    78,    79,    16,    73,    18,    19,    20,
       4,     5,     6,     7,     8,    74,     0,    75,    76,    77,
      78,    79,    16,    17,    18,    19,    20,     0,     0,    16,
      17,    18,    19,    20,     0,     0,     0,     0,    21,    22,
       0,     0,     0,    23,     0,     0,     0,    24,     0,     0,
       0,     0,    50,    99,     0,    21,    22,     0,     0,     0,
      23,     0,     0,     0,    24,     0,     0,     0,     0,    50,
       0,     0,    21,    22,     0,     0,     0,    23,     0,    21,
      22,    24,     0,    55,    23,     0,     0,     0,    24,    35,
      36,    37,    38,    39,    40,    43,    44,    45,    46,    47,
       0,     0,    48,     4,     5,     6,     7,     8,     0,     0,
      41,    42,    43,    44,    45,    46,    47,     0,     0,    48,
       0,     0,     0,     0,     0,    49,    35,    36,    37,    38,
      39,    40,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    41,    42,    43,
      44,    45,    46,    47,     0,     0,    48,     0,     0,     0,
       0,     0,   101,    35,    36,    37,    38,    39,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    41,    42,    43,    44,    45,    46,
      47,     0,     0,    48,     0,     0,     0,     0,     0,   107,
      35,    36,    37,    38,    39,    40,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    41,    42,    43,    44,    45,    46,    47,     0,     0,
      48,     0,     0,     0,     0,     0,   110,    35,    36,    37,
      38,    39,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    41,    42,
      43,    44,    45,    46,    47,     0,     0,    48,     0,     0,
       0,     0,     0,   113,    35,    36,    37,    38,    39,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    41,    42,    43,    44,    45,
      46,    47,     0,     0,    48,     0,     0,     0,     0,     0,
     118,    35,    36,    37,    38,    39,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    41,    42,    43,    44,    45,    46,    47,     0,
       0,    48,     0,     0,     0,     0,     0,   120,    35,    36,
      37,    38,    39,    40,     0,     0,     0,     0,     0,     0,
       0,     0,    35,    36,    37,    38,    39,    40,     0,    41,
      42,    43,    44,    45,    46,    47,     0,     0,    48,     0,
       0,     0,    58,    41,    42,    43,    44,    45,    46,    47,
       0,     0,    48,     0,     0,     0,   111,    35,    36,    37,
      38,    39,    40,     0,     0,     0,     0,     0,     0,     0,
       0,    35,    36,    37,    38,    39,    40,     0,    41,    42,
      43,    44,    45,    46,    47,     0,     0,    48,     0,     0,
       0,   112,    41,    42,    43,    44,    45,    46,    47,     0,
       0,    48,     0,     0,     0,   123,    35,    36,    37,    38,
      39,    40,     0,     0,     0,     0,    35,    36,    37,    38,
      39,     0,     0,     0,     0,     0,     0,    41,    42,    43,
      44,    45,    46,    47,     0,     0,    48,    41,    42,    43,
      44,    45,    46,    47,     0,     0,    48,    35,    36,    37,
      38,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    41,    42,
      43,    44,    45,    46,    47,     0,     0,    48
  };

  const signed char
  parser::yycheck_[] =
  {
      13,     8,     9,    10,    11,    12,     0,     2,    21,    22,
      23,    24,     3,     4,     5,     6,     7,    30,    60,    14,
      62,     4,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    62,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    26,    15,
      16,    17,    18,    19,    84,    64,    65,    64,    53,    60,
      60,    62,    53,    54,    77,    66,    66,    58,    55,    56,
      57,    62,     4,    60,    52,    66,    89,    90,    91,    92,
      93,   111,   112,    64,    65,    67,    60,    53,    54,   119,
      62,    62,    58,   123,    62,   108,    62,    66,     4,    66,
     113,    67,    68,    14,    53,    -1,    -1,   120,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    -1,
      15,    16,    17,    18,    19,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    -1,    15,    16,    17,
      18,    19,     3,     4,     5,     6,     7,    -1,    -1,     3,
       4,     5,     6,     7,    -1,    -1,    -1,    -1,    53,    54,
      -1,    -1,    -1,    58,    -1,    -1,    -1,    62,    -1,    -1,
      -1,    -1,    67,    68,    -1,    53,    54,    -1,    -1,    -1,
      58,    -1,    -1,    -1,    62,    -1,    -1,    -1,    -1,    67,
      -1,    -1,    53,    54,    -1,    -1,    -1,    58,    -1,    53,
      54,    62,    -1,    64,    58,    -1,    -1,    -1,    62,    30,
      31,    32,    33,    34,    35,    53,    54,    55,    56,    57,
      -1,    -1,    60,     8,     9,    10,    11,    12,    -1,    -1,
      51,    52,    53,    54,    55,    56,    57,    -1,    -1,    60,
      -1,    -1,    -1,    -1,    -1,    66,    30,    31,    32,    33,
      34,    35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    51,    52,    53,
      54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,
      -1,    -1,    66,    30,    31,    32,    33,    34,    35,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    51,    52,    53,    54,    55,    56,
      57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    66,
      30,    31,    32,    33,    34,    35,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    51,    52,    53,    54,    55,    56,    57,    -1,    -1,
      60,    -1,    -1,    -1,    -1,    -1,    66,    30,    31,    32,
      33,    34,    35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    51,    52,
      53,    54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    66,    30,    31,    32,    33,    34,    35,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    51,    52,    53,    54,    55,
      56,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,
      66,    30,    31,    32,    33,    34,    35,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    51,    52,    53,    54,    55,    56,    57,    -1,
      -1,    60,    -1,    -1,    -1,    -1,    -1,    66,    30,    31,
      32,    33,    34,    35,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    30,    31,    32,    33,    34,    35,    -1,    51,
      52,    53,    54,    55,    56,    57,    -1,    -1,    60,    -1,
      -1,    -1,    64,    51,    52,    53,    54,    55,    56,    57,
      -1,    -1,    60,    -1,    -1,    -1,    64,    30,    31,    32,
      33,    34,    35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    31,    32,    33,    34,    35,    -1,    51,    52,
      53,    54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,
      -1,    64,    51,    52,    53,    54,    55,    56,    57,    -1,
      -1,    60,    -1,    -1,    -1,    64,    30,    31,    32,    33,
      34,    35,    -1,    -1,    -1,    -1,    30,    31,    32,    33,
      34,    -1,    -1,    -1,    -1,    -1,    -1,    51,    52,    53,
      54,    55,    56,    57,    -1,    -1,    60,    51,    52,    53,
      54,    55,    56,    57,    -1,    -1,    60,    30,    31,    32,
      33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    51,    52,
      53,    54,    55,    56,    57,    -1,    -1,    60
  };

  const signed char
  parser::yystos_[] =
  {
       0,    70,    71,     0,     8,     9,    10,    11,    12,    72,
      75,    76,     4,    60,    62,    66,     3,     4,     5,     6,
       7,    53,    54,    58,    62,    80,    64,    73,    74,    76,
      62,    80,    80,    80,    80,    30,    31,    32,    33,    34,
      35,    51,    52,    53,    54,    55,    56,    57,    60,    66,
      67,    78,    64,    65,     4,    64,    80,    81,    64,    80,
      80,    80,    80,    80,    80,    80,    80,    80,    80,    80,
      80,    80,    80,     4,    13,    15,    16,    17,    18,    19,
      68,    76,    77,    78,    79,    80,    78,    74,    64,    65,
      60,    62,    62,    62,    66,    80,    66,    66,     4,    68,
      77,    66,    80,    80,    80,    80,    80,    66,    60,    66,
      66,    64,    64,    66,    80,    77,    77,    80,    66,    14,
      66,    77,    80,    64,    77
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    69,    70,    71,    71,    71,    72,    72,    73,    73,
      74,    75,    75,    76,    76,    76,    76,    76,    77,    77,
      77,    77,    77,    77,    77,    77,    77,    77,    77,    77,
      77,    78,    78,    79,    79,    80,    80,    80,    80,    80,
      80,    80,    80,    80,    80,    80,    80,    80,    80,    80,
      80,    80,    80,    80,    80,    80,    80,    80,    80,    80,
      81,    81
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     1,     0,     2,     2,     6,     5,     1,     3,
       2,     3,     5,     1,     1,     1,     1,     1,     2,     1,
       5,     7,     5,     9,     2,     3,     2,     2,     3,     5,
       4,     3,     2,     1,     2,     1,     1,     1,     1,     1,
       4,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     2,     2,     3,
       1,     3
  };


#if YYDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "INTEGER_LITERAL",
  "IDENTIFIER", "STRING_LITERAL", "CHAR_LITERAL", "FLOAT_LITERAL", "INT",
  "CHAR", "FLOAT_TYPE", "DOUBLE", "VOID", "IF", "ELSE", "WHILE", "FOR",
  "RETURN", "BREAK", "CONTINUE", "DO", "SWITCH", "CASE", "DEFAULT",
  "STRUCT", "TYPEDEF", "CONST", "STATIC", "EXTERN", "SIZEOF", "EQ", "NE",
  "LE", "GE", "AND", "OR", "INC", "DEC", "ADD_ASSIGN", "SUB_ASSIGN",
  "MUL_ASSIGN", "DIV_ASSIGN", "MOD_ASSIGN", "LSHIFT_ASSIGN",
  "RSHIFT_ASSIGN", "AND_ASSIGN", "OR_ASSIGN", "XOR_ASSIGN", "LSHIFT",
  "RSHIFT", "ARROW", "'<'", "'>'", "'+'", "'-'", "'*'", "'/'", "'%'",
  "'!'", "'~'", "'='", "'.'", "'('", "'['", "')'", "','", "';'", "'{'",
  "'}'", "$accept", "program", "translation_unit", "function_definition",
  "parameter_list", "parameter", "declaration", "type_specifier",
  "statement", "block", "statement_list", "expression", "expression_list", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const short
  parser::yyrline_[] =
  {
       0,    72,    72,    76,    77,    78,    82,    85,    92,    96,
     103,   109,   112,   118,   119,   120,   121,   122,   126,   130,
     133,   140,   151,   158,   165,   168,   171,   174,   177,   180,
     183,   189,   192,   198,   204,   213,   216,   219,   222,   225,
     228,   231,   235,   238,   241,   244,   247,   250,   253,   256,
     259,   262,   265,   268,   271,   274,   277,   280,   283,   286,
     292,   296
  };

  void
  parser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  parser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  parser::symbol_kind_type
  parser::yytranslate_ (int t) YY_NOEXCEPT
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const signed char
    translate_table[] =
    {
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    58,     2,     2,     2,    57,     2,     2,
      62,    64,    55,    53,    65,    54,    61,    56,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    66,
      51,    60,    52,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    63,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    67,     2,    68,    59,     2,     2,     2,
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
      45,    46,    47,    48,    49,    50
    };
    // Last valid token kind.
    const int code_max = 305;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

} // yy
#line 2143 "parser.tab.cc"

#line 302 "parser.y"


void yy::parser::error(const location_type& loc, const std::string& msg) {
    std::cerr << "Error at line " << loc.begin.line << ": " << msg << std::endl;
}

