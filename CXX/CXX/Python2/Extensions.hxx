//-----------------------------------------------------------------------------
//
// Copyright (c) 1998 - 2007, The Regents of the University of California
// Produced at the Lawrence Livermore National Laboratory
// All rights reserved.
//
// This file is part of PyCXX. For details,see http://cxx.sourceforge.net/. The
// full copyright notice is contained in the file COPYRIGHT located at the root
// of the PyCXX distribution.
//
// Redistribution  and  use  in  source  and  binary  forms,  with  or  without
// modification, are permitted provided that the following conditions are met:
//
//  - Redistributions of  source code must  retain the above  copyright notice,
//    this list of conditions and the disclaimer below.
//  - Redistributions in binary form must reproduce the above copyright notice,
//    this  list of  conditions  and  the  disclaimer (as noted below)  in  the
//    documentation and/or materials provided with the distribution.
//  - Neither the name of the UC/LLNL nor  the names of its contributors may be
//    used to  endorse or  promote products derived from  this software without
//    specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR  IMPLIED WARRANTIES, INCLUDING,  BUT NOT  LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND  FITNESS FOR A PARTICULAR  PURPOSE
// ARE  DISCLAIMED.  IN  NO  EVENT  SHALL  THE  REGENTS  OF  THE  UNIVERSITY OF
// CALIFORNIA, THE U.S.  DEPARTMENT  OF  ENERGY OR CONTRIBUTORS BE  LIABLE  FOR
// ANY  DIRECT,  INDIRECT,  INCIDENTAL,  SPECIAL,  EXEMPLARY,  OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT  LIMITED TO, PROCUREMENT OF  SUBSTITUTE GOODS OR
// SERVICES; LOSS OF  USE, DATA, OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER
// CAUSED  AND  ON  ANY  THEORY  OF  LIABILITY,  WHETHER  IN  CONTRACT,  STRICT
// LIABILITY, OR TORT  (INCLUDING NEGLIGENCE OR OTHERWISE)  ARISING IN ANY  WAY
// OUT OF THE  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
// DAMAGE.
//
//-----------------------------------------------------------------------------

#ifndef __CXX_Extensions__h
#define __CXX_Extensions__h


#ifdef _MSC_VER
// disable warning C4786: symbol greater than 255 character,
// okay to ignore
#pragma warning( disable: 4786 )
#endif

#include "CXX/WrapPython.h"
#include "CXX/Version.hxx"
#include "CXX/Python2/Config.hxx"
#include "CXX/Python2/CxxDebug.hxx"
#include "CXX/Python2/Objects.hxx"

extern "C" { extern PyObject py_object_initializer; }

#include <vector>

// std::map / hash_map selection and declarations ----------------------------
#if !defined( PYCXX_USING_HASH_MAP )

#include <map>

#else

#if defined( __GNUC__ ) && !defined( _STLPORT_VERSION )
    #include <ext/hash_map>
#else
    #include <hash_map>
#endif
#if defined( _STLPORT_VERSION )
    #define __PYCXX_HASHMAP_NAMESPACE std
    using namespace std;
#elif defined( _MSC_VER ) && !defined( __INTEL_COMPILER ) && !defined( __ICC ) && !defined( __ICL ) && !defined( __ECC )
    #define __PYCXX_HASHMAP_NAMESPACE stdext
    using namespace stdext;
#elif defined( __INTEL_COMPILER ) || defined( __ICC ) || defined( __ICL ) || defined( __ECC )
    #define __PYCXX_HASHMAP_NAMESPACE stdext
    using namespace stdext;
#elif defined( __GNUC__ )
    #define __PYCXX_HASHMAP_NAMESPACE __gnu_cxx
    using namespace __gnu_cxx;
#else
    #define __PYCXX_HASHMAP_NAMESPACE std
    using namespace std;
#endif

class __pycxx_str_hash_func
{
public:
    enum
    {
        // parameters for hash table
        bucket_size = 4, // 0 < bucket_size
        min_buckets = 8  // min_buckets = 2 ^^ N, 0 < N
    };

    // http://www.azillionmonkeys.com/qed/hash.html
    size_t operator()( const std::string &str ) const
    { 
        const unsigned char * data = reinterpret_cast<const unsigned char *>( str.c_str() );
        int len =( int )str.length();
        unsigned int hash = len;
        unsigned int tmp;
        int rem;

        if( len <= 0 || data == NULL )
            return 0;

        rem = len & 3;
        len >>= 2;

        /* Main loop */
        for( ;len > 0; len-- )
        {
            hash  +=( data[1] << 8 ) | data[0];
            tmp    =((( data[3] << 8 ) | data[2] ) << 11 ) ^ hash;
            hash   =( hash << 16 ) ^ tmp;
            data  += 2*sizeof( unsigned short );
            hash  += hash >> 11;
        }

        /* Handle end cases */
        switch( rem )
        {
            case 3: hash +=( data[1] << 8 ) | data[0];
                    hash ^= hash << 16;
                    hash ^= data[sizeof( unsigned short )] << 18;
                    hash += hash >> 11;
                    break;
            case 2: hash +=( data[1] << 8 ) | data[0];
                    hash ^= hash << 11;
                    hash += hash >> 17;
                    break;
            case 1: hash += *data;
                    hash ^= hash << 10;
                    hash += hash >> 1;
        }

        /* Force "avalanching" of final 127 bits */
        hash ^= hash << 3;
        hash += hash >> 5;
        hash ^= hash << 4;
        hash += hash >> 17;
        hash ^= hash << 25;
        hash += hash >> 6;

        return hash;
    }

    bool operator()( const std::string &str_1, const std::string &str_2 ) const
    {    
        // test if str_1 ordered before str_2
        return str_1 < str_2;
    }
};
#endif // PYCXX_USING_HASH_MAP
// ----------------------------------------------------------------------

namespace Py
{
    class ExtensionModuleBase;

    // Make an Exception Type for use in raising custom exceptions
    class ExtensionExceptionType : public Object
    {
    public:
        ExtensionExceptionType();
        virtual ~ExtensionExceptionType();

        // call init to create the type
        void init( ExtensionModuleBase &module, const std::string &name, ExtensionExceptionType &parent );
        void init( ExtensionModuleBase &module, const std::string &name );
    };

    class MethodTable 
    {
    public:
        MethodTable();
        virtual ~MethodTable();

        void add( const char *method_name, PyCFunction f, const char *doc="", int flag=1 );
        PyMethodDef *table();

    protected:
        std::vector<PyMethodDef> t;    // accumulator of PyMethodDef's
        PyMethodDef *mt;        // Actual method table produced when full

        static PyMethodDef method( const char* method_name, PyCFunction f, int flags=1, const char* doc="" );

    private:
        //
        // prevent the compiler generating these unwanted functions
        //
        MethodTable( const MethodTable &m );    //unimplemented
        void operator=( const MethodTable &m );    //unimplemented

    }; // end class MethodTable

    // Note: Python calls noargs as varargs buts args==NULL
    extern "C" typedef PyObject *(*method_noargs_call_handler_t)( PyObject *_self, PyObject * );
    extern "C" typedef PyObject *(*method_varargs_call_handler_t)( PyObject *_self, PyObject *_args );
    extern "C" typedef PyObject *(*method_keyword_call_handler_t)( PyObject *_self, PyObject *_args, PyObject *_dict );

    template<class T>
    class MethodDefExt : public PyMethodDef
    {
    public:
        typedef Object (T::*method_noargs_function_t)();
        typedef Object (T::*method_varargs_function_t)( const Tuple &args );
        typedef Object (T::*method_keyword_function_t)( const Tuple &args, const Dict &kws );

        // NOARGS
        MethodDefExt
        ( 
            const char *_name,
            method_noargs_function_t _function,
            method_noargs_call_handler_t _handler,
            const char *_doc
        )
        {
            ext_meth_def.ml_name = const_cast<char *>( _name );
            ext_meth_def.ml_meth = reinterpret_cast<method_varargs_call_handler_t>( _handler );
            ext_meth_def.ml_flags = METH_NOARGS;
            ext_meth_def.ml_doc = const_cast<char *>( _doc );

            ext_noargs_function = _function;
            ext_varargs_function = NULL;
            ext_keyword_function = NULL;
        }

        // VARARGS
        MethodDefExt
        ( 
            const char *_name,
            method_varargs_function_t _function,
            method_varargs_call_handler_t _handler,
            const char *_doc
        )
        {
            ext_meth_def.ml_name = const_cast<char *>( _name );
            ext_meth_def.ml_meth = reinterpret_cast<method_varargs_call_handler_t>( _handler );
            ext_meth_def.ml_flags = METH_VARARGS;
            ext_meth_def.ml_doc = const_cast<char *>( _doc );

            ext_noargs_function = NULL;
            ext_varargs_function = _function;
            ext_keyword_function = NULL;
        }

        // VARARGS + KEYWORD
        MethodDefExt
        ( 
            const char *_name,
            method_keyword_function_t _function,
            method_keyword_call_handler_t _handler,
            const char *_doc
        )
        {
            ext_meth_def.ml_name = const_cast<char *>( _name );
            ext_meth_def.ml_meth = reinterpret_cast<method_varargs_call_handler_t>( _handler );
            ext_meth_def.ml_flags = METH_VARARGS|METH_KEYWORDS;
            ext_meth_def.ml_doc = const_cast<char *>( _doc );

            ext_noargs_function = NULL;
            ext_varargs_function = NULL;
            ext_keyword_function = _function;
        }

        ~MethodDefExt()
        {}

        PyMethodDef ext_meth_def;
        method_noargs_function_t ext_noargs_function;    
        method_varargs_function_t ext_varargs_function;    
        method_keyword_function_t ext_keyword_function;    
    };
} // Namespace Py

#include "CXX/Python2/ExtensionModule.hxx"
#include "CXX/Python2/PythonType.hxx"
#include "CXX/Python2/ExtensionTypeBase.hxx"
#include "CXX/Python2/ExtensionOldType.hxx"
#include "CXX/Python2/ExtensionType.hxx"

// End of CXX_Extensions.h
#endif
