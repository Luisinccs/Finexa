$PSScriptRoot = Split-Path -Parent $MyInvocation.MyCommand.Definition
Set-Location $PSScriptRoot

Write-Host "Rebuilding FinexaCommon for Web..."

if (Test-Path "build") {
    Remove-Item -Recurse -Force "build"
}

.\build_web.ps1
