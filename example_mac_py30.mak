#
#	Build the example on Mac OS X for version 3.0
#
CCC=g++
CCCFLAGS=-c -fPIC -I/Library/Frameworks/Python.framework/Versions/3.0/include/python3.0 -I.
LDSHARED=$(CCC) -bundle -g -F/Library/Frameworks -framework System -framework Python
LDLIBS=
PYTHON=/Library/Frameworks/Python.framework/Versions/3.0/Resources/Python.app/Contents/MacOS/Python

include example_common.mak
