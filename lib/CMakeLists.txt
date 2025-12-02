add_library(glad
    STATIC
        glad/src/glad.c
)

target_include_directories(glad PUBLIC glad/include)

add_subdirectory(glfw)

add_library(glm INTERFACE)
target_include_directories(glm INTERFACE glm)

file(GLOB IMGUI_SRC
    imgui/*.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)

add_library(imgui STATIC ${IMGUI_SRC})
target_include_directories(imgui
    PUBLIC
        imgui
        imgui/backends
)

target_link_libraries(imgui PUBLIC glfw)

add_library(stb_image 
    STATIC
        stb_image/src/stb_image.cpp
)

target_include_directories(stb_image PUBLIC stb_image/include)