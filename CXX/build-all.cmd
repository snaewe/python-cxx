setlocal
setup-msvc90
c:\python26\python setup_makefile.py win32 win32.mak
nmake -f win32.mak clean all

c:\python31\python setup_makefile.py win32 win32.mak
nmake -f win32.mak clean all

endlocal
