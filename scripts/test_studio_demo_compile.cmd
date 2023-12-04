rem Initialize the developer environment just like a developer box. Note that 'call' keyword that ensures that the script does not exist after 
rem calling the other batch file.
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\Common7\Tools\VsDevCmd.bat" -arch=amd64 -host_arch=amd64 -winsdk=10.0.16299.0

rem Set Python path
SET PATH=%PATH%;"C:\Program Files\Python36";"C:\Program Files\Python36\scripts"

rem Save working directory so that we can restore it back after building everything. This will make developers happy and then 
rem switch to the folder this script resides in. Don't assume absolute paths because on the build host and on the dev host the locations may be different.
pushd "%~dp0"

cd ..\test\guix_studio_test\test_demo\demo_compile
cmake .
ctest -j8 -T test --no-compress-output  --test-output-size-passed 4096 --test-output-size-failed 4096 -O studio_test_demo_compile_report.txt --output-junit studio_test_demo_compile_report.xml

rem Save exit code for dotnet build.
set EX=%ERRORLEVEL%

rem Exit with explicit 0 code so that build does not fail.
exit /B %EX%
