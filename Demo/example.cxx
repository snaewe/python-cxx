//----------------------------------*-C++-*----------------------------------//
// Copyright 1998-1999 The Regents of the University of California. 
// All rights reserved. See LEGAL.LLNL for full text and disclaimer.
//---------------------------------------------------------------------------//

#ifdef _MSC_VER
// disable warning C4786: symbol greater than 255 character,
// nessesary to ignore as <map> causes lots of warning
#pragma warning(disable: 4786)
#endif


#include "Python.h"
#include "CXX/Objects.hxx"
#include "CXX/Extensions.hxx"

#include "r.hxx"  // Extension object
extern std::string test_extension_object();

#include <algorithm>
using namespace Py;
using namespace std;

static string
test_String() {
    String s("hello");
    Char blank = ' ';
    String r1("world in brief", 5);
    s = s + blank + r1;
    s = s * 2;
    if(string(s) != "hello worldhello world") {
        return "failed (1) '" + string(s) + "'";
    }
    // test conversion
    string w = static_cast<string>(s);
    string w2 = (string) s;
    if(w != w2) {
        return "failed (2)";
    }
    String r2("12345 789");
    Char c6 = r2[5];
    if(c6 != blank) {
        std::cout << "|" << c6 << "|" << std::endl;
        return "failed (3)";
    }
    return "ok";
}

static string
test_numbers() {
    // test the basic numerical classes
    Int i;
    Int j(2);
    Int k = Int(3);
    if (! (j < k)) return "failed (1)";
    if (! (j == j)) return "failed (2)";
    if (! (j != k)) return "failed (3)";
    if (! (j <= k)) return "failed (4)";
    if (! (k >= j)) return "failed (5)";
    if (! (k > j)) return "failed (6)";
    if (! (j <= j)) return "failed (7)";
    if (! (j >= Int(2))) return "failed (8)";

    i = 2;
    Float a;
    a = 3 + i; //5.0
    Float b(4.0);
    a = (1.0 + 2*a + (b*3.0)/2.0 + k)/Float(5); // 4.0
    i = a - 1.0; // 3
    if(i != k) {
        return "failed 9";
    }
//  Complex c1(1.0, 0.0), c2(0.0, 1.0);
//  if(c2*c2 != -c1) return "failed 10";
//  if(c1.real() != 1.0) return "failed 11";
//  if(Py_complex(c1).real != 1.0) return "failed 12";
    return "ok";
}

static string 
test_List_iterators (const List& x, List& y) {
    vector<Object> v;
    Sequence::iterator j;
    int k = 0;
    for(Sequence::const_iterator i= x.begin(); i != x.end(); ++i) {
        if ((*i).isList()) {
            ++k;
        }
    }
    if(k!=1) return "failed List iterators (1)";

    k = 0;
    for(j = y.begin(); j != y.end(); ++j) {
        *j = Int(k++);
        v.push_back (*j);
    }
    
    k = 0;
    for(j = y.begin(); j != y.end(); j++) {
        if(*j != Int(k)) return "failed List iterators (2)";
        if(v[k] != Int(k)) return "failed List iterators (3)";
        ++k;
    }
    String o1("Howdy");
    Int o2(1);
    int caught_it = 0;
    try {
        o2 = o1;
    } 
    catch (Exception& e) {
        caught_it = 1;
        e.clear();
    }
    if(!caught_it) return "failed exception catch (4).";
    return "ok";
}

static List
test_List_references (List& x) {
    List y;
    for(List::size_type i=0; i < x.length(); ++i) {
        if (x[i].isList()) {
            y = x[i];
        }
    }
    return y;
}

static string
test_List() {
    // test the List class
    List a;
    List ans, aux;
    aux.append(Int(3));
    aux.append(Float(6.0));

    Object b;
    Int i(3);
    Float x(6.0);
    Float c(10.0), d(20.0);
    a.append(i);
    a.append(x);
    a.append(Float(0.0));
    b = a[0]; 
    a[2] = b;
    a.append(c+d);
    a.append(aux);
// a is now [3, 6.0, 3, 30.0, aux]

    ans.append(Int(3));
    ans.append(Float(6.0));
    ans.append(Int(3));
    ans.append(Float(30.0));
    ans.append(aux);

    List::iterator l1, l2;
    for(l1= a.begin(), l2 = ans.begin();
        l1 != a.end() && l2 != ans.end();
        ++l1, ++l2) 
    {
        if(*l1 != *l2) return "failed 1" + a.as_string();
    }

    if (test_List_references (a)!= aux) {
        return "failed 2" + test_List_references(a).as_string();
    }
    return test_List_iterators(ans, a);
}

static string
test_Dict() {
    // test the Dict class
    Dict a,b;
    List v;
    String s("two");
    a["one"] = Int(1);
    a[s] = Int(2);
    a["three"] = Int(3);
    if(Int(a["one"]) != Int(1)) return "failed 1a " + a.as_string();
    if(Int(a[s]) != Int(2)) return "failed 1b " + a.as_string();

    v = a.values();
    sort(v.begin(), v.end());

    for(int k = 1; k < 4; ++k) {
        if(v[k-1] != Int(k)) return "failed 2 " + v.as_string();
    }
    
    b = a;
    b.clear();
    if(b.keys().length() != 0) {
        return "failed 3 " + b.as_string();
    }
    return "ok";
}

static string
test_Tuple() {
    // test the Tuple class
    Tuple a(3);
    Tuple t1;
    Float f1(1.0), f2(2.0), f3(3.0);
    a[0] = f1; // should be ok since no other reference owned
    a[1] = f2;
    a[2] = f3;
    Tuple b(a);
    int k = 0;
    for(Tuple::iterator i = b.begin(); i != b.end(); ++i) {
        if(*i != Float(++k)) return "failed 1 " + b.as_string();
    }

    t1 = a;
    try {
        t1[0] = Int(1); // should fail, tuple has multiple references
        return "failed 2";
    }
    catch (Exception& e) {
       e.clear();
    }
    return "ok";
}

static string 
test_STL() {
    int ans1;
    List w;
    List wans;
    wans.append(Int(1));
    wans.append(Int(1));
    wans.append(Int(2));
    wans.append(Int(3));
    wans.append(Int(4));
    wans.append(Int(5));
    w.append(Int(5));
    w.append(Int(1));
    w.append(Int(4));
    w.append(Int(2));
    w.append(Int(3));
    w.append(Int(1));
    ans1 = count(w.begin(), w.end(), Float(1.0));
    if (ans1 != 2) {
        return "failed count test";
    }
    sort(w.begin(), w.end());
    if (w != wans) {
        return "failed sort test";
    }
    return "ok";
}

extern "C" void initexample();

class example_module : public ExtensionModule<example_module>
{
public:
	example_module()
		: ExtensionModule<example_module>( "example" )
	{
		r::init_type();

		add_varargs_method("sum", &example_module::ex_sum, "sum(arglist) = sum of arguments");
		add_varargs_method("test", &example_module::ex_test, "test(arglist) runs a test suite");
		add_varargs_method("r", &example_module::new_r, "r(start,stop,stride)");
		add_keyword_method("kw", &example_module::ex_keyword, "kw()");

		initialize( "documentation for the example module" );

		Dict d( moduleDictionary() );

		Object b(asObject(new r(1,10,2)));

		d["a_constant"] = b.getAttr("c");
	}

	virtual ~example_module() {}

private:
	Object ex_keyword( const Tuple &args, const Dict &kws )
	{
		std::cout << "Called with " << args.length() << " normal arguments." << std::endl;
		List names( kws.keys() );
		std::cout << "and with " << names.length() << " keyword arguments:" << std::endl;
		for( int i=0; i< names.length(); i++ )
			{
			String name( names[i] );
			std::cout << "    " << name << std::endl;
			}
		
		return Int(0);
	}

	Object new_r (const Tuple &rargs)
	{
		if (rargs.length() < 2 || rargs.length() > 3)
		{
			throw RuntimeError("Incorrect # of args to r(start,stop [,step]).");
		}

		Int start(rargs[0]);
		Int stop(rargs[1]);
		Int step(1);
		if (rargs.length() == 3)
		{
			step = rargs[2];
		}
		if (long(start) > long(stop) + 1 || long(step) == 0)
		{
			throw RuntimeError("Bad arguments to r(start,stop [,step]).");
		}
		return asObject(new r(start, stop, step));
	}

	Object ex_sum (const Tuple &a)
	{
		// this is just to test the function verify_length:
		try
			{
			a.verify_length(0);
			std::cout << "I see that you refuse to give me any work to do." << endl;
			}
		catch (Exception& e)
			{
			e.clear();
			std::cout << "I will now add up your elements, oh great one." << endl;
			}


		Float f(0.0);
		for( int i = 0; i < a.length(); i++ )
		{    
			Float g (a[i]);
			f = f + g;
		}

			return f;
	}

	Object ex_test( const Tuple &a) 
	{
		try
		{
			String s("this should fail");
			Int k(s.ptr());
		}
		catch (TypeError& e)
		{
			std::cout << "Correctly caught " << type(e) << endl;
			std::cout << "  Exception value: " << value(e) << endl;
			std::cout << "  Exception traceback: " << trace(e) << endl;
			e.clear();
		}
		std::cout << "Numbers: " << test_numbers() << endl;
		std::cout << "String: " << test_String() << endl;
		std::cout << "List: " << test_List() << endl;
		std::cout << "Dict: " << test_Dict() << endl;
		std::cout << "Tuple: " << test_Tuple() << endl;
		std::cout << "STL test: " << test_STL() << endl;
		std::cout << "Extension object test: " << test_extension_object() << endl;

		List b(a);
		Tuple c(b);
		if( c != a)
		{
			std::cout << "Tuple/list conversion failed.\n";
		}

		Module m("sys");
		Object s = m.getAttr("stdout");
		Object nun;
		nun = PyObject_CallMethod(s.ptr(), "write", "s", "Module test ok.\n");
		return Nothing();
	}
};

// symbol required for the debug version
void initexample_d()
	{ initexample(); }


void initexample()
{
	static example_module* example = new example_module;
}
