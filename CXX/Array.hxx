#ifndef CXX_ARRAY_H__
#define CXX_ARRAY_H__
#include "CXX_Objects.h"
#include "Numeric/arrayobject.h"

namespace Py {

class Array: public Sequence
{
public:
    virtual bool accepts (PyObject *pyob) const {
        return pyob && PyArray_Check (pyob);
    }
    
    explicit Array (PyObject *pyob, bool owned = false): Sequence(pyob, owned) {
        validate();
    }
    
    Array(const Object& other): Sequence(*other) {
        validate();
    }
    
    Array& operator= (const Object& rhs) {
        return (*this = *rhs);
    }
    
    Array& operator= (PyObject* rhsp) {
        if(ptr() == rhsp) return *this;
        set(rhsp);
        return *this;
    }
    
    explicit Array (int n=0, PyArray_TYPES t = PyArray_DOUBLE)
        : Sequence(PyArray_FromDims(1, &n, t), true) {
        validate();
    }

    Array clone() const {
        PyObject *p = PyArray_CopyFromObject(ptr(), species(), rank(), rank());
        return Array(p, true);
    }

    int species() const {
        return PyArray_ObjectType(ptr(), 0);
    }

    int rank() const {
        return ((PyArrayObject*) ptr())->nd;
    }

    int dimension(int i) const {
        if (1 <= i && i <= rank()) {
            return ((PyArrayObject*) ptr())->dimensions[i-1];
        } else {
            return 1;
        }
    }

    int is_contiguous() const {
        return PyArray_ISCONTIGUOUS ((PyArrayObject*) ptr());
    }

    char* to_C() const {
        return ((PyArrayObject*) ptr())->data;
    }

    Array as_contiguous() {
        if (is_contiguous()) return Array(ptr());
        return Array((PyObject*)PyArray_ContiguousFromObject(ptr(), species(), 1, 0), true);
    }        
};

// the recommended species if converting to array
int species(const Object& ob) {
    return PyArray_ObjectType(*ob, 0);
}

int species(PyObject *pyob) {
    return PyArray_ObjectType(pyob, 0);
}

Array toArray(PyObject* p) {
    int t = species(p);
    if (t < 0) { 
        throw RuntimeError("Unsuitable object for toArray");
    }
    return Array(PyArray_CopyFromObject(p, t, 1, 0), true);
}

Array toArray(const Object&ob) {
    return toArray(*ob);
}

} // end namespace Py
#endif
