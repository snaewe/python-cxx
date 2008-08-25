#
#	BUild the example on Linux for version 3.0
#
CCC=g++ -c -g
CC=gcc -c -g
CCCFLAGS=-fPIC -I/usr/local/include/python3.0 -I. -DPy_DEBUG
LDSHARED=g++ -g -shared
LDLIBS=
PYTHON=python3.0

include example_common.mak
