OFILES=formatter-generator.o lexer-base.o parse-tree.o cpp11-generator.o \
  parser-generator.o parser-base.o stringprintf.o classp.o \
  classp.yacc.o classp.lex.o
CCFILES=${OFILES:.o=.cc}
YHEADERS=classp.yacc.hh location.hh stack.hh position.hh
GENERATE_TESTS=t1 t2 t3 t4 t5 t6 t7 t8 t9 prec1 assign1 example1 example2 classp_lang
SAMPLES_TESTS=samples t1 t2 t3 t4 t5 assign1 example1 example2
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
# the +skeleton+.[hlu] were originally generated from +skeleton+.classp by the
# parser but are now edited by hand. The following rules are used to make sure
# that the generation continues to work. It's both a fallback tool and an
# extra check on code generation.
#
GEN_DIR=generated-skeleton-files
GEN_FILES=$(GEN_DIR)/+skeleton+.h $(GEN_DIR)/+skeleton+.l $(GEN_DIR)/+skeleton+.y

$(GEN_FILES): +skeleton+.classp classp
	mkdir -p $(GEN_DIR) && cp --no-preserve=all +skeleton+.classp $(GEN_DIR)
	$(CLASSP) $(GEN_DIR)/+skeleton+.classp

check-gen: $(GEN_FILES)
	@echo 'checking skeleton generation'
	@diff -q +skeleton+.h $(GEN_DIR)/+skeleton+.h ;\
	  diff -q +skeleton+.l $(GEN_DIR)/+skeleton+.l ;\
	  diff -q +skeleton+.y $(GEN_DIR)/+skeleton+.y

DIFF_CMD=diff -y --suppress-common-lines
diff-gen:
	$(DIFF_CMD) +skeleton+.h $(GEN_DIR)/+skeleton+.h ; true
	$(DIFF_CMD) +skeleton+.l $(GEN_DIR)/+skeleton+.l ; true
	$(DIFF_CMD) +skeleton+.y $(GEN_DIR)/+skeleton+.y ; true

copy-generated-files:
	cp $(GEN_FILES) .

#
# Installation, testing, and cleaning
#
install: $(INSTALL_DIR)/classp

$(INSTALL_DIR)/classp: checked_build
	cp classp $(INSTALL_DIR)

tests: checked_build
	./test.sh $(GENERATE_TESTS) -rc samples

samples: checked_build
	./test.sh -r $(SAMPLES_TESTS)


clean:
	rm -f classp $(OFILES) classp.lex.cc classp.yacc.cc $(YHEADERS) *.d \
    skeleton-generator.h
	rm -rf $(GEN_DIR)

.PHONY: checked_build install clean check-gen diff-gen copy-generated-files

# Dependency management
$(OFILES) : $(YHEADERS)

# pull in dependency info for existing .o files
-include $(OFILES:.o=.d)
