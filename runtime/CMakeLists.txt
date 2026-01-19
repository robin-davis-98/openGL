project(modelViewer)

add_executable(${PROJECT_NAME}  WIN32
    main.cpp
)

if(MSVC OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set_target_properties(${PROJECT_NAME} PROPERTIES 
    LINK_FLAGS "-Xlinker /SUBSYSTEM:WINDOWS -Xlinker /ENTRY:mainCRTStartup"
)
endif()

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        engine
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/engine/include
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:engine>
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

add_dependencies(modelViewer copy_assets)
