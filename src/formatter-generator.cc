/*
 * This file is a part of the Classp parser, formatter, and AST generator.
 * Author: David Gudeman
 * Description: Functions to generate the formatter.
 *
 * Copyright 015 Google Inc.
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
#include "parse-tree.h"

#include "lexer-base.h"
#include "parser-base.h"

namespace classp {

class FormatInfo {
 public:
  FormatInfo(ostream& out, Precedence precedence,
             Associativity assoc)
      : stream_(out), precedence_(precedence), assoc_(assoc) {}

  void PutPrecedence(bool is_first, bool is_last) const {
    if (precedence_ <= 0 || !(is_first || is_last)) {
      stream_ << "0";
    } else if (is_first && assoc_ != AssocLeft) {
      stream_ << precedence_ - 1;
    } else if (is_last && assoc_ != AssocRight) {
      stream_ << precedence_ - 1;
    } else {
      stream_ << precedence_;
    }
  }
  ostream& stream_;
  Precedence precedence_;
  Associativity assoc_;
};

void ParseTree::GenerateFormatter(const FormatInfo& format_info,
                                  const string& separator, bool is_first,
                                  bool is_last) {
  format_info.stream_ << separator << "Classpformat(";
  Print(format_info.stream_);
  format_info.stream_ << ");";
}

void ParseTreeClassDecl::GenerateClassFormatterDeclarations(ostream& out) {
  if (has_precedence && precedences.size() > 0) {
    out << "  void bracketFormat(ostream& out, AstNode* self) override;\n";
  }
  for (auto syntax : syntax_list) {
    if (!syntax->is_self) {
      out
          << "  void format(ostream& out, int precedence) override;\n";
      break;
    }
  }
}

void ParseTreeClassDecl::GenerateClassFormatter(ostream& out) {
  ParseTreeSyntaxDecl* self_syntax = nullptr;
  ParseTreeSyntaxDecl* main_syntax = nullptr;

  // find the first self and non-self syntaxes in the list
  for (auto syntax : syntax_list) {
    if (syntax->is_self && !self_syntax) {
      self_syntax = syntax;
      if (main_syntax) break;
    } else if (!syntax->is_self && !main_syntax) {
      main_syntax = syntax;
      if (self_syntax) break;
    }
  }
  if (has_precedence && precedences.size() > 0) {
    // This is a class that has to be parsed with subclasses with precedence.
    out << "\nvoid " << class_name
        << "::bracketFormat(ostream& out, AstNode* self) {";
    if (self_syntax) {
      self_syntax->GenerateClassFormatter(out, "\n  ");
    } else {
      // This is probably an error of some sort. If this generated code gets
      // executed then there is a precedence inversion in the tree but no
      // bracketing syntax for handling it.
      out << "\n    self->format(out, 0);";
    }
    out << "\n  }\n";
  }
  if (main_syntax) {
    out
        << "\nvoid " << class_name
        << "::format(ostream& out, int precedence) {";
    main_syntax->GenerateClassFormatter(out, "\n  ");
    out << "\n}\n";
  }
}

void ParseTreeSyntaxDecl::GenerateClassFormatter(ostream& out,
                                                 const string& separator) {
  FormatInfo format_info(out, precedence, assoc);
  if (!is_self && class_def->has_precedence) {
    out << separator << "if (precedence <= " << precedence
                     << ") {";
    syntax->GenerateFormatter(format_info, separator + "  ", true, true);
    out << separator << "} else {" << separator
                     << "  bracketFormat(out, this);" << separator << "}";
  } else {
    syntax->GenerateFormatter(format_info, separator, true, true);
  }
}

void ParseTreeSymbol::GenerateFormatter(const FormatInfo& format_info,
                                        const string& separator, bool is_first,
                                        bool is_last) {
  format_info.stream_ << separator << "out << \" " << value << " \";";
}

void ParseTreeUnop::GenerateFormatter(const FormatInfo& format_info,
                                      const string& separator, bool is_first,
                                      bool is_last) {
  switch (op) {
    case token::TOK_STAR:
    case token::TOK_PLUS: {
      GenerateArrayFormatter(format_info, separator, is_first, is_last,
                             operand1(), nullptr);
      break;
    }
    default:
      assert(false);
  }
}

void ParseTreeBinop::GenerateFormatter(const FormatInfo& format_info,
                                       const string& separator, bool is_first,
                                       bool is_last) {
  string s;
  switch (op) {
    case token::TOK_STAR:
    case token::TOK_PLUS: {
      GenerateArrayFormatter(format_info, separator, is_first, is_last,
                             operand1(), operand2());
      return;
    }
    case token::TOK_LBRACE: {
      GenerateCaseFormatter(format_info, separator, operand1(), operand2());
      return;
    }
  }
  assert(false);
}

void ParseTreeAttribute::GenerateFormatter(const FormatInfo& format_info,
                                           const string& separator,
                                           bool is_first, bool is_last) {
  format_info.stream_ << separator << "classpFormat(out";
  if (class_val && class_val->has_precedence && format_info.precedence_ >= 0) {
    format_info.stream_ << ", ";
    format_info.PutPrecedence(is_first, is_last);
  } else {
    format_info.stream_ << ", 0";
  }
  format_info.stream_ << ", " << attribute_name;
  if (is_array) format_info.stream_ << "[i]";
  format_info.stream_ << ");";
}

void ParseTreeItemList::GenerateFormatter(const FormatInfo& format_info,
                                          const string& separator,
                                          bool is_first, bool is_last) {
  int n = array.size() - 1;
  for (int i = 0; i <= n; i++) {
    ParseTreeSymbol* str = array[i]->AsSymbol();
    if (str) {
      format_info.stream_ << separator << "out << \" " << str->value
                          << " \"";
      while (++i <= n) {
        str = array[i]->AsSymbol();
        if (!str) break;
        format_info.stream_ << " << \"" << str->value << " \"";
      }
      format_info.stream_ << ";";
      if (i > n) break;
    }
    array[i]->GenerateFormatter(format_info, separator, is_first && i == 0,
                               is_last && i == n);
  }
}

void ParseTreeAltList::GenerateFormatter(const FormatInfo& format_info,
                                         const string& separator, bool is_first,
                                         bool is_last) {
  const char* stmt_sep = "if (";
  bool empty_allowed = false;
  ParseTree* semi_determined = nullptr;
  for (auto tree : array) {
    ParseTreeSymbol* s = tree->AsSymbol();
    if (s && s->value.size() == 0) {
      // Don't print anything for the empty alternate.
      empty_allowed = true;
      continue;
    }
    stringstream condition;
    switch (tree->GenerateAlternateCondition(condition)) {
      case FULLY_DETERMINING: {
        // If this one is true, we stop here and are done with the alternation.
        break;
      }
      case SEMI_DETERMINING: {
        // This is an attribute with a default value. The returned condition will
        // only be true if the attribute does not have the default value so we
        // will not print alternates where this is the case. However if we get to
        // the end and have not printed anything, we can come back to this one.
        if (!semi_determined) semi_determined = tree;
        break;
      }
      case NOT_DETERMINING: {
        // TODO(dgudeman): this case could be treated like another default to
        // be printed if nothing else works.
        assert(false);
        continue;
      }
    }
    format_info.stream_ << separator << stmt_sep << condition.str();
    format_info.stream_ << ") {";
    tree->GenerateFormatter(format_info, separator + "  ", is_first, is_last);
    stmt_sep = "} else if (";
  }
  if (!empty_allowed && semi_determined) {
    // If nothing else worked, print the first default value seen.
    format_info.stream_ << separator << "} else {";
    semi_determined->GenerateFormatter(
        format_info, separator + "  ", is_first, is_last);
  } else {
    assert(empty_allowed);
  }
  format_info.stream_ << separator << "}";
}

void ParseTree::GenerateAltIterFormatter(const FormatInfo& format_info,
                                         const string& separator, bool is_first,
                                         bool is_last,
                                         ParseTreeAltList* alt_list,
                                         ParseTree* t2) {
  bool first = true;
  const string separator2 = separator + "  ";
  if (t2) format_info.stream_ << separator << "bool found1_ = false;";
  for (auto tree : alt_list->array) {
    ParseTreeSymbol* s = tree->AsSymbol();
    if (s && s->value.size() == 0) {
      // Don't print anything for the empty alternate.
      continue;
    }
    format_info.stream_ << separator << "if (";
    if (tree->GenerateAlternateCondition(format_info.stream_) ==
        NOT_DETERMINING) {
      format_info.stream_ << "true";
    }
    format_info.stream_ << ") {";
    if (first) {
      first = false;
    } else if (t2) {
      format_info.stream_ << separator2 << "if (found1_) {";
      t2->GenerateFormatter(format_info, separator2 + "  ", is_first, is_last);
      format_info.stream_ << separator2 << "}";
    }
    if (tree->HasArrayAttribute()) {
      GenerateArrayFormatter(format_info, separator2, is_first, is_last, tree,
                             t2);
    } else {
      tree->GenerateFormatter(format_info, separator2, is_first, is_last);
    }
    if (t2) format_info.stream_ << separator2 << "found1_ = true;";
    format_info.stream_ << separator << "}";
  }
}

void ParseTree::GenerateCaseFormatter(const FormatInfo& format_info,
                                      const string& separator, ParseTree* t1,
                                      ParseTree* t2) {
  ParseTreeAttribute* attribute = t1->AsAttribute();
  assert(attribute);
  ParseTreeAltList* alt_list = t2->AsAltList();
  assert(alt_list);
  const char* else_token = "";
  if (attribute->is_optional) {
    ParseTree* default_val = nullptr;
    for (auto tree : alt_list->array) {
      assert(tree->IsAssignment());
      ParseTreeBinop* assignment = tree->AsBinop();
      if (assignment->operand1() == nullptr) {
        default_val = assignment->operand2();
        break;
      }
    }
    format_info.stream_ << separator << "if (!has_"
                        << attribute->attribute_name << ") {";
    if (default_val) {
      default_val->GenerateFormatter(format_info, separator + "  ", false,
                                     false);
    }
    format_info.stream_ << separator << "}";
    else_token = "} else ";
  }
  for (auto tree : alt_list->array) {
    ParseTreeBinop* assignment = tree->AsBinop();
    if (assignment->operand1() == nullptr) continue;
    format_info.stream_ << separator << else_token << "if ("
                        << attribute->attribute_name << " == ";
    assignment->operand2()->GenerateExpression(format_info.stream_);
    format_info.stream_ << ") {";
    assignment->operand1()->GenerateFormatter(format_info, separator + "  ",
                                              false, false);
    else_token = "} else ";
  }
  format_info.stream_ << separator << "}";
}

void ParseTree::GenerateArrayFormatter(const FormatInfo& format_info,
                                       const string& separator, bool is_first,
                                       bool is_last, ParseTree* t1,
                                       ParseTree* t2) {
  ParseTreeAltList* alt_list = t1->AsAltList();
  if (alt_list) {
    GenerateAltIterFormatter(format_info, separator, is_first, is_last,
                             alt_list, t2);
    return;
  }
  ParseTreeAttribute* attr = t1->AsAttribute();
  ParseTreeItemList* item_list = t1->AsItemList();
  if (item_list) {
    // Find the first attribute in the list which must be an array.
    for (auto elem : item_list->array) {
      attr = elem->AsAttribute();
      if (attr) {
        assert(attr->is_array);
        break;
      }
    }
  }
  if (attr) {
    // We either have a single array or a list containing at least one array.
    // In either case attr is one of the arrays.
    format_info.stream_ << separator << "for (size_t i = 0; i < "
                        << attr->attribute_name << ".size(); i++) {";
    if (t2) {
      format_info.stream_ << separator << "  if (i > 0) {";
      t2->GenerateFormatter(format_info, separator + "    ", false, false);
      //<< separator << "    stream << \"" << *str->value << " \";"
      format_info.stream_ << separator << "  }";
    }
    t1->GenerateFormatter(format_info, separator + "  ", is_first, is_last);
    format_info.stream_ << separator << "}";
  } else {
    // Just generate the pattern once.
    t1->GenerateFormatter(format_info, separator, is_first, is_last);
  }
}

AlternateDetermination ParseTreeMulti::GenerateAlternateCondition(ostream& stream) {
  stringstream semi_determining;
  const char* separator = "";
  int num_semi_determining = 0;
  for (auto elem : array) {
    stringstream local_stream;
    switch (elem->GenerateAlternateCondition(local_stream)) {
      case FULLY_DETERMINING: {
        // This alternate must be printed if and only if the condition is true
        // so there is no value in looking further.
        stream << local_stream.str();
        return FULLY_DETERMINING;
      }
      case SEMI_DETERMINING: {
        // This alternate must be printed if the condition is true, but if the
        // condition is false then it does not determine anything.
        semi_determining << separator << local_stream.str();
        separator = ") || (";
        num_semi_determining++;
        continue;
      }
      case NOT_DETERMINING: {
        // There is no condition and no information about whether to print this
        // alternate.
        continue;
      }
    }
  }
  if (num_semi_determining == 0) return NOT_DETERMINING;
  if (num_semi_determining == 1) stream << semi_determining.str();
  else stream << "(" << semi_determining.str() << ")";
  return SEMI_DETERMINING;
}

AlternateDetermination ParseTreeAttribute::GenerateAlternateCondition(ostream& stream) {
  if (is_optional) {
    stream << "has_" << attribute_name;
    return FULLY_DETERMINING;
  }
  if (is_array) {
    stream << "(!" << attribute_name << ".empty())";
    return FULLY_DETERMINING;
  }
  if (default_value) {
    stream << attribute_name << " != ";
    default_value->Print(stream);
    return SEMI_DETERMINING;
  }
  return NOT_DETERMINING;
}

bool ParseTreeSyntaxDecl::IsBracketSyntax() {
  if (!is_self) return false;
  ParseTreeItemList* item_list = syntax->AsItemList();
  if (!item_list) return false;
  if (item_list->array.size() < 3) return false;
  // skip over a prefix of strings in the list
  int i = 0;
  for (; i < item_list->array.size(); i++) {
    ParseTreeSymbol* s = item_list->array[i]->AsSymbol();
    if (!s) break;
  }
  if (i < 1) return false;  // the first item is not a string
  if (i >= item_list->array.size() - 1) return false;  // There is no room for
                                                      // strings at the end.
  // Check that the first non-string is the self attribute.
  ParseTreeAttribute* attr = item_list->array[i]->AsAttribute();
  if (!attr || !attr->is_self) return false;
  // Make sure the remaining elements are all strings.
  for (; i < item_list->array.size(); i++) {
    ParseTreeSymbol* s = item_list->array[i]->AsSymbol();
    if (!s) return false;
  }
  return true;
}

}  // namespace classp
