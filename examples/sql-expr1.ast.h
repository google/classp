// Generated include files
#include directory/xyz.yacc.h
#include directory/location.hh
#include directory/position.hh

typedef map<string, AttributeUnion> AttributeMap;

// forward declarations
class AndOp;
class BetweenOp;
class Binop;
class DivideOp;
class EqOp;
class Expression;
class FunctionCall;
class GeOp;
class GtOp;
class IntegerLiteral;
class LeOp;
class LtOp;
class MinusOp;
class NeOp;
class NegateOp;
class NotOp;
class OrOp;
class PlusOp;
class TimesOp;
class Trinop;
class Unop;
class Variable;

// class definitions
class Expression {
  Expression(Parser* parser, const yyParser::location_type& location)
      : AstBase(parser, location) {
  }

  static Expression* parse(istream& input, ostream& errors);

  void print(ostream& out) override {
  }

  void bracketFormat(ostream stream, Expression* self) {
    self->format(stream, 0);
  }

};

class Variable: public Expression {
  Variable(Parser* parser, const yyParser::location_type& location, identifier* id)
      : AstBase(parser, location) {
    this->id = id;
  }

  void print(ostream& out) override {
    Expression::Print();

    out << ", id = ";
    classpPrint(out, id);
  }

  void format(ostream stream, int precedence) override {
    if (precedence <= 8) {
      classpFormat(stream, 0, id);
    } else {
      bracketFormat(stream, this);
    }
  }

  identifier* id = nullptr;
};

class IntegerLiteral: public Expression {
  IntegerLiteral(Parser* parser, const yyParser::location_type& location, int64_t val)
      : AstBase(parser, location) {
    this->val = val;
  }

  void print(ostream& out) override {
    Expression::Print();

    out << ", val = ";
    classpPrint(out, val);
  }

  void format(ostream stream, int precedence) override {
    if (precedence <= 8) {
      classpFormat(stream, 0, val);
    } else {
      bracketFormat(stream, this);
    }
  }

  int64_t val;
};

class Unop: public Expression {
  Unop(Parser* parser, const yyParser::location_type& location, Expression* arg1)
      : AstBase(parser, location) {
    this->arg1 = arg1;
  }

  void print(ostream& out) override {
    Expression::Print();

    out << ", arg1 = ";
    classpPrint(out, arg1);
  }

  Expression* arg1 = nullptr;
};

class Binop: public Expression {
  Binop(Parser* parser, const yyParser::location_type& location, Expression* arg1, Expression* arg2)
      : AstBase(parser, location) {
    this->arg1 = arg1;
    this->arg2 = arg2;
  }

  void print(ostream& out) override {
    Expression::Print();

    out << ", arg1 = ";
    classpPrint(out, arg1);
    out << ", arg2 = ";
    classpPrint(out, arg2);
  }

  Expression* arg1 = nullptr;
  Expression* arg2 = nullptr;
};

class Trinop: public Expression {
  Trinop(Parser* parser, const yyParser::location_type& location, Expression* arg1, Expression* arg2, Expression* arg3)
      : AstBase(parser, location) {
    this->arg1 = arg1;
    this->arg2 = arg2;
    this->arg3 = arg3;
  }

  void print(ostream& out) override {
    Expression::Print();

    out << ", arg1 = ";
    classpPrint(out, arg1);
    out << ", arg2 = ";
    classpPrint(out, arg2);
    out << ", arg3 = ";
    classpPrint(out, arg3);
  }

  Expression* arg1 = nullptr;
  Expression* arg2 = nullptr;
  Expression* arg3 = nullptr;
};

class OrOp: public Binop {
  OrOp(Parser* parser, const yyParser::location_type& location)
      : AstBase(parser, location) {
  }

  void print(ostream& out) override {
    Binop::Print();

  }

  void format(ostream stream, int precedence) override {
    if (precedence <= 1) {
      classpFormat(stream, 1, arg1);
      stream << " or ";
      classpFormat(stream, 0, arg2);
    } else {
      bracketFormat(stream, this);
    }
  }

};

class AndOp: public Binop {
  AndOp(Parser* parser, const yyParser::location_type& location)
      : AstBase(parser, location) {
  }

  void print(ostream& out) override {
    Binop::Print();

  }

  void format(ostream stream, int precedence) override {
    if (precedence <= 2) {
      classpFormat(stream, 2, arg1);
      stream << " and ";
      classpFormat(stream, 1, arg2);
    } else {
      bracketFormat(stream, this);
    }
  }

};

class NotOp: public Binop {
  NotOp(Parser* parser, const yyParser::location_type& location)
      : AstBase(parser, location) {
  }

  void print(ostream& out) override {
    Binop::Print();

  }

  void format(ostream stream, int precedence) override {
    if (precedence <= 3) {
      stream << " not ";
      classpFormat(stream, 2, arg1);
    } else {
      bracketFormat(stream, this);
    }
  }

};

class EqOp: public Binop {
  EqOp(Parser* parser, const yyParser::location_type& location)
      : AstBase(parser, location) {
  }

  void print(ostream& out) override {
    Binop::Print();

  }

  void format(ostream stream, int precedence) override {
    if (precedence <= 4) {
      classpFormat(stream, 3, arg1);
      stream << " = ";
      classpFormat(stream, 3, arg2);
    } else {
      bracketFormat(stream, this);
    }
  }

};

class NeOp: public Binop {
  NeOp(Parser* parser, const yyParser::location_type& location)
      : AstBase(parser, location) {
  }

  void print(ostream& out) override {
    Binop::Print();

  }

  void format(ostream stream, int precedence) override {
    if (precedence <= 4) {
      classpFormat(stream, 3, arg1);
      stream << " != ";
      classpFormat(stream, 3, arg2);
    } else {
      bracketFormat(stream, this);
    }
  }

};

class LtOp: public Binop {
  LtOp(Parser* parser, const yyParser::location_type& location)
      : AstBase(parser, location) {
  }

  void print(ostream& out) override {
    Binop::Print();

  }

  void format(ostream stream, int precedence) override {
    if (precedence <= 4) {
      classpFormat(stream, 3, arg1);
      stream << " < ";
      classpFormat(stream, 3, arg2);
    } else {
      bracketFormat(stream, this);
    }
  }

};

class LeOp: public Binop {
  LeOp(Parser* parser, const yyParser::location_type& location)
      : AstBase(parser, location) {
  }

  void print(ostream& out) override {
    Binop::Print();

  }

  void format(ostream stream, int precedence) override {
    if (precedence <= 4) {
      classpFormat(stream, 3, arg1);
      stream << " <= ";
      classpFormat(stream, 3, arg2);
    } else {
      bracketFormat(stream, this);
    }
  }

};

class GtOp: public Binop {
  GtOp(Parser* parser, const yyParser::location_type& location)
      : AstBase(parser, location) {
  }

  void print(ostream& out) override {
    Binop::Print();

  }

  void format(ostream stream, int precedence) override {
    if (precedence <= 4) {
      classpFormat(stream, 3, arg1);
      stream << " > ";
      classpFormat(stream, 3, arg2);
    } else {
      bracketFormat(stream, this);
    }
  }

};

class GeOp: public Binop {
  GeOp(Parser* parser, const yyParser::location_type& location)
      : AstBase(parser, location) {
  }

  void print(ostream& out) override {
    Binop::Print();

  }

  void format(ostream stream, int precedence) override {
    if (precedence <= 4) {
      classpFormat(stream, 3, arg1);
      stream << " >= ";
      classpFormat(stream, 3, arg2);
    } else {
      bracketFormat(stream, this);
    }
  }

};

class BetweenOp: public Trinop {
  BetweenOp(Parser* parser, const yyParser::location_type& location)
      : AstBase(parser, location) {
  }

  void print(ostream& out) override {
    Trinop::Print();

  }

  void format(ostream stream, int precedence) override {
    if (precedence <= 4) {
      classpFormat(stream, 3, arg1);
      stream << " between ";
      classpFormat(stream, 0, arg2);
      stream << " and ";
      classpFormat(stream, 3, arg3);
    } else {
      bracketFormat(stream, this);
    }
  }

};

class PlusOp: public Binop {
  PlusOp(Parser* parser, const yyParser::location_type& location)
      : AstBase(parser, location) {
  }

  void print(ostream& out) override {
    Binop::Print();

  }

  void format(ostream stream, int precedence) override {
    if (precedence <= 5) {
      classpFormat(stream, 5, arg1);
      stream << " + ";
      classpFormat(stream, 4, arg2);
    } else {
      bracketFormat(stream, this);
    }
  }

};

class MinusOp: public Binop {
  MinusOp(Parser* parser, const yyParser::location_type& location)
      : AstBase(parser, location) {
  }

  void print(ostream& out) override {
    Binop::Print();

  }

  void format(ostream stream, int precedence) override {
    if (precedence <= 5) {
      classpFormat(stream, 5, arg1);
      stream << " - ";
      classpFormat(stream, 4, arg2);
    } else {
      bracketFormat(stream, this);
    }
  }

};

class TimesOp: public Binop {
  TimesOp(Parser* parser, const yyParser::location_type& location)
      : AstBase(parser, location) {
  }

  void print(ostream& out) override {
    Binop::Print();

  }

  void format(ostream stream, int precedence) override {
    if (precedence <= 6) {
      classpFormat(stream, 6, arg1);
      stream << " * ";
      classpFormat(stream, 5, arg2);
    } else {
      bracketFormat(stream, this);
    }
  }

};

class DivideOp: public Binop {
  DivideOp(Parser* parser, const yyParser::location_type& location)
      : AstBase(parser, location) {
  }

  void print(ostream& out) override {
    Binop::Print();

  }

  void format(ostream stream, int precedence) override {
    if (precedence <= 6) {
      classpFormat(stream, 6, arg1);
      stream << " / ";
      classpFormat(stream, 5, arg2);
    } else {
      bracketFormat(stream, this);
    }
  }

};

class NegateOp: public Unop {
  NegateOp(Parser* parser, const yyParser::location_type& location)
      : AstBase(parser, location) {
  }

  void print(ostream& out) override {
    Unop::Print();

  }

  void format(ostream stream, int precedence) override {
    if (precedence <= 7) {
      stream << " - ";
      classpFormat(stream, 6, arg1);
    } else {
      bracketFormat(stream, this);
    }
  }

};

class FunctionCall: public Expression {
  FunctionCall(Parser* parser, const yyParser::location_type& location, identifier* functionName, vector<Expression*>* args)
      : AstBase(parser, location) {
    this->functionName = functionName;
    this->args = args;
  }

  void print(ostream& out) override {
    Expression::Print();

    out << ", functionName = ";
    classpPrint(out, functionName);
    out << ", args = ";
    classpPrint(out, args);
  }

  void format(ostream stream, int precedence) override {
    if (precedence <= 8) {
      classpFormat(stream, 0, functionName);
      stream << " ( ";
      for (int i = 0; i < args->size(); i++) {
        if (i > 0) {
          stream << " , ";
        }
        classpFormat(stream, 0, args[i]);
      }
      stream << " ) ";
    } else {
      bracketFormat(stream, this);
    }
  }

  identifier* functionName = nullptr;
  vector<Expression*>* args = nullptr;
};


