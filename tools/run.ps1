# run.ps1
$projectRoot = Resolve-Path "$PSScriptRoot/.."
$exePath = "$projectRoot/build/bin/modelViewer.exe"

if (-Not (Test-Path $exePath)) {
    Write-Host "Executable not found. Building runtime first..."
    & "$projectRoot/scripts/buildRuntime.ps1"
}

Write-Host "Running runtime..."
& $exePath
