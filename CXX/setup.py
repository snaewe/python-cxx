#!/usr/bin/env python

import os
from glob import glob
from distutils.core import setup

setup (name = "CXX_example",
       version = "4.0",
       maintainer = "Paul Dubois",
       maintainer_email = "dubois@users.sourceforge.net",
       description = "Facility for extending Python with C++",
       url = "http://cxx.sourceforge.net",

       packages = ['CXX'],
       package_dir = {'CXX': ''},
       include_dirs = ['Include'],
       ext_modules = [('example',
                       { 'sources' : ['Demo/example.cxx',
                                      'Demo/r.cxx',
                                      'Demo/rtest.cxx',
				      'Src/cxxsupport.cxx',
                                      'Src/cxx_extensions.cxx',
                                      'Src/cxxextensions.c'],
                       }
                      ),
                     ]
       )
