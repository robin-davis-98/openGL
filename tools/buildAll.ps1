$projectRoot = Resolve-Path "$PSScriptRoot/.."
$buildDir = "$projectRoot/build/debug"

# Clear previous Debug build artifacts
Remove-Item "$buildDir" -Recurse -Force -ErrorAction SilentlyContinue

# Toolchain if needed
$toolchain = "-DCMAKE_TOOLCHAIN_FILE=`"$projectRoot/cmake/clang-mingw-toolchain.cmake`""

# Configure Debug build for entire project (engine + runtime)
cmake -S "$projectRoot" -B $buildDir -DCMAKE_BUILD_TYPE=Debug $toolchain

# Build everything in Debug
cmake --build "$buildDir" --config Debug
