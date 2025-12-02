project(glad)

add_library(${PROJECT_NAME} STATIC)
message(ADDING_LIBRARY="${PROJECT_NAME}")

target_sources(${PROJECT_NAME} PRIVATE src/glad.c)

target_include_directories(${PROJECT_NAME} 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/extern/glfw/include
)