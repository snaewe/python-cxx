import os, sys
from distutils.core import setup, Extension

support_dir = os.path.normpath(
                   os.path.join(
			sys.prefix,
			'etc',
			'python%d.%d' % (sys.version_info[0],sys.version_info[1]),
			'CXX') )

from distutils import sysconfig
save_init_posix = sysconfig._init_posix
def my_init_posix():
	print 'my_init_posix: changing gcc to g++'
	save_init_posix()
	g = sysconfig._config_vars
	g['CC'] = 'g++ -fPIC'
	g['LDSHARED'] = 'g++ -shared'


if os.name == 'posix':
	print 'Plugging in C++ patch to distutils'
	sysconfig._init_posix = my_init_posix

setup (name = "CXXDemo",
       version = "5.1",
       maintainer = "Barry Scott",
       maintainer_email = "barry-scott@users.sourceforge.net",
       description = "Demo of facility for extending Python with C++",
       url = "http://cxx.sourceforge.net",
       
       packages = ['CXX'],
       package_dir = {'CXX': '.'},
       ext_modules = [
         Extension('CXX.example',
                   sources = ['example.cxx',
                         'range.cxx',
                         'rangetest.cxx',
                         os.path.join(support_dir,'cxxsupport.cxx'),
                         os.path.join(support_dir,'cxx_extensions.cxx'),
                         os.path.join(support_dir,'cxxextensions.c'),
                         ],
            )
       ]
)
