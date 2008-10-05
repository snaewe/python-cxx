import simple
simple.func()
simple.func( 4, 5 )
simple.func( 4, 5, name=6, value=7 )

cls = simple.cls()
cls.cls_func_noargs()
cls.cls_func_varargs()
cls.cls_func_varargs( 4 )
cls.cls_func_keyword()
cls.cls_func_keyword( name=6, value=7 )
cls.cls_func_keyword( 4, 5 )
cls.cls_func_keyword( 4, 5, name=6, value=7 )
