/* Minimal main program -- everything is loaded from the library */

#include "Python.h"
#include <iostream>
#include "CXX_Array.h"
#include <algorithm>

using namespace Py;

extern "C" int Py_Main(int argc, char** argv);
extern "C" void Py_Initialize();
extern "C" void Py_Finalize();
//extern "C" void PyImport_ImportModule(char *name);

int
main(int argc, char** argv)
{
    PyObject *ptype;
    PyObject *pvalue;
    PyObject *ptrace;

	std::cout << "Array facility test" << std::endl;
	Py_Initialize();
	import_array();
	PyImport_ImportModule("Numeric");

    try {
        Array t(12);
        for(int i=0; i < t.length(); ++i) t[i] = Float(i / 2.0);
        std::cout << t << std::endl;
        std::cout << "Rank is " << t.rank() << std::endl;
        std::cout << "Species is " << t.species() << std::endl;
        std::cout << "dimension(1) = " << t.dimension(1) << std::endl;
        std::cout << "Pointers " << t.ptr() << ", " << (void *) t.to_C() << 
            ", " << t.as_contiguous().ptr() << std::endl;
        List w(3);
        w[0] = Float(9.0);
        w[1] = Float(4.0);
        w[2] = Float(1.0);
        Array from_list = toArray(w);
        std::cout << "A list: " << w << std::endl;
        std::cout << "toArray: " << from_list << std::endl;
        std::cout << from_list.type() << std::endl;
	//return Py_Main(argc, argv);
        std::cout << "list+list: " << from_list + from_list << std::endl;
        std::cout << "add 2: " << from_list + Int(2) << std::endl;
		std::cout << "list[1]: " << from_list[1] << std::endl;
		std::sort(from_list.begin(), from_list.end());
		std::cout << "sorted: " << from_list << std::endl;
    }
    catch(Exception&) {
        std::cout << "Test failed via Exception" << std::endl;
        PyErr_Fetch (&ptype, &pvalue, &ptrace);
        Object v(pvalue, true);
        std::cout << v << std::endl;
    }

    Py_Finalize();
	return 0;
}
