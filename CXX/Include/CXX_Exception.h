//----------------------------------*-C++-*----------------------------------//
// Copyright 1998 The Regents of the University of California. 
// All rights reserved. See LEGAL.LLNL for full text and disclaimer.
//---------------------------------------------------------------------------//

#ifndef __CXX_Exception_h
#define __CXX_Exception_h

#include "Python.h"
#include "CXX_Config.h"

#include STANDARD_HEADER(string)
#include STANDARD_HEADER(iostream)
// This mimics the Python structure, in order to minimize confusion
NAMESPACE_BEGIN(Py)
    
    class Exception {
    public:
        explicit Exception () {}
        
        Exception (const STD::string& reason) {
            PyErr_SetString (PyExc_RuntimeError, reason.c_str());
        }
        
        Exception (PyObject* exception, const STD::string& reason) {
            PyErr_SetString (exception, reason.c_str());
        }
        
        void clear() // clear the error
            // technically but not philosophically const
        {
            PyErr_Clear();
        }

     };
// Abstract
	class StandardError: public Exception {
	protected: 
		explicit StandardError() {}
	};
    
	class LookupError: public StandardError {
	protected: 
		explicit LookupError() {}
	};

	class ArithmeticError: public StandardError {
	protected: 
		explicit ArithmeticError(){}
	};

	class EnvironmentError: public StandardError {
	protected: 
		explicit EnvironmentError(){}
	};

// Concrete

    class TypeError: public StandardError  {
    public:
        TypeError (const STD::string& reason)
            : StandardError()
        {
            PyErr_SetString (PyExc_TypeError,reason.c_str());
        }
    };
    
    class IndexError: public LookupError  {
    public:
        IndexError (const STD::string& reason)
            : LookupError()
        {
            PyErr_SetString (PyExc_IndexError, reason.c_str());
        }
    };
    
    class AttributeError: public StandardError {
    public:
        AttributeError (const STD::string& reason)
            : StandardError()
        {
            PyErr_SetString (PyExc_AttributeError, reason.c_str());
        }		
    };
    
    class NameError: public StandardError  {
    public:
        NameError (const STD::string& reason)
            : StandardError()
        {
            PyErr_SetString (PyExc_NameError, reason.c_str());
        }
    };
    
    class RuntimeError: public StandardError {
    public:
        RuntimeError (const STD::string& reason)
            : StandardError()
        {
            PyErr_SetString (PyExc_RuntimeError, reason.c_str());
        }
    };
    
    class SystemError: public StandardError  {
    public:
        SystemError (const STD::string& reason)
            : StandardError()
        {
            PyErr_SetString (PyExc_SystemError,reason.c_str());
        }
    };
    
    class KeyError: public LookupError	{
    public:
        KeyError (const STD::string& reason)
            : LookupError()
        {
            PyErr_SetString (PyExc_KeyError,reason.c_str());
        }
    };
    
    
    class ValueError: public StandardError {
    public:
        ValueError (const STD::string& reason)
            : StandardError()
        {
            PyErr_SetString (PyExc_ValueError, reason.c_str());
        }
    };
    
    class OverflowError: public ArithmeticError  {
    public:
        OverflowError (const STD::string& reason)
            : ArithmeticError()
        {
            PyErr_SetString (PyExc_OverflowError, reason.c_str());
        }		
    };
    
    class ZeroDivisionError: public ArithmeticError  {
    public:
        ZeroDivisionError (const STD::string& reason)
            : ArithmeticError() 
        {
            PyErr_SetString (PyExc_ZeroDivisionError, reason.c_str());
        }
    };
    
    class FloatingPointError: public ArithmeticError  {
    public:
        FloatingPointError (const STD::string& reason)
            : ArithmeticError() 
        {
            PyErr_SetString (PyExc_FloatingPointError, reason.c_str());
        }
    };
    
    class MemoryError: public StandardError  {
    public:
        MemoryError (const STD::string& reason)
            : StandardError()
        {
            PyErr_SetString (PyExc_MemoryError, reason.c_str());
        }	
    };
    
    class SystemExit: public StandardError  {
    public:
        SystemExit (const STD::string& reason)
            : StandardError() 
        {
            PyErr_SetString (PyExc_SystemExit,reason.c_str());
        }
    };
    
NAMESPACE_END // Py

#endif
