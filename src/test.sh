#!/bin/bash

# This file is a part of the Classp parser, formatter, and AST generator.
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


# usage: test.sh <tests1>* [ --run ] <tests2>*
# for each file f in <tests1>* copy the file from ../tests to a tmp directory
# and run classp on it, then compare the generated files to the golden files in
# ../tests. If --run is seen, then for all following files, also build them
# with -DPARSER_TEST (this causes generation of an executable that will run
# the samples given in the source file), then run the samples test.

compare_parse=1
run_tests=0
do_updates=0
errors=0

TESTDIR=/tmp/classparser
CLASSP=`pwd`/classp
INC=`cd ../include && pwd`
GOLD=../tests

function check-and-update() {
  if [[ ! -a $1 ]] ; then return ; fi
  if ( ! diff --brief $1 $2 >&/dev/null ) ; then
    echo UPDATING $2
    if [[ -a $2 && -n $CHECKOUT ]] ; then
      # the gold file exists and there is a checkout command defined
      $CHECKOUT $2
    fi
    cp $1 $2
  fi
}

if [[ $1 == -U ]] ; then
  # update files that are already generated
  compare_parse=0
  run_tests=0
  do_updates=1
  shift
else
  mkdir -p $TESTDIR
  rm -f $TESTDIR/*
fi

for f in $* ; do
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
  (-Uc)
    # build files and update the changed ones
    compare_parse=1
    run_tests=0
    do_updates=1
    continue
    ;;
  esac
  if (( compare_parse || run_tests )) ; then
    # copy the test to a temp dir and run classp on it
    cp $GOLD/$f $TESTDIR
    if (( $? )) ; then
      echo "Missing test '$f'"
      ((errors+=1))
      continue
    fi
    echo "--- parsing $f ---"
    $CLASSP $TESTDIR/$f
    if (( $? )) ; then
      # generation of the classp files failed
      ((errors+=1))
      continue
    fi
  fi
  if (( do_updates )) ; then
    for out in $TESTDIR/$f.[hly] ; do
      check-and-update $out $GOLD/`basename $out`
    done
    if [[ -a $GOLD/$f.out ]] ; then
      # there is a .out file recorded so copy it as well
      check-and-update $TESTDIR/$f.out $GOLD/$f.out
    fi
  elif (( compare_parse )) ; then
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
      g++ -g -o $f.bin -std=c++11 -I$INC -Wall -DPARSER_TEST \
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
done
if ((do_updates)) ; then
  true
elif ((errors)) ; then
  echo "$errors ERRORS"
else
  echo "all tests passed"
fi
