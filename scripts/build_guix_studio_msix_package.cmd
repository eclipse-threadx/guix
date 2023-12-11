rem Initialize the developer environment just like a developer box. Note that 'call' keyword that ensures that the script does not exist after 
rem calling the other batch file.
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\Common7\Tools\VsDevCmd.bat" -arch=amd64 -host_arch=amd64 -winsdk=10.0.16299.0

rem Save working directory so that we can restore it back after building everything. This will make developers happy and then 
rem switch to the folder this script resides in. Don't assume absolute paths because on the build host and on the dev host the locations may be different.
pushd "%~dp0"

cd ..\guix_studio\build\vs_2019
msbuild msix_package_project\msix_package_project.sln /p:Configuration=Release /p:UapAppxPackageBuildMode=CI /p:GenerateAppInstallerFile=false /p:Platform=x86

exit /B %ERRORLEVEL%
