# Copyright (c) 2026 Eclipse ThreadX contributors
#
# This program and the accompanying materials are made available under the
# terms of the MIT License which is available at
# https://opensource.org/licenses/MIT.
#
# SPDX-License-Identifier: MIT

function Get-StudioReleaseVersion {
    [CmdletBinding()]
    param(
        [Parameter(Mandatory = $true)]
        [string]$Version
    )

    if ($Version -notmatch '^([0-9]+)\.([0-9]+)\.([0-9]+)\.([0-9]+)$') {
        throw "Version must use the form major.minor.patch.build, for example 6.5.1.202602."
    }

    $major = [int]$matches[1]
    $minor = [int]$matches[2]
    $patch = [int]$matches[3]
    $buildText = $matches[4]
    $build = [int]$buildText
    $revisionText = $buildText
    if ($revisionText.Length -gt 2) {
        $revisionText = $revisionText.Substring($revisionText.Length - 2)
    }
    $revisionText = $revisionText.PadLeft(2, '0')
    $revision = [int]$revisionText

    foreach ($part in @($major, $minor, $patch, $revision)) {
        if ($part -lt 0 -or $part -gt 65535) {
            throw "Version component '$part' is outside the Win32/MSIX range 0..65535."
        }
    }

    [PSCustomObject]@{
        FullVersion = $Version
        ShortVersion = "$major.$minor.$patch"
        MajorMinorVersion = "$major.$minor"
        Major = $major
        Minor = $minor
        Patch = $patch
        Build = $build
        BuildText = $buildText
        Revision = $revision
        RevisionText = $revisionText
        Win32Version = "$major.$minor.$patch.$revision"
        RcNumericVersion = "$major,$minor,$patch,$revision"
        MsixVersion = "$major.$minor.$patch.$revisionText"
    }
}

function Find-WindowsSignTool {
    [CmdletBinding()]
    param()

    $pathCommand = Get-Command signtool.exe -ErrorAction SilentlyContinue
    if ($pathCommand) {
        return $pathCommand.Source
    }

    $roots = @(
        (Join-Path ${env:ProgramFiles(x86)} 'Windows Kits\10\bin'),
        (Join-Path $env:ProgramFiles 'Windows Kits\10\bin')
    ) | Where-Object { $_ -and (Test-Path -LiteralPath $_) }

    foreach ($root in $roots) {
        $versions = Get-ChildItem -LiteralPath $root -Directory | Sort-Object Name -Descending
        foreach ($version in $versions) {
            foreach ($arch in @('x64', 'x86')) {
                $candidate = Join-Path $version.FullName "$arch\signtool.exe"
                if (Test-Path -LiteralPath $candidate) {
                    return $candidate
                }
            }
        }
    }

    throw "Could not find signtool.exe. Install the Windows SDK or run from a Visual Studio developer environment."
}

Export-ModuleMember -Function Get-StudioReleaseVersion, Find-WindowsSignTool
