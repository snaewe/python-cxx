//----------------------------------*-C++-*----------------------------------//
// Copyright 1998 The Regents of the University of California. 
// All rights reserved. See Legal.htm for full text and disclaimer.
//---------------------------------------------------------------------------//

#include "CXX_Objects.h"
namespace Py {

    Type Object::type () const { 
        return Type (PyObject_Type (p), true);
    }
    
    String Object::str () const {
        return String (PyObject_Str (p), true);
    }
    
    String Object::repr () const { 
        return String (PyObject_Repr (p), true);
    }
    
    std::string Object::as_string() const {
        return static_cast<std::string>(str());
    }
        
    bool Object::isType (const Type& t) const { 
        return type ().ptr() == t.ptr();
    }
    
    Char::operator String() const {
        return String(ptr());
    }

    
    // output
    std::ostream& operator<< (std::ostream& os, const Object& ob) {
        return (os << static_cast<std::string>(ob.str()));
    }  

} // Py
