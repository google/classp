#!/bin/bash

# usage: test.sh <tests1>* [ --run ] <tests2>*
# for each file f in <tests1>* copy the file from ../tests to a tmp directory
# and run classp on it, then compare the generated files to the golden files in
# ../tests. If --run is seen, then for all following files, also build them
# with -DPARSER_TEST (this causes generation of an executable that will run
# the samples given in the source file), then run the samples test.

compare_parse=1
run_tests=0
tests="$*"
errors=0

TESTDIR=/tmp/classparser
mkdir -p $TESTDIR
rm -f $TESTDIR/*
CLASSP=`pwd`/classp
INC=`cd ../include && pwd`
GOLD=../tests

for f in $tests ; do
  case $f in
  (-rc|-cr)
    compare_parse=1
    run_tests=1
    continue
    ;;
  (-c)
    compare_parse=1
    run_tests=0
    continue
    ;;
  (-r)
    compare_parse=0
    run_tests=1
    continue
    ;;
  esac
  cp $GOLD/$f $TESTDIR
  if (( $? )) ; then
    echo "Missing test '$f'"
    ((errors+=1))
    continue
  fi
  echo "--- parsing $f ---"
  $CLASSP $TESTDIR/$f
  if (( $? )) ; then
    ((errors+=1))
  else
    if (( compare_parse )) ; then
      # compare all of the test output files
      for out in $TESTDIR/$f.[hly] ; do
        diff --brief $out $GOLD/`basename $out`
        ((errors+=$?))
      done
    fi
    if (( run_tests )) ; then
      # build and run the samples
      echo "--- building and running $f ---"
      ( \
        cd $TESTDIR && \
        bison -o $f.yacc.cc $f.y && \
        flex -o $f.lex.cc $f.l && \
        g++ -o $f.bin -g -std=c++11 -I$INC -Wall -DPARSER_TEST \
            $f.yacc.cc $f.lex.cc && \
        ./$f.bin --samples ; \
      ) >$TESTDIR/$f.out
      cond=$?
      if [[ -a $TESTDIR/$f.out && -a $GOLD/$f.out ]] ; then
        # if there is a .out file recorded, then ignore errors in the run
        # and only show errors if there is a diff in the .out files.
        diff --brief $TESTDIR/$f.out $GOLD/$f.out
        ((errors+=$?))
      elif (( $cond )) ; then
        # show an error if any of the building and running steps failed
        echo "$f failed the samples test"
        ((errors+=1))
      fi
    fi
  fi
done
if ((errors)) ; then
  echo "$errors ERRORS"
else
  echo "all tests passed"
fi
