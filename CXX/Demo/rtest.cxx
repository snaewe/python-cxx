#ifdef _MSC_VER
// disable warning C4786: symbol greater than 255 character,
// nessesary to ignore as <map> causes lots of warning
#pragma warning(disable: 4786)
#endif

#include "CXX/CXX_Extensions.h"
#include "r.h"
// This test also illustrates using the Py namespace explicitly

std::string test_extension_object() 
{ 
	Py::Tuple a; // just something that isn't an r...
    r* r1 = new r(1, 20, 3);
    if(r::check(a)) {
        std::cout << "r::check failed (1).";
    }
    if(!r::check(r1)) {
        return "r::check failed (2).";
    }

    R r2(1, 10, 2);
    if(r2[1] != Py::Int(3)) {
        return "R check failed. ";
    }

        // calling an extension object method using getattr
	Py::Callable w(r2.getAttr("amethod"));
	Py::Tuple args(1);
	Py::Int j(3);
    args[0]=j;
	Py::List answer(w.apply(args));
    if(answer[0] != r2)
		return ("Extension object test failed (1)");
    if(answer[1] != args[0])
		return ("Extension object test failed (2)");

	Py::Tuple nv(3);
    nv[0] = Py::Int(1);
    nv[1] = Py::Int(20);
    nv[2] = Py::Int(3);
	Py::Tuple unused;
	Py::List r2value;
    r2.assign(unused, nv);
    r2value = r2.value(unused);
    if(r2value[1] != Py::Int(4))
		return("Extension object test failed. (3)");

        // repeat using getattr
    w = r2.getAttr("assign");
	Py::Tuple the_arguments(2);
    the_arguments[0] = unused;
    the_arguments[1] = nv;
    w.apply(the_arguments);

    w = r2.getAttr("value");
	Py::Tuple one_arg(1);
    one_arg[0] = unused;
    r2value = w.apply(one_arg);
    if(r2value[1] != Py::Int(4))
		return("Extension object test failed. (4)");
    r* rheap = new r(1, 10, 2);
    delete rheap;
    delete r1;
    r rlocal(1,20,2);
    return "ok.";
}
