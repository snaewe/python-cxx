import sys
sys.path.insert( 0, 'pyds%d%d' % (sys.version_info[0], sys.version_info[1]) )

import example
example.test()
