#ifndef __r__h
#define __r__h
#include "CXX_Extensions.h"

#include STANDARD_HEADER(strstream)
USING(STD::ostrstream)
USING(namespace Py)

// Making an extension object
class r: public PythonExtension<r> {
public:
    long	start;
    long	stop;
    long	step;
    r(long start_, long stop_, long step_ = 1L) 
    {
        start = start_;
        stop = stop_;
        step = step_;
        std::cout << "r created " << this << std::endl;
    }

    virtual ~r()
    {
        std::cout << "r destroyed " << this << std::endl;
    }

    static void init_type(void);

    long length() const {
        return (stop - start + 1)/step;
    }

    long item(int i) const {
        return start + i * step;
    }

    r* slice(int i, int j) const {
        int first = start + i * step;
        int last = start + j * step;
        return new r(first, last, step);
    }
    
    r* extend(int k) const {
        return new r(start, stop + k, step);      
    }
    
    STD::string asString() const {
        ostrstream s;
        s << "r(" << start << ", " << stop << ", " << step << ")" << STD::ends;
        return STD::string(s.str());
    }  
    
    // override functions from PythonExtension
    virtual Py::Object repr();
    virtual Py::Object getattr( const char *name );

    virtual int sequence_length();
    virtual Py::Object sequence_item( int i );
    virtual Py::Object sequence_concat( const Py::Object &j );
    virtual Py::Object sequence_slice( int i, int j );

    // define python methods of this object
    Py::Object amethod (const Py::Tuple& args);
    Py::Object value (const Py::Tuple& args);
    Py::Object assign (const Py::Tuple& args); 
	Py::Object reference_count (const Py::Tuple& args) 
	{
		return Int(this->ob_refcnt);
	}

    Py::Object c_value(const Tuple&) const {
        List result;
        for(int i = start; i <= stop; i += step) {
            result.append(Int(i));
        }
        return result;
    }

    void c_assign(const Tuple&, const Py::Object& rhs) {
        Tuple w(rhs);
        w.verify_length(3);
        start = long(Int(w[0]));
        stop = long(Int(w[1]));
        step = long(Int(w[2]));
    }
};

class R: public SeqBase<Int> {
public:
    
    explicit R (PyObject *pyob): SeqBase<Int>(pyob) {
        validate();
    }

    explicit R(int start, int stop, int step = 1) 
        :SeqBase<Int>(FromAPI(new r(start, stop, step)))
    {
    }

    R(const R& other): SeqBase<Int>(*other) {
        validate();
    }
    
    R& operator= (const Py::Object& rhs) {
        return (*this = *rhs);
    }
    
    R& operator= (PyObject* rhsp) {
        if(ptr() == rhsp) return *this;
        set(rhsp);
        return *this;
    }
    
    virtual bool accepts(PyObject *pyob) const {
        return pyob && r::check(pyob);
    }
    
    Py::Object value(const Tuple& t) const {
        return static_cast<r *>(ptr())->c_value(t);
    }

    void assign(const Tuple& t, const Object& rhs) {
        static_cast<r *>(ptr())->c_assign(t, rhs);
    }
};
#endif

