project(engine)

add_library(${PROJECT_NAME}
    SHARED
        src/app.cpp
        
        src/window/window.cpp
        
        src/gui/gui.cpp
        
        src/utils/file.cpp

        src/renderer/renderTarget.cpp
        src/renderer/renderer.cpp
        src/renderer/scene.cpp
        src/renderer/model.cpp
        src/renderer/mesh.cpp
        src/renderer/material.cpp
        src/renderer/shader.cpp
        src/renderer/ubo.cpp
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