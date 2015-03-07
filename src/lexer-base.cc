/*
 * This file is a part of the Classp parser, formatter, and AST generator.
 * Description: Implementation of class LexerBase.
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
#include "lexer-base.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "parser-base.h"

namespace classp {

LexerBase::LexerBase(FILE* source, ParserBase* parser)
    : parser_(parser),
      source_(source),
      file_name_(new string("input")),
      location_(file_name_.get()),
      initial_token_(yyParser::token::TOK_EOF) {}

LexerBase::~LexerBase() {
  // This should be done by the parent, but it uses delete, not delete[]...
  delete[] yy_state_buf;
  yy_state_buf = NULL;

  if (yy_start_stack != NULL) {
    free(yy_start_stack);
    yy_start_stack = NULL;
  }
}

int LexerBase::LexerInput(char* buf, int max_size) {
  if (source_ == NULL) {
    // The file was not found.
    return 0;
  } else {
    int i = 0;
    // The following condition ensures that we can store a decoded rune.
    while (i < max_size) {
      const int c = getc(source_);
      if (c == EOF) {
        break;
      } else {
        buf[i] = c;
        ++i;
      }
    }
    return i;
  }
}

void LexerBase::LexerError(const char* msg) {
  parser_->Error(location_,
                 StringPrintf("%s at %s:%d.%d-%d.%d",
                              msg,
                              location_.begin.filename->c_str(),
                              location_.begin.line,
                              location_.begin.column,
                              location_.end.line,
                              location_.end.column));
}

const std::string* LexerBase::file_name() {
  return file_name_.get();
}

void LexerBase::UnputAndResetLocation(char c) {
  assert(location_.end.column > 0);
  location_.end.column--;
  yyunput(c, yytext);
}

}  // namespace classp
