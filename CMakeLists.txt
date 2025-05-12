cmake_minimum_required(VERSION 3.26.0)
project(shadowMapping)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/cmake/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/cmake/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/cmake/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/cmake/bin/Debug)
set(CMAKE_SOLUTION_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/cmake)

include_directories(${CMAKE_SOURCE_DIR}/includes)

find_package(OpenGL REQUIRED)

link_directories(${CMAKE_SOURCE_DIR}/lib)

set(SOURCE_FILES
        includes/GL/glad.cpp
        includes/imgui/imgui.cpp
        includes/imgui/imgui_draw.cpp
        includes/imgui/imgui_widgets.cpp
        includes/imgui/imgui_tables.cpp
        includes/imgui/imgui_impl_glfw.cpp
        includes/imgui/imgui_impl_opengl3.cpp
        main.cpp
        includes/mine/Shader.cpp
        includes/mine/Camera.cpp
        includes/mine/auxiliary.cpp
        includes/mine/Mesh.cpp
        includes/mine/Model.cpp
)

add_executable(shadowMapping ${SOURCE_FILES})
set_target_properties(shadowMapping PROPERTIES OUTPUT_NAME "main")

if(WIN32)
        if(MSVC)
                add_compile_options(/wd4005)
        endif()

        target_link_libraries(shadowMapping
                ${OPENGL_LIBRARIES}
                glfw3
                assimp-vc143-mtd
        )
elseif(APPLE)
        find_library(COCOA_LIBRARY Cocoa)
        find_library(IOKIT_LIBRARY IOKit)
        find_library(CORE_VIDEO_LIBRARY CoreVideo)
        target_link_libraries(shadowMapping
                ${OPENGL_LIBRARIES}
                glfw
                ${COCOA_LIBRARY}
                ${IOKIT_LIBRARY}
                ${CORE_VIDEO_LIBRARY}
        )
else()
        target_link_libraries(shadowMapping
                ${OPENGL_LIBRARIES}
                glfw
                dl
        )
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")