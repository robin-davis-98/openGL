$projectRoot = Resolve-Path "$PSScriptRoot/.."
$buildDir = "$projectRoot/build/release"

# Clear previous build artifacts
Remove-Item "$buildDir" -Recurse -Force -ErrorAction SilentlyContinue

# Toolchain if needed
$toolchain = "-DCMAKE_TOOLCHAIN_FILE=`"$projectRoot/cmake/clang-mingw-toolchain.cmake`""

# Configure Release build
cmake -S "$projectRoot" -B $buildDir -DCMAKE_BUILD_TYPE=Release $toolchain

# Build Release
cmake --build "$buildDir" --config Release
