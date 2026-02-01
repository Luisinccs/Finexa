# Build and Deploy script for Finexa Web Application

$PSScriptRoot = Split-Path -Parent $MyInvocation.MyCommand.Definition
Set-Location $PSScriptRoot

$DIST_DIR = "dist"
$COMMON_BUILD_DIR = "../Common/Compiler/web/build"
$DC_VIEWS_SRC = "../../DualComponents/DcViews/web/Production/dc-views.js"

# 1. Clean and Prepare Dist
if (Test-Path $DIST_DIR) {
    Remove-Item -Recurse -Force $DIST_DIR
}
New-Item -ItemType Directory -Path $DIST_DIR
New-Item -ItemType Directory -Path "$DIST_DIR/src"

Write-Host "Deploying Finexa Web App to $DIST_DIR..."

# 2. Copy Common WASM/JS artifacts
if (-not (Test-Path "$COMMON_BUILD_DIR/finexa_common.js")) {
    Write-Error "Common WASM artifacts not found. Please run Common/Compiler/web/build_web.ps1 first."
    exit 1
}
Copy-Item "$COMMON_BUILD_DIR/finexa_common.js" "$DIST_DIR/"
Copy-Item "$COMMON_BUILD_DIR/finexa_common.wasm" "$DIST_DIR/"

# 3. Copy UI Library
if (-not (Test-Path $DC_VIEWS_SRC)) {
    Write-Warning "dc-views.js not found at $DC_VIEWS_SRC. Attempting to find in sibling folder..."
    # Fallback search if path is different in user's machine
    $DC_VIEWS_SRC = Resolve-Path "../../DualComponents/DcViews/web/Production/dc-views.js" -ErrorAction SilentlyContinue
}

if ($DC_VIEWS_SRC -and (Test-Path $DC_VIEWS_SRC)) {
    Copy-Item $DC_VIEWS_SRC "$DIST_DIR/"
} else {
    Write-Error "Could not find dc-views.js. Please check paths."
    exit 1
}

# 4. Copy Application Files
Copy-Item "index.html" "$DIST_DIR/"
Copy-Item "src/*.js" "$DIST_DIR/"

Write-Host "`nSuccess! Finexa Web App is ready in $DIST_DIR"
Write-Host "To test, run:"
Write-Host "cd $DIST_DIR"
Write-Host "live-server ."
