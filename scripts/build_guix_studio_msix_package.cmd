@echo off
setlocal

rem Initialize the Visual Studio 2022 developer environment.
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if exist "%VSWHERE%" (
    for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.Component.MSBuild -property installationPath`) do set "VSINSTALL=%%i"
)

if not defined VSINSTALL (
    echo Visual Studio 2022 with MSBuild was not found.
    exit /B 1
)

call "%VSINSTALL%\Common7\Tools\VsDevCmd.bat" -arch=amd64 -host_arch=amd64
if ERRORLEVEL 1 exit /B %ERRORLEVEL%

rem Save working directory so that we can restore it back after building everything. This will make developers happy and then 
rem switch to the folder this script resides in. Don't assume absolute paths because on the build host and on the dev host the locations may be different.
pushd "%~dp0"

cd ..\guix_studio\build\vs_2022
msbuild msix_package_project\msix_package_project.sln /p:Configuration=Release /p:UapAppxPackageBuildMode=CI /p:GenerateAppInstallerFile=false /p:Platform=x86

set "BUILD_STATUS=%ERRORLEVEL%"
popd
exit /B %BUILD_STATUS%
