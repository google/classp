# put a backslash in front of any backslashes or double quotes
1,$s/\\/\\\\/g
1,$s/"/\\"/g

# put the line inside double quotes with a newline at the end
1,$s/.*/      "&\\n"/

# replace any references to "+skeleton+" with the namespace name
1,$s/@\([^@][^@]*\)@/" << \1 << "/g

# print a file header
1i\
/* THIS IS AN AUTOMATICALLY GENERATED FILE. DO NOT EDIT IT. */

# generate the lexer
/BEGIN LEXER/c\
\
void ParserBase::PrintLexer(ostream& out) {\
  out <<\
      "/* BEGIN LEXER */\\n"

/BEGIN GENERATED TOKENS/,/END GENERATED TOKENS/c\
      "  /* BEGIN GENERATED TOKENS */\\n";\
  PrintTokenLexers(out);\
  out << \
      "  /* END GENERATED TOKENS */\\n"

/END LEXER/c\
      "/* END LEXER */\\n";\
}

# Generate the parser
/BEGIN PARSER/c\
\
void ParserBase::PrintParser(ostream& out) {\
  out <<\
      "/* BEGIN PARSER */\\n"

/BEGIN CODE PREFIX/,/END CODE PREFIX/c\
      "/* BEGIN CODE PREFIX */\\n";\
  PrintCodePrefix(out);\
  out << \
      "/* END CODE PREFIX */\\n"

/BEGIN GENERATED TOKEN DECLARATIONS/,/END GENERATED TOKEN DECLARATIONS/c\
      "/* BEGIN GENERATED TOKEN DECLARATIONS */\\n";\
  PrintTokenDeclarations(out);\
  out << \
      "/* END GENERATED TOKEN DECLARATIONS */\\n"

/BEGIN NONTERMINAL TYPES/,/END NONTERMINAL TYPES/c\
      "/* BEGIN NONTERMINAL TYPES */\\n";\
  PrintNonterminalTypes(out);\
  out << \
      "/* END NONTERMINAL TYPES */\\n"

/BEGIN PARSEABLE/,/END PARSEABLE/c\
      "/* BEGIN PARSEABLE */\\n";\
  PrintParseableClassRules(out);\
  out << \
      "/* END PARSEABLE */\\n"

/BEGIN PRODUCTIONS/,/END PRODUCTIONS/c\
      "/* BEGIN PRODUCTIONS */\\n";\
  PrintProductions(out);\
  out << \
      "/* END PRODUCTIONS */\\n"

/BEGIN SAMPLES/,/END SAMPLES/c\
      "/* BEGIN SAMPLES */\\n";\
  PrintSampleTests(out);\
  out << \
      "/* END SAMPLES */\\n"

/BEGIN PARSE PARSEABLE/,/END PARSE PARSEABLE/c\
      "/* BEGIN PARSE PARSEABLE */\\n";\
  PrintParseParseable(out);\
  out << \
      "/* END PARSE PARSEABLE */\\n"

/END PARSER/c\
      "/* END PARSER */\\n";\
}

/BEGIN METHOD DEFINITIONS/,/END METHOD DEFINITIONS/c\
      "/* BEGIN METHOD DEFINITIONS */\\n";\
  PrintMethodDefinitioms(out);\
  out << \
      "/* END METHOD DEFINITIONS */\\n"

# generate the header
/BEGIN HEADER/c\
\
void ParserBase::PrintHFile(ostream& out) {\
  out << \
      "/* BEGIN HEADER */\\n"

/BEGIN FORWARD DECLARATIONS/,/END FORWARD DECLARATIONS/c\
      "/* BEGIN FORWARD DECLARATIONS */\\n";\
  PrintForwardDeclarations(out);\
  out << \
      "/* END FORWARD DECLARATIONS */\\n"

/BEGIN CLASS DEFINITIONS/,/END CLASS DEFINITIONS/c\
      "/* BEGIN CLASS DEFINITIONS */";\
  PrintClassDefinitions(out);\
  out << \
      "/* END CLASS DEFINITIONS */\\n"

/END HEADER/c\
      "/* END HEADER */\\n";\
}
