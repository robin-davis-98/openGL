project(stb_image)

add_library(${PROJECT_NAME} STATIC)
message(ADDING_LIBRARY="${PROJECT_NAME}")

target_sources(${PROJECT_NAME} PRIVATE src/stb_image.cpp)

target_include_directories(${PROJECT_NAME} 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)