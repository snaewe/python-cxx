#include "CXX_Extensions.h"

namespace Py {

//================================================================================
//
//	Implementation of MethodTable
//
//================================================================================

PyMethodDef MethodTable::method( const char* method_name, PyCFunction f, int flags, const char* doc ) 
{
	PyMethodDef m;
	m.ml_name = const_cast<char*>( method_name );
	m.ml_meth = f;
	m.ml_flags = flags;
	m.ml_doc = const_cast<char*>( doc );
	return m;
}
MethodTable::MethodTable()
{
	t.push_back( method( 0, 0, 0, 0 ) );
	mt = 0;
}

MethodTable::~MethodTable()
{
	delete [] mt;
}

void MethodTable::add( const char* method_name, PyCFunction f, const char* doc, int flag )
{
	if( !mt )
		{
		t.insert( t.end()-1, method( method_name, f, flag, doc ) );
		}
	else
		{
		throw RuntimeError( "Too late to add a module method!" );
		}
}

PyMethodDef* MethodTable::table()
{    
    if( !mt )
    {
	int t1size = t.size();
	mt = new PyMethodDef[t1size];
	int j = 0;
	for( std::vector<PyMethodDef>::iterator i = t.begin(); i != t.end(); i++ )
	{
	    mt[j++] = *i;
	}
    }
    return mt;
}

//================================================================================
//
//	Implementation of ExtensionModule
//
//================================================================================
ExtensionModuleBase::ExtensionModuleBase( const char *name )
	: module_name( name )
	, method_table()
	{}

ExtensionModuleBase::~ExtensionModuleBase()
	{}


class ExtensionModuleBasePtr : public PythonExtension<ExtensionModuleBasePtr>
	{
public:
	ExtensionModuleBasePtr( ExtensionModuleBase *_module )
		: module( _module )
		{}
	virtual ~ExtensionModuleBasePtr()
		{}

	ExtensionModuleBase *module;
	};


void ExtensionModuleBase::initialize( const char *module_doc )
	{
	PyObject *module_ptr = new ExtensionModuleBasePtr( this );

	Py_InitModule4
		(
		const_cast<char *>( module_name.c_str() ),	// name
		method_table.table(),				// methods
		const_cast<char *>( module_doc ),		// docs
		module_ptr,					// pass to functions as "self"
		PYTHON_API_VERSION				// API version
		);
	}

Py::Module ExtensionModuleBase::module(void) const
	{
	return Module( module_name );
	}

Py::Dict ExtensionModuleBase::moduleDictionary(void) const
	{
	return module().getDict();
	}

//--------------------------------------------------------------------------------

//================================================================================
//
//	Implementation of PythonType
//
//================================================================================
void PythonType::standard_dealloc( PyObject* p )
{
	PyMem_DEL( p );
}

void PythonType::supportSequenceType()
{
	if( !sequence_table )
	{
	    sequence_table = new PySequenceMethods;
	    table->tp_as_sequence = sequence_table;
	    sequence_table->sq_length = sequence_length_handler;
	    sequence_table->sq_concat = sequence_concat_handler;
	    sequence_table->sq_repeat = sequence_repeat_handler;
	    sequence_table->sq_item = sequence_item_handler;
	    sequence_table->sq_slice = sequence_slice_handler;

	    sequence_table->sq_ass_item = sequence_ass_item_handler;	// BAS setup seperately?
	    sequence_table->sq_ass_slice = sequence_ass_slice_handler;	// BAS setup seperately?
	}
}

void PythonType::supportMappingType()
{
	if( !mapping_table )
	{
	    mapping_table = new PyMappingMethods;
	    table->tp_as_mapping = mapping_table;
	    mapping_table->mp_length = mapping_length_handler;
	    mapping_table->mp_subscript = mapping_subscript_handler;
	    mapping_table->mp_ass_subscript = mapping_ass_subscript_handler;	// BAS setup seperately?
	}
}

void PythonType::supportNumberType()
{
	if( !number_table )
	{
	    number_table = new PyNumberMethods;
	    table->tp_as_number = number_table;
	    number_table->nb_add = number_add_handler;
	    number_table->nb_subtract = number_subtract_handler;
	    number_table->nb_multiply = number_multiply_handler;
	    number_table->nb_divide = number_divide_handler;
	    number_table->nb_remainder = number_remainder_handler;
	    number_table->nb_divmod = number_divmod_handler;
	    number_table->nb_power = number_power_handler;
	    number_table->nb_negative = number_negative_handler;
	    number_table->nb_positive = number_positive_handler;
	    number_table->nb_absolute = number_absolute_handler;
	    number_table->nb_nonzero = number_nonzero_handler;
	    number_table->nb_invert = number_invert_handler;
	    number_table->nb_lshift = number_lshift_handler;
	    number_table->nb_rshift = number_rshift_handler;
	    number_table->nb_and = number_and_handler;
	    number_table->nb_xor = number_xor_handler;
	    number_table->nb_or = number_or_handler;
	    number_table->nb_coerce = 0;
	    number_table->nb_int = number_int_handler;
	    number_table->nb_long = number_long_handler;
	    number_table->nb_float = number_float_handler;
	    number_table->nb_oct = number_oct_handler;
	    number_table->nb_hex = number_hex_handler;
	}
}

void PythonType::supportBufferType()
{
	if( !buffer_table )
	{
	    buffer_table = new PyBufferProcs;
	    table->tp_as_buffer = buffer_table;
	    buffer_table->bf_getreadbuffer = buffer_getreadbuffer_handler;
	    buffer_table->bf_getwritebuffer = buffer_getwritebuffer_handler;
	    buffer_table->bf_getsegcount = buffer_getsegcount_handler;
	}
}

// if you define one sequence method you must define 
// all of them except the assigns

PythonType::PythonType( size_t basic_size, int itemsize )
	: table( new PyTypeObject )
	, sequence_table( NULL )
	, mapping_table( NULL )
	, number_table( NULL )
	, buffer_table( NULL )
	{
	*reinterpret_cast<PyObject*>( table ) = py_object_initializer;
	table->ob_type = &PyType_Type;
	table->ob_size = 0;
	table->tp_name = "unknown";
	table->tp_basicsize = basic_size;
	table->tp_itemsize = itemsize;
	table->tp_dealloc = ( destructor ) standard_dealloc;
	table->tp_print = 0;
	table->tp_getattr = 0;
	table->tp_setattr = 0;
	table->tp_compare = 0;
	table->tp_repr = 0;
	table->tp_as_number = 0;
	table->tp_as_sequence = 0;
	table->tp_as_mapping =  0;
	table->tp_hash = 0;
	table->tp_call = 0;
	table->tp_str = 0;
	table->tp_getattro = 0;
	table->tp_setattro = 0;
	table->tp_as_buffer = 0;
	table->tp_flags = 0L;
	table->tp_doc = 0;
	table->tp_xxx5 = 0L;
	table->tp_xxx6 = 0L;
	table->tp_xxx7 = 0L;
	table->tp_xxx8 = 0L;

#ifdef COUNT_ALLOCS
	table->tp_alloc = 0;
	table->tp_free = 0;
	table->tp_maxalloc = 0;
	table->tp_next = 0;
#endif
	}

PythonType::~PythonType( )
{
	delete table;
	delete sequence_table;
	delete mapping_table;
	delete number_table;
	delete buffer_table;
};

PyTypeObject* PythonType::type_object( ) const {return table;}

void PythonType::name( const char* nam )
{
	table->tp_name = const_cast<char *>( nam );
}

void PythonType::doc( const char* d )
{
	table->tp_doc = const_cast<char *>( d );
}

void PythonType::dealloc( void( *f )( PyObject* ))
{
	table->tp_dealloc = f;
}

void PythonType::supportPrint()
{
	table->tp_print = print_handler;
}

void PythonType::supportGetattr()
{
	table->tp_getattr = getattr_handler;
}

void PythonType::supportSetattr()
{
	table->tp_setattr = setattr_handler;
}

void PythonType::supportGetattro()
{
	table->tp_getattro = getattro_handler;
}

void PythonType::supportSetattro()
{
	table->tp_setattro = setattro_handler;
}

void PythonType::supportCompare()
{
	table->tp_compare = compare_handler;
}

void PythonType::supportRepr()
{
	table->tp_repr = repr_handler;
}

void PythonType::supportStr()
{
	table->tp_str = str_handler;
}

void PythonType::supportHash()
{
	table->tp_hash = hash_handler;
}

void PythonType::supportCall()
{
	table->tp_call = call_handler;
}

//--------------------------------------------------------------------------------
//
//	Handlers
//
//--------------------------------------------------------------------------------
int PythonType::print_handler( PyObject *self, FILE *fp, int flags )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return p->print( fp, flags );
		}
	catch( Py::Exception & )
		{
		return -1;	// indicate error
		}
	}

PyObject* PythonType::getattr_handler( PyObject *self, char *name )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return new_reference_to( p->getattr( name ) );
		}
	catch( Py::Exception & )
		{
		return NULL;	// indicate error
		}
	}

int PythonType::setattr_handler( PyObject *self, char *name, PyObject *value )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return p->setattr( name, Py::Object( value ) );
		}
	catch( Py::Exception & )
		{
		return -1;	// indicate error
		}
	}

PyObject* PythonType::getattro_handler( PyObject *self, PyObject *name )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return new_reference_to( p->getattro( Py::Object( name ) ) );
		}
	catch( Py::Exception & )
		{
		return NULL;	// indicate error
		}
	}

int PythonType::setattro_handler( PyObject *self, PyObject *name, PyObject *value )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return p->setattro( Py::Object( name ), Py::Object( value ) );
		}
	catch( Py::Exception & )
		{
		return -1;	// indicate error
		}
	}

int PythonType::compare_handler( PyObject *self, PyObject *other )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return p->compare( Py::Object( other ) );
		}
	catch( Py::Exception & )
		{
		return -1;	// indicate error
		}
	}

PyObject* PythonType::repr_handler( PyObject *self )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return new_reference_to( p->repr() );
		}
	catch( Py::Exception & )
		{
		return NULL;	// indicate error
		}
	}

PyObject* PythonType::str_handler( PyObject *self )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return new_reference_to( p->str() );
		}
	catch( Py::Exception & )
		{
		return NULL;	// indicate error
		}
	}

long PythonType::hash_handler( PyObject *self )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return p->hash();
		}
	catch( Py::Exception & )
		{
		return -1;	// indicate error
		}
	}

PyObject* PythonType::call_handler( PyObject *self, PyObject *args, PyObject *kw )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return new_reference_to( p->call( Py::Object( args ), Py::Object( kw ) ) );
		}
	catch( Py::Exception & )
		{
		return NULL;	// indicate error
		}
	}


// Sequence methods
int PythonType::sequence_length_handler( PyObject *self )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return p->sequence_length();
		}
	catch( Py::Exception & )
		{
		return -1;	// indicate error
		}
	}

PyObject* PythonType::sequence_concat_handler( PyObject *self, PyObject *other )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return new_reference_to( p->sequence_concat( Py::Object( other ) ) );
		}
	catch( Py::Exception & )
		{
		return NULL;	// indicate error
		}
	}

PyObject* PythonType::sequence_repeat_handler( PyObject *self, int count )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return new_reference_to( p->sequence_repeat( count ) );
		}
	catch( Py::Exception & )
		{
		return NULL;	// indicate error
		}
	}

PyObject* PythonType::sequence_item_handler( PyObject *self, int index )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return new_reference_to( p->sequence_item( index ) );
		}
	catch( Py::Exception & )
		{
		return NULL;	// indicate error
		}
	}

PyObject* PythonType::sequence_slice_handler( PyObject *self, int first, int last )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return new_reference_to( p->sequence_slice( first, last ) );
		}
	catch( Py::Exception & )
		{
		return NULL;	// indicate error
		}
	}

int PythonType::sequence_ass_item_handler( PyObject *self, int index, PyObject *value )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return p->sequence_ass_item( index, Py::Object( value ) );
		}
	catch( Py::Exception & )
		{
		return -1;	// indicate error
		}
	}

int PythonType::sequence_ass_slice_handler( PyObject *self, int first, int last, PyObject *value )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return p->sequence_ass_slice( first, last, Py::Object( value ) );
		}
	catch( Py::Exception & )
		{
		return -1;	// indicate error
		}
	}

// Mapping
int PythonType::mapping_length_handler( PyObject *self )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return p->mapping_length();
		}
	catch( Py::Exception & )
		{
		return -1;	// indicate error
		}
	}

PyObject* PythonType::mapping_subscript_handler( PyObject *self, PyObject *key )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return new_reference_to( p->mapping_subscript( Py::Object( key ) ) );
		}
	catch( Py::Exception & )
		{
		return NULL;	// indicate error
		}
	}

int PythonType::mapping_ass_subscript_handler( PyObject *self, PyObject *key, PyObject *value )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return p->mapping_ass_subscript( Py::Object( key ), Py::Object( value ) );
		}
	catch( Py::Exception & )
		{
		return -1;	// indicate error
		}
	}

// Number
int PythonType::number_nonzero_handler( PyObject *self )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return p->number_nonzero();
		}
	catch( Py::Exception & )
		{
		return -1;	// indicate error
		}
	}

PyObject* PythonType::number_negative_handler( PyObject *self )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return new_reference_to( p->number_negative() );
		}
	catch( Py::Exception & )
		{
		return NULL;	// indicate error
		}
	}

PyObject* PythonType::number_positive_handler( PyObject *self )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return new_reference_to( p->number_positive() );
		}
	catch( Py::Exception & )
		{
		return NULL;	// indicate error
		}
	}

PyObject* PythonType::number_absolute_handler( PyObject *self )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return new_reference_to( p->number_absolute() );
		}
	catch( Py::Exception & )
		{
		return NULL;	// indicate error
		}
	}

PyObject* PythonType::number_invert_handler( PyObject *self )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return new_reference_to( p->number_invert() );
		}
	catch( Py::Exception & )
		{
		return NULL;	// indicate error
		}
	}

PyObject* PythonType::number_int_handler( PyObject *self )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return new_reference_to( p->number_int() );
		}
	catch( Py::Exception & )
		{
		return NULL;	// indicate error
		}
	}

PyObject* PythonType::number_float_handler( PyObject *self )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return new_reference_to( p->number_float() );
		}
	catch( Py::Exception & )
		{
		return NULL;	// indicate error
		}
	}

PyObject* PythonType::number_long_handler( PyObject *self )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return new_reference_to( p->number_long() );
		}
	catch( Py::Exception & )
		{
		return NULL;	// indicate error
		}
	}

PyObject* PythonType::number_oct_handler( PyObject *self )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return new_reference_to( p->number_oct() );
		}
	catch( Py::Exception & )
		{
		return NULL;	// indicate error
		}
	}

PyObject* PythonType::number_hex_handler( PyObject *self )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return new_reference_to( p->number_hex() );
		}
	catch( Py::Exception & )
		{
		return NULL;	// indicate error
		}
	}

PyObject* PythonType::number_add_handler( PyObject *self, PyObject *other )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return new_reference_to( p->number_add( Py::Object( other ) ) );
		}
	catch( Py::Exception & )
		{
		return NULL;	// indicate error
		}
	}

PyObject* PythonType::number_subtract_handler( PyObject *self, PyObject *other )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return new_reference_to( p->number_subtract( Py::Object( other ) ) );
		}
	catch( Py::Exception & )
		{
		return NULL;	// indicate error
		}
	}

PyObject* PythonType::number_multiply_handler( PyObject *self, PyObject *other )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return new_reference_to( p->number_multiply( Py::Object( other ) ) );
		}
	catch( Py::Exception & )
		{
		return NULL;	// indicate error
		}
	}

PyObject* PythonType::number_divide_handler( PyObject *self, PyObject *other )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return new_reference_to( p->number_divide( Py::Object( other ) ) );
		}
	catch( Py::Exception & )
		{
		return NULL;	// indicate error
		}
	}

PyObject* PythonType::number_remainder_handler( PyObject *self, PyObject *other )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return new_reference_to( p->number_remainder( Py::Object( other ) ) );
		}
	catch( Py::Exception & )
		{
		return NULL;	// indicate error
		}
	}

PyObject* PythonType::number_divmod_handler( PyObject *self, PyObject *other )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return new_reference_to( p->number_divmod( Py::Object( other ) ) );
		}
	catch( Py::Exception & )
		{
		return NULL;	// indicate error
		}
	}

PyObject* PythonType::number_lshift_handler( PyObject *self, PyObject *other )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return new_reference_to( p->number_lshift( Py::Object( other ) ) );
		}
	catch( Py::Exception & )
		{
		return NULL;	// indicate error
		}
	}

PyObject* PythonType::number_rshift_handler( PyObject *self, PyObject *other )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return new_reference_to( p->number_rshift( Py::Object( other ) ) );
		}
	catch( Py::Exception & )
		{
		return NULL;	// indicate error
		}
	}

PyObject* PythonType::number_and_handler( PyObject *self, PyObject *other )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return new_reference_to( p->number_and( Py::Object( other ) ) );
		}
	catch( Py::Exception & )
		{
		return NULL;	// indicate error
		}
	}

PyObject* PythonType::number_xor_handler( PyObject *self, PyObject *other )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return new_reference_to( p->number_xor( Py::Object( other ) ) );
		}
	catch( Py::Exception & )
		{
		return NULL;	// indicate error
		}
	}

PyObject* PythonType::number_or_handler( PyObject *self, PyObject *other )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return new_reference_to( p->number_or( Py::Object( other ) ) );
		}
	catch( Py::Exception & )
		{
		return NULL;	// indicate error
		}
	}

PyObject* PythonType::number_power_handler( PyObject *self, PyObject *x1, PyObject *x2 )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return new_reference_to( p->number_power( Py::Object( x1 ), Py::Object( x2 ) ) );
		}
	catch( Py::Exception & )
		{
		return NULL;	// indicate error
		}
	}

// Buffer
int PythonType::buffer_getreadbuffer_handler( PyObject *self, int index, void **pp )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return p->buffer_getreadbuffer( index, pp );
		}
	catch( Py::Exception & )
		{
		return -1;	// indicate error
		}
	}

int PythonType::buffer_getwritebuffer_handler( PyObject *self, int index, void **pp )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return p->buffer_getwritebuffer( index, pp );
		}
	catch( Py::Exception & )
		{
		return -1;	// indicate error
		}
	}

int PythonType::buffer_getsegcount_handler( PyObject *self, int *count )
	{
	try
 		{
		PythonExtensionBase *p = static_cast<PythonExtensionBase *>( self );
		return p->buffer_getsegcount( count );
		}
	catch( Py::Exception & )
		{
		return -1;	// indicate error
		}
	}


//================================================================================
//
//	Implementation of PythonExtensionBase
//
//================================================================================
void PythonExtensionBase::missing_method()
{
	throw RuntimeError( "Extension object missing a required method." );
}

int PythonExtensionBase::print( FILE *, int )
	{ missing_method(); return -1; }

Py::Object PythonExtensionBase::getattr( const char* )
	{ missing_method(); return Py::Nothing(); }

int PythonExtensionBase::setattr( const char*, const Py::Object & )
	{ missing_method(); return -1; }

Py::Object PythonExtensionBase::getattro( const Py::Object & )
	{ missing_method(); return Py::Nothing(); }

int PythonExtensionBase::setattro( const Py::Object &, const Py::Object & )
	{ missing_method(); return -1; }

int PythonExtensionBase::compare( const Py::Object & )
	{ missing_method(); return -1; }

Py::Object PythonExtensionBase::repr()
	{ missing_method(); return Py::Nothing(); }

Py::Object PythonExtensionBase::str()
	{ missing_method(); return Py::Nothing(); }

long PythonExtensionBase::hash()
	{ missing_method(); return -1; }

Py::Object PythonExtensionBase::call( const Py::Object &, const Py::Object & )
	{ missing_method(); return Py::Nothing(); }


	// Sequence methods
int PythonExtensionBase::sequence_length()
	{ missing_method(); return -1; }

Py::Object PythonExtensionBase::sequence_concat( const Py::Object & )
	{ missing_method(); return Py::Nothing(); }

Py::Object PythonExtensionBase::sequence_repeat( int )
	{ missing_method(); return Py::Nothing(); }

Py::Object PythonExtensionBase::sequence_item( int )
	{ missing_method(); return Py::Nothing(); }

Py::Object PythonExtensionBase::sequence_slice( int, int )
	{ missing_method(); return Py::Nothing(); }

int PythonExtensionBase::sequence_ass_item( int, const Py::Object & )
	{ missing_method(); return -1; }

int PythonExtensionBase::sequence_ass_slice( int, int, const Py::Object & )
	{ missing_method(); return -1; }


	// Mapping
int PythonExtensionBase::mapping_length()
	{ missing_method(); return -1; }

Py::Object PythonExtensionBase::mapping_subscript( const Py::Object & )
	{ missing_method(); return Py::Nothing(); }

int PythonExtensionBase::mapping_ass_subscript( const Py::Object &, const Py::Object & )
	{ missing_method(); return -1; }


	// Number
int PythonExtensionBase::number_nonzero()
	{ missing_method(); return -1; }

Py::Object PythonExtensionBase::number_negative()
	{ missing_method(); return Py::Nothing(); }

Py::Object PythonExtensionBase::number_positive()
	{ missing_method(); return Py::Nothing(); }

Py::Object PythonExtensionBase::number_absolute()
	{ missing_method(); return Py::Nothing(); }

Py::Object PythonExtensionBase::number_invert()
	{ missing_method(); return Py::Nothing(); }

Py::Object PythonExtensionBase::number_int()
	{ missing_method(); return Py::Nothing(); }

Py::Object PythonExtensionBase::number_float()
	{ missing_method(); return Py::Nothing(); }

Py::Object PythonExtensionBase::number_long()
	{ missing_method(); return Py::Nothing(); }

Py::Object PythonExtensionBase::number_oct()
	{ missing_method(); return Py::Nothing(); }

Py::Object PythonExtensionBase::number_hex()
	{ missing_method(); return Py::Nothing(); }

Py::Object PythonExtensionBase::number_add( const Py::Object & )
	{ missing_method(); return Py::Nothing(); }

Py::Object PythonExtensionBase::number_subtract( const Py::Object & )
	{ missing_method(); return Py::Nothing(); }

Py::Object PythonExtensionBase::number_multiply( const Py::Object & )
	{ missing_method(); return Py::Nothing(); }

Py::Object PythonExtensionBase::number_divide( const Py::Object & )
	{ missing_method(); return Py::Nothing(); }

Py::Object PythonExtensionBase::number_remainder( const Py::Object & )
	{ missing_method(); return Py::Nothing(); }

Py::Object PythonExtensionBase::number_divmod( const Py::Object & )
	{ missing_method(); return Py::Nothing(); }

Py::Object PythonExtensionBase::number_lshift( const Py::Object & )
	{ missing_method(); return Py::Nothing(); }

Py::Object PythonExtensionBase::number_rshift( const Py::Object & )
	{ missing_method(); return Py::Nothing(); }

Py::Object PythonExtensionBase::number_and( const Py::Object & )
	{ missing_method(); return Py::Nothing(); }

Py::Object PythonExtensionBase::number_xor( const Py::Object & )
	{ missing_method(); return Py::Nothing(); }

Py::Object PythonExtensionBase::number_or( const Py::Object & )
	{ missing_method(); return Py::Nothing(); }

Py::Object PythonExtensionBase::number_power( const Py::Object &, const Py::Object & )
	{ missing_method(); return Py::Nothing(); }


	// Buffer
int PythonExtensionBase::buffer_getreadbuffer( int, void** )
	{ missing_method(); return -1; }

int PythonExtensionBase::buffer_getwritebuffer( int, void** )
	{ missing_method(); return -1; }

int PythonExtensionBase::buffer_getsegcount( int* )
	{ missing_method(); return -1; }

//--------------------------------------------------------------------------------
//
//	Method call handlers for
//		PythonExtensionBase
//		ExtensionModuleBase
//
//--------------------------------------------------------------------------------

}
