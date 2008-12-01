import simple
simple.func()
simple.func( 4, 5 )
simple.func( 4, 5, name=6, value=7 )

old_style_class = simple.old_style_class()
old_style_class.old_style_class_func_noargs()
old_style_class.old_style_class_func_varargs()
old_style_class.old_style_class_func_varargs( 4 )
old_style_class.old_style_class_func_keyword()
old_style_class.old_style_class_func_keyword( name=6, value=7 )
old_style_class.old_style_class_func_keyword( 4, 5 )
old_style_class.old_style_class_func_keyword( 4, 5, name=6, value=7 )

new_style_class = simple.new_style_class()
new_style_class.new_style_class_func_noargs()
new_style_class.new_style_class_func_varargs()
new_style_class.new_style_class_func_varargs( 4 )
new_style_class.new_style_class_func_keyword()
new_style_class.new_style_class_func_keyword( name=6, value=7 )
new_style_class.new_style_class_func_keyword( 4, 5 )
new_style_class.new_style_class_func_keyword( 4, 5, name=6, value=7 )
print( 'dir(new_style_class)',dir(new_style_class) )

class derived_new_style_class(simple.new_style_class):
    def __init__( self, arg ):
        simple.new_style_class.__init__( self )
        self.arg = arg

    def derived_func( self ):
        print( 'derived_func => %r' % self.arg )
        self.new_style_class_func_noargs()

dnsc = derived_new_style_class( [1,3,7] )
print( 'dir(dnsc)',dir(dnsc) )
dnsc.derived_func()
dnsc.new_style_class_func_noargs()
