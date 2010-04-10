#
#	BUild the example on Linux for version 2.6
#
PREFIX=/usr
CCC=g++ -g -c
CCCFLAGS=-fPIC -I$(PREFIX)/include/python2.6 -I.
LDSHARED=g++ -g -shared
LDLIBS=
PYTHON=python2.6

include example_python2_common.mak
