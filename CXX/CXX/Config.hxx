#ifndef __PyCXX_config_hh__
#define __PyCXX_config_hh__

// Macros to deal with deficiencies in compilers
#define STANDARD_LIBRARY_HAS_ITERATOR_TRAITS 1

//
// Microsoft VC++ defines _WIN32
// older versions of this header used MS_WIN32
//
#if defined( _WIN32 ) || defined( MS_WIN32 )
#undef STANDARD_LIBRARY_HAS_ITERATOR_TRAITS
#define STANDARD_LIBRARY_HAS_ITERATOR_TRAITS 0
#endif

#if STANDARD_LIBRARY_HAS_ITERATOR_TRAITS
#define random_access_iterator_parent(itemtype) std::random_access_iterator<itemtype, int>
#else
#define random_access_iterator_parent(itemtype) std::iterator<std::random_access_iterator_tag,itemtype,int>
#endif

#endif //  __PyCXX_config_hh__
