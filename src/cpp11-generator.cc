/*
 * This file is a part of the Classp parser, formatter, and AST generator.
 * Author: David Gudeman
 * Description: Functions to generate the c++11 class definitions.
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

void ParseTreeClassDecl::GenerateClassDefinition(ostream& out) {
  out << "class " << class_name;
  if (parent_classes.size() > 0) {
    const char* separator = ": public ";
    for (auto elem : parent_classes) {
      out << separator << elem;
      separator = ", ";
    }
  } else {
    out << ": public AstNode";
  }
  out << " {\n public:\n  "
      << "string className() override { return \"" << class_name
      << "\"; }\n  ";
  GenerateConstructorDeclaration(out);
  out << ";\n";
  if (parseable) {
    out << "  static " << class_name
        << "* parse(istream& input, ostream& errors);\n";
  }
  GenerateClassDeclarations(out);
  out << "\n};\n";
}

void ParseTreeClassDecl::GenerateMethodDefinitions(ostream& out) {
  GenerateConstructor(out);

  if (parseable) {
    out << class_name
        << "* " << class_name << "::parse(istream& input, ostream& errors) {\n"
        "  return dynamic_cast<" << class_name << "*>(" << parser->namespace_tag
        << "::parse(input, errors));\n"
        "}\n";
  }

  // Generate the printer.
  out << "void " << class_name << "::printMembers(ostream& out) {";
  for (auto elem : parent_classes) {
    out << "\n  " << elem << "::printMembers(out);\n";
  }
  for (auto elem : class_body) {
    ParseTreeAttribute* attribute = elem->AsAttribute();
    if (attribute) {
      attribute->GeneratePrinter(out);
    }
  }
  out << "\n}\n";

  GenerateClassFormatter(out);
}

void ParseTreeAttribute::GeneratePrinter(ostream& stream) {
  if (is_optional && !default_value) {
    stream << "\n  if (has_" << attribute_name << ") {"
           << "\n    out << \" " << attribute_name << ":\";"
           << "\n    classpPrint(out, " << attribute_name << ");"
           << "\n  } else {"
           << "\n    out << \"" << attribute_name << "[not defined]\";\n    }";
  } else {
    stream << "\n  out << \" " << attribute_name << ":\";";
    if (is_optional) {
      stream << "\n  if (!has_" << attribute_name << ") {"
          << "\n    out << \"[defaults to]\";";
    }
    stream << "\n  classpPrint(out, " << attribute_name << ");";
  }
}

void ParseTreeClassDecl::GenerateClassDeclarations(ostream& out) {
  out << "  void printMembers(ostream& out) override;\n";

  GenerateClassFormatterDeclarations(out);

  // Generate the attribute declarations.
  for (auto elem : class_body) {
    ParseTreeAttribute* attribute = elem->AsAttribute();
    if (attribute) {
      out << "\n  " << attribute->declare_full_type << " "
                       << attribute->attribute_name;
      if (attribute->is_ptr) out << " = nullptr";
      out << ";";
    }
  }

  // Generate the has_ declarations for optional attributes.
  for (auto elem : class_body) {
    ParseTreeAttribute* attribute = elem->AsAttribute();
    if (attribute && attribute->is_optional) {
      out << "\n  bool has_" << attribute->attribute_name << " = false;";
    }
  }
}

void ParseTreeClassDecl::GenerateConstructorDeclaration(ostream& out) {
  out << class_name << "("
      << "ParseState parseState";
  for (const auto& attribute : required_params) {
    out << ", " << attribute->declare_full_type << " "
                       << attribute->attribute_name;
  }
  if (optional_params.size() > 0) {
    out << ", AttributeMap& keyword_args";
  }
  out << ")";
}

void ParseTreeClassDecl::GenerateConstructor(ostream& out) {
  out << class_name << "::";
  GenerateConstructorDeclaration(out);
  const char* separator = "\n    : ";
  if (parent_classes.size() == 0) {
    out << separator << "AstNode(parseState)";
    separator = "\n    , ";
  } else {
    // Generate the parent constructors.
    for (auto elem : parent_classes) {
      out << separator << elem << "(parseState";
      parser->GetClassDecl(elem)->GenerateConstructorArgs(out);
      out << ")";
      separator = "\n    , ";
    }
  }
  // Generate initializers for the constructor parameters.
  for (int i = num_inherited_required_params; i < required_params.size(); i++) {
    const string& name = required_params[i]->attribute_name;
    out << separator << name << "(" << name << ")";
    separator = "\n    , ";
  }
  out << " {";
  if (optional_params.size() > 0) {
    // Generate initializers for members in the keyword list.
    for (const auto& attribute : optional_params) {
      const string& name = attribute->attribute_name;
        out << "\n  ";
      if (attribute->default_value) {
        out << "if (!(";
      }
      if (attribute->is_optional) {
        out << "has_" << name << " = ";
      }
      out << "keyword_args.Take(\""<< name << "\", " << name << ")";
      if (attribute->default_value) {
        out << ")) {\n    " << name << " = ";
        attribute->default_value->GenerateExpression(out);
        out << ";\n  }";
      } else {
        out << ";";
      }
    }
  }
  out << "\n}\n\n";
}

void ParseTreeClassDecl::GenerateConstructorArgs(ostream& out) {
  // Generate the required params in order.
  for (auto attr : required_params) {
    out << ", " << attr->attribute_name;
  }
  if (optional_params.size() > 0) {
    out << ", keyword_args";
  }
}

}  // namespace classp
