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
    [string]$Version,

    [string]$InstallerPath = "",
    [switch]$CheckSignature,
    [switch]$WriteSha256
)

$ErrorActionPreference = "Stop"

Import-Module "$PSScriptRoot\guix_studio_release.psm1" -Force

$release = Get-StudioReleaseVersion -Version $Version
$repoRoot = [System.IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..'))
$appPath = Join-Path $repoRoot 'guix_studio\build\vs_2022\Release\guix_studio.exe'
if ($InstallerPath) {
    $installerPath = [System.IO.Path]::GetFullPath($InstallerPath)
}
else {
    $installerPath = Join-Path $repoRoot "guix_studio\installer\output\guix_studio_setup_version_$($release.FullVersion).exe"
}

foreach ($path in @($appPath, $installerPath)) {
    if (-not (Test-Path -LiteralPath $path)) {
        throw "Missing expected file: $path"
    }
}

foreach ($path in @($appPath, $installerPath)) {
    $versionInfo = (Get-Item -LiteralPath $path).VersionInfo
    if ($versionInfo.FileVersion.Trim() -ne $release.FullVersion) {
        throw "$path has FileVersion '$($versionInfo.FileVersion)' instead of '$($release.FullVersion)'."
    }
    if ($versionInfo.ProductVersion.Trim() -ne $release.FullVersion) {
        throw "$path has ProductVersion '$($versionInfo.ProductVersion)' instead of '$($release.FullVersion)'."
    }
}

if ($CheckSignature) {
    $signtool = Find-WindowsSignTool
    foreach ($path in @($appPath, $installerPath)) {
        & $signtool verify /pa /v $path
        if ($LASTEXITCODE -ne 0) {
            throw "Signature verification failed for $path."
        }
    }
}

if ($WriteSha256) {
    $hash = Get-FileHash -LiteralPath $installerPath -Algorithm SHA256
    $checksumPath = "$installerPath.sha256"
    "$($hash.Hash) *$(Split-Path -Leaf $installerPath)" | Set-Content -LiteralPath $checksumPath -Encoding ASCII
    Write-Host "Wrote $checksumPath"
}

Write-Host "Verified GUIX Studio installer $($release.FullVersion)."
