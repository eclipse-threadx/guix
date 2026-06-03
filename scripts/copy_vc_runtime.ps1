# Copyright (c) 2026 Eclipse ThreadX contributors
#
# This program and the accompanying materials are made available under the
# terms of the MIT License which is available at
# https://opensource.org/licenses/MIT.
#
# SPDX-License-Identifier: MIT

[CmdletBinding()]
param(
    [string]$Architecture = "x86",
    [string]$Destination = ""
)

$ErrorActionPreference = "Stop"

function Find-VisualStudioInstallPath {
    $vswherePath = Join-Path ${env:ProgramFiles(x86)} "Microsoft Visual Studio\Installer\vswhere.exe"
    if (Test-Path -LiteralPath $vswherePath) {
        $installPath = & $vswherePath -latest -products * -requires Microsoft.VisualStudio.Component.VC.Redist.14.Latest -property installationPath
        if ($LASTEXITCODE -eq 0 -and $installPath -and (Test-Path -LiteralPath $installPath)) {
            return $installPath.Trim()
        }
    }

    $candidates = @(
        (Join-Path $env:ProgramFiles "Microsoft Visual Studio\2022\Enterprise"),
        (Join-Path $env:ProgramFiles "Microsoft Visual Studio\2022\Professional"),
        (Join-Path $env:ProgramFiles "Microsoft Visual Studio\2022\Community"),
        (Join-Path $env:ProgramFiles "Microsoft Visual Studio\2022\BuildTools")
    )

    foreach ($candidate in $candidates) {
        if (Test-Path -LiteralPath $candidate) {
            return $candidate
        }
    }

    throw "Could not find a Visual Studio 2022 installation with VC++ redistributable files."
}

function Find-VcRedistPath {
    param(
        [Parameter(Mandatory = $true)]
        [string]$Architecture
    )

    $roots = @()
    if ($env:VCToolsRedistDir -and (Test-Path -LiteralPath $env:VCToolsRedistDir)) {
        $roots += $env:VCToolsRedistDir
    }

    $installPath = Find-VisualStudioInstallPath
    $roots += Join-Path $installPath "VC\Redist\MSVC"

    foreach ($root in ($roots | Select-Object -Unique)) {
        if (-not (Test-Path -LiteralPath $root)) {
            continue
        }

        $directCandidate = Join-Path $root $Architecture
        if ((Test-Path -LiteralPath (Join-Path $directCandidate "Microsoft.VC143.CRT")) -and
            (Test-Path -LiteralPath (Join-Path $directCandidate "Microsoft.VC143.MFC"))) {
            return $directCandidate
        }

        $versions = Get-ChildItem -LiteralPath $root -Directory | Sort-Object Name -Descending
        foreach ($version in $versions) {
            $candidate = Join-Path $version.FullName $Architecture
            if ((Test-Path -LiteralPath (Join-Path $candidate "Microsoft.VC143.CRT")) -and
                (Test-Path -LiteralPath (Join-Path $candidate "Microsoft.VC143.MFC"))) {
                return $candidate
            }
        }
    }

    throw "Could not find app-local VC143 runtime files for $Architecture."
}

$scriptDirectory = Split-Path -Parent $MyInvocation.MyCommand.Path
if (-not $Destination) {
    $Destination = Join-Path $scriptDirectory "..\guix_studio\installer\vc_runtime\$Architecture"
}

$destinationPath = [System.IO.Path]::GetFullPath($Destination)
$redistPath = Find-VcRedistPath -Architecture $Architecture
$redistSets = @("Microsoft.VC143.CRT", "Microsoft.VC143.MFC", "Microsoft.VC143.MFCLOC")

if (Test-Path -LiteralPath $destinationPath) {
    Remove-Item -LiteralPath $destinationPath -Recurse -Force
}

New-Item -ItemType Directory -Path $destinationPath | Out-Null

foreach ($setName in $redistSets) {
    $sourceDirectory = Join-Path $redistPath $setName
    if (-not (Test-Path -LiteralPath $sourceDirectory)) {
        continue
    }

    Get-ChildItem -LiteralPath $sourceDirectory -Filter "*.dll" -File |
        Copy-Item -Destination $destinationPath -Force
}

Write-Host "Copied VC++ runtime files from $redistPath to $destinationPath"
