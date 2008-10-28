//
//  Copyright (c) 2008 Barry A. Scott
//
//
//  simple_moduile.cxx
//
//  This module defines a single function.
//
#ifdef _MSC_VER
// disable warning C4786: symbol greater than 255 character,
// nessesary to ignore as <map> causes lots of warning
#pragma warning(disable: 4786)
#endif

#include "CXX/Objects.hxx"
#include "CXX/Extensions.hxx"

#include <assert.h>

class new_style_class: public Py::PythonClass< new_style_class >
{
public:
    new_style_class( Py::Tuple &args, Py::Dict &kwds )
    : Py::PythonClass< new_style_class >::PythonClass( args, kwds )
    {
        std::cout << "new_style_class c'tor Called with " << args.length() << " normal arguments." << std::endl;
        Py::List names( kwds.keys() );
        std::cout << "and with " << names.length() << " keyword arguments:" << std::endl;
        for( Py::List::size_type i=0; i< names.length(); i++ )
        {
            Py::String name( names[i] );
            std::cout << "    " << name << std::endl;
        }
    }

    virtual ~new_style_class()
    {
        std::cout << "~new_style_class." << std::endl;
    }

    static void init_type(void)
    {
        behaviors().name( "new_style_class" );
        behaviors().doc( "documentation for new_style_class class" );
        behaviors().supportGetattr();

        add_noargs_method( "new_style_class_func_noargs", &new_style_class::new_style_class_func_noargs );
        add_varargs_method( "new_style_class_func_varargs", &new_style_class::new_style_class_func_varargs );
        add_keyword_method( "new_style_class_func_keyword", &new_style_class::new_style_class_func_keyword );

        // Call to make the type ready for use
        behaviors().readyType();
    }

    // override functions from PythonExtension
    virtual Py::Object getattr( const char *name )
    {
        return getattr_methods( name );
    }

    Py::Object new_style_class_func_noargs( void )
    {
        bpt();
        std::cout << "new_style_class_func_noargs Called." << std::endl;
        return Py::None();
    }

    Py::Object new_style_class_func_varargs( const Py::Tuple &args )
    {
        std::cout << "new_style_class_func_varargs Called with " << args.length() << " normal arguments." << std::endl;
        return Py::None();
    }

    Py::Object new_style_class_func_keyword( const Py::Tuple &args, const Py::Dict &kwds )
    {
        std::cout << "new_style_class_func_keyword Called with " << args.length() << " normal arguments." << std::endl;
        Py::List names( kwds.keys() );
        std::cout << "and with " << names.length() << " keyword arguments:" << std::endl;
        for( Py::List::size_type i=0; i< names.length(); i++ )
        {
            Py::String name( names[i] );
            std::cout << "    " << name << std::endl;
        }
        return Py::None();
    }
};


class old_style_class: public Py::PythonExtension< old_style_class >
{
public:
    old_style_class()
    {
    }

    virtual ~old_style_class()
    {
    }

    static void init_type(void)
    {
        behaviors().name( "old_style_class" );
        behaviors().doc( "documentation for old_style_class class" );
        behaviors().supportGetattr();

        add_noargs_method( "old_style_class_func_noargs", &old_style_class::old_style_class_func_noargs );
        add_varargs_method( "old_style_class_func_varargs", &old_style_class::old_style_class_func_varargs );
        add_keyword_method( "old_style_class_func_keyword", &old_style_class::old_style_class_func_keyword );
    }

    // override functions from PythonExtension
    virtual Py::Object getattr( const char *name )
    {
        return getattr_methods( name );
    }

    Py::Object old_style_class_func_noargs( void )
    {
        std::cout << "old_style_class_func_noargs Called." << std::endl;
        return Py::None();
    }

    Py::Object old_style_class_func_varargs( const Py::Tuple &args )
    {
        std::cout << "old_style_class_func_varargs Called with " << args.length() << " normal arguments." << std::endl;
        return Py::None();
    }

    Py::Object old_style_class_func_keyword( const Py::Tuple &args, const Py::Dict &kwds )
    {
        std::cout << "old_style_class_func_keyword Called with " << args.length() << " normal arguments." << std::endl;
        Py::List names( kwds.keys() );
        std::cout << "and with " << names.length() << " keyword arguments:" << std::endl;
        for( Py::List::size_type i=0; i< names.length(); i++ )
        {
            Py::String name( names[i] );
            std::cout << "    " << name << std::endl;
        }
        return Py::None();
    }
};

class simple_module : public Py::ExtensionModule<simple_module>
{
public:
    simple_module()
    : Py::ExtensionModule<simple_module>( "simple" ) // this must be name of the file on disk e.g. simple.so or simple.pyd
    {
        old_style_class::init_type();
        new_style_class::init_type();

        add_varargs_method("old_style_class", &simple_module::factory_old_style_class, "documentation for old_style_class()");
        add_keyword_method("func", &simple_module::func, "documentation for func()");

        // after initialize the moduleDictionary will exist
        initialize( "documentation for the simple module" );

        Py::Dict d( moduleDictionary() );
        d["var"] = Py::String( "var value" );
        Py::Object x( new_style_class::type() );
        d["new_style_class"] = x;
    }

    virtual ~simple_module()
    {}

private:
    Py::Object func( const Py::Tuple &args, const Py::Dict &kwds )
    {
        std::cout << "Called with " << args.length() << " normal arguments." << std::endl;
        Py::List names( kwds.keys() );
        std::cout << "and with " << names.length() << " keyword arguments:" << std::endl;
        for( Py::List::size_type i=0; i< names.length(); i++ )
        {
            Py::String name( names[i] );
            std::cout << "    " << name << std::endl;
        }

        return Py::None();
    }

    Py::Object factory_old_style_class( const Py::Tuple &rargs )
    {
        return Py::asObject( new old_style_class );
    }
};

extern "C" PyObject *PyInit_simple()
{
#if defined(PY_WIN32_DELAYLOAD_PYTHON_DLL)
    Py::InitialisePythonIndirectPy::Interface();
#endif

    static simple_module* simple = new simple_module;
    return simple->module().ptr();
}

// symbol required for the debug version
extern "C" PyObject *PyInit_simple_d()
{ 
    return PyInit_simple();
}
