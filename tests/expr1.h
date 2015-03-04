/* BEGIN HEADER */
#ifndef expr1_INCLUDE_
#define expr1_INCLUDE_

#include <assert.h>
#include <unordered_map>
#include <utility>

#include "classp.h"

// Include files generated by bison
#include "expr1.yacc.hh"
#include "location.hh"
#include "position.hh"

namespace expr1 {
using std::istream;
using std::ostream;
using classp::classpPrint;
using classp::classpFormat;
using classp::AttributeMap;

// Location and state information from the parser.
typedef location ParseState;

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

// Base class for expr1 AST nodes
class AstNode : public classp::ClasspNode {
 public:
  string className() override { return "AstNode"; }
  AstNode(ParseState parseState)
    : parseState(parseState) {
    }

  // Write out a bracketed form of this AST from the declared syntax.
  virtual void bracketFormat(std::ostream& out, AstNode* self) {
    assert(false);
  }

  ParseState parseState;
};

/* BEGIN CLASS DEFINITIONS */
class Expression: public AstNode {
 public:
  string className() override { return "Expression"; }
  Expression(ParseState parseState);
  static Expression* parse(istream& input, ostream& errors);
  void printMembers(ostream& out) override;
  void bracketFormat(ostream& out, AstNode* self) override;

};

class Variable: public Expression {
 public:
  string className() override { return "Variable"; }
  Variable(ParseState parseState, identifier id);
  void printMembers(ostream& out) override;
  void format(ostream& out, int precedence) override;

  identifier id;
};

class IntegerLiteral: public Expression {
 public:
  string className() override { return "IntegerLiteral"; }
  IntegerLiteral(ParseState parseState, int val);
  void printMembers(ostream& out) override;
  void format(ostream& out, int precedence) override;

  int val;
};

class CaseExpression: public Expression {
 public:
  string className() override { return "CaseExpression"; }
  CaseExpression(ParseState parseState, Expression* testExpr, Expression* elseResult, AttributeMap& keyword_args);
  void printMembers(ostream& out) override;
  void format(ostream& out, int precedence) override;

  Expression* testExpr = nullptr;
  vector<Expression*> conditions;
  vector<Expression*> results;
  Expression* elseResult = nullptr;
};

class Invocation: public Expression {
 public:
  string className() override { return "Invocation"; }
  Invocation(ParseState parseState, Expression* arg1, AttributeMap& keyword_args);
  void printMembers(ostream& out) override;

  Expression* arg1 = nullptr;
  Expression* arg2 = nullptr;
  Expression* arg3 = nullptr;
  bool has_arg2 = false;
  bool has_arg3 = false;
};

class OrOp: public Invocation {
 public:
  string className() override { return "OrOp"; }
  OrOp(ParseState parseState, Expression* arg1, AttributeMap& keyword_args);
  void printMembers(ostream& out) override;
  void format(ostream& out, int precedence) override;

};

class AndOp: public Invocation {
 public:
  string className() override { return "AndOp"; }
  AndOp(ParseState parseState, Expression* arg1, AttributeMap& keyword_args);
  void printMembers(ostream& out) override;
  void format(ostream& out, int precedence) override;

};

class NotOp: public Invocation {
 public:
  string className() override { return "NotOp"; }
  NotOp(ParseState parseState, Expression* arg1, AttributeMap& keyword_args);
  void printMembers(ostream& out) override;
  void format(ostream& out, int precedence) override;

};

class EqOp: public Invocation {
 public:
  string className() override { return "EqOp"; }
  EqOp(ParseState parseState, Expression* arg1, AttributeMap& keyword_args);
  void printMembers(ostream& out) override;
  void format(ostream& out, int precedence) override;

};

class NeOp: public Invocation {
 public:
  string className() override { return "NeOp"; }
  NeOp(ParseState parseState, Expression* arg1, AttributeMap& keyword_args);
  void printMembers(ostream& out) override;
  void format(ostream& out, int precedence) override;

};

class LtOp: public Invocation {
 public:
  string className() override { return "LtOp"; }
  LtOp(ParseState parseState, Expression* arg1, AttributeMap& keyword_args);
  void printMembers(ostream& out) override;
  void format(ostream& out, int precedence) override;

};

class LeOp: public Invocation {
 public:
  string className() override { return "LeOp"; }
  LeOp(ParseState parseState, Expression* arg1, AttributeMap& keyword_args);
  void printMembers(ostream& out) override;
  void format(ostream& out, int precedence) override;

};

class GtOp: public Invocation {
 public:
  string className() override { return "GtOp"; }
  GtOp(ParseState parseState, Expression* arg1, AttributeMap& keyword_args);
  void printMembers(ostream& out) override;
  void format(ostream& out, int precedence) override;

};

class GeOp: public Invocation {
 public:
  string className() override { return "GeOp"; }
  GeOp(ParseState parseState, Expression* arg1, AttributeMap& keyword_args);
  void printMembers(ostream& out) override;
  void format(ostream& out, int precedence) override;

};

class PlusOp: public Invocation {
 public:
  string className() override { return "PlusOp"; }
  PlusOp(ParseState parseState, Expression* arg1, AttributeMap& keyword_args);
  void printMembers(ostream& out) override;
  void format(ostream& out, int precedence) override;

};

class MinusOp: public Invocation {
 public:
  string className() override { return "MinusOp"; }
  MinusOp(ParseState parseState, Expression* arg1, AttributeMap& keyword_args);
  void printMembers(ostream& out) override;
  void format(ostream& out, int precedence) override;

};

class TimesOp: public Invocation {
 public:
  string className() override { return "TimesOp"; }
  TimesOp(ParseState parseState, Expression* arg1, AttributeMap& keyword_args);
  void printMembers(ostream& out) override;
  void format(ostream& out, int precedence) override;

};

class DivideOp: public Invocation {
 public:
  string className() override { return "DivideOp"; }
  DivideOp(ParseState parseState, Expression* arg1, AttributeMap& keyword_args);
  void printMembers(ostream& out) override;
  void format(ostream& out, int precedence) override;

};

class NegateOp: public Invocation {
 public:
  string className() override { return "NegateOp"; }
  NegateOp(ParseState parseState, Expression* arg1, AttributeMap& keyword_args);
  void printMembers(ostream& out) override;
  void format(ostream& out, int precedence) override;

};

class FunctionCall: public Expression {
 public:
  string className() override { return "FunctionCall"; }
  FunctionCall(ParseState parseState, identifier functionName, vector<Expression*> args);
  void printMembers(ostream& out) override;
  void format(ostream& out, int precedence) override;

  identifier functionName;
  vector<Expression*> args;
};
/* END CLASS DEFINITIONS */

}  // namespace expr1
#endif // expr1_INCLUDE_

/* END HEADER */