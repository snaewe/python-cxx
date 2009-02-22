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

#ifndef __CXX_ExtensionModule__h
#define __CXX_ExtensionModule__h

namespace Py
{
    class ExtensionModuleBase
    {
    public:
        ExtensionModuleBase( const char *name );
        virtual ~ExtensionModuleBase();

        Module module( void ) const;            // only valid after initialize() has been called
        Dict moduleDictionary( void ) const;    // only valid after initialize() has been called

        virtual Object invoke_method_noargs( const std::string &_name ) = 0;
        virtual Object invoke_method_varargs( const std::string &_name, const Tuple &_args ) = 0;
        virtual Object invoke_method_keyword( const std::string &_name, const Tuple &_args, const Dict &_keywords ) = 0;

        const std::string &name() const;
        const std::string &fullName() const;

        // what is returned from PyInit_<module> function
        Object moduleObject( void ) const;

    protected:
        // Initialize the module
        void initialize( const char *module_doc );

        const std::string m_module_name;
        const std::string m_full_module_name;
        MethodTable m_method_table;
#if PY3
        PyModuleDef m_module_def;
#endif
        PyObject *m_module;

    private:
        //
        // prevent the compiler generating these unwanted functions
        //
        ExtensionModuleBase( const ExtensionModuleBase & );     //unimplemented
        void operator=( const ExtensionModuleBase & );          //unimplemented
    };

    // Note: Python calls noargs as varargs buts args==NULL
    extern "C" PyObject *method_noargs_call_handler( PyObject *_self_and_name_tuple, PyObject * );
    extern "C" PyObject *method_varargs_call_handler( PyObject *_self_and_name_tuple, PyObject *_args );
    extern "C" PyObject *method_keyword_call_handler( PyObject *_self_and_name_tuple, PyObject *_args, PyObject *_keywords );

    extern "C" void do_not_dealloc( void * );

    template<TEMPLATE_TYPENAME T>
    class ExtensionModule : public ExtensionModuleBase
    {
    public:
        ExtensionModule( const char *name )
        : ExtensionModuleBase( name )
        {}
        virtual ~ExtensionModule()
        {}

    protected:
        typedef Object (T::*method_noargs_function_t)();
        typedef Object (T::*method_varargs_function_t)( const Tuple &args );
        typedef Object (T::*method_keyword_function_t)( const Tuple &args, const Dict &kws );

#if defined( PYCXX_USING_HASH_MAP )
        typedef __PYCXX_HASHMAP_NAMESPACE::hash_map<std::string, MethodDefExt<T> *, __pycxx_str_hash_func> method_map_t;
#else
        typedef std::map<std::string, MethodDefExt<T> *> method_map_t;
#endif

        static void add_noargs_method( const char *name, method_noargs_function_t function, const char *doc="" )
        {
            method_map_t &mm = methods();
            mm[ std::string( name ) ] = new MethodDefExt<T>( name, function, method_noargs_call_handler, doc );
        }

        static void add_varargs_method( const char *name, method_varargs_function_t function, const char *doc="" )
        {
            method_map_t &mm = methods();
            mm[ std::string( name ) ] = new MethodDefExt<T>( name, function, method_varargs_call_handler, doc );
        }

        static void add_keyword_method( const char *name, method_keyword_function_t function, const char *doc="" )
        {
            method_map_t &mm = methods();
            mm[ std::string( name ) ] = new MethodDefExt<T>( name, function, method_keyword_call_handler, doc );
        }

        void initialize( const char *module_doc="" )
        {
            ExtensionModuleBase::initialize( module_doc );
            Dict dict( moduleDictionary() );

            //
            // put each of the methods into the modules dictionary
            // so that we get called back at the function in T.
            //
            method_map_t &mm = methods();
            EXPLICIT_TYPENAME method_map_t::const_iterator i;

            for( i=mm.begin(); i != mm.end(); ++i )
            {
                MethodDefExt<T> *method_definition = (*i).second;

                static PyObject *self = PyCObject_FromVoidPtr( this, do_not_dealloc );

                Tuple args( 2 );
                args[0] = Object( self );
                args[1] = String( ( *i ).first );

                PyObject *func = PyCFunction_New
                                    ( 
                                    &method_definition->ext_meth_def,
                                    new_reference_to( args )
                                    );

                dict[( *i ).first ] = Object( func );
            }
        }

    protected:    // Tom Malcolmson reports that derived classes need access to these
        static method_map_t &methods( void )
        {
            static method_map_t *map_of_methods = NULL;
            if( map_of_methods == NULL )
                map_of_methods = new method_map_t;

            return *map_of_methods;
        }

        // this invoke function must be called from within a try catch block
        virtual Object invoke_method_noargs( const std::string &name )
        {
            method_map_t &mm = methods();
            MethodDefExt<T> *meth_def = mm[ name ];
            if( meth_def == NULL )
            {
                std::string error_msg( "CXX - cannot invoke noargs method named " );
                error_msg += name;
                throw RuntimeError( error_msg );
            }

            // cast up to the derived class
            T *self = static_cast<T *>( this );

            return (self->*meth_def->ext_noargs_function)();
        }

        // this invoke function must be called from within a try catch block
        virtual Object invoke_method_varargs( const std::string &name, const Tuple &args )
        {
            method_map_t &mm = methods();
            MethodDefExt<T> *meth_def = mm[ name ];
            if( meth_def == NULL )
            {
                std::string error_msg( "CXX - cannot invoke varargs method named " );
                error_msg += name;
                throw RuntimeError( error_msg );
            }

            // cast up to the derived class
            T *self = static_cast<T *>( this );

            return (self->*meth_def->ext_varargs_function)( args );
        }

        // this invoke function must be called from within a try catch block
        virtual Object invoke_method_keyword( const std::string &name, const Tuple &args, const Dict &keywords )
        {
            method_map_t &mm = methods();
            MethodDefExt<T> *meth_def = mm[ name ];
            if( meth_def == NULL )
            {
                std::string error_msg( "CXX - cannot invoke keyword method named " );
                error_msg += name;
                throw RuntimeError( error_msg );
            }

            // cast up to the derived class
            T *self = static_cast<T *>( this );

            return (self->*meth_def->ext_keyword_function)( args, keywords );
        }

    private:
        //
        // prevent the compiler generating these unwanted functions
        //
        ExtensionModule( const ExtensionModule<T> & );    //unimplemented
        void operator=( const ExtensionModule<T> & );    //unimplemented
    };
} // Namespace Py


// End of __CXX_ExtensionModule__h
#endif
