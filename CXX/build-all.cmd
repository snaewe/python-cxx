setlocal
set BUILD_TYPE=Release
set ACTION=
if "%1" == "/debug" set BUILD_TYPE=Debug
if "%2" == "/debug" set BUILD_TYPE=Debug
if "%1" == "/clean" set ACTION=/clean
if "%2" == "/clean" set ACTION=/clean

setlocal
if "%ACTION%" == "" set ACTION=/build
call setup-msvc90
echo on
rmdir /s /q Release31
rmdir /s /q pyds31
vcexpress example_py31.sln %ACTION% "%BUILD_TYPE%"  /project "example"
copy Release31\BuildLog.htm Release31\BuildLog_example.htm
vcexpress example_py31.sln %ACTION% "%BUILD_TYPE%"  /project "pycxx_iter"
copy Release31\BuildLog.htm Release31\BuildLog_pycxx_iter.htm
endlocal

endlocal
