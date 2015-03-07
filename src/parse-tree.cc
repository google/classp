/*
 * This file is a part of the Classp parser, formatter, and AST generator.
 * Description: Functions to build and analyze the parse tree.
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
#include "parse-tree.h"

#include <algorithm>

#include "lexer-base.h"
#include "parser-base.h"

namespace classp {

void ParseTree::Error(const std::string& msg) { parser->Error(location, msg); }

void ParseTree::Fatal(const std::string& msg) { parser->Fatal(location, msg); }

Associativity ParseTreeIdentifier::GetAssoc() {
  if (value == "left" || value == "assoc") return AssocLeft;
  if (value == "right") return AssocRight;
  if (value == "nassoc") return AssocNassoc;
  Error("Identifier is not an associativity.");
  return AssocNone;
}

ParseTreeClassDecl::ParseTreeClassDecl(ParserBase* parser,
                                       const yyParser::location_type& location,
                                       const string& class_name,
                                       const vector<string>& parent_classes,
                                       const vector<ParseTree*>& class_body)
    : ParseTree(parser, location),
      class_name(class_name),
      parent_classes(parent_classes),
      class_body(class_body) {
  parser->class_decls_[class_name] = this;
}

ParseTreeAttribute::ParseTreeAttribute(
    ParserBase* parser, const yyParser::location_type& location,
    const string& attribute_name, const string& source_type,
    const bool is_optional, const bool is_array, ParseTree* default_value,
    ParseTree* syntax_decl)
    : ParseTree(parser, location),
      attribute_name(attribute_name),
      source_type(source_type),
      is_optional(is_optional),
      is_array(is_array),
      default_value(default_value),
      syntax_decl(syntax_decl ? syntax_decl->AsSyntax() : nullptr) {}

void ParseTreeUnop::Print(ostream& stream_out) const {
  stream_out << op;
  operand1()->Print(stream_out);
}

void ParseTreeBinop::Print(ostream& stream_out) const {
  stream_out << "(";
  operand1()->Print(stream_out);
  stream_out << ") " << op << " (";
  operand2()->Print(stream_out);
  stream_out << ")";
}

void ParseTreeArray::Print(ostream& stream_out) const {
  stream_out << "[";
  for (auto elem : array) {
    stream_out << " ";
    elem->Print(stream_out);
  }
  stream_out << " ]";
}

void ParseTreeClassDecl::Print(ostream& stream_out) const {
  stream_out << "class ";
  stream_out << class_name << "{\n";
  for (auto elem : class_body) {
    elem->Print(stream_out);
  }
  stream_out << " \n}\n";
}

void ParseTreeAttribute::Print(ostream& stream_out) const {
  stream_out << "  ";
  if (is_optional) stream_out << "optional ";
  stream_out << source_type << " " << attribute_name;
  if (is_array) stream_out << "[]";
  if (syntax_decl) syntax_decl->Print(stream_out);
  stream_out << ";\n";
}

void ParseTreeSyntaxDecl::Print(ostream& stream_out) const {
  stream_out << "syntax(";
  syntax->Print(stream_out);
  stream_out << " )";
}

void ParseTreeItemList::Print(ostream& stream_out) const {
  stream_out << "(";
  for (auto spec : array) {
    stream_out << " ";
    spec->Print(stream_out);
  }
  stream_out << " )";
}

void ParseTreeAltList::Print(ostream& stream_out) const {
  stream_out << "(";
  bool first = true;
  for (auto spec : array) {
    if (first)
      first = false;
    else
      stream_out << " | ";
    spec->Print(stream_out);
  }
  stream_out << " )";
}

void ParseTreeArray::Decorate1() {
  for (auto elem : array) {
    elem->Decorate1();
  }
}

void ParseTreeClassDecl::AddParsingSubclass(ParseTreeClassDecl* subclass) {
  parsing_subclasses.push_back(subclass);
}

void ParseTreeClassDecl::AddAttributes(
    const map<string, ParseTreeAttribute*>& attrs) {
  for (const auto& elem : attrs) {
    if (elem.second->is_self) continue;
    if (attr_list.count(elem.first) > 0) {
      Error(StringPrintf(
          "inherited attribute %s conflicts with another attribute with the "
          "same name",
          elem.first.c_str()));
    } else {
      attr_list.insert(elem);
    }
  }
}

void ParseTreeClassDecl::Decorate1() {
  for (const auto& elem : parent_classes) {
    ParseTreeClassDecl* parent = parser->GetClassDecl(elem);
    if (parent) {
      parent->AddParsingSubclass(this);
    } else {
      Error(StringPrintf("parent class '%s' for class '%s' does not exist.",
                         elem.c_str(), class_name.c_str()));
    }
  }
  bool has_samples = false;
  for (auto elem : class_body) {
    ParseTreeAttribute* attribute = elem->AsAttribute();
    if (attribute) {
      if (attr_list[attribute->attribute_name]) {
        parser->Error(
            StringPrintf("Attribute '%s' is declared twice in class '%s'",
                        attribute->attribute_name.c_str(), class_name.c_str()));
        return;
      }
      attribute->Decorate1(this);
      attr_list[attribute->attribute_name] = attribute;
    } else {
      ParseTreeSyntaxDecl* syntax = elem->AsSyntax();
      if (syntax) {
        syntax->Decorate1(this, nullptr);
        syntax_list.push_back(syntax);
      } else {
        ParseTreeIdentifier* id = elem->AsIdentifier();
        if (id) {
          // %parseable is a special attribute that says to create a production
          // only for this class and those it needs for parsing.
          if (id->value == "parseable") {
            parseable = true;
            is_parsed = true;
            parser->AddStart(this);
          } else {
            Error(
                StringPrintf("Unkown class attribute: %s", id->value.c_str()));
          }
        } else {
          ParseTreeSample* sample = dynamic_cast<ParseTreeSample*>(elem);
          if (sample) {
            has_samples = true;
            parser->AddSample(class_name, sample);
          }
        }
      }
    }
  }
  if (has_samples && !parseable) {
    parser->Error(StringPrintf(
        "Class '%s' has samples but is not parseable",
        class_name.c_str()));
  }
  // Create the special self attribute and add it to the class.
  ParseTreeAttribute* self_attr =
      new ParseTreeAttribute(parser, location, this);
  self_attr->Decorate1(this);
  attr_list[SELF_ATTR_NAME] = self_attr;
}

void ParseTreeAttribute::Decorate1(ParseTreeClassDecl* class_def) {
  containing_class = class_def;

  // Set up the type information.
  class_val = parser->GetClassDecl(source_type);
  if (class_val) {
    base_type = source_type;
    declare_base_type = ClassDeclareType(source_type);
    is_ptr = true;
  } else {
    base_type = declare_base_type = source_type;
    is_ptr = false;
  }
  if (is_array) {
    full_type = StringPrintf("vector<%s>", declare_base_type.c_str());
    declare_full_type = full_type;
    is_ptr = false;
  } else {
    full_type = base_type;
    declare_full_type = declare_base_type;
  }

  // Decorate1 for the attribute-local syntax
  if (syntax_decl) {
    syntax_decl->Decorate1(class_def, this);
  }
}

void ParseTreeClassDecl::Validate() {
  int i = 0;
  for (auto elem : parent_classes) {
    if (parser->class_decls_.count(elem) == 0) {
      Error(StringPrintf("parent class '%s' does not exist", elem.c_str()));
    }
    if (++i > 1) {
      // Some code has been written to handle multiple parent classes, but it
      // hasn't been tested and the semantics have not been completelyl thought
      // out.
      Error("Only one parent class is supported.");
    }
  }
}

// This must happen after Decorate1.
void ParseTreeArray::Decorate2() {
  // Make a list of all top-level classes
  vector<ParseTreeClassDecl*> classes;
  for (auto elem : array) {
    ParseTreeClassDecl* class_def = elem->AsClass();
    if (class_def && class_def->parent_classes.empty()) {
      classes.push_back(class_def);
    }
  }
  // For each class in classes, add all of its attributes to each of its
  // children, and then add the children to the end so that they will get
  // processed in their turn.
  for (int i = 0; i < classes.size(); i++) {
    auto class_def = classes[i];
    for (auto subclass : class_def->parsing_subclasses) {
      subclass->AddAttributes(class_def->attr_list);
      classes.push_back(subclass);
    }
  }

  // Now that all of the direct and inherited attributes are in each class's
  // attribute list, we can resolve expressions.
  for (auto elem : classes) {
    elem->ResolveSyntax();
  }

  // Set up arguments for the constructor. This must happen after
  // ResolveSyntax(). This relies on the order of classes.
  for (auto elem : classes) {
    elem->SetConstructorArguments();
  }

  // If nothing is declared parseable, we assume this is a test file
  // and generate productions for everything that does not have a parent.
  // TODO(dgudeman): put this behavior under a switch.
  if (!parser->start_class_) {
    for (auto elem : classes) {
      if (elem->parent_classes.empty()) {
        elem->is_parsed = true;
        parser->AddClassProduction(elem);
      }
    }
  }
}

void ParseTreeClassDecl::ResolveSyntax() {
  // Resolve all syntaxes
  for (auto syntax : syntax_list) {
    syntax->ResolveSyntax(false);
  }
}

void ParseTreeClassDecl::SetConstructorArguments() {
  // First add required params from ancestors. This function must be called
  // in a pre-order traversal to make sure that the parents are processed
  // before children, otherwise this fails.
  for (const auto& parent : parent_classes) {
    auto class_def = parser->class_decls_[parent];
    for (auto attr : class_def->required_params) {
      required_params.push_back(attr);
    }
    // If any ancestor has the keyword arg, then the constructor for this
    // class must have it also so as to pass it up.
    if (class_def->has_keyword_arg) has_keyword_arg = true;
  }
  num_inherited_required_params = required_params.size();
  // Add this class's own parameters, both required and optional.
  for (const auto& parse_tree : class_body) {
    ParseTreeAttribute* attr = parse_tree->AsAttribute();
    if (attr) {
      if (attr->is_nested || attr->is_optional || attr->default_value) {
        optional_params.insert(attr);
      } else {
        attr->is_required = true;
        required_params.push_back(attr);
      }
    }
  }
  if (optional_params.size() > 0) has_keyword_arg = true;
}

ParseTreeAttribute* ParseTreeAttribute::SetFormatted(bool nested_alternate) {
  if (!is_case_assigned) {
    parser->AddParsedType(source_type, declare_base_type);
  }
  if (nested_alternate) {
    is_nested = true;
    if (!is_optional && !is_array && !default_value) {
      Error(StringPrintf(
          "attribute '%s' cannot appear in an alternate because it is "
          "not optional, not an array, and has no default",
          attribute_name.c_str()));
    }
#if 0 // these should be enabled, possibly as warnings, but they cause lots of
      // test to fail.
  } else if (is_optional) {
    parser->Error(
      StringPrintf("Attibute '%s' is declared optional but is not parsed in an"
                   " optional context.", attribute_name.c_str()));
  } else if (default_value) {
    parser->Error(
      StringPrintf("Attibute '%s' has a default value which can never be used"
                   " because it is not parsed in an optional context.",
                   attribute_name.c_str()));
#endif
  }
  return this;
}

ParseTree* ParseTreeAttribute::GetSyntax(ParseTreeSyntaxDecl* syntax_def,
                                         bool nested_alternate) {
  if (syntax_decl) {
    // This attribute has local syntax. Resolve the local syntax and return it.
    if (in_syntax_decl) {
      // We are inside the local synax of an attribute x and we see the name x.
      // Don't go into infinite recursion, just treat this like it is the self
      // token.
      return SetFormatted(nested_alternate);
    }
    assert(containing_class);
    in_syntax_decl = true;
    syntax_decl->ResolveSyntax(nested_alternate);
    in_syntax_decl = false;
    return syntax_decl->syntax;
  }
  if (is_self) {
    // This is the special "self" attribute $$. If we are resolving an
    // attribute-local syntax, then replace with the attribute.
    if (syntax_def->attr_def) {
      return syntax_def->attr_def->SetFormatted(nested_alternate);
    }
    if (nested_alternate) {
      Error("'self' is not allowed in a nested alternate in class syntax");
      return this;
    }
    // Otherwise this is a class self rule.
    syntax_def->is_self = true;
  }
  return SetFormatted(nested_alternate);
}

ParseTreeClassDecl* ParseTreeClassDecl::LookupSuperclass(const string& name) {
  for (auto s : parent_classes) {
    if (s == name) {
      assert(parser->class_decls_.count(name) > 0);
      return parser->class_decls_[name];
    }
  }
  return nullptr;
}

ParseTree* ParseTreeIdentifier::ResolveSyntax(ParseTreeSyntaxDecl* syntax_def,
                                              bool nested_alternate) {
  ParseTreeAttribute* attribute =
      syntax_def->class_def->LookupAttribute(value);
  if (attribute) {
    return attribute->GetSyntax(syntax_def, nested_alternate);
  }
  Error(StringPrintf("attribute %s does not exist", value.c_str()));
  return this;
}

ParseTree* ParseTreeMulti::ResolveSyntax(ParseTreeSyntaxDecl* syntax_def,
                                         bool nested_alternate) {
  for (auto& elem : array) {
    elem = elem->ResolveSyntax(syntax_def, nested_alternate);
  }
  return this;
}

ParseTree* ParseTreeUnop::ResolveSyntax(ParseTreeSyntaxDecl* syntax_def,
                                        bool nested_alternate) {
  if ((op == token::TOK_PLUS || op == token::TOK_STAR) &&
      !operand1()->AsIdentifier()) {
    // If the left side of an iterator is not an identifier (which must be an
    // array attribute) then treat it as a nested context because it is
    // implemented with keyword args. The logic to implement this is scattered;
    // search for KEYWORD_ARRAYS.
    ParseTreeMulti::ResolveSyntax(syntax_def, true);
  } else {
    ParseTreeMulti::ResolveSyntax(syntax_def, nested_alternate);
  }
  return this;
}

ParseTreeAttribute* ParseTreeSyntaxDecl::GetAttributeBeingFormatted(
    ParseTreeIdentifier* id) {
  if (id->value == SELF_ATTR_NAME && attr_def) return attr_def;
  return class_def->LookupAttribute(id->value);
}

ParseTree* ParseTreeBinop::ResolveSyntax(ParseTreeSyntaxDecl* syntax_def,
                                         bool nested_alternate) {
  if (IsAssignment()) {
    operand1()->ResolveSyntax(syntax_def, nested_alternate);
  } else if ((op == token::TOK_PLUS || op == token::TOK_STAR) &&
             !operand1()->AsIdentifier()) {
    // If the left side of an iterator is not an identifier (which must be an
    // array attribute) then treat it as a nested context because it is
    // implemented with keyword args. The logic to implement this is scattered;
    // search for KEYWORD_ARRAYS.
    ParseTreeMulti::ResolveSyntax(syntax_def, true);
  } else if (op == token::TOK_LBRACE) {
    // In a case pattern the identifier is not really parsed, it is just
    // assigned a value based on parsing something else.
    ParseTreeAttribute* attribute =
        syntax_def->GetAttributeBeingFormatted(operand1()->AsIdentifier());
    if (attribute) {
      attribute->is_case_assigned = true;
    }
    ParseTreeMulti::ResolveSyntax(syntax_def, nested_alternate);
  } else {
    ParseTreeMulti::ResolveSyntax(syntax_def, nested_alternate);
  }
  return this;
}

void ParseTreeSyntaxDecl::Decorate1(ParseTreeClassDecl* class_def,
                                    ParseTreeAttribute* attr_def) {
  this->class_def = class_def;
  this->attr_def = attr_def;

  // Set up the features (currently just associativity and precedence).
  for (auto elem : features) {
    ParseTreeBinop* binop = elem->AsBinop();
    if (!binop) {
      Error("unknown feature");
      continue;
    }
    if (assoc == AssocNone) {
      assoc = binop->operand1()->GetAssoc();
      precedence = binop->operand2()->GetFloat();
      if (precedence <= 0 || precedence >= 20) {
        Error(
            "Precedence must be between 0 and 20, not including the endpoints");
      }
    } else {
      Error("Only one associativity/precedence is allowed on a syntax.");
    }
  }
}

void ParseTreeSyntaxDecl::ResolveSyntax(bool nested_alternate) {
  syntax = syntax->ResolveSyntax(this, nested_alternate);
}

ParseTree* ParseTreeAltList::ResolveSyntax(ParseTreeSyntaxDecl* syntax_def,
                                           bool nested_alternate) {
  int i = 0;
  for (auto& elem : array) {
    ParseTreeSymbol* s = elem->AsSymbol();
    if (s && s->value.size() == 0) {
      if (i != array.size() - 1) {
        Error("The empty alternative must be last.");
      }
    }
    elem = elem->ResolveSyntax(syntax_def, true);
    if (!s && !elem->IsAssignment() && !elem->HasOptionalAttribute()) {
      Error(
          "Every alternate must have an optional attribute which may be"
          " declared optional or array or have a default value.");
    }
    i++;
  }
  return this;
}

bool ParseTreeMulti::HasOptionalAttribute() {
  for (auto elem : array) {
    if (elem && elem->HasOptionalAttribute()) return true;
  }
  return false;
}

ParseTree* ParseTreeItemList::ResolveSyntax(ParseTreeSyntaxDecl* syntax_def,
                                            bool nested_alternate) {
  for (auto& elem : array) {
    elem = elem->ResolveSyntax(syntax_def, nested_alternate);
  }
  return this;
}

void ParseTreeClassDecl::CopySyntaxToParsedAncestors(
    ParseTreeClassDecl* source) {
  if (is_parsed) {
    for (auto syntax_decl : source->syntax_list) {
      parsing_syntax.push_back(new ParseTreeSyntaxDecl(*syntax_decl));
    }
  }
  for (auto elem : parent_classes) {
    ParseTreeClassDecl* class_decl = parser->GetClassDecl(elem);
    if (class_decl) {
      class_decl->CopySyntaxToParsedAncestors(source);
    }
  }
}

bool compare_by_precedence(ParseTreeSyntaxDecl* v1, ParseTreeSyntaxDecl* v2) {
  return v1->precedence < v2->precedence;
}

void ParseTreeClassDecl::SetupPrecedences() {
  // Gather all of the precedences of all parsing syntaxes.
  bool has_unspecified_precedence = false;
  for (auto syntax_decl : parsing_syntax) {
    if (syntax_decl->precedence > 0) {
      precedences.insert(syntax_decl->precedence);
    } else if (syntax_decl->precedence < 0) {
      has_unspecified_precedence = true;
    }
  }

  if (precedences.empty()) return;

  if (has_unspecified_precedence) {
    // Add a max precedence one higher than the current max to represent the
    // value of the unspecified precedence.
    Precedence max_precedence = *precedences.rbegin() + 1;
    precedences.emplace(max_precedence);

    // These are the parsing syntaxes as opposed to the formatting sytnaxes. We
    // set unspecified precedences to max in order to make the parser generator
    // easier to write.
    for (auto syntax_decl : parsing_syntax) {
      if (syntax_decl->precedence < 0) {
        syntax_decl->precedence = max_precedence;
      }
    }
  }
  SetHasPrecedence();
  std::stable_sort(
      parsing_syntax.begin(), parsing_syntax.end(), compare_by_precedence);

  // Tell the parser that that there are multiple productions for this class.
  parser->AddPrecedenceProductions(class_name, precedences);
}

void ParseTreeArray::Decorate3() {
  // Classes that are not directly referenced in a parsing statement (is_parsed)
  // do not need productions generated for them. Copy the syntaxes of such
  // classes to their is_parsed ancestor classes. The rule for the syntax will
  // be generated in the production for those classes. For consistency, we also
  // copy the syntaxes of is_parsed classes to their is_parsed ancestors.
  for (auto elem : parser->class_decls_) {
    elem.second->CopySyntaxToParsedAncestors(elem.second);
  }

  // At this point, each parsed class has a commplete set of syntaxes to
  // to generate. Now set up precedences for each parsed class.
  for (auto class_def : parser->class_productions_) {
    class_def->SetupPrecedences();
  }
}

void ParseTreeClassDecl::SetHasPrecedence() {
  // This might be a class that has no syntaxes with precedence, but it has a
  // super class
  has_precedence = true;
  for (auto class_def : parsing_subclasses) {
    class_def->SetHasPrecedence();
  }
}

void ParseTreeClassDecl::AddPrecedenceParsed(Precedence prec) {
  has_precedence = true;
  if (is_parsed) precedences.insert(prec);
  for (auto elem : parent_classes) {
    ParseTreeClassDecl* class_decl = parser->GetClassDecl(elem);
    if (class_decl) {
      class_decl->AddPrecedenceParsed(prec);
    }
  }
}

ParseTreeAttribute* ParseTreeClassDecl::LookupAttribute(string attribute_name) {
  if (attr_list.count(attribute_name) == 0) return nullptr;
  return attr_list[attribute_name];
}

void ParseTreeIdentifier::GenerateExpression(ostream& stream) {
  stream << value;
}

ParseTreeSample::ParseTreeSample(ParserBase* parser,
                    const yyParser::location_type& location,
                    ParseTreeSymbol* sample, ParseTree* result)
    : ParseTree(parser, location),
      sample(sample->value) {
  if (result == nullptr) {
    type = EXPECT_SUCCEED;
  } else {
    ParseTreeIdentifier* id = result->AsIdentifier();
    if (id) {
      if (id->value == "succeed") type = EXPECT_SUCCEED;
      else if (id->value == "fail") type = EXPECT_FAIL;
      else if (id->value == "print") type = PRINT_RESULT;
      else parser->Error("Illegal identifier in sample() expression");
    } else {
      ParseTreeSymbol* symbol = result->AsSymbol();
      if (symbol) {
        type = COMPARE_RESULT;
        expected_result = symbol->value;
      } else {
        parser->Error("Wrong type second argument for sample()");
      }
    }
  }
}

void ParseTreeSample::PrintExpectedResult(ostream& out) {
  switch (type) {
    case EXPECT_SUCCEED: {
      out << "kSucceed";
      break;
    }
    case EXPECT_FAIL: {
      out << "kFail";
      break;
    }
    case PRINT_RESULT: {
      out << "kPrint";
      break;
    }
    case COMPARE_RESULT: {
      out << QuoteStringForC(expected_result);
      break;
    }
  }
}

}  // namespace classp
