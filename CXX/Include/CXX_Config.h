#ifndef __py_config_hh__
#define __py_config_hh__
// Macros to deal with deficiencies in compilers
#define COMPILER_SUPPORTS_NAMESPACES 1
#define STANDARD_LIBRARY_HAS_ITERATOR_TRAITS 1
#ifdef MS_WIN32
#undef STANDARD_LIBRARY_HAS_ITERATOR_TRAITS
#define STANDARD_LIBRARY_HAS_ITERATOR_TRAITS 0
#endif

#if COMPILER_SUPPORTS_NAMESPACES
#define NAMESPACE_BEGIN(name) namespace name {
#define NAMESPACE_END }
#define STD std
#define USING(phrase) using phrase;
#define STANDARD_HEADER(name) <name>
#else
#define NAMESPACE_BEGIN(name)
#define NAMESPACE_END
#define STD
#define USING(phrase) 
#define CXX_QUOTE(thing) #thing
#define STANDARD_HEADER(name) CXX_QUOTE(name.h)
#endif

#if STANDARD_LIBRARY_HAS_ITERATOR_TRAITS
#define random_access_iterator_parent(itemtype) STD::random_access_iterator<itemtype, int>
#else
#define random_access_iterator_parent(itemtype) STD::iterator<STD::random_access_iterator_tag,itemtype,int>
#endif

#endif                          // __py_config_hh__

