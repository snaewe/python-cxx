#
#	example_common.mak
#
#	include this mak file after defining the variables it needs
#
COMMON_OBJECTS=cxxsupport.o cxx_extensions.o cxxextensions.o IndirectPythonInterface.o
EXAMPLE_OBJECTS=example.o range.o rangetest.o $(COMMON_OBJECTS)
PYCXX_ITER_OBJECTS=pycxx_iter.o $(COMMON_OBJECTS)

all: example.so pycxx_iter.so

#
#	Example
#
example.so: $(EXAMPLE_OBJECTS)
	$(LDSHARED) -o $@ $(EXAMPLE_OBJECTS) $(LDLIBS)

example.o: Demo/example.cxx
	$(CCC) $(CCCFLAGS) -o $@ $<

range.o: Demo/range.cxx
	$(CCC) $(CCCFLAGS) -o $@ $<

rangetest.o: Demo/rangetest.cxx
	$(CCC) $(CCCFLAGS) -o $@ $<

#
#	pycxx_iter
#
pycxx_iter.so: $(PYCXX_ITER_OBJECTS)
	$(LDSHARED) -o $@ $(PYCXX_ITER_OBJECTS) $(LDLIBS)

pycxx_iter.o: Demo/pycxx_iter.cxx
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
	rm -f pycxx_iter.so

#
#	test rule
#
test: example.so pycxx_iter.so
	PYTHONPATH=. $(PYTHON) Demo/test_example.py
	PYTHONPATH=. $(PYTHON) Demo/test_pycxx_iter.py
