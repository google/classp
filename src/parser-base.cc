/*
 * This file is a part of the Classp parser, formatter, and AST generator.
 * Author: David Gudeman
 * Description: Implementation of ParserBase.
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
#include "parser-base.h"

#include <fstream>
#include <cstdarg>
#include <ctype.h>

#include "lexer-base.h"

static inline bool is_wchar(int c) { return isalnum(c) || c == '_'; }

namespace classp {

void yyParser::error(const location_type& loc, const std::string& msg) {
  parser->Error(loc, msg);
}

ParserBase::ParserBase(FILE* source) : source_(source), lexer_(nullptr) {
  lexer_ = new LexerBase(source, this);
}

void ParserBase::AddStart(ParseTreeClassDecl* start_class) {
  if (start_class_) Error("Only one start rule is allowed.");
  start_class_ = start_class;
  AddClassProduction(start_class);
}

void ParserBase::AddSample(const string& class_name, ParseTreeSample* sample) {
  samples.emplace(class_name, sample);
}

void ParserBase::AddClassProduction(ParseTreeClassDecl* c) {
  if (c->production_required) return;
  c->production_required = true;
  class_productions.push_back(c);
  AddParsedType(c->class_name, ClassDeclareType(c->class_name));
}

ParserBase::~ParserBase() { delete lexer_; }

void ParserBase::Error(const yyParser::location_type& loc,
                       const std::string& msg) {
  std::cerr << "ERROR: " << loc << ":" << msg << "\n";
  num_errors_++;
}

void ParserBase::Error(const std::string& msg) {
  std::cerr << "ERROR: " << msg << "\n";
  num_errors_++;
}

void ParserBase::Fatal(const yyParser::location_type& loc,
                       const std::string& msg) {
  std::cerr << "FATAL: " << loc << ":" << msg << "\n";
  exit(1);
}

ParseTree* ParserBase::Parse() {
  ParseTree* result = nullptr;
  yyParser yyparser(this, &result);
  yyparser.parse();
  if (result) {
    decl_list = result->AsArray();
    assert(decl_list);
    decl_list->Decorate1();
    if (num_errors_ == 0) decl_list->Decorate2();
    if (num_errors_ == 0) decl_list->Decorate3();
    if (num_errors_ == 0) {
      for (auto& elem : decl_list->array) {
        ParseTreeClassDecl* class_decl = elem->AsClass();
        if (class_decl) {
          class_decl->GenerateProductions();
          stringstream class_def_stream;
          class_decl->GenerateClassDefinition(class_def_stream);
          class_defs_.emplace_back(class_def_stream.str());
        }
      }
      if (!start_class_ && !class_productions.empty()) {
        start_class_ = class_productions[0];
      }
    }
  }
  if (num_errors_ > 0) {
    std::cerr << num_errors_ << " errors.\n";
  }
  if (num_errors_ == 0) return result;
  return nullptr;
}

void ParserBase::PrintTokenLexers(ostream& out) {
  for (auto elem : token_to_name_) {
    for (const char* s = elem.first.c_str(); *s; s++) {
      if (!is_wchar(*s)) out << '\\';
      out << *s;
    }
    out << " { return YYParser::make_" << elem.second << "(yylloc); }\n";
  }
}

void ParserBase::PrintCodePrefix(ostream& out) {
  if (!code_prefix_.empty()) {
    out << "/* begin user-included code prefix */\n" << code_prefix_
        << "\n/* end user-included code prefix */\n";
  }
}

void ParserBase::PrintTokenDeclarations(ostream& out) {
  for (auto elem : token_to_name_) {
    out << "%token " << elem.second << " \"`" << elem.first << "`\"\n";
  }
#if 0
  for (auto elem : token_name_to_definition_) {
    out << "%token " << elem.first << "\n";
  }
#endif
}

void ParserBase::PrintNonterminalTypes(ostream& out) {
  for (auto elem : nonterminal_to_tag_) {
    if (token_name_to_definition_.count(elem.first) == 0) {
      out << "%type <" << tag_to_nonterminal_type_[elem.second]
          << "> " << elem.first << "\n";
    }
  }
}

void ParserBase::PrintProductions(ostream& out) {
  for (auto elem : rules_) {
    out << elem << "\n";
  }
}

void ParserBase::PrintForwardDeclarations(ostream& out) {
  for (auto elem : class_decls_) {
    out << "class " << elem.first << ";\n";
  }
}

void ParserBase::PrintClassDefinitions(ostream& out) {
  const char* separator = "";
  for (auto elem : class_defs_) {
    out << separator << elem;
    separator = "\n";
  }
}

void ParserBase::PrintParseableClassRules(ostream& out) {
  if (start_class_) {
    out << "  : " << GetTypeMatcher(start_class_->class_name)
        << " TOK_EOF { *result = $1; }\n";
  }
}

void ParserBase::PrintParseParseable(ostream& out) {
  if (start_class_ && start_class_->parseable) {
    out << "  AstNode* result = " << start_class_->class_name
        << "::parse(input, out);\n";
  } else {
    out << "  AstNode* result = nullptr;\n";
  }
}

void ParserBase::PrintMethodDefinitioms(ostream& out) {
  for (auto& elem : decl_list->array) {
    ParseTreeClassDecl* class_decl = elem->AsClass();
    if (class_decl) {
      class_decl->GenerateMethodDefinitions(out);
    }
  }
}

string QuoteStringForC(const string& s) {
  // TODO(dgudeman): This won't work if ')#A#' appears in the string.
  return "R\"#A#(" + s + ")#A#\"";
}

void ParserBase::PrintSampleTests(ostream& out) {
  for (const auto& elem : samples) {
    out << "  num_errors += ParseSample<" << elem.first << ">("
        << QuoteStringForC(elem.second->sample) << ", ";
    elem.second->PrintExpectedResult(out);
    out <<");\n";
  }
}

// Include the generated file that builds the skeleton.
#include "skeleton-generator.h"

void ParserBase::AddParsedType(const string& type_name,
                               const string& nonterminal_type) {
  assert(nonterminal_type.size() > 0);
  if (type_to_nonterminal_.count(type_name) != 0) {
    assert(tag_to_nonterminal_type_["type_" + type_name] == nonterminal_type);
    return;
  }

  string tag = "type_" + type_name;
  string nonterminal;
  // TODO (dgudeman): remove token_name_to_definition_
  if (type_name == "bool") {
    nonterminal = "TOK_BOOL";
    token_name_to_definition_["TOK_BOOL"] = "/* user defined */";
  } else if (type_name == "int") {
    nonterminal = "TOK_INT64";
    token_name_to_definition_["TOK_INT64"] = "/* user defined */";
  } else if (type_name == "string") {
    nonterminal = "TOK_STRING";
    token_name_to_definition_["TOK_STRING"] = "/* user defined */";
  } else if (type_name == "identifier") {
    nonterminal = "TOK_IDENTIFIER";
    token_name_to_definition_["TOK_IDENTIFIER"] = "/* user defined */";
  } else {
    ParseTreeClassDecl* class_decl = GetClassDecl(type_name);
    if (class_decl) {
      nonterminal = "class_" + type_name;
      class_decl->is_parsed = true;
    } else {
      // A user-defined type
      nonterminal = "UTOK_" + type_name;
      token_name_to_definition_["TOK_IDENTIFIER"] = "/* user defined */";
      return;
    }
  }
  // add to the various tables
  tag_to_nonterminal_type_[tag] = nonterminal_type;
  // TODO (dgudeman): don't add this to nonterminal_to_tag_ if it's a token type
  nonterminal_to_tag_[nonterminal] = tag;
  type_to_nonterminal_[type_name] = nonterminal;
  type_to_nonterminal_type_[type_name] = nonterminal_type;
}

void ParserBase::AddPrecedenceProductions(const string& class_name,
                                          set<Precedence> precedences) {
  assert(precedence_classes.count(class_name) == 0);
  precedence_classes.emplace(class_name);
  const string& nonterminal = type_to_nonterminal_[class_name];
  const string& tag = nonterminal_to_tag_[nonterminal];
  auto iter = precedences.begin();
  // skip the first precedence because that one uses the base name without a
  // precedence number.
  for (++iter; iter != precedences.end(); ++iter) {
    nonterminal_to_tag_[PrecedenceNonterminal(nonterminal, *iter)] = tag;
  }
}

string ParserBase::GetTypeMatcher(const string& type_name, int precedence) {
  ParseTreeClassDecl* class_decl = GetClassDecl(type_name);
  if (class_decl) {
    AddClassProduction(class_decl);
  }
  if (precedence_classes.count(type_name) > 0) {
    return PrecedenceNonterminal(type_to_nonterminal_[type_name], precedence);
  } else {
    return type_to_nonterminal_[type_name];
  }
}

string ParserBase::GetStringMatcher(const string& literal_string, int* count) {
  if (literal_string.empty()) {
    if (count) *count = 0;
    return "";
  }
  *count = 1;
  for (auto s1 = literal_string.c_str(); *s1; s1++) {
    if (!is_wchar(*s1)) return GetSymbolMatcher(literal_string);
  }
  return GetWordMatcher(literal_string);
}

string ParserBase::GetWordMatcher(const string& token) {
  if (token_to_name_.count(token) != 0) return token_to_name_[token];
  string name = "WORD_" + token;
  token_to_name_[token] = name;
  return name;
}

string ParserBase::GetSymbolMatcher(const string& token) {
  static int tok_num = 0;
  if (token_to_name_.count(token) != 0) return token_to_name_[token];
  string name;
  if (token.size() == 1) {
    switch (token[0]) {
      case ',':
        name = "TOK_COMMA";
        break;
      case '.':
        name = "TOK_PERIOD";
        break;
      case '(':
        name = "TOK_LPAREN";
        break;
      case ')':
        name = "TOK_RPAREN";
        break;
      case ';':
        name = "TOK_SEMICOLON";
        break;
      case '{':
        name = "TOK_LBRACE";
        break;
      case '}':
        name = "TOK_RBRACE";
        break;
      case '+':
        name = "TOK_PLUS";
        break;
      case '-':
        name = "TOK_MINUS";
        break;
      case '*':
        name = "TOK_STAR";
        break;
      case '/':
        name = "TOK_SLASH";
        break;
      case '\\':
        name = "TOK_BACKSLASH";
        break;
      case '=':
        name = "TOK_EQ";
        break;
      case '<':
        name = "TOK_LT";
        break;
      case '>':
        name = "TOK_GT";
        break;
      case '|':
        name = "TOK_BAR";
        break;
      case '!':
        name = "TOK_BANG";
        break;
      case '@':
        name = "TOK_AT";
        break;
      case '#':
        name = "TOK_HASH";
        break;
      case '$':
        name = "TOK_DOLLAR";
        break;
      case '%':
        name = "TOK_PERCENT";
        break;
      case '^':
        name = "TOK_UPARROW";
        break;
      case '&':
        name = "TOK_AMPERSAND";
        break;
    }
  }
  if (name.empty()) name = StringPrintf("SYM__%d", tok_num++);
  token_to_name_[token] = name;
  return name;
}

string ExtractTag(const string& declared_type) {
  if (declared_type == "AttributeMap*") {
    return "attr_map";
  }
  if (declared_type == "int64_t") {
    return "type_int";
  }
  const int array_prefix_length = 7;
  if (declared_type.substr(0, array_prefix_length) == "vector<") {
    // A pointer to a vector of class pointers: vector<type*>* or vector<type>*
    int type_name_length = declared_type.size() - array_prefix_length;
    if (declared_type[declared_type.size() - 3] == '*')
      type_name_length -= 3;
    else
      type_name_length -= 2;
    return "array_" + declared_type.substr(7, type_name_length);
  }
  if (declared_type[declared_type.size() - 1] == '*') {
    // A pointer to a class
    return "type_" + declared_type.substr(0, declared_type.size() - 1);
  }
  return "type_" + declared_type;
}

string ParserBase::CreateNontermial(const string& declared_type,
                                    const char* format, ...) {
  bool uniquify = false;
  const char* fmt = format;
  if (fmt[0] == '%' && fmt[1] == 'Q') {
    // This is a flag calling for a uniquify marker on this nonterminal even if
    // there is no duplicate at the moment. We skip past this flag to pass the
    // format on to StringAppendV because StringAppendV doesn't know what to do
    // with it.
    uniquify = true;
    fmt = format + 2;
  }

  // construct the nonterminal name.
  va_list ap;
  va_start(ap, format);
  string nonterminal;
  StringAppendV(&nonterminal, fmt, ap);
  va_end(ap);

  int n = ++uniquifier_[nonterminal];  // defaults to 1 for new nonterminals
  if (uniquify || n > 1) {
    // Uniquify the nonterminal name.
    nonterminal = StringPrintf("%s__%d", nonterminal.c_str(), n);
  }

  // Add info to the tables that aren't restricted to base types.
  string tag = ExtractTag(declared_type);
  tag_to_nonterminal_type_[tag] = declared_type;
  nonterminal_to_tag_[nonterminal] = tag;

  return nonterminal;
}

string PrecedenceNonterminal(const string& nonterminal, int prec) {
  if (prec <= 0) return nonterminal;
  return StringPrintf("%s_p%d", nonterminal.c_str(), prec);
}

}  // namespace classp
