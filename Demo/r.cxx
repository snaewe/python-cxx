#ifdef _MSC_VER
// disable warning C4786: symbol greater than 255 character,
// nessesary to ignore as <map> causes lots of warning
#pragma warning(disable: 4786)
#endif

#include "r.h"
// Connect r objects to Python


Py::Object r::repr()
{
	return String(asString());
}

int r::sequence_length()
{
    return length();
}

Py::Object r::sequence_item(int i) 
{
    return Int(item(i));
}

Py::Object r::sequence_concat( const Py::Object &j)
{
    Int k(j);
    return Py::asObject(extend(int(k)));
}

Py::Object r::sequence_slice(int i, int j)
{
    return Py::asObject(slice(i,j));
}


Py::Object r::getattr( const char *name )
{
    if(STD::string(name) == "c")
	return Float(300.0);

    if(STD::string(name) == "start")
	return Int(start);

    return getattr_methods( name );
}

// "regular" methods...
Py::Object r::amethod( const Py::Tuple &t ) 
{
	STD::cout << "r_amethod size() = " << t.size() << STD::endl << STD::flush;

	t.verify_length(1);

	STD::cout << "r_amethod verify_length" << STD::endl << STD::flush;

	List result;
	result.append(Py::Object(this));
	result.append(t[0]);

	return result;
}

Py::Object r::value( const Py::Tuple &t )
{
    return c_value(t);
}

Py::Object r::assign( const Py::Tuple &t ) 
{
	

	t.verify_length(2);

	Py::Tuple t1(t[0]); // subscripts
	Py::Object o2(t[1]); // rhs;
	c_assign (t1, o2);

	return Nothing();
}

void r::init_type()
{
	behaviors().name("r");
	behaviors().doc("r objects: start, stop, step");
	behaviors().supportRepr();
	behaviors().supportGetattr();
	behaviors().supportSequenceType();

	add_varargs_method("amethod", &r::amethod,
		"demonstrate how to document amethod");
	add_varargs_method("assign", &r::assign);
	add_varargs_method("value", &r::value);
	add_varargs_method("reference_count", &r::reference_count);
}
