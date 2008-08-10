#
#	Build the example on Mac OS X for version 3.0
#
PY_LD_VER=3.0
CCC=g++ -g
CC=gcc -g
CCCFLAGS=-c -fPIC -I/Library/Frameworks/Python.framework/Versions/$(PY_LD_VER)/include/python$(PY_LD_VER) -I.
LDSHARED=$(CCC) -bundle -g -F/Library/Frameworks -framework System \
	/Library/Frameworks/Python.framework/Versions/$(PY_LD_VER)/Python
LDLIBS=
PYTHON=/Library/Frameworks/Python.framework/Versions/$(PY_LD_VER)/Resources/Python.app/Contents/MacOS/Python

include example_common.mak
