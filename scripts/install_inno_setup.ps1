# Copyright (c) 2024 Microsoft Corporation
# Copyright (c) 2026 Eclipse ThreadX contributors
#
# This program and the accompanying materials are made available under the
# terms of the MIT License which is available at
# https://opensource.org/licenses/MIT.
#
# SPDX-License-Identifier: MIT

$ErrorActionPreference = "Stop"

if (Test-Path "$PSScriptRoot\win-installer-helper.psm1") 
{
    Import-Module "$PSScriptRoot\win-installer-helper.psm1"
} 
elseif (Test-Path "C:\win-installer-helper.psm1") 
{
   Import-Module "C:\win-installer-helper.psm1"
}

[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12 

Start-Setup

try	{


    Get-File -Url "https://jrsoftware.org/download.php/is.exe" -fileName "innosetup.exe"
    Install-FromExe -Path "C:\Downloads\innosetup.exe" -Arguments "/VERYSILENT /SUPPRESSMSGBOXES /NORESTART"
}
catch
{
    $_.Exception | Format-List 
    exit -1
}
finally 
{
    Stop-Setup
}
