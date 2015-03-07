# This file is a part of the Classp parser, formatter, and AST generator.
# Description: Used to generate skeleton-generator.h from the skeleton files.
#
# Copyright 2015 Google Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# skip any lines with the skeleton comment marker "@#"
/^@#/d

# put a backslash in front of any backslashes or double quotes
/@@.*@@/!s/\\/\\\\/g
/@@.*@@/!s/"/\\"/g

# put the line inside double quotes with a newline at the end
/@@.*@@/!s/.*/      "&\\n"/

# replace any references to "@expr@" with code to evaluate and insert expr
/@@.*@@/!s/@\([^@][^@]*\)@/" << \1 << "/g

# handle any blank lines
/^ *$/c\
      "\\n"

# print a file header
1i\
/* THIS IS AN AUTOMATICALLY GENERATED FILE. DO NOT EDIT IT. */

# Generate code to create and end the print functions
/BEGIN LEXER/c\
\
void ParserBase::PrintLexer(ostream& out) {\
  out <<\
      "/* BEGIN LEXER */\\n"

/END LEXER/c\
      "/* END LEXER */\\n";\
}

/BEGIN PARSER/c\
\
void ParserBase::PrintParser(ostream& out) {\
  out <<\
      "/* BEGIN PARSER */\\n"

/END PARSER/c\
      "/* END PARSER */\\n";\
}

/BEGIN HEADER/c\
\
void ParserBase::PrintHFile(ostream& out) {\
  out << \
      "/* BEGIN HEADER */\\n"

/END HEADER/c\
      "/* END HEADER */\\n";\
}

# Generate code to add the inline function calls
/^ *@@/s/^\( *\)@@\(.*\)@@/      ;\n\1\2\n\1out <</
