/*tokens*/
%token TOK_LPAREN "`(`"
%token TOK_RPAREN "`)`"
%token TOK_COMMA "`,`"
%token TOK_IDENTIFIER
%token TOK_INT64


%union AttributeUnion {
  vector<Expression*>* array_Expression;
  Expression* type_Expression;
  identifier* type_identifier;
  int64_t type_int;
}

/*nonterminal types*/
%type <type_identifier> TOK_IDENTIFIER
%type <type_int> TOK_INT64
%type <array_Expression> array0_Expression__1
%type <type_Expression> class_Expression
%type <type_Expression> class_Expression_p1
%type <type_Expression> class_Expression_p2
%type <type_Expression> class_Expression_p3
%type <type_Expression> class_Expression_p4
%type <type_Expression> class_Expression_p5
%type <type_Expression> class_Expression_p6
%type <type_Expression> class_Expression_p7
%type <type_Expression> class_Expression_p8


%%

/*productions*/
class_Expression
  : TOK_IDENTIFIER {
      $$ = new Variable(parser, @$, $1); }
  | TOK_INT64 {
      $$ = new IntegerLiteral(parser, @$, $1); }
  |  TOK_IDENTIFIER TOK_LPAREN  array0_Expression__1 TOK_RPAREN {
      $$ = new FunctionCall(parser, @$, $1, $3); }
  ;

array0_Expression__1
  : { $$ = new vector<Expression*>(); }
  | class_Expression{ $$ = new vector<Expression*>(); $$.push_back(parser, $@, $1); }
  | array0_Expression__1 TOK_COMMA class_Expression { $$ = $1; $$.push_back($3); }
























%%
