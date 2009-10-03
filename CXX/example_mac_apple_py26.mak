#
#	Build the example on Mac OS X for version 2.6
#
CC=gcc -arch x86_64
CCC=g++ -arch x86_64
CCCFLAGS=-c -g -fPIC -I/System/Library/Frameworks/Python.framework/Versions/2.6/include/python2.6 -I.
LDSHARED=$(CCC) -bundle -g -u _PyMac_Error -F/Library/Frameworks -framework System \
	/System/Library/Frameworks/Python.framework/Versions/2.6/Python
LDLIBS=
PYTHON=/usr/bin/Python

include example_python2_common.mak
