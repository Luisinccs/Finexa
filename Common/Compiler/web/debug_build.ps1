$PSScriptRoot = Split-Path -Parent $MyInvocation.MyCommand.Definition
Set-Location "$PSScriptRoot/build"

# Source EMSDK
$EMSDK_PATH = "../../../../LibreriasExternas/.emsdk"
if (Test-Path "$EMSDK_PATH/emsdk_env.ps1") { . "$EMSDK_PATH/emsdk_env.ps1" }

if (-not (Get-Command nmake -ErrorAction SilentlyContinue)) {
    Write-Host "NMake not found in PATH"
} else {
    Write-Host "NMake found at $(Get-Command nmake | Select-Object -ExpandProperty Source)"
}

# Run NMake and redirect to file
& nmake 2>&1 | Out-File -FilePath nmake_log.txt
Get-Content nmake_log.txt
