# Resolve project root and build directory
$projectRoot = Resolve-Path "$PSScriptRoot/.."
$buildDir = "$projectRoot/build"

# Ensure build directory exists
if (-Not (Test-Path $buildDir)) {
    Write-Host "Build directory not found. Creating and configuring..."
    New-Item -ItemType Directory -Force -Path $buildDir | Out-Null

    # Configure CMake
    cmake -S $projectRoot -B $buildDir -DCMAKE_BUILD_TYPE=Release
}

# Build the 'docs' target
Write-Host "Building Doxygen docs..."
cmake --build $buildDir --target project_docs --config Release

Write-Host "Documentation build complete. Check $buildDir/docs or your output folder."