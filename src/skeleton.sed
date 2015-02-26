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
