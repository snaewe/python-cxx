#ifdef _MSC_VER
// disable warning C4786: symbol greater than 255 character,
// nessesary to ignore as <map> causes lots of warning
#pragma warning(disable: 4786)
#endif

#include "CXX_Extensions.h"
#include "r.h"

STD::string test_extension_object() 
{ 
        Tuple a; // just something that isn't an r...
        r* r1 = new r(1, 20, 3);
        if(r::check(a)) {
            STD::cout << "r::check failed (1).";
        }
        if(!r::check(r1)) {
            return "r::check failed (2).";
        }

        R r2(1, 10, 2);
        if(r2[1] != Int(3)) {
            return "R check failed. ";
        }

        // calling an extension object method using getattr
        Callable w(r2.getAttr("amethod"));
        Tuple args(1);
        Int j(3);
        args[0]=j;
        List answer(w.apply(args));
        if(answer[0] != r2)
			return ("Extension object test failed (1)");
        if(answer[1] != args[0])
			return ("Extension object test failed (2)");

        Tuple nv(3);
        nv[0] = Int(1);
        nv[1] = Int(20);
        nv[2] = Int(3);
        Tuple unused;
        List r2value;
        r2.assign(unused, nv);
        r2value = r2.value(unused);
        if(r2value[1] != Int(4))
			return("Extension object test failed. (3)");

        // repeat using getattr
        w = r2.getAttr("assign");
        Tuple the_arguments(2);
        the_arguments[0] = unused;
        the_arguments[1] = nv;
        w.apply(the_arguments);

        w = r2.getAttr("value");
        Tuple one_arg(1);
        one_arg[0] = unused;
        r2value = w.apply(one_arg);
        if(r2value[1] != Int(4))
			return("Extension object test failed. (4)");
        r* rheap = new r(1, 10, 2);
        delete rheap;
        delete r1;
        r rlocal(1,20,2);
        return "ok.";
}
