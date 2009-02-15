#
#	example_python3_common.mak
#
#	include this mak file after defining the variables it needs
#
COMMON_OBJECTS=cxxsupport.o cxx_extensions.o cxxextensions.o IndirectPythonInterface.o
SIMPLE_OBJECTS=simple.o
SIMPLE2_OBJECTS=simple2.o
EXAMPLE_OBJECTS=example.o range.o rangetest.o
PYCXX_ITER_OBJECTS=pycxx_iter.o

all: simple.so simple2.so example.so pycxx_iter.so

#
#	Simple
#
simple.so: $(SIMPLE_OBJECTS) $(COMMON_OBJECTS)
	$(LDSHARED) -o $@ $(SIMPLE_OBJECTS) $(COMMON_OBJECTS) $(LDLIBS)

simple.o: Demo/Python3/simple.cxx
	$(CCC) $(CCCFLAGS) -o $@ $<

#
#	Simple2
#
simple2.so: $(SIMPLE2_OBJECTS) $(COMMON_OBJECTS)
	$(LDSHARED) -o $@ $(SIMPLE2_OBJECTS) $(COMMON_OBJECTS) $(LDLIBS)

simple2.o: Demo/Python3/simple2.cxx
	$(CCC) $(CCCFLAGS) -o $@ $<

#
#	Example
#
example.so: $(EXAMPLE_OBJECTS) $(COMMON_OBJECTS)
	$(LDSHARED) -o $@ $(EXAMPLE_OBJECTS) $(COMMON_OBJECTS) $(LDLIBS)

example.o: Demo/Python3/example.cxx
	$(CCC) $(CCCFLAGS) -o $@ $<

range.o: Demo/Python3/range.cxx
	$(CCC) $(CCCFLAGS) -o $@ $<

rangetest.o: Demo/Python3/rangetest.cxx
	$(CCC) $(CCCFLAGS) -o $@ $<

#
#	pycxx_iter
#
pycxx_iter.so: $(PYCXX_ITER_OBJECTS) $(COMMON_OBJECTS)
	$(LDSHARED) -o $@ $(PYCXX_ITER_OBJECTS) $(COMMON_OBJECTS) $(LDLIBS)

pycxx_iter.o: Demo/Python3/pycxx_iter.cxx
	$(CCC) $(CCCFLAGS) -o $@ $<

#
#	common objects
#
cxxsupport.o: Src/cxxsupport.cxx
	$(CCC) $(CCCFLAGS) -o $@ $<

cxx_extensions.o: Src/cxx_extensions.cxx
	$(CCC) $(CCCFLAGS) -o $@ $<

cxxextensions.o: Src/cxxextensions.c
	$(CC) -c $(CCCFLAGS) -o $@ $<

IndirectPythonInterface.o: Src/IndirectPythonInterface.cxx
	$(CCC) $(CCCFLAGS) -o $@ $< 

#
#	Clean rule
#
clean:
	rm -f *.o
	rm -f example.so
	rm -f simple.so
	rm -f simple2.so
	rm -f pycxx_iter.so

#
#	test rules
#
test: test_simple test_iter test_example

test_simple: simple.so
	PYTHONPATH=. $(PYTHON) Demo/Python3/test_simple.py

test_iter: pycxx_iter.so
	PYTHONPATH=. $(PYTHON) Demo/Python3/test_pycxx_iter.py

test_example: example.so
	PYTHONPATH=. $(PYTHON) Demo/Python3/test_example.py
