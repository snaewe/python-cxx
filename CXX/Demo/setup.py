import os, sys
from distutils.core import setup
support_dir = os.path.join(sys.prefix, "etc", "CXX")
setup (name = "CXXDemo",
       version = "5.0",
       maintainer = "Paul Dubois",
       maintainer_email = "dubois@users.sourceforge.net",
       description = "Demo of facility for extending Python with C++",
       url = "http://cxx.sourceforge.net",
       
       packages = ['CXX'],
       package_dir = {'CXX': '.'},
       ext_modules = [
         ('CXX.example',
            {'sources' : ['example.cxx',
                         'r.cxx',
                         'rtest.cxx',
                         os.path.join(support_dir,'cxxsupport.cxx'),
                         os.path.join(support_dir,'cxx_extensions.cxx'),
                         os.path.join(support_dir,'cxxextensions.c'),
                         ],
            }
         )
       ]
)
