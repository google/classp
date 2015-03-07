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
#include <stdlib.h>
#include <unistd.h>

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
  int opt;
  bool build = false;
  bool execute = false;
  bool run_samples = false;
  char* include_dir = nullptr;

  while ((opt = getopt(argc, argv, "bxsI:")) > 0) {
    switch (opt) {
    case 'b': build = true; break;
    case 'x': execute = true; break;
    case 's': run_samples = true; break;
    case 'I': include_dir = optarg; break;
    case '?': exit(1);
    }
  }
  if (run_samples) execute = true;
  if (execute) build = true;
  if (build && !include_dir) {
    include_dir = getenv("CLASSP_INCLUDE");
    if (!include_dir) {
      std::cerr << "must specify an include dir if building the test program\n";
      exit(1);
    }
  }
  if (optind != argc - 1) {
    std::cerr << "usage: classp -[bxs] [-I<include-dir>] <input-file-name>\n";
    exit(1);
  }

  string directory;
  string base;
  string extension;
  char* filename = argv[optind];
  SplitFileName(filename, &directory, &base, &extension);
  string base_pathname = directory + base;

  if (extension.size() > 0 && extension != ".classp") {
    std::cerr << "The only extension allowed is .classp\n";
    exit(1);
  }

  FILE* fp = fopen(filename, "r");
  if (!fp) {
    // maybe the open failed because we have to add the .classp extension
    if (base_pathname == filename) {
      string classp_name = base_pathname + ".classp";
      fp = fopen(classp_name.c_str(), "r");
    }
    if (!fp) {
      std::cerr << "can't open " << filename << "\n";
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
    exit(1);
  }
  if (build) {
    const int kBufSize = 1024;
    char buf[kBufSize];
    const char* base_name = base_pathname.c_str();

    snprintf(buf, kBufSize, "bison -o %s.yacc.cc %s.y", base_name, base_name);
    std::cerr << "executing: " << buf << "\n";
    if (system(buf)) exit(1);

    snprintf(buf, kBufSize, "flex -o %s.lex.cc %s.l", base_name, base_name);
    std::cerr << "executing: " << buf << "\n";
    if (system(buf)) exit(1);

    snprintf(buf, kBufSize, "g++ -g -o %s.bin -std=c++11 -I%s -Wall -DPARSER_TEST %s.yacc.cc %s.lex.cc",
        base_name, include_dir, base_name, base_name);
    std::cerr << "executing: " << buf << "\n";
    if (system(buf)) exit(1);

    if (!execute) exit(0);
    snprintf(buf, kBufSize, "%s.bin %s", base_name, run_samples ? "--samples" : "");
    std::cerr << "executing: " << buf << "\n";
    if (system(buf)) exit(1);
  }

  return 0;
}
