@rem Copyright (c) 2024 Microsoft Corporation
@rem Copyright (c) 2026 Eclipse ThreadX contributors
@rem
@rem This program and the accompanying materials are made available under the
@rem terms of the MIT License which is available at
@rem https://opensource.org/licenses/MIT.
@rem
@rem SPDX-License-Identifier: MIT

@echo off
setlocal EnableDelayedExpansion
setlocal EnableExtensions
set SCRIPT=%~1
shift /1
set ARGS=

:collect_args
if "%~1"=="" goto run_script
set ARGS=!ARGS! "%~1"
shift /1
goto collect_args

:run_script
"%SystemRoot%\System32\WindowsPowerShell\v1.0\powershell.exe" -ExecutionPolicy Bypass -NoLogo -NonInteractive -NoProfile -WindowStyle Hidden -File "%SCRIPT%" !ARGS!
exit /B %ERRORLEVEL%
