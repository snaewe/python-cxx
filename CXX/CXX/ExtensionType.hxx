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
    class PythonType
    {
    public:
        // if you define one sequence method you must define 
        // all of them except the assigns

        PythonType( size_t base_size, int itemsize, const char *default_name );
        virtual ~PythonType();

        const char *getName() const;
        const char *getDoc() const;

        PyTypeObject *type_object() const;
        PythonType &name( const char *nam );
        PythonType &doc( const char *d );
        PythonType &dealloc( void( *f )( PyObject * ) );

        PythonType &supportClass( void );
        PythonType &supportPrint( void );
        PythonType &supportGetattr( void );
        PythonType &supportSetattr( void );
        PythonType &supportGetattro( void );
        PythonType &supportSetattro( void );
        PythonType &supportCompare( void );
        PythonType &supportRichCompare( void );
        PythonType &supportRepr( void );
        PythonType &supportStr( void );
        PythonType &supportHash( void );
        PythonType &supportCall( void );
        PythonType &supportIter( void );

        PythonType &supportSequenceType( void );
        PythonType &supportMappingType( void );
        PythonType &supportNumberType( void );
        PythonType &supportBufferType( void );

    protected:
        PyTypeObject            *table;
        PySequenceMethods       *sequence_table;
        PyMappingMethods        *mapping_table;
        PyNumberMethods         *number_table;
        PyBufferProcs           *buffer_table;

        void init_sequence();
        void init_mapping();
        void init_number();
        void init_buffer();

    private:
        //
        // prevent the compiler generating these unwanted functions
        //
        PythonType( const PythonType &tb );    // unimplemented
        void operator=( const PythonType &t );    // unimplemented

    }; // end of PythonType



    // Class PythonExtension is what you inherit from to create
    // a new Python extension type. You give your class itself
    // as the template paramter.

    // There are two ways that extension objects can get destroyed.
    // 1. Their reference count goes to zero
    // 2. Someone does an explicit delete on a pointer.
    // In(1) the problem is to get the destructor called 
    //      We register a special deallocator in the Python type object
    //      (see behaviors()) to do this.
    // In(2) there is no problem, the dtor gets called.

    // PythonExtension does not use the usual Python heap allocator, 
    // instead using new/delete. We do the setting of the type object
    // and reference count, usually done by PyObject_New, in the 
    // base class ctor.

    // This special deallocator does a delete on the pointer.


    class PythonExtensionBase : public PyObject
    {
    public:
        PythonExtensionBase();
        virtual ~PythonExtensionBase();

    public:
        virtual int print( FILE *, int );
        virtual Object getattr( const char * ) = 0;
        virtual int setattr( const char *, const Object & );
        virtual Object getattro( const Object & );
        virtual int setattro( const Object &, const Object & );
        virtual int compare( const Object & );
        virtual Object rich_compare( const Object &, int );
        virtual Object repr();
        virtual Object str();
        virtual long hash();
        virtual Object call( const Object &, const Object & );
        virtual Object iter();
        virtual PyObject *iternext();

        // Sequence methods
        virtual int sequence_length();
        virtual Object sequence_concat( const Object & );
        virtual Object sequence_repeat( Py_ssize_t );
        virtual Object sequence_item( Py_ssize_t );
        virtual Object sequence_slice( Py_ssize_t, Py_ssize_t );
        virtual int sequence_ass_item( Py_ssize_t, const Object & );
        virtual int sequence_ass_slice( Py_ssize_t, Py_ssize_t, const Object & );

        // Mapping
        virtual int mapping_length();
        virtual Object mapping_subscript( const Object & );
        virtual int mapping_ass_subscript( const Object &, const Object & );

        // Number
        virtual int number_nonzero();
        virtual Object number_negative();
        virtual Object number_positive();
        virtual Object number_absolute();
        virtual Object number_invert();
        virtual Object number_int();
        virtual Object number_float();
        virtual Object number_long();
        virtual Object number_oct();
        virtual Object number_hex();
        virtual Object number_add( const Object & );
        virtual Object number_subtract( const Object & );
        virtual Object number_multiply( const Object & );
        virtual Object number_divide( const Object & );
        virtual Object number_remainder( const Object & );
        virtual Object number_divmod( const Object & );
        virtual Object number_lshift( const Object & );
        virtual Object number_rshift( const Object & );
        virtual Object number_and( const Object & );
        virtual Object number_xor( const Object & );
        virtual Object number_or( const Object & );
        virtual Object number_power( const Object &, const Object & );

        // Buffer
        virtual Py_ssize_t buffer_getreadbuffer( Py_ssize_t, void** );
        virtual Py_ssize_t buffer_getwritebuffer( Py_ssize_t, void** );
        virtual Py_ssize_t buffer_getsegcount( Py_ssize_t* );

    private:
        void missing_method( void );
        static PyObject *method_call_handler( PyObject *self, PyObject *args );
    };

    template<TEMPLATE_TYPENAME T> class PythonExtension
    : public PythonExtensionBase 
    {
    public:
        static PyTypeObject *type_object() 
        {
            return behaviors().type_object();
        }

        static bool check( PyObject *p )
        {
            // is p like me?
            return p->ob_type == type_object();
        }

        static bool check( const Object& ob )
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
        explicit PythonExtension()
        : PythonExtensionBase()
        {
            PyObject_INIT( this, type_object() );

            // every object must support getattr
            behaviors().supportGetattr();
        }

        virtual ~PythonExtension()
        {} 

        static PythonType &behaviors()
        {
            static PythonType* p;
            if( p == NULL ) 
            {
#if defined( _CPPRTTI ) || defined( __GNUG__ )
                const char *default_name =( typeid( T ) ).name();
#else
                const char *default_name = "unknown";
#endif
                p = new PythonType( sizeof( T ), 0, default_name );
                p->dealloc( extension_object_deallocator );
            }

            return *p;
        }

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

// trying to fake out being a class for help()
//            else if( name == "__bases__"  )
//            {
//                return Py::Tuple( 0 );
//            }
//            else if( name == "__module__"  )
//            {
//                return Py::Nothing();
//            }
//            else if( name == "__dict__"  )
//            {
//                return Py::Dict();
//            }

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

            self[0] = Object( this );
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

                PyObject *self_in_cobject = self_and_name_tuple[0].ptr();
                T *self = static_cast<T *>( self_in_cobject );

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
                    result = (self->*meth_def->ext_noargs_function)();
                }
                catch( std::__stl_debug_exception )
                {
                    // throw cxx::RuntimeError( sErrMsg );
                    throw RuntimeError( "Error message not set yet." );
                }
                #else
                result = (self->*meth_def->ext_noargs_function)();
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

        static void extension_object_deallocator( PyObject* t )
        {
            delete (T *)( t );
        }

        //
        // prevent the compiler generating these unwanted functions
        //
        explicit PythonExtension( const PythonExtension<T>& other );
        void operator=( const PythonExtension<T>& rhs );
    };

    //
    // ExtensionObject<T> is an Object that will accept only T's.
    //
    template<TEMPLATE_TYPENAME T>
    class ExtensionObject: public Object
    {
    public:

        explicit ExtensionObject( PyObject *pyob )
        : Object( pyob )
        {
            validate();
        }

        ExtensionObject( const ExtensionObject<T>& other )
        : Object( *other )
        {
            validate();
        }

        ExtensionObject( const Object& other )
        : Object( *other )
        {
            validate();
        }

        ExtensionObject& operator=( const Object& rhs )
        {
            return( *this = *rhs );
        }

        ExtensionObject& operator=( PyObject* rhsp )
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
        T *extensionObject( void )
        {
            return static_cast<T *>( ptr() );
        }
    };
} // Namespace Py

// End of __CXX_ExtensionClass__h
#endif
