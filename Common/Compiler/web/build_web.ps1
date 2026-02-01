# Script de compilación para Finexa Common (Web)
# Utiliza Emscripten y el deploy de DcViewModels

$PSScriptRoot = Split-Path -Parent $MyInvocation.MyCommand.Definition
Set-Location $PSScriptRoot

# 1. Rutas de Dependencias (Convertidas a absolutas para evitar problemas con emcc)
$EMSDK_PATH = Resolve-Path "c:\Users\dandr\Desarrollo\LibreriasExternas\.emsdk"
$DC_VM_DEPLOY = Resolve-Path "c:\Users\dandr\Desarrollo\Deploys\DcViewModels\web"

# 2. Configurar entorno Emscripten
if (Test-Path "$EMSDK_PATH\emsdk_env.ps1") {
    . "$EMSDK_PATH\emsdk_env.ps1"
} elseif (Test-Path "$EMSDK_PATH\emsdk_env.bat") {
    & cmd /c "$EMSDK_PATH\emsdk_env.bat && set" | ForEach-Object {
        if ($_ -match "=") {
            $v = $_.Split("=", 2)
            if ($v[0] -ne "") { [System.Environment]::SetEnvironmentVariable($v[0], $v[1]) }
        }
    }
}

# 3. Compilar Librería Estática (FinexaCommon)
if (-not (Test-Path "build")) { New-Item -ItemType Directory -Path "build" }
Set-Location build

Write-Host "Building FinexaCommon Static Library..."
$generator = "NMake Makefiles"
& emcmake cmake ../../../ -G "$generator" -DCMAKE_BUILD_TYPE=Release
& cmake --build .

# Verificar si se generó la librería
if (-not (Test-Path "libFinexaCommon.a")) {
    Write-Error "libFinexaCommon.a no fue generada."
    exit 1
}

# 4. Linkear con DcViewModels (WASM)
Write-Host "Linking Finexa Common Web Assembly..."

$DC_VIEWMODELS_LIB = "$DC_VM_DEPLOY\lib\libDcViewModels.a"

& emcc ../src/Bindings.cpp `
    libFinexaCommon.a `
    "$DC_VIEWMODELS_LIB" `
    -I../include `
    -I"$DC_VM_DEPLOY\include" `
    -I"$PSScriptRoot\..\..\Core\include" `
    -I"$PSScriptRoot\..\..\ViewModels\include" `
    -std=c++17 `
    -lembind `
    -O3 `
    -o finexa_common.js `
    -s MODULARIZE=1 `
    -s EXPORT_NAME='createFinexaCommon' `
    -s WASM=1 `
    -s ALLOW_MEMORY_GROWTH=1 `
    -s ENVIRONMENT=web

Write-Host "Build completado: build/finexa_common.js"
