@rem Copyright (c) 2026 Eclipse ThreadX contributors
@rem
@rem This program and the accompanying materials are made available under the
@rem terms of the MIT License which is available at
@rem https://opensource.org/licenses/MIT.
@rem
@rem SPDX-License-Identifier: MIT

@echo off

rem Save working directory so that we can restore it back after copying the runtime files.
pushd "%~dp0"

powershell.exe -ExecutionPolicy Bypass -NoLogo -NonInteractive -NoProfile -File copy_vc_runtime.ps1

exit /B %ERRORLEVEL%
