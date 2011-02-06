#!/bin/bash
set -e
OS=${1:-macosx}

for PYTHON in \
    python2.4 \
    python2.5 \
    python2.6 \
    python2.7 \
    python3.1
do
    if which $PYTHON >/dev/null
    then
        ${PYTHON} setup_makefile.py ${OS} tmp-$PYTHON.mak
        make -f tmp-$PYTHON.mak clean
        make -f tmp-$PYTHON.mak test
    fi
done
