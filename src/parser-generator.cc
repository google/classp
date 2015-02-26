/*
 * This file is a part of the Classp parser, formatter, and AST generator.
 * Author: David Gudeman
 * Description: Functions to generate the parser (the bison code).
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

// A RuleInfo carries the information needed to create a single rule for
// a single syntax attribute.
struct RuleInfo {
  RuleInfo(ostream& stream, ParseTreeSyntaxDecl* syntax_def,
           const set<Precedence>& precedences,
           KeywordMatches* keyword_matches)
      : stream(stream),
        class_def(syntax_def->class_def),
        assoc(syntax_def->assoc),
        precedence(syntax_def->precedence),
        keyword_matches(keyword_matches) {
    if (precedence < 0) {
      next_precedence = -1;
    } else {
      auto iter = precedences.find(precedence);
      assert(iter != precedences.end());
      ++iter;
      if (iter == precedences.end())
        next_precedence = -1;
      else
        next_precedence = *iter;
      // The min precedence in the class gets converted to 0
      iter = precedences.begin();
      if (precedence == *iter) {
        precedence = 0;
      }
    }
  }

  RuleInfo(ostream& stream, RuleInfo* rule_info)
      : stream(stream),
        class_def(rule_info->class_def),
        assoc(rule_info->assoc),
        precedence(rule_info->precedence),
        next_precedence(rule_info->next_precedence),
        keyword_matches(rule_info->keyword_matches),
        parent(rule_info),
        array_elements(rule_info->array_elements) {}

  ~RuleInfo() {
    if (parent) {
      // This is a nested rule, meaning that it finds attributes for an outer
      // rule.
      // In the outer rule, the attributes that are positional parameters here
      // will
      // be keyword parameters, so add each posititional parameter to the keywords
      // of the outer RuleInfo.
      for (const auto& elem : positional_matches) {
        // If elem.second is nullptr, then it represents the position of an
        // AttributeMap. Any of these that have to be passed back should already
        // be in positional_matches,
        if (elem.second) {
          parent->keyword_matches->insert(elem.second);
        }
      }
    }
  }
#if 0
  // Return the precedence for the first element in a list.
  Precedence PrecedenceFirst() {
    if (assoc != AssocLeft) {
      return rule_info->next_precedence;
    } else if (precedence == min) {
        return 0;
    } else {
      return precedence;
    }
  }

  // Return the precedence for the last element in a list;
  Precedence PrecedenceLast() {
    if (assoc != AssocRight) {
      return rule_info->next_precedence;
    } else if (precedence == min) {
        return 0;
    } else {
      return precedence;
    }
  }
#endif
  // Adds a position argument attribute at position.
  void AddPositionalArg(int position, ParseTreeAttribute* attribute) {
    assert(attribute);
    positional_matches.emplace_back(position, attribute);
  }

  // Adds a keword map at position in the current positional parameters.
  void AddKeywordArgs(int position) {
    positional_matches.emplace_back(position, nullptr);
  }

  ostream& stream;
  ParseTreeClassDecl* class_def;
  Associativity assoc;
  Precedence precedence;
  Precedence next_precedence;
  KeywordMatches* keyword_matches;
  PositionalMatches positional_matches;
  RuleInfo* parent = nullptr;
  // If true then array attributes are treated as elements and added to the
  // array. If false then array attributes are assumed to represent the entire
  // array.
  bool array_elements = false;
};

// Calls tree->GenerateMatcher() but handles the case where tree is nullptr.
int GenerateTreeMatcher(ParseTree* tree, RuleInfo* rule_info, int position,
                        bool is_last) {
  if (tree) {
    return tree->GenerateMatcher(rule_info, position, is_last);
  }
  return position;
}

void ParseTreeClassDecl::GenerateProductions() {
  int rule_pos = parser->rules_.size();
  parser->rules_.emplace_back("");

  // The stream where the class parsing rule is constructed.
  stringstream out;

  // Generate the production for recognizing the class.
  if (has_precedence) {
    // Generate rules that are specific to this production.
    if (is_parsed) GeneratePrecedenceProductions(out);
  } else if (is_parsed &&
             (syntax_list.size() > 0 || parsing_subclasses.size() > 0)) {
    GenerateSingleProduction(out);
  }
  parser->rules_[rule_pos] = out.str();
}

void ParseTreeClassDecl::GenerateSingleProduction(ostream& out) {
  // When you parse a class A, you have to consider not only the syntax of A
  // but also the sytnax of its subclasses, say B and C. This is because an A
  // might actually be a B or C. If there is no precedence in the syntax then
  // the production for class A would be something like this:
  //   A: <syntax-for-A> {$$ = new A(...);}
  //    | B {$$ = $1;}
  //    | C {$$ = $1;}
  //    ;
  // The whole thing a production and each individual line is a rule.
  //
  // If the syntax has precedences, then it is more complicated and is done in
  // another function.

  // In the grammar file, we want the class rules to come before all the
  // auxiliary rules used to parse the class, but those get generated before the
  // class is finished, so we reserve a space in the rules_ list for the class.
  const string nonterminal = parser->GetTypeMatcher(class_name);
  out << nonterminal;
  const char* separator = "\n  : ";
  for (auto syntax : syntax_list) {
    out << separator;
    syntax->GenerateSyntaxRule(out, precedences);
    separator = "\n  | ";
  }
  for (auto elem : parsing_subclasses) {
    // Generate rules for subclass syntax
    out << separator << parser->GetTypeMatcher(elem->class_name)
                << " { $$ = $1; }";
    separator = "\n  | ";
  }
  out << "\n  ;\n";
}

void ParseTreeClassDecl::GeneratePrecedenceProductions(ostream& out) {
  // Take the example
  //   class A { syntax ( '(' $$ ')' ); }
  //   class B extends A { A v1; A v2; syntax %left 4 ( v1 '+' v2 ); }
  //   class C extends A { A v1; A v2; syntax %left 6 ( v1 '*' v2 ); }
  //   class D extends A { int v; syntax (v); }
  // Unspecified precedences are treated like one more than the maximum
  // specified precedence.
  // We label productions with bindings: _p4, _p6, _p7 and the productions
  // look something like this:
  //    A: A_p4 {$$ = $1;}                        // main production for A
  //     ;
  //    A_p4: A_p4 '+' A_p6 {$$ = new B($1, $3);} // rule for B in A
  //        | A_p6          {$$ = $1;}            // ending rule
  //        ;
  //    A_p6: A_p6 '*' A_p7 {$$ = new C($1, $3);} // rule for C in A
  //        | A_p7          {$$ = $1;}            // ending rule
  //        ;
  //    A_p7: '(' A ')' {$$ = $2;}                // rule for parens in A
  //        | INT_TOK   {$$ = $1;}                // rule for D in A
  //        ;                                     // no ending rule
  // Be aware that a class can have multiple syntaxes with multiple precedences,
  // so we can't for example just have A_p1 reference a generic production for
  // B; it has to use the right syntax with the right precedence.

  // If this is a subclass that is never parsed on its own, like B, C, and D in
  // the example above, then no precedences were assigned to it and we do not
  // generate a production for it.

  // Sort the syntaxes by precedence.
  multimap<Precedence, ParseTreeSyntaxDecl*> syntaxes;
  AddAllSyntaxes(&syntaxes);
  if (syntaxes.empty()) return;

  // Now generate individual productions for each syntax.
  const string nonterminal = parser->GetTypeMatcher(class_name);
  Precedence current_precedence = syntaxes.begin()->first;
  out << nonterminal;
  const char* separator = "\n  : ";
  for (auto elem : syntaxes) {
    if (current_precedence != elem.first) {
      // We have found a new precedence in the list, so end the previous
      // production and start a new one for the new precedence. Before ending
      // the previous production, add the ending rule that parses the next
      // precedence.
      out << separator << PrecedenceNonterminal(nonterminal, elem.first)
                  << " { $$ = $1; }\n  ;\n"
                  << PrecedenceNonterminal(nonterminal, elem.first);
      current_precedence = elem.first;
      separator = "\n  : ";
    }
    out << separator;
    elem.second->GenerateSyntaxRule(out, precedences);
    if (elem.second->class_def != this) {
      out << "  // " << elem.second->class_def->class_name;
    }
    separator = "\n  | ";
  }
  out << "\n  ;\n";
}

void ParseTreeClassDecl::AddAllSyntaxes(
    multimap<Precedence, ParseTreeSyntaxDecl*>* syntaxes) {
  for (auto syntax : syntax_list) {
    syntaxes->emplace(syntax->precedence, syntax);
  }
  for (auto class_def : parsing_subclasses) {
    class_def->AddAllSyntaxes(syntaxes);
  }
}

void ParseTreeSyntaxDecl::GenerateSyntaxRule(
    ostream& out, const set<Precedence>& precedences) {
  // Generate the grammar rule.
  RuleInfo rule_info(out, this, precedences, new KeywordMatches());
  syntax->GenerateMatcher(&rule_info, 1, true);
  if (is_self) {
    if (rule_info.positional_matches.size() != 1) {
      Error("Multiple attributes in a self syntax");
    } else if (rule_info.keyword_matches->size() > 0) {
      Error("Alternate attributes in a self syntax");
    } else {
      out << " { $$ = $" << rule_info.positional_matches[0].first
                  << "; }";
    }
    return;
  }
  class_def->GenerateAction(&rule_info);
}

void ParseTreeClassDecl::GenerateAction(RuleInfo* rule_info) {
  rule_info->stream << " {";

  // There can be multiple AttributeMaps, one for each alternation
  // ((A|B) (C|D)). Consolidate the AttributeMaps into one to pass to the
  // constructor.
  bool has_keywords = false;
  for (const auto& elem : rule_info->positional_matches) {
    if (elem.second != nullptr) {
      continue;
    }
    // elem is an AttributeMap
    if (!has_keywords) {
      // We have not encountered a keyword arg yet, so let's pick elem
      // as THE keyword arg and add all the others to it.
      rule_info->stream
          << "\n      AttributeMap keywords = $" << elem.first << ";";
      has_keywords = true;
    } else {
      // There is already a keyword arg, so just add elem to it.
      rule_info->stream << "\n      keywords.Merge($"
                        << elem.first << ");";
    }
  }

  // Add any positional matches that are not in the constructor to the keywords.
  for (const auto& elem : rule_info->positional_matches) {
    if (elem.second != nullptr && !elem.second->is_required) {
      if (!has_keywords) {
        rule_info->stream
            << "\n      AttributeMap keywords = AttributeMap();";
        has_keywords = true;
      }
      rule_info->stream << "\n      keywords.Add(\""
          << elem.second->attribute_name << "\", $" << elem.first << ");";
    }
  }

  // Generate the constructor call.
  rule_info->stream << "\n      $$ = new ";
  GenerateConstructorCall(rule_info->stream, rule_info->positional_matches,
                          has_keywords);
  rule_info->stream << "; }";
}

void ParseTreeClassDecl::GenerateConstructorCall(
    ostream& stream, const PositionalMatches& positional_matches,
    bool has_keywords) {

  // Create a map mapping parameters to their positions in the rule.
  map<ParseTreeAttribute*, int> params;
  for (const auto& elem : positional_matches) {
    if (elem.second == nullptr) continue; // an AttributeMap
    params[elem.second] = elem.first;
  }
  stream << class_name << "(@$";
  // Generate the params in order.
  for (auto attr : required_params) {
    const auto& elem = params.find(attr);
    if (elem != params.end()) {
      stream << ", $" << elem->second;
    }
  }
  if (has_keywords) {
    assert(optional_params.size() > 0);
    stream << ", keywords";
  } else if (optional_params.size() > 0) {
    stream << ", nullptr";
  }
  stream << ")";
}

int ParseTreeSymbol::GenerateMatcher(RuleInfo* rule_info, int position,
                                     bool is_last) {
  int count;
  rule_info->stream << parser->GetStringMatcher(value, &count);
  return position + count;
}

int ParseTreeUnop::GenerateMatcher(RuleInfo* rule_info, int position,
                                   bool is_last) {
  switch (op) {
    case token::TOK_STAR: {
      return GenerateArrayMatcher(rule_info, 0, operand1(), nullptr, position);
    }
    case token::TOK_PLUS: {
      return GenerateArrayMatcher(rule_info, 1, operand1(), nullptr, position);
      break;
    }
  }
  assert(false);
  return position;
}

int ParseTreeBinop::GenerateMatcher(RuleInfo* rule_info, int position,
                                    bool is_last) {
  switch (op) {
    case token::TOK_STAR: {
      return GenerateArrayMatcher(rule_info, 0, operand1(), operand2(),
                                  position);
    }
    case token::TOK_PLUS: {
      return GenerateArrayMatcher(rule_info, 1, operand1(), operand2(),
                                  position);
    }
    case token::TOK_LBRACE: {
      return GenerateAssignmentListMatcher(rule_info, operand1(), operand2(),
                                           position);
    }
    case token::TOK_RIGHTARROW: {
      assert(false);  // this should have been handled elsewhere
    }
  }
  assert(false);
  return position;
}

int ParseTreeAttribute::GenerateMatcher(RuleInfo* rule_info, int position,
                                        bool is_last) {
  int precedence = rule_info->precedence;
  if (precedence >= 0) {
    // Suppose we have a simple language that has, among other operators, the
    // following that all have the same precedence:
    //   '+' %left 5
    //   '<' %nonassoc 5
    //   '!' %right 5
    //   '?' ':' %left 5
    // The rules for Expr5 look like this:
    //   Expr5:
    //       Expr5 '+' Expr4;
    //     | Expr4 '<' Expr4;
    //     | Expr4 '!' Expr5;
    //     | Expr5 '?' Expr ':' Expr4
    //     ;
    // The classes at the beginning and end get a precedence based on the
    // current precedence. Any classes that are not the very first thing or
    // the very last thing in the rule get precedence 0 (which is Expr).

    if (position == 1) {
      if (rule_info->assoc != AssocLeft) {
        precedence = rule_info->next_precedence;
      }
    } else if (is_last) {
      if (rule_info->assoc != AssocRight) {
        precedence = rule_info->next_precedence;
      }
    } else {
      precedence = -1;
    }
  }
  string nonterminal = parser->GetTypeMatcher(source_type, precedence);
  rule_info->stream << nonterminal;
  rule_info->AddPositionalArg(position, this);
  return position + 1;
}

int ParseTreeItemList::GenerateMatcher(RuleInfo* rule_info, int position,
                                       bool is_last) {
  int n = array.size() - 1;
  for (int i = 0; i <= n; i++) {
    rule_info->stream << " ";
    // list[i] is the last element at its level only if this is the last element
    // at its own level and it's the last element of the array.
    position =
        GenerateTreeMatcher(array[i], rule_info, position, is_last && i == n);
  }
  return position;
}

void ParseTree::GenerateRuleForAlternate(RuleInfo* rule_info) {
  GenerateMatcher(rule_info, 1, true);
  GenerateAction(rule_info);
}

void ParseTree::GenerateAction(RuleInfo* rule_info) {
  rule_info->stream << " {";
  // Assign an AttributeMap to $$.
  int keyword_pos = 0;
  for (const auto& elem : rule_info->positional_matches) {
    if (elem.second != nullptr) {
      continue;
    }
    // elem is an AttributeMap
    if (keyword_pos == 0) {
      // We have not encountered a keyword arg yet, so let's pick elem
      // as THE keyword arg and add all the others to it.
      keyword_pos = elem.first;
      rule_info->stream << "\n    $$ = $" << keyword_pos << ";";
    } else {
      // There is already a keyword arg, so just add elem to it.
      rule_info->stream << "\n    $$"
                        << "->insert($" << elem.first << "->begin(), $"
                        << elem.first << "->end());"
                        << "\n    delete $" << elem.first << ";";
    }
  }
  if (keyword_pos == 0) {
    // There were no AttributeMaps created lower down in the tree.
    rule_info->stream << "\n    $$ = AttributeMap();";
  }
  for (const auto& elem : rule_info->positional_matches) {
    if (!elem.second) {
      continue;
    }
    if (rule_info->array_elements && elem.second->is_array) {
      rule_info->stream << "\n    $$.Push(\""
                        << elem.second->attribute_name << "\", $" << elem.first
                        << ");";
    } else {
      rule_info->stream << "\n    $$.Add(\""
                        << elem.second->attribute_name << "\", $" << elem.first
                        << ");";
    }
  }
  rule_info->stream << " }";
}

void ParseTreeBinop::GenerateRuleForAlternate(RuleInfo* rule_info) {
  if (op == token::TOK_RIGHTARROW) {
    int original_size = rule_info->positional_matches.size() +
                        rule_info->keyword_matches->size();
    GenerateTreeMatcher(operand1(), rule_info, 1, true);
    if (original_size !=
        rule_info->positional_matches.size() +
            rule_info->keyword_matches->size()) {
      Error(
          "No attribute matches are allowed inside an assignment list pattern");
    }
    rule_info->stream << " { $$ = ";
    operand2()->GenerateExpression(rule_info->stream);
    rule_info->stream << "; }";
  } else {
    ParseTree::GenerateRuleForAlternate(rule_info);
  }
}

int ParseTree::GenerateAssignmentListMatcher(RuleInfo* rule_info, ParseTree* t1,
                                             ParseTree* t2, int position) {
  ParseTreeAttribute* attribute = t1->AsAttribute();
  if (!attribute) {
    Error("The assignee of the assignment must be an attribute.");
    return position;
  }
  ParseTreeAltList* alt_list = t2->AsAltList();
  if (!alt_list) {
    Error("The matcher inside braces must be an alt list.");
    return position;
  }
  for (auto tree : alt_list->array) {
    if (!tree->IsAssignment()) {
      Error("Member of assignment list is not an assignment pattern.");
      return position;
    }
  }
  string nonterminal =
      parser->CreateNontermial(attribute->declare_base_type, "%Qtyped_%s",
                               attribute->base_type.c_str());
  rule_info->stream << nonterminal;
  rule_info->AddPositionalArg(position, attribute);
  alt_list->GenerateProduction(rule_info, nonterminal);
  return position + 1;
}

int ParseTreeAltList::GenerateMatcher(RuleInfo* rule_info, int position,
                                      bool is_last) {
  string nonterminal = parser->CreateNontermial(
      "AttributeMap", "%Qalt_%s", rule_info->class_def->class_name.c_str());
  rule_info->stream << " " << nonterminal;
  // Indicate the position of the AttributeMap in the matched line.
  rule_info->AddKeywordArgs(position);

  GenerateProduction(rule_info, nonterminal);
  return position + 1;
}

void ParseTreeAltList::GenerateProduction(RuleInfo* rule_info,
                                          const string& nonterminal) {
  stringstream out;
  out << nonterminal;
  const char* separator = "\n  : ";
  for (auto elem : array) {
    out << separator;
    // Create a RuleInfo with a new stream and a new positional parameter list
    // because rule_info is for the rule that uses this production, not this
    // production.
    RuleInfo rule_info2(out, rule_info);
    elem->GenerateRuleForAlternate(&rule_info2);
    separator = "\n  | ";
  }
  out << "\n  ;\n";
  parser->rules_.emplace_back(out.str());
}

int ParseTree::GenerateArrayMatcher(RuleInfo* rule_info, int min_size,
                                    ParseTree* t1, ParseTree* t2,
                                    int position) {
  if (t2 && !t2->IsLiteral()) {
    Error("right child of '*' must be a symbol");
    return position;
  }
  ParseTreeAttribute* attribute = t1->AsAttribute();
  if (attribute) {
    if (!attribute->is_array) {
      Error(StringPrintf("%s must be an array",
                         attribute->attribute_name.c_str()));
      return position;
    }
    // This is an optimized case: an iterator where the left child is an
    // array can return the array directly and does not need to be a keyword
    // arg. The logic to implement this is scattered; search for KEYWORD_ARRAYS.
    return GenerateSingleArrayMatcher(rule_info, min_size, attribute, t2,
                                      position);
  }
  ParseTreeItemList* item_list = t1->AsItemList();
  if (item_list) {
    return GenerateMultiArrayMatcher(rule_info, min_size, item_list, t2,
                                     position);
  }
  ParseTreeAltList* alt_list = t1->AsAltList();
  if (alt_list) {
    return GenerateAltIteratorMatcher(rule_info, min_size, alt_list, t2,
                                      position);
  }
  Error("illegal left-side of iterator");
  return position;
}

int ParseTree::GenerateSingleArrayMatcher(RuleInfo* rule_info, int min_size,
                                          ParseTreeAttribute* attribute,
                                          ParseTree* separator_pattern,
                                          int position) {
  string source_type = attribute->source_type;
  string base_nonterminal = parser->GetTypeMatcher(source_type);
  // GetTypeMatcher(source_type) above must be called before referencing
  // type_to_nonterminal_type_[source_type].
  string nonterminal =
      parser->CreateNontermial(attribute->declare_full_type, "%Qarray%d_%s",
                               min_size, source_type.c_str());
  rule_info->stream << " " << nonterminal;
  assert(attribute->declare_full_type ==
         parser->GetNonterminalType(nonterminal));
  rule_info->AddPositionalArg(position, attribute);

  // Generate the actual rule to match the array.
  stringstream out;
  RuleInfo rule_info2(out, rule_info);
  out << nonterminal;
  const char* separator = "\n  : ";
  if (min_size == 0) {
    out << separator << "{ $$ = " << attribute->full_type
                << "(); }";
    separator = "\n  | ";
  }
  if (min_size == 1 || separator_pattern) {
    out << separator << base_nonterminal << "{ $$ = "
                << attribute->full_type
                << "(); $$.emplace_back($1); }";
    separator = "\n  | ";
  }
  out << separator << nonterminal << " ";
  int position2 = GenerateTreeMatcher(separator_pattern, &rule_info2, 2, false);
  out << " " << base_nonterminal << " { $$ = $1; $$.emplace_back($"
              << position2 << "); }\n";
  parser->rules_.emplace_back(out.str());

  return position + 1;
}

int ParseTree::GenerateMultiArrayMatcher(RuleInfo* rule_info, int min_size,
                                         ParseTreeItemList* item_list,
                                         ParseTree* separator_pattern,
                                         int position) {
  bool found_array = false;
  for (auto elem : item_list->array) {
    ParseTreeAttribute* attr = elem->AsAttribute();
    if (attr) {
      if (!attr->is_array) {
        Error(
            StringPrintf("%s must be an array", attr->attribute_name.c_str()));
        return position;
      }
      found_array = true;
    }
  }
  string nonterminal = parser->CreateNontermial(
      "AttributeMap", "multiarray%s_%s", min_size == 0 ? "STAR" : "PLUS",
      rule_info->class_def->class_name.c_str());
  rule_info->stream << " " << nonterminal;
  // Indicate the position of the AttributeMap in the matched line.
  if (found_array) {
    rule_info->AddKeywordArgs(position);
  }

  // Generate the actual rule to match the arrays.
  stringstream out;
  out << nonterminal;
  const char* separator = "\n  : ";
  if (min_size == 0) {
    out << separator << "{ $$ = AttributeMap(); }";
    separator = "\n  | ";
  }
  if (min_size == 1 || separator_pattern) {
    RuleInfo rule_info2(out, rule_info);
    out << separator;
    GenerateTreeMatcher(item_list, &rule_info2, 1, false);
    out << " {\n      $$ = AttributeMap();";
    for (const auto& elem : rule_info2.positional_matches) {
      out
          << "\n      $$.Push<" << elem.second->declare_base_type << ">(\""
          << elem.second->attribute_name << "\", " << elem.first << ");";
    }
    out << " }";
    separator = "\n  | ";
  }
  {
    RuleInfo rule_info2(out, rule_info);
    out << separator << nonterminal << " ";
    int position2 =
        GenerateTreeMatcher(separator_pattern, &rule_info2, 2, false);
    position2 = GenerateTreeMatcher(item_list, &rule_info2, position2, false);
    out << " {\n      $$ = $1;";
    for (const auto& elem : rule_info2.positional_matches) {
      out << "\n      $$.Push(\"" << elem.second->attribute_name
                  << "\", $" << elem.first << ");";
    }
    out << " }";
  }
  out << "\n  ;\n";
  parser->rules_.emplace_back(out.str());
  return position + 1;
}

int ParseTree::GenerateAltIteratorMatcher(RuleInfo* rule_info, int min_size,
                                          ParseTreeAltList* alt_list,
                                          ParseTree* separator_pattern,
                                          int position) {
  string nonterminal = parser->CreateNontermial(
      "AttributeMap", "altiter%s_%s", min_size == 0 ? "STAR" : "PLUS",
      rule_info->class_def->class_name.c_str());
  rule_info->stream << " " << nonterminal;
  // Indicate the position of the AttributeMap in the matched line.
  rule_info->AddKeywordArgs(position);

  // Generate the actual rule to match the arrays.
  stringstream out;
  out << nonterminal;
  const char* separator = "\n  : ";
  if (min_size == 0) {
    out << separator << "{ $$ = AttributeMap(); }";
    separator = "\n  | ";
  }
  // There is no way to directly get the nonterminal symbol generated by
  // GenerateTreeMatcher() so we create a special stream just to get it.
  stringstream alt_nonterminal_stream;
  RuleInfo rule_info2(alt_nonterminal_stream, rule_info);
  rule_info2.array_elements = true;
  int position2 = GenerateTreeMatcher(alt_list, &rule_info2, 1, false);
  // GenerateTreeMatcher should only generate one nonterminal in this situation.
  assert(position2 == 2);
  string alt_nonterminal = alt_nonterminal_stream.str();

  if (min_size == 1 || separator_pattern) {
    // Generate the non-empty base case: alt_A { $$ = $1 }
    out << separator << alt_nonterminal << " { $$ = $1; }";
    separator = "\n  | ";
  }
  out << separator << nonterminal << " ";
  // Indicate the position of the AttributeMap from the recursive rule.
  rule_info2.AddKeywordArgs(1);
  RuleInfo rule_info3(out, rule_info);
  position2 = GenerateTreeMatcher(separator_pattern, &rule_info3, 2, false);
  out << alt_nonterminal;
  out << " { $$ = $1; $$->Merge($" << position2 << "); }";

  out << "\n  ;\n";
  parser->rules_.emplace_back(out.str());
  return position + 1;
}

}  // namespace classp
