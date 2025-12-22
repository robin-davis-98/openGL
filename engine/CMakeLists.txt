project(engine)

add_library(${PROJECT_NAME}
    SHARED
        src/app.cpp
        
        src/window/window.cpp
        
        src/gui/gui.cpp
        
        src/utils/file.cpp

        src/renderTarget.cpp
        src/renderer.cpp
        src/scene.cpp
        src/shader.cpp
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        ENGINE_EXPORTS
)

target_include_directories(${PROJECT_NAME}
        PUBLIC
            include
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/gui
            ${CMAKE_SOURCE_DIR}/lib/glad/include
            ${CMAKE_SOURCE_DIR}/lib/imgui
            ${CMAKE_SOURCE_DIR}/lib/imgui/backends
            ${CMAKE_SOURCE_DIR}/lib/stb_image/include
            ${CMAKE_SOURCE_DIR}/lib/glm
)

find_package(OpenGL REQUIRED)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        OpenGL::GL
        glfw
        imgui
        glad
        glm
        stb_image
)