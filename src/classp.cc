/*
 * This file is a part of the Classp parser, formatter, and AST generator.
 * Description: main() and some helper functions.
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
#include "parser-base.h"
#include "lexer-base.h"

#include <stdio.h>
#include <fstream>
using std::ofstream;

#ifdef _WIN32
#define PATH_SEPARATOR '\\'
#else
#define PATH_SEPARATOR '/'
#endif

static void SplitFileName(const string& name, string* directory, string* base, string* extension) {
  size_t last_separator = name.rfind(PATH_SEPARATOR);
  string filename;
  if (last_separator == string::npos) {
    *directory = "";
    filename = name;
  } else {
    *directory = name.substr(0, last_separator+1);
    filename = name.substr(last_separator+1);
  }
  size_t last_dot = filename.rfind('.');
  if (last_dot == string::npos) {
    *base = filename;
    *extension = "";
  } else {
    *base = filename.substr(0, last_dot);
    *extension = filename.substr(last_dot);
  }
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "usage: classp <input-file-name>\n";
    exit(1);
  }

  string directory;
  string base;
  string extension;
  SplitFileName(argv[1], &directory, &base, &extension);
  string base_pathname = directory + base;

  if (extension.size() > 0 && extension != ".classp") {
    std::cerr << "The only extension allowed is .classp\n";
    exit(1);
  }

  FILE* fp = fopen(argv[1], "r");
  if (!fp) {
    // maybe the open failed because we have to add the .classp extension
    if (base_pathname == argv[1]) {
      string classp_name = base_pathname + ".classp";
      fp = fopen(classp_name.c_str(), "r");
    }
    if (!fp) {
      std::cerr << "can't open " << argv[1] << "\n";
      exit(1);
    }
  }

  classp::ParserBase parser(fp);
  classp::ParseTree* result = parser.Parse();
  fclose(fp);
  if (result) {
    ofstream yfile(base_pathname + ".y");
    ofstream hfile(base_pathname + ".h");
    ofstream lfile(base_pathname + ".l");
    parser.SetNamespace(base);
    parser.PrintParser(yfile);
    parser.PrintHFile(hfile);
    parser.PrintLexer(lfile);
  } else {
    std::cerr << "parse failed\n";
    return 1;
  }
  return 0;
}
