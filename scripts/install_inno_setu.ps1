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


    Get-File -Url "http://www.jrsoftware.org/download.php/is.exe" -fileName "innosetup-6.0.3.exe"
    Install-FromExe -Path "C:\Downloads\innosetup-6.0.3.exe" -Arguments "/VERYSILENT /SUPPRESSMSGBOXES"
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
