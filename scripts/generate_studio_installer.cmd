@rem Copyright (c) 2024 Microsoft Corporation
@rem Copyright (c) 2026 Eclipse ThreadX contributors
@rem
@rem This program and the accompanying materials are made available under the
@rem terms of the MIT License which is available at
@rem https://opensource.org/licenses/MIT.
@rem
@rem SPDX-License-Identifier: MIT

rem Save working directory so that we can restore it back after building everything. This will make developers happy and then 
rem switch to the folder this script resides in. Don't assume absolute paths because on the build host and on the dev host the locations may be different.
pushd "%~dp0"

cd ..\guix_studio\installer

rem generate studio installer
"C:\Program Files (x86)\Inno Setup 6\ISCC.exe" /Q "guix_installer_release.iss"

exit /B %ERRORLEVEL%