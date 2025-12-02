$projectRoot = Resolve-Path "$PSScriptRoot/.."
$buildDir = "$projectRoot/build"

# Clear previous build artifacts
Remove-Item "$buildDir" -Recurse -Force -ErrorAction SilentlyContinue

$toolchain = "-DCMAKE_TOOLCHAIN_FILE=`"$projectRoot/cmake/clang-mingw-toolchain.cmake`""

# Configure
cmake -S "$projectRoot" -B $buildDir -DCMAKE_BUILD_TYPE=Release $toolchain

# Build
cmake --build "$buildDir" --config Release