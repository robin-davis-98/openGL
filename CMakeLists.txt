cmake_minimum_required(VERSION 3.20)
project("OpenGL Model Viewer")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

find_package(Doxygen REQUIRED)

if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_SOURCE_DIR}/docs/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_BINARY_DIR}/docs)

    add_custom_target(project_docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_IN}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating API documentation"
        VERBATIM
    )
endif()

option(USE_TRACY "Enable Tracy profiler" ON)

if(USE_TRACY AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_subdirectory(${CMAKE_SOURCE_DIR}/lib/tracy)
    add_compile_definitions(TRACY_ENABLE)
endif()

add_subdirectory(lib)
add_subdirectory(engine)
add_subdirectory(runtime)

