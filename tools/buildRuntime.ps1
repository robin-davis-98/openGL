$projectRoot = Resolve-Path "$PSScriptRoot/.."
$buildDir = "$projectRoot/build/debug"

# Ensure build folder exists
if (!(Test-Path $buildDir)) {
    New-Item -ItemType Directory -Path $buildDir | Out-Null
}

$toolchain = "-DCMAKE_TOOLCHAIN_FILE=`"$projectRoot/cmake/clang-mingw-toolchain.cmake`""

# Configure project if not already configured
cmake -S "$projectRoot/runtime" -B $buildDir -DCMAKE_BUILD_TYPE=Debug $toolchain

# Build only runtime
cmake --build "$buildDir" --target modelViewer --config Debug
