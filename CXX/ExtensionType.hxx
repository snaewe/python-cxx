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

#ifndef __CXX_ExtensionClass__h
#define __CXX_ExtensionClass__h


namespace Py
{
    struct PythonClassInstance
    {
        PyObject_HEAD
        PythonExtensionBase *cxx_object;
    };

    template<TEMPLATE_TYPENAME T> class PythonClass
    : public PythonExtensionBase 
    {
    protected:
        explicit PythonClass( PythonClassInstance *self, Tuple &args, Dict &kwds )
        : PythonExtensionBase()
        , m_self( self )
        {
            // we are a class
            behaviors().supportClass();

            // every object must support getattr
            behaviors().supportGetattr();
        }

        virtual ~PythonClass()
        {} 

        static PythonType &behaviors()
        {
            static PythonType *p;
            if( p == NULL ) 
            {
#if defined( _CPPRTTI ) || defined( __GNUG__ )
                const char *default_name = (typeid( T )).name();
#else
                const char *default_name = "unknown";
#endif
                p = new PythonType( sizeof( T ), 0, default_name );
                p->set_tp_new( extension_object_new );
                p->set_tp_init( extension_object_init );
                p->set_tp_dealloc( extension_object_deallocator );
            }

            return *p;
        }

        static PyObject *extension_object_new( PyTypeObject *subtype, PyObject *args, PyObject *kwds )
        {
#ifdef PYCXX_DEBUG
            std::cout << "extension_object_new()" << std::endl;
#endif
            PythonClassInstance *o = reinterpret_cast<PythonClassInstance *>( subtype->tp_alloc( subtype, 0 ) );
            if( o == NULL )
                return NULL;

            o->cxx_object = NULL;

            PyObject *self = reinterpret_cast<PyObject *>( o );
#ifdef PYCXX_DEBUG
            std::cout << "extension_object_new() => self=0x" << std::hex << reinterpret_cast< unsigned int >( self ) << std::dec << std::endl;
#endif
            return self;
        }

        static int extension_object_init( PyObject *_self, PyObject *args_, PyObject *kwds_ )
        {
            try
            {
                Py::Tuple args( args_ );
                Py::Dict kwds;
                if( kwds_ != NULL )
                    kwds = kwds_;

                PythonClassInstance *self = reinterpret_cast<PythonClassInstance *>( _self );
#ifdef PYCXX_DEBUG
                std::cout << "extension_object_init( self=0x" << std::hex << reinterpret_cast< unsigned int >( self ) << std::dec << " )" << std::endl;
                std::cout << "    self->cxx_object=0x" << std::hex << reinterpret_cast< unsigned int >( self->cxx_object ) << std::dec << std::endl;
#endif

                if( self->cxx_object == NULL )
                {
                    self->cxx_object = new T( self, args, kwds );
#ifdef PYCXX_DEBUG
                    std::cout << "    self->cxx_object=0x" << std::hex << reinterpret_cast< unsigned int >( self->cxx_object ) << std::dec << std::endl;
#endif
                }
                else
                {
#ifdef PYCXX_DEBUG
                    std::cout << "    reinit - self->cxx_object=0x" << std::hex << reinterpret_cast< unsigned int >( self->cxx_object ) << std::dec << std::endl;
#endif
                    self->cxx_object->reinit( args, kwds );
                }
            }
            catch( Exception & )
            {
                return -1;
            }
            return 0;
        }

        static void extension_object_deallocator( PyObject *_self )
        {
            PythonClassInstance *self = reinterpret_cast< PythonClassInstance * >( _self );
#ifdef PYCXX_DEBUG
            std::cout << "extension_object_deallocator( self=0x" << std::hex << reinterpret_cast< unsigned int >( self ) << std::dec << " )" << std::endl;
            std::cout << "    self->cxx_object=0x" << std::hex << reinterpret_cast< unsigned int >( self->cxx_object ) << std::dec << std::endl;
#endif
            delete self->cxx_object;
        }


    public:
        static PyTypeObject *type_object()
        {
            return behaviors().type_object();
        }

        static Object type()
        {
            return Object( reinterpret_cast<PyObject *>( behaviors().type_object() ) );
        }

        static bool check( PyObject *p )
        {
            // is p like me?
            return p->ob_type == type_object();
        }

        static bool check( const Object &ob )
        {
            return check( ob.ptr() );
        }

        //
        // every object needs getattr implemented
        // to support methods
        //
        virtual Object getattr( const char *name )
        {
            return getattr_methods( name );
        }

    protected:
        typedef Object (T::*method_noargs_function_t)();
        typedef Object (T::*method_varargs_function_t)( const Tuple &args );
        typedef Object (T::*method_keyword_function_t)( const Tuple &args, const Dict &kws );

#if defined( PYCXX_USING_HASH_MAP )
        typedef __PYCXX_HASHMAP_NAMESPACE::hash_map<std::string, MethodDefExt<T> *, __pycxx_str_hash_func> method_map_t;
#else
        typedef std::map<std::string, MethodDefExt<T> *> method_map_t;
#endif

        // support the default attributes, __name__, __doc__ and methods
        virtual Object getattr_default( const char *_name )
        {
            std::string name( _name );

            if( name == "__name__" && type_object()->tp_name != NULL )
            {
                return Py::String( type_object()->tp_name );
            }

            if( name == "__doc__" && type_object()->tp_doc != NULL )
            {
                return Py::String( type_object()->tp_doc );
            }

            return getattr_methods( _name );
        }

        // turn a name into function object
        virtual Object getattr_methods( const char *_name )
        {
            std::string name( _name );

            method_map_t &mm = methods();

            EXPLICIT_TYPENAME method_map_t::const_iterator i;

            if( name == "__methods__" )
            {
                List methods;

                for( i = mm.begin(); i != mm.end(); ++i )
                    methods.append( String( ( *i ).first ) );

                return methods;
            }

            // see if name exists and get entry with method
            i = mm.find( name );
            if( i == mm.end() )
                throw AttributeError( name );

            Tuple self( 2 );

            self[0] = Object( reinterpret_cast<PyObject *>( m_self ) );
            self[1] = String( name );

            MethodDefExt<T> *method_definition = i->second;

            PyObject *func = PyCFunction_New( &method_definition->ext_meth_def, self.ptr() );

            return Object( func, true );
        }

        // check that all methods added are unique
        static void check_unique_method_name( const char *name )
        {
            method_map_t &mm = methods();
            EXPLICIT_TYPENAME method_map_t::const_iterator i;
            i = mm.find( name );
            if( i != mm.end() )
                throw AttributeError( name );
        }

        static void add_noargs_method( const char *name, method_noargs_function_t function, const char *doc="" )
        {
            check_unique_method_name( name );
            method_map_t &mm = methods();
            mm[ std::string( name ) ] = new MethodDefExt<T>( name, function, method_noargs_call_handler, doc );
        }

        static void add_varargs_method( const char *name, method_varargs_function_t function, const char *doc="" )
        {
            check_unique_method_name( name );
            method_map_t &mm = methods();
            mm[ std::string( name ) ] = new MethodDefExt<T>( name, function, method_varargs_call_handler, doc );
        }

        static void add_keyword_method( const char *name, method_keyword_function_t function, const char *doc="" )
        {
            check_unique_method_name( name );
            method_map_t &mm = methods();
            mm[ std::string( name ) ] = new MethodDefExt<T>( name, function, method_keyword_call_handler, doc );
        }

    private:
        static method_map_t &methods( void )
        {
            static method_map_t *map_of_methods = NULL;
            if( map_of_methods == NULL )
                map_of_methods = new method_map_t;

            return *map_of_methods;
        }

        // Note: Python calls noargs as varargs buts args==NULL
        static PyObject *method_noargs_call_handler( PyObject *_self_and_name_tuple, PyObject * )
        {
            try
            {
                Tuple self_and_name_tuple( _self_and_name_tuple );

                Object _self_in_cobject = self_and_name_tuple[0];
                PyObject *self_in_cobject = _self_in_cobject.ptr();
                PythonClassInstance *instance_wrapper = reinterpret_cast<PythonClassInstance *>( self_in_cobject );
                T *instance = static_cast<T *>( instance_wrapper->cxx_object );

#ifdef PYCXX_DEBUG
                std::cout << "method_noargs_call_handler( self=0x" << std::hex << reinterpret_cast< unsigned int >( self_in_cobject ) << std::dec << " )" << std::endl;
                std::cout << "    self->cxx_object=0x" << std::hex << reinterpret_cast< unsigned int >( instance ) << std::dec << std::endl;
                bpt();
#endif

                String name( self_and_name_tuple[1] );

                method_map_t &mm = methods();

                EXPLICIT_TYPENAME method_map_t::const_iterator i;                
                i = mm.find( name );
                if( i == mm.end() )
                    return 0;

                MethodDefExt<T> *meth_def = i->second;

                Object result;

                // Adding try & catch in case of STL debug-mode exceptions.
                #ifdef _STLP_DEBUG
                try
                {
                    result = (instance->*meth_def->ext_noargs_function)();
                }
                catch( std::__stl_debug_exception )
                {
                    // throw cxx::RuntimeError( sErrMsg );
                    throw RuntimeError( "Error message not set yet." );
                }
                #else
                result = (instance->*meth_def->ext_noargs_function)();
                #endif // _STLP_DEBUG

                return new_reference_to( result.ptr() );
            }
            catch( Exception & )
            {
                return 0;
            }
        }

        static PyObject *method_varargs_call_handler( PyObject *_self_and_name_tuple, PyObject *_args )
        {
            try
            {
                Tuple self_and_name_tuple( _self_and_name_tuple );

                PyObject *self_in_cobject = self_and_name_tuple[0].ptr();
                T *self = static_cast<T *>( self_in_cobject );

                String name( self_and_name_tuple[1] );

                method_map_t &mm = methods();

                EXPLICIT_TYPENAME method_map_t::const_iterator i;                
                i = mm.find( name );
                if( i == mm.end() )
                    return 0;

                MethodDefExt<T> *meth_def = i->second;

                Tuple args( _args );

                Object result;

                // Adding try & catch in case of STL debug-mode exceptions.
                #ifdef _STLP_DEBUG
                try
                {
                    result = (self->*meth_def->ext_varargs_function)( args );
                }
                catch( std::__stl_debug_exception )
                {
                    throw RuntimeError( "Error message not set yet." );
                }
                #else
                result = (self->*meth_def->ext_varargs_function)( args );
                #endif // _STLP_DEBUG

                return new_reference_to( result.ptr() );
            }
            catch( Exception & )
            {
                return 0;
            }
        }

        static PyObject *method_keyword_call_handler( PyObject *_self_and_name_tuple, PyObject *_args, PyObject *_keywords )
        {
            try
            {
                Tuple self_and_name_tuple( _self_and_name_tuple );

                PyObject *self_in_cobject = self_and_name_tuple[0].ptr();
                T *self = static_cast<T *>( self_in_cobject );

                String name( self_and_name_tuple[1] );

                method_map_t &mm = methods();

                EXPLICIT_TYPENAME method_map_t::const_iterator i;                
                i = mm.find( name );
                if( i == mm.end() )
                    return 0;

                MethodDefExt<T> *meth_def = i->second;

                Tuple args( _args );

                // _keywords may be NULL so be careful about the way the dict is created
                Dict keywords;
                if( _keywords != NULL )
                    keywords = Dict( _keywords );

                Object result( ( self->*meth_def->ext_keyword_function )( args, keywords ) );

                return new_reference_to( result.ptr() );
            }
            catch( Exception & )
            {
                return 0;
            }
        }

    private:
        PythonClassInstance *m_self;

    private:
        //
        // prevent the compiler generating these unwanted functions
        //
        explicit PythonClass( const PythonClass<T> &other );
        void operator=( const PythonClass<T> &rhs );
    };

    //
    // ExtensionObject<T> is an Object that will accept only T's.
    //
    template<TEMPLATE_TYPENAME T>
    class PythonClassObject: public Object
    {
    public:

        explicit PythonClassObject( PyObject *pyob )
        : Object( pyob )
        {
            validate();
        }

        PythonClassObject( const PythonClassObject<T> &other )
        : Object( *other )
        {
            validate();
        }

        PythonClassObject( const Object &other )
        : Object( *other )
        {
            validate();
        }

        PythonClassObject &operator=( const Object &rhs )
        {
            *this = *rhs;
            return *this;
        }

        PythonClassObject &operator=( PyObject *rhsp )
        {
            if( ptr() != rhsp )
                set( rhsp );
            return *this;
        }

        virtual bool accepts( PyObject *pyob ) const
        {
            return( pyob && T::check( pyob ) );
        }

        //
        //    Obtain a pointer to the PythonExtension object
        //
        T *getCxxObject( void )
        {
            return static_cast<T *>( ptr() );
        }
    };
} // Namespace Py

// End of __CXX_ExtensionClass__h
#endif
