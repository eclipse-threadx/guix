# Copyright (c) 2026 Eclipse ThreadX contributors
#
# This program and the accompanying materials are made available under the
# terms of the MIT License which is available at
# https://opensource.org/licenses/MIT.
#
# SPDX-License-Identifier: MIT

[CmdletBinding()]
param(
    [string]$Version = "",
    [switch]$SignInstaller,
    [string]$CertificatePath = $env:WINDOWS_SIGNING_PFX_PATH,
    [string]$OutputBaseFilename = "",
    [string]$InnoCompiler = "C:\Program Files (x86)\Inno Setup 6\ISCC.exe"
)

$ErrorActionPreference = "Stop"

Import-Module "$PSScriptRoot\guix_studio_release.psm1" -Force

if (-not (Test-Path -LiteralPath $InnoCompiler)) {
    throw "Could not find Inno Setup compiler: $InnoCompiler"
}

& "$PSScriptRoot\copy_vc_runtime.ps1"

$isccArgs = @('/Q')

if ($Version) {
    $release = Get-StudioReleaseVersion -Version $Version
    $isccArgs += @(
        "/DStudioFullVersion=`"$($release.FullVersion)`"",
        "/DStudioShortVersion=`"$($release.ShortVersion)`"",
        "/DStudioMajorMinorVersion=`"$($release.MajorMinorVersion)`"",
        "/DStudioVersionInfoVersion=`"$($release.Win32Version)`""
    )
}

if ($OutputBaseFilename) {
    $isccArgs += "/F$OutputBaseFilename"
}

if ($SignInstaller) {
    if (-not $CertificatePath) {
        throw "CertificatePath or WINDOWS_SIGNING_PFX_PATH is required when SignInstaller is set."
    }

    $resolvedCertificatePath = Resolve-Path -LiteralPath $CertificatePath
    $signScript = Join-Path $PSScriptRoot 'sign_windows_binary.ps1'
    $signCommand = "powershell.exe -ExecutionPolicy Bypass -NoLogo -NonInteractive -NoProfile -File `$q$signScript`$q -Path `$q`$f`$q -CertificatePath `$q$($resolvedCertificatePath.Path)`$q -Description `$qGUIX Studio Setup`$q"
    $isccArgs += @('/DSIGN_INSTALLER', "/Sstudio=$signCommand")
}

$isccArgs += 'guix_installer_release.iss'

Push-Location (Join-Path $PSScriptRoot '..\guix_studio\installer')
try {
    & $InnoCompiler @isccArgs
    if ($LASTEXITCODE -ne 0) {
        throw "ISCC failed with exit code $LASTEXITCODE."
    }
}
finally {
    Pop-Location
}
