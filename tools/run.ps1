$projectRoot = Resolve-Path "$PSScriptRoot/.."
$exePath = "$projectRoot/build/debug/bin/modelViewer.exe"

if (Test-Path $exePath) {
    & $exePath
} else {
    Write-Host "Executable not found. Please run buildAll or buildRuntime first."
}
