import simple

print( '--- module func ---' )
simple.func()
simple.func( 4, 5 )
simple.func( 4, 5, name=6, value=7 )

print( '--- old_style_class func ---' )
old_style_class = simple.old_style_class()
old_style_class.old_style_class_func_noargs()
old_style_class.old_style_class_func_varargs()
old_style_class.old_style_class_func_varargs( 4 )
old_style_class.old_style_class_func_keyword()
old_style_class.old_style_class_func_keyword( name=6, value=7 )
old_style_class.old_style_class_func_keyword( 4, 5 )
old_style_class.old_style_class_func_keyword( 4, 5, name=6, value=7 )

print( '--- new_style_class func ---' )
new_style_class = simple.new_style_class()
print( dir( new_style_class ) )
new_style_class.new_style_class_func_noargs()
new_style_class.new_style_class_func_varargs()
new_style_class.new_style_class_func_varargs( 4 )
new_style_class.new_style_class_func_keyword()
new_style_class.new_style_class_func_keyword( name=6, value=7 )
new_style_class.new_style_class_func_keyword( 4, 5 )
new_style_class.new_style_class_func_keyword( 4, 5, name=6, value=7 )
new_style_class = None

print( '--- Derived func ---' )
class Derived(simple.new_style_class):
    def __init__( self ):
        simple.new_style_class.__init__( self )

    def derived_func( self ):
        print( 'derived_func' )
        super( Derived, self ).new_style_class_func_noargs()

    def new_style_class_func_noargs( self ):
        print( 'derived new_style_class_func_noargs' )

d = Derived()
print( dir( d ) )
d.derived_func()
d.new_style_class_func_noargs()
d.new_style_class_func_varargs()
d.new_style_class_func_varargs( 4 )
d.new_style_class_func_keyword()
d.new_style_class_func_keyword( name=6, value=7 )
d.new_style_class_func_keyword( 4, 5 )
d.new_style_class_func_keyword( 4, 5, name=6, value=7 )

print( d.value )
d.value = "a string"
print( d.value )
d.new_var = 99
