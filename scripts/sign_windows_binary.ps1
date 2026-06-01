# Copyright (c) 2026 Eclipse ThreadX contributors
#
# This program and the accompanying materials are made available under the
# terms of the MIT License which is available at
# https://opensource.org/licenses/MIT.
#
# SPDX-License-Identifier: MIT

[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [string]$Path,

    [Parameter(Mandatory = $true)]
    [string]$CertificatePath,

    [string]$CertificatePassword = $env:WINDOWS_SIGNING_PFX_PASSWORD,
    [string]$Description = "GUIX Studio",
    [string]$TimestampUrl = "http://timestamp.digicert.com",
    [switch]$SkipVerify
)

$ErrorActionPreference = "Stop"

Import-Module "$PSScriptRoot\guix_studio_release.psm1" -Force

$resolvedPath = Resolve-Path -LiteralPath $Path
$resolvedCertificatePath = Resolve-Path -LiteralPath $CertificatePath
$signtool = Find-WindowsSignTool

$signArgs = @(
    'sign',
    '/fd', 'SHA256',
    '/tr', $TimestampUrl,
    '/td', 'SHA256',
    '/f', $resolvedCertificatePath.Path,
    '/d', $Description
)

if ($CertificatePassword) {
    $signArgs += @('/p', $CertificatePassword)
}

$signArgs += $resolvedPath.Path

& $signtool @signArgs
if ($LASTEXITCODE -ne 0) {
    throw "signtool sign failed for $($resolvedPath.Path)."
}

if (-not $SkipVerify) {
    & $signtool verify /pa /v $resolvedPath.Path
    if ($LASTEXITCODE -ne 0) {
        throw "signtool verify failed for $($resolvedPath.Path)."
    }
}
