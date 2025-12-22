# Resolve project root
$projectRoot = Resolve-Path "$PSScriptRoot/.."

# Default settings
$defaultChoice = 1

# Helper: clear/create build directory
function Prepare-BuildDir {
    param($dir)
    if (Test-Path $dir) {
        Remove-Item $dir -Recurse -Force -ErrorAction SilentlyContinue
    }
    New-Item -ItemType Directory -Path $dir | Out-Null
}

# Present menu
Write-Host "======================"
Write-Host "OpenGL Model Viewer Build"
Write-Host "======================"
Write-Host "Select what to build:"
Write-Host "1) Debug - Build All (default)"
Write-Host "2) Debug - Build Engine"
Write-Host "3) Debug - Build Runtime"
Write-Host "4) Debug - Build Docs"
Write-Host "5) Release - Build All"

$choice = Read-Host "Enter choice [1-5]"
if ([string]::IsNullOrEmpty($choice)) { $choice = $defaultChoice }

# Optional: toolchain
$toolchain = "-DCMAKE_TOOLCHAIN_FILE=`"$projectRoot/cmake/clang-mingw-toolchain.cmake`""

switch ($choice) {
    "1" {
        $buildType = "Debug"
        $target = "all"
    }
    "2" {
        $buildType = "Debug"
        $target = "engine"
    }
    "3" {
        $buildType = "Debug"
        $target = "modelViewer"
    }
    "4" {
        $buildType = "Debug"
        $target = "project_docs"
    }
    "5" {
        $buildType = "Release"
        $target = "all"
    }
    default {
        Write-Host "Invalid choice. Using default: Debug - Build All"
        $buildType = "Debug"
        $target = "all"
    }
}

# Prepare build directory
$buildDir = "$projectRoot/build/$($buildType.ToLower())"
Prepare-BuildDir $buildDir

# Configure CMake
Write-Host "`nConfiguring project ($buildType)..."
cmake -S "$projectRoot" -B "$buildDir" -G "Ninja" -DCMAKE_BUILD_TYPE="$buildType" $toolchain

# Build target
Write-Host "`nBuilding target: $target..."
cmake --build "$buildDir" --config "$buildType" --target "$target"

Write-Host "`nBuild finished: $buildType / $target"
