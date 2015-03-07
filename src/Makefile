OFILES=formatter-generator.o lexer-base.o parse-tree.o cpp11-generator.o \
  parser-generator.o parser-base.o stringprintf.o classp.o \
  classp.yacc.o classp.lex.o
CCFILES=${OFILES:.o=.cc}
YHEADERS=classp.yacc.hh location.hh stack.hh position.hh
GENERATE_TESTS=t1 t2 t3 t4 t5 t6 t7 t8 t9 t10 t11 \
    prec1 prec2 prec4 assign1 example1 example2 classp_lang expr1
SAMPLES_TESTS=samples t1 t2 t3 t4 t5 t11 \
    assign1 example1 example2 expr1 prec2 prec4
SKELETON_FILES=skeleton.h skeleton.y skeleton.l

c=-g
CXXFLAGS:=$(c) -std=c++11 -Wall -Wno-sign-compare
LEX=/usr/bin/flex
YACC=/usr/bin/bison
# This is for calling classp, not building it
CLASSP=./classp
INSTALL_DIR=../bin

%.o: %.cc
	$(CXX) -c $(CXXFLAGS) $*.cc -o $*.o
	$(CXX) -MM $(CXXFLAGS) $*.cc > $*.d

checked_build: classp
#	@make check-gen

classp: $(OFILES)
	$(CXX) -o $@ $(OFILES)

classp.lex.cc: classp.l
	$(LEX) -o $@ classp.l

classp.yacc.cc $(YHEADERS): classp.y
	$(YACC) -o classp.yacc.cc classp.y

parser-base.o : skeleton-generator.h

skeleton-generator.h : skeleton.sed $(SKELETON_FILES)
	sed -f skeleton.sed $(SKELETON_FILES) >$@

#
# Testing
#
tests: checked_build
	./test.sh $(GENERATE_TESTS) -rc samples

samples: checked_build
	./test.sh -r $(SAMPLES_TESTS)

update-all-generate-tests: tests
	./test.sh -U $(GENERATE_TESTS) samples

#
# Installation and cleaning
#
install: $(INSTALL_DIR)/classp

$(INSTALL_DIR)/classp: checked_build
	cp classp $(INSTALL_DIR)

clean:
	rm -f classp $(OFILES) classp.lex.cc classp.yacc.cc $(YHEADERS) *.d \
    skeleton-generator.h

.PHONY: checked_build install clean tests samples update-all-generate-tests

# Dependency management
$(OFILES) : $(YHEADERS)

# pull in dependency info for existing .o files
-include $(OFILES:.o=.d)
