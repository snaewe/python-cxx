#
#	BUild the example on Linux for version 2.6
#
CCC=g++ -c
CCCFLAGS=-fPIC -I/usr/include/python2.6 -I.
LDSHARED=g++ -shared
LDLIBS=
PYTHON=python2.6

include example_python2_common.mak
