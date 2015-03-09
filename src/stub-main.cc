/*
 * This file is a part of the Classp parser, formatter, and AST generator.
 * Description: A stub main used to test the library builds.
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
#include "t1.h"
#include <fstream>
#include <sstream>

int main(int argc, char** argv) {
  std::stringstream in("a x 2 y 3 b x 4 y 5 c x 6 z 7");
  t1::AstNode* result = t1::D::parse(in, std::cout);
  if (result) {
    std::cout << "Succeeded:\n";
    result->format(std::cout);
    std::cout << "\n";
  } else {
    std::cout << "Parse failed.\n";
    return 1;
  }
  return 0;
}
