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

    [string]$RepoRoot = ""
)

$ErrorActionPreference = "Stop"

Import-Module "$PSScriptRoot\guix_studio_release.psm1" -Force

function Set-AsciiFile {
    param(
        [Parameter(Mandatory = $true)]
        [string]$Path,

        [Parameter(Mandatory = $true)]
        [string]$Content
    )

    [System.IO.File]::WriteAllText($Path, $Content, [System.Text.Encoding]::ASCII)
}

function Replace-Required {
    param(
        [Parameter(Mandatory = $true)]
        [string]$Content,

        [Parameter(Mandatory = $true)]
        [string]$Pattern,

        [Parameter(Mandatory = $true)]
        [string]$Replacement,

        [Parameter(Mandatory = $true)]
        [string]$Description
    )

    if (-not [regex]::IsMatch($Content, $Pattern)) {
        throw "Could not update $Description."
    }

    return [regex]::Replace($Content, $Pattern, $Replacement)
}

if (-not $RepoRoot) {
    $RepoRoot = [System.IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..'))
}

$release = Get-StudioReleaseVersion -Version $Version

$gxApi = Join-Path $RepoRoot 'common\inc\gx_api.h'
$studioProject = Join-Path $RepoRoot 'guix_studio\StudioXProject.h'
$studioRc = Join-Path $RepoRoot 'guix_studio\studiox.rc'
$appxManifest = Join-Path $RepoRoot 'guix_studio\build\vs_2022\msix_package_project\Package.appxmanifest'
$innoScript = Join-Path $RepoRoot 'guix_studio\installer\guix_installer_release.iss'

$content = Get-Content -LiteralPath $gxApi -Raw
$content = Replace-Required $content '(?m)^(/\*\s+)\d+\.\d+\.\d+(\s+\*/)' "`${1}$($release.ShortVersion)`${2}" 'gx_api.h banner version'
$content = Replace-Required $content '(?m)^#define GUIX_MAJOR_VERSION\s+\d+' "#define GUIX_MAJOR_VERSION  $($release.Major)" 'GUIX_MAJOR_VERSION'
$content = Replace-Required $content '(?m)^#define GUIX_MINOR_VERSION\s+\d+' "#define GUIX_MINOR_VERSION  $($release.Minor)" 'GUIX_MINOR_VERSION'
$content = Replace-Required $content '(?m)^#define GUIX_PATCH_VERSION\s+\d+' "#define GUIX_PATCH_VERSION  $($release.Patch)" 'GUIX_PATCH_VERSION'
$content = Replace-Required $content '(?m)^#define GUIX_BUILD_VERSION\s+\d+' "#define GUIX_BUILD_VERSION  $($release.Build)" 'GUIX_BUILD_VERSION'
$content = Replace-Required $content "(?m)^#define GUIX_HOTFIX_VERSION\s+'.*'" "#define GUIX_HOTFIX_VERSION ' '" 'GUIX_HOTFIX_VERSION'
Set-AsciiFile -Path $gxApi -Content $content

$content = Get-Content -LiteralPath $studioProject -Raw
$content = Replace-Required $content '(?m)^#define STUDIOX_VERSION_NUMBER\s+\d+' "#define STUDIOX_VERSION_NUMBER $($release.Build)" 'STUDIOX_VERSION_NUMBER'
Set-AsciiFile -Path $studioProject -Content $content

$content = Get-Content -LiteralPath $studioRc -Raw
$content = Replace-Required $content '(?m)^ FILEVERSION\s+\d+,\d+,\d+,\d+' " FILEVERSION $($release.RcNumericVersion)" 'studiox.rc FILEVERSION'
$content = Replace-Required $content '(?m)^ PRODUCTVERSION\s+\d+,\d+,\d+,\d+' " PRODUCTVERSION $($release.RcNumericVersion)" 'studiox.rc PRODUCTVERSION'
$content = Replace-Required $content 'VALUE "FileVersion", "[^"]+"' "VALUE `"FileVersion`", `"$($release.FullVersion)`"" 'studiox.rc FileVersion string'
$content = Replace-Required $content 'VALUE "ProductVersion", "[^"]+"' "VALUE `"ProductVersion`", `"$($release.FullVersion)`"" 'studiox.rc ProductVersion string'
Set-AsciiFile -Path $studioRc -Content $content

$content = Get-Content -LiteralPath $appxManifest -Raw
$content = Replace-Required $content '(?s)(<Identity\b[^>]*\bVersion=")[^"]+(")' "`${1}$($release.MsixVersion)`${2}" 'Package.appxmanifest Identity Version'
Set-AsciiFile -Path $appxManifest -Content $content

$content = Get-Content -LiteralPath $innoScript -Raw
$content = Replace-Required $content '#define StudioFullVersion "[^"]+"' "#define StudioFullVersion `"$($release.FullVersion)`"" 'Inno StudioFullVersion'
$content = Replace-Required $content '#define StudioShortVersion "[^"]+"' "#define StudioShortVersion `"$($release.ShortVersion)`"" 'Inno StudioShortVersion'
$content = Replace-Required $content '#define StudioMajorMinorVersion "[^"]+"' "#define StudioMajorMinorVersion `"$($release.MajorMinorVersion)`"" 'Inno StudioMajorMinorVersion'
$content = Replace-Required $content '#define StudioVersionInfoVersion "[^"]+"' "#define StudioVersionInfoVersion `"$($release.Win32Version)`"" 'Inno StudioVersionInfoVersion'
Set-AsciiFile -Path $innoScript -Content $content

Write-Host "Updated GUIX Studio release version to $($release.FullVersion)."
Write-Host "Win32 version: $($release.Win32Version)"
Write-Host "MSIX version: $($release.MsixVersion)"
