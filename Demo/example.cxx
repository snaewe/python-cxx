//-----------------------------------------------------------------------------
//
// Copyright (c) 1998 - 2007, The Regents of the University of California
// Produced at the Lawrence Livermore National Laboratory
// All rights reserved.
//
// This file is part of PyCXX. For details, see http://cxx.sourceforge.net. The
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

#ifdef _MSC_VER
// disable warning C4786: symbol greater than 255 character,
// nessesary to ignore as <map> causes lots of warning
#pragma warning(disable: 4786)
#endif

#include "CXX/Objects.hxx"
#include "CXX/Extensions.hxx"

#include <assert.h>

#include "range.hxx"  // Extension object
extern std::string test_extension_object();

#include <algorithm>

static std::string test_String()
{
    Py::String s( "hello" );
    Py::Char blank = ' ';
    Py::String r1( "world in brief", 5 );
    s = s + blank + r1;
    s = s * 2;
    if( std::string(s) != "hello worldhello world" )
    {
        return "failed string (1) '" + std::string( s ) + "'";
    }
    // test conversion
    std::string w = static_cast<std::string>( s );
    std::string w2 = s;
    if( w != w2 )
    {
        return "failed string (2)";
    }
    Py::String r2( "12345 789" );
    Py::Char c6 = r2[5];
    if( c6 != blank )
    {
        std::cout << "|" << c6 << "|" << std::endl;
        return "failed string (3)";
    }
    return "ok";
}

static std::string
test_boolean()
{
    bool passed = true;

    Py::Object o;
    Py::Boolean pb1;
    Py::Boolean pb2;
    bool b1;

    o = Py::True();
    if( o.isTrue() )
        { std::cout << "OK: T1: True" << std::endl; }
    else
        { std::cout << "Bad: T1: False" << std::endl; passed = false; }

    pb1 = o;
    if( pb1 )
        { std::cout << "OK: T2: True" << std::endl; }
    else
        { std::cout << "Bad: T2: False" << std::endl; passed = false; }

    b1 = pb1;
    if( b1 )
        { std::cout << "OK: T3: True" << std::endl; }
    else
        { std::cout << "Bad: T3: False" << std::endl; passed = false; }

    pb2 = pb1;
    if( pb2 )
        { std::cout << "OK: T4: True" << std::endl; }
    else
        { std::cout << "Bad: T4: False" << std::endl; passed = false; }

    pb2 = true;
    if( pb2 )
        { std::cout << "OK: T5: True" << std::endl; }
    else
        { std::cout << "Bad: T5: False" << std::endl; passed = false; }


    o = Py::False();
    if( o.isTrue() )
        { std::cout << "Bad: F1: True" << std::endl; passed = false; }
    else
        { std::cout << "OK: F1: False" << std::endl; }

    pb1 = o;
    if( pb1 )
        { std::cout << "Bad: F2: True" << std::endl; passed = false; }
    else
        { std::cout << "OK: F2: False" << std::endl; }

    b1 = pb1;
    if( b1 )
        { std::cout << "Bad: F3: True" << std::endl; passed = false; }
    else
        { std::cout << "OK: F3: False" << std::endl; }

    pb2 = pb1;
    if( pb2 )
        { std::cout << "Bad: F4: True" << std::endl; passed = false; }
    else
        { std::cout << "OK: F4: False" << std::endl; }

    pb2 = false;
    if( pb2 )
        { std::cout << "Bad: F5: True" << std::endl; passed = false; }
    else
        { std::cout << "OK: F5: False" << std::endl; }

    if( passed )
        return "ok";
    else
        return "failed (4)";
}

class TestError
{
public:
    TestError( const std::string &description )
    : m_description( description )
    {}

    ~TestError()
    {}

    std::string m_description;
};

template <TEMPLATE_TYPENAME T> static void test_assert_scaler( char *description, char *type, T benchmark, T value )
{
    std::ostringstream full_description;
    full_description << description << ": " << type << " benchmark=" << benchmark << " " << type << " value=" << value << std::ends;

    if( benchmark != value )
    {
        throw TestError( full_description.str() );
    }
    else
    {
        std::cout << "PASSED: " << full_description.str() << std::endl;
    }
}

static void test_assert( char *description, bool benchmark, bool value )
{
    test_assert_scaler( description, "bool", benchmark, value );
}

static void test_assert( char *description, long benchmark, long value )
{
    test_assert_scaler( description, "long", benchmark, value );
}

static void test_assert( char *description, double benchmark, double value )
{
    test_assert_scaler( description, "float", benchmark, value );
}

static void test_long()
{
    long cxx_long1( 100 );
    long cxx_long2( 0 );
    long cxx_long3( 0 );
    Py::Long py_long1( 100 );
    Py::Long py_long2( 0 );
    Py::Long py_long3( 0 );

    test_assert( "long constructor", cxx_long1, py_long1.as_long() );

    cxx_long2 = cxx_long1++;
    py_long2 = py_long1++;
    test_assert( "long num++", cxx_long2, py_long2.as_long() );

    cxx_long2 = ++cxx_long1;
    py_long2 = ++py_long1;
    test_assert( "long ++num", cxx_long2, py_long2.as_long() );

    cxx_long2 = cxx_long1--;
    py_long2 = py_long1--;
    test_assert( "long num--", cxx_long2, py_long2.as_long() );

    cxx_long2 = --cxx_long1;
    py_long2 = --py_long1;
    test_assert( "long --num", cxx_long2, py_long2.as_long() );

    cxx_long1 = 1000;
    py_long1 = 1000;
    test_assert( "long num =", cxx_long1, py_long1.as_long() );

    // comparison tests
    cxx_long1 = 2;
    cxx_long2 = 3;
    cxx_long3 = 3;
    py_long1 = cxx_long1;
    py_long2 = cxx_long2;
    py_long3 = cxx_long3;
   

    test_assert( "long operator < ", cxx_long1 <  cxx_long2, py_long1.as_long() <  py_long2.as_long() );
    test_assert( "long operator < ", cxx_long2 <  cxx_long1, py_long2.as_long() <  py_long1.as_long() );

    test_assert( "long operator ==", cxx_long2 == cxx_long3, py_long2.as_long() == py_long3.as_long() );
    test_assert( "long operator ==", cxx_long1 == cxx_long3, py_long1.as_long() == py_long3.as_long() );

    test_assert( "long operator !=", cxx_long1 != cxx_long2, py_long1.as_long() != py_long2.as_long() );
    test_assert( "long operator !=", cxx_long2 != cxx_long3, py_long2.as_long() != py_long3.as_long() );

    test_assert( "long operator <=", cxx_long1 <= cxx_long2, py_long1.as_long() <= py_long2.as_long() );
    test_assert( "long operator <=", cxx_long2 <= cxx_long3, py_long2.as_long() <= py_long3.as_long() );
    test_assert( "long operator <=", cxx_long2 <= cxx_long1, py_long2.as_long() <= py_long1.as_long() );

    test_assert( "long operator >=", cxx_long2 >= cxx_long1, py_long2.as_long() >= py_long1.as_long() );
    test_assert( "long operator >=", cxx_long2 >= cxx_long3, py_long2.as_long() >= py_long3.as_long() );
    test_assert( "long operator >=", cxx_long1 >= cxx_long2, py_long1.as_long() >= py_long2.as_long() );

    test_assert( "long operator > ", cxx_long2 >  cxx_long1, py_long2.as_long() >  py_long1.as_long() );
    test_assert( "long operator > ", cxx_long1 >  cxx_long2, py_long1.as_long() >  py_long2.as_long() );
}

static void test_float()
{
    double cxx_float1( 100 );
    double cxx_float2( 0 );
    double cxx_float3( 0 );
    Py::Float py_float1( 100.0 );
    Py::Float py_float2( 0.0 );
    Py::Float py_float3( 0.0 );

    test_assert( "float constructor", cxx_float1, py_float1.as_double() );

    cxx_float1 = 1000;
    py_float1 = 1000;
    test_assert( "float num =", cxx_float1, py_float1.as_double() );

    // comparison tests
    cxx_float1 = 2;
    cxx_float2 = 3;
    cxx_float3 = 3;
    py_float1 = cxx_float1;
    py_float2 = cxx_float2;
    py_float3 = cxx_float3;
   

    test_assert( "float operator < ", cxx_float1 <  cxx_float2, py_float1.as_double() <  py_float2.as_double() );
    test_assert( "float operator < ", cxx_float2 <  cxx_float1, py_float2.as_double() <  py_float1.as_double() );

    test_assert( "float operator ==", cxx_float2 == cxx_float3, py_float2.as_double() == py_float3.as_double() );
    test_assert( "float operator ==", cxx_float1 == cxx_float3, py_float1.as_double() == py_float3.as_double() );

    test_assert( "float operator !=", cxx_float1 != cxx_float2, py_float1.as_double() != py_float2.as_double() );
    test_assert( "float operator !=", cxx_float2 != cxx_float3, py_float2.as_double() != py_float3.as_double() );

    test_assert( "float operator <=", cxx_float1 <= cxx_float2, py_float1.as_double() <= py_float2.as_double() );
    test_assert( "float operator <=", cxx_float2 <= cxx_float3, py_float2.as_double() <= py_float3.as_double() );
    test_assert( "float operator <=", cxx_float2 <= cxx_float1, py_float2.as_double() <= py_float1.as_double() );

    test_assert( "float operator >=", cxx_float2 >= cxx_float1, py_float2.as_double() >= py_float1.as_double() );
    test_assert( "float operator >=", cxx_float2 >= cxx_float3, py_float2.as_double() >= py_float3.as_double() );
    test_assert( "float operator >=", cxx_float1 >= cxx_float2, py_float1.as_double() >= py_float2.as_double() );

    test_assert( "float operator > ", cxx_float2 >  cxx_float1, py_float2.as_double() >  py_float1.as_double() );
    test_assert( "float operator > ", cxx_float1 >  cxx_float2, py_float1.as_double() >  py_float2.as_double() );
}

static std::string test_numbers()
{
    test_long();
    test_float();

    // test the basic numerical classes
    Py::Long i;
    Py::Long j(2);
    Py::Long k = Py::Long(3);

    i = 2;

    Py::Float a;
    a = 3 + i; //5.0
    Py::Float b( 4.0 );
    a = (1.0 + 2*a + (b*3.0)/2.0 + k)/Py::Float(5); // 4.0
    i = a - 1.0; // 3

    test_assert( "number calculation", i.as_long(), k.as_long() );

    return "ok";
}

static std::string test_List_iterators( const Py::List &x, Py::List &y )
{

    std::cout << "test_List_iterators" << std::endl;
    std::vector<Py::Object> v;
    Py::Sequence::iterator j;
    Py::Long k( 0 );    // qqq k = 0 does not compile
    for( Py::Sequence::const_iterator i = x.begin(); i != x.end(); ++i )
    {
        if( (*i).isList() )
        {
            ++k;
        }
    }

    if( k.as_long() != 1 )
        return "failed List iterators (1)";

    k = 0;
    for( j = y.begin(); j != y.end(); ++j )
    {
        *j = Py::Long( k++ );
        std::cout << "test_List_iterators v.push_back( " << (*j).repr().as_string() << " )" << std::endl;
        v.push_back( *j );
    }

    k = 0;
    for( j = y.begin(); j != y.end(); ++j )
    {
        std::cout   << "test_List_iterators"
                    << " k=" << k.repr().as_string()
                    << " *j=" << (*j).repr().as_string()
                    << std::endl;
        if( *j != k )
            return "failed List iterators (2)";

        if( v[ k.as_long() ] != k )
            return "failed List iterators (3)";

        ++k;
    }
    Py::String o1( "Howdy" );
    Py::Long o2( 1 );
    Py::Boolean caught_it( false );
    try
    {
        o2 = o1;
    } 
    catch( Py::Exception& e )
    {
        caught_it = true;
        e.clear();
    }
    if( !caught_it )
        return "failed lis iterators (4).";

    return "ok";
}

static Py::List test_List_references( Py::List &x )
{
    Py::List y;
    for( Py::List::size_type i=0; i < x.length(); ++i )
    {
        std::cout   << "test_List_references"
                    << " isList()=" << x[i].isList()
                    << " x[" << i << "]=" << x[i].repr().as_string()
                    << std::endl;
        if( x[i].isList() )
        {
            y = x[i];
        }
    }
    std::cout   << "test_List_references"
                << " y=" << y.repr().as_string()
                << std::endl;

    return y;
}

static std::string test_List()
{
    // test the Py::List class
    Py::List a;
    Py::List ans;
    Py::List aux;

    aux.append( Py::Long( 3 ) );
    aux.append( Py::Float( 6.0 ) );

    Py::Object b;
    Py::Long i( 3 );
    Py::Float x( 6.0 );
    Py::Float c( 10.0 ), d( 20.0 );
    a.append( i );
    a.append( x );
    a.append( Py::Float( 0.0 ) );
    b = a[0]; 
    a[2] = b;
    a.append( c + d );
    a.append( aux );
    // a is now [3, 6.0, 3, 30.0, aux]

    ans.append( Py::Long(3) );
    ans.append( Py::Float(6.0) );
    ans.append( Py::Long(3) );
    ans.append( Py::Float(30.0) );
    ans.append( aux );

    Py::List::iterator l1, l2;
    for( l1= a.begin(), l2 = ans.begin();
            l1 != a.end() && l2 != ans.end();
                ++l1, ++l2 )
    {
        if( *l1 != *l2 )
            return "failed list (1)" + a.as_string();
    }

    if( a != ans )
    {
        return "failed list (2)";
    }

    if( test_List_references( a ) != aux )
    {
        return "failed list (3) " + test_List_references(a).as_string();
    }

    return test_List_iterators( ans, a );
}

static std::string test_Dict()
{
    // test the Dict class
    Py::Dict a,b;
    Py::List v;
    Py::String s( "two" );
    a["one"] = Py::Long( 1 );
    a[s] = Py::Long( 2 );
    a["three"] = Py::Long( 3 );
    if( Py::Long( a["one"] ) != Py::Long( 1 ) )
        return "failed dict (1) " + a.as_string();
    if( Py::Long( a[s] ) != Py::Long( 2 ) )
        return "failed dict (2) " + a.as_string();

    v = a.values();
#if 0
    std::sort( v.begin(), v.end() );

    for( Py::Long k = 1; k < 4; ++k )
    {
        if( v[k-1] != Py::Long( k ) )
            return "failed dict (3) " + v.as_string();
    }
#endif

    b = a;
    b.clear();
    if( b.keys().length() != 0 )
    {
        return "failed dict (4) " + b.as_string();
    }
    return "ok";
}

static std::string test_Tuple()
{
    // test the Tuple class
    Py::Tuple a( 3 );
    Py::Tuple t1;
    Py::Float f1( 1.0 ), f2( 2.0 ), f3( 3.0 );
    a[0] = f1; // should be ok since no other reference owned
    a[1] = f2;
    a[2] = f3;
    Py::Tuple b( a );
    Py::Long k( 0 );
    for( Py::Tuple::iterator i = b.begin(); i != b.end(); ++i )
    {
        if( *i != Py::Float( ++k ) )
            return "failed tuple (1) " + b.as_string();
    }

    t1 = a;
    try
    {
        t1[0] = Py::Long( 1 ); // should fail, tuple has multiple references
        return "failed tuple (2)";
    }
    catch( Py::Exception& e )
    {
        e.clear();
    }
    return "ok";
}

static std::string test_STL()
{
    Py::Long ans1;
    Py::List w;
    Py::List wans;
    wans.append( Py::Long(1) );
    wans.append( Py::Long(1) );
    wans.append( Py::Long(2) );
    wans.append( Py::Long(3) );
    wans.append( Py::Long(4) );
    wans.append( Py::Long(5) );
    w.append( Py::Long(5) );
    w.append( Py::Long(1) );
    w.append( Py::Long(4) );
    w.append( Py::Long(2) );
    w.append( Py::Long(3) );
    w.append( Py::Long(1) );
    ans1 = std::count( w.begin(), w.end(), Py::Float(1.0) );
    if( ans1.as_long() != 2 )
    {
        return "failed STL (1)";
    }
#if 0
    std::sort( w.begin(), w.end() );
    if( w != wans )
    {
        return "failed STKL (2)";
    }
#endif

    Py::Dict d;
    Py::String s1( "blah" );
    Py::String s2( "gorf" );
    d[ "one" ] = s1;
    d[ "two" ] = s1;
    d[ "three" ] = s2;
    d[ "four" ] = s2;

    Py::Dict::iterator it( d.begin() );
    //    it = d.begin();       // this (using the assignment operator) is causing
                                // a problem; if I just use the copy ctor it works fine.
    for( ; it != d.end(); ++it )
    {
        Py::Dict::value_type    vt( *it );
        Py::String rs = vt.second.repr();
        std::string ls = rs.operator std::string();
        fprintf( stderr, "%s\n", ls.c_str() );
    }

    return "ok";
}

void debug_check_ref_queue()
{
#ifdef Py_TRACE_REFS
    // create an element to find the queue
    Py::Long list_element;

    PyObject *p_slow = list_element.ptr();
    PyObject *p_fast = p_slow;

    do
    {
        assert( p_slow->_ob_next->_ob_prev == p_slow );
        assert( p_slow->_ob_prev->_ob_next == p_slow );

        p_slow = p_slow->_ob_next;
        p_fast = p_slow->_ob_next->_ob_next;

        assert( p_slow != p_fast );    
    }
    while( p_slow != list_element.ptr() );

#endif
}


class example_module : public Py::ExtensionModule<example_module>
{
public:
    example_module()
    : Py::ExtensionModule<example_module>( "example" )
    {
        range::init_type();

        add_varargs_method( "string", &example_module::ex_string, "string( s ) = return string" );
        add_varargs_method( "sum", &example_module::ex_sum, "sum( arglist ) = sum of arguments" );
        add_varargs_method( "test", &example_module::ex_test, "test( arglist ) runs a test suite" );
        add_varargs_method( "range", &example_module::new_r, "range( start, stop, step )" );
        add_keyword_method( "kw", &example_module::ex_keyword, "kw()" );

        initialize( "documentation for the example module" );

        Py::Dict d( moduleDictionary() );

        Py::Object b( Py::asObject( new range( 1, 10, 2 ) ) );

        d["a_constant"] = b.getAttr("c");
        d["a_range"] = b;
    }

    virtual ~example_module()
    {}

private:
    Py::Object ex_keyword( const Py::Tuple &args, const Py::Dict &kws )
    {
        std::cout << "Called with " << args.length() << " normal arguments." << std::endl;
        Py::List names( kws.keys() );
        std::cout << "and with " << names.length() << " keyword arguments:" << std::endl;
        for( Py::List::size_type i=0; i< names.length(); i++ )
        {
            Py::String name( names[i] );
            std::cout << "    " << name << std::endl;
        }

        return Py::Long(0);
    }

    Py::Object new_r (const Py::Tuple &rargs)
    {
        if (rargs.length() < 2 || rargs.length() > 3)
        {
            throw Py::RuntimeError("Incorrect # of args to range(start,stop [,step]).");
        }

        Py::Long start( rargs[0] );
        Py::Long stop( rargs[1] );
        Py::Long step( 1 );
        if (rargs.length() == 3)
        {
            step = rargs[2];
        }
        std::cout   << "new_r"
                    << " start: " << start.as_long()
                    << " stop: " << stop.as_long()
                    << " step: " << step.as_long()
                    << std::endl;
        if( start.as_long() > stop.as_long() + 1 || step.as_long() == 0 )
        {
            throw Py::RuntimeError("Bad arguments to range( start, stop [,step] )");
        }

        return Py::asObject( new range( start.as_long(), stop.as_long(), step.as_long() ) );
    }

    Py::Object ex_string( const Py::Tuple &a )
    {
        std::cout << "ex_std::string: s1 is first arg" << std::endl;
        Py::Object o1( a[0] );
        std::cout << "ex_string: s1.isString() " << o1.isString() << std::endl;

        if( o1.isString() )
        {
            Py::String s1( o1 );

            std::cout << "ex_string: s1.size() " << s1.size() << std::endl;
            std::cout << "ex_string: s2 is s1.encode( utf-8 )" << std::endl;
            Py::Bytes b1( s1.encode( "utf-8" ) );
            std::cout << "ex_string: s1.isString() " << b1.isString() << std::endl;
            std::cout << "ex_string: s1.size() " << b1.size() << std::endl;
            return b1;
        }
        else
        {
            Py::Bytes b1( o1 );
            std::cout << "ex_string: s1 is b1.decode( utf-8 )" << std::endl;
            Py::String s1( b1.decode( "utf-8" ) );
            std::cout << "ex_string: s1.isString() " << s1.isString() << std::endl;
            std::cout << "ex_string: s1.size() " << s1.size() << std::endl;
            return s1;
        }
    }

    Py::Object ex_sum (const Py::Tuple &a)
    {
        // this is just to test the function verify_length:
        try
        {
            a.verify_length(0);
            std::cout << "I see that you refuse to give me any work to do." << std::endl;
        }
        catch (Py::Exception& e)
        {
            e.clear();
            std::cout << "I will now add up your elements, oh great one." << std::endl;
        }


        Py::Float f(0.0);
        for( Py::Sequence::size_type i = 0; i < a.length(); i++ )
        {    
            Py::Float g (a[i]);
            f = f + g;
        }

        return f;
    }

    Py::Object ex_test( const Py::Tuple &a) 
    {
        debug_check_ref_queue();

        std::cout << "Example Test starting" << std::endl;
        try
        {
            Py::String s("this should fail");
            std::cout << "Trying to convert a Py::String to an Py::Long" << std::endl;
            Py::Long k(s.ptr());
        }
        catch (Py::TypeError& e)
        {
            std::cout << "Correctly caught " << Py::type(e) << std::endl;
            std::cout << "  Py::Exception value: " << Py::value(e) << std::endl;
            std::cout << "  Py::Exception traceback: " << Py::trace(e) << std::endl;
            e.clear();
        }

        try
        {
            debug_check_ref_queue();
            std::string result = test_boolean();
            std::cout << "Py::Boolean: " << result << std::endl;
            debug_check_ref_queue();
            std::cout << "Numbers: " << test_numbers() << std::endl;
            debug_check_ref_queue();
            std::cout << "Py::String: " << test_String() << std::endl;
            debug_check_ref_queue();
            std::cout << "Py::List: " << test_List() << std::endl;
            debug_check_ref_queue();
            std::cout << "Py::Dict: " << test_Dict() << std::endl;
            debug_check_ref_queue();
            std::cout << "Py::Tuple: " << test_Tuple() << std::endl;
            debug_check_ref_queue();
            std::cout << "STL test: " << test_STL() << std::endl;
            debug_check_ref_queue();
            std::cout << "Extension object test: " << test_extension_object() << std::endl;
            debug_check_ref_queue();
        }
        catch( TestError &e )
        {
            std::cout << "FAILED: Test error - " << e.m_description << std::endl;
        }

        Py::List b(a);
        Py::Tuple c(b);
        if( c != a)
        {
            std::cout << "failed (1) Py::Tuple/list conversion.\n";
        }

        Py::Module m("sys");
        Py::Object s = m.getAttr("stdout");
        Py::Object nun;
        nun = PyObject_CallMethod(s.ptr(), "write", "s", "Module test ok.\n");
        return Py::None();
    }
};

extern "C" PyObject *PyInit_example()
{
#if defined(PY_WIN32_DELAYLOAD_PYTHON_DLL)
    Py::InitialisePythonIndirectPy::Interface();
#endif

    static example_module *example = new example_module;

    return example->module().ptr();
}

// symbol required for the debug version
extern "C" PyObject *PyInit_example_d()
{ 
    return PyInit_example();
}
