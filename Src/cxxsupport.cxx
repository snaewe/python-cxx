//----------------------------------*-C++-*----------------------------------//
// Copyright 1998 The Regents of the University of California. 
// All rights reserved. See Legal.htm for full text and disclaimer.
//---------------------------------------------------------------------------//

#include "CXX_Objects.h"
NAMESPACE_BEGIN(Py)

    Type Object::type () const { 
        return Type (FromAPI(PyObject_Type (p)));
    }
    
    String Object::str () const {
        return String (FromAPI(PyObject_Str (p)));
    }
    
    String Object::repr () const { 
        return String (FromAPI(PyObject_Repr (p)));
    }
    
    STD::string Object::as_string() const {
        return static_cast<STD::string>(str());
    }
        
    bool Object::isType (const Type& t) const { 
        return type ().ptr() == t.ptr();
    }
    
    Char::operator String() const {
        return String(ptr());
    }

    
    // output
    STD::ostream& operator<< (STD::ostream& os, const Object& ob) {
        return (os << static_cast<STD::string>(ob.str()));
    }  

NAMESPACE_END
