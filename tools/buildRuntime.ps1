# buildRuntime.ps1
$projectRoot = Resolve-Path "$PSScriptRoot/.."
$buildDir = "$projectRoot/build"
$toolchain = "-DCMAKE_TOOLCHAIN_FILE=`"$projectRoot/cmake/clang-mingw-toolchain.cmake`""

# Configure (only if build dir missing)
if (-Not (Test-Path $buildDir)) {
    cmake -S "$projectRoot" -B $buildDir -DCMAKE_BUILD_TYPE=Release $toolchain
}

# Build only the runtime target
cmake --build "$buildDir" --config Release --target modelViewer
