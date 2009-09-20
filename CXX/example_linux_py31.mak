#
#	BUild the example on Linux for version 3.1
#
CCC=g++ -c -g
CC=gcc -c -g
CCCFLAGS=-fPIC -I/usr/local/include/python3.1 -I. -DPy_DEBUG -UPYCXX_DEBUG
LDSHARED=g++ -g -shared
LDLIBS=
PYTHON=python3.1

include example_python3_common.mak
