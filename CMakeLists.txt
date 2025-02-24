cmake_minimum_required(VERSION 3.30)
project(NE_OpenGL
        VERSION 1.0.0
        DESCRIPTION "Non-Euclidean OpenGL Engine"
        LANGUAGES CXX
)

# Compilation options
option(BUILD_TESTS "Build test cases" OFF)
option(ENABLE_WARNINGS "Enable warning flags" ON)

# Global settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Warnings settings
if (ENABLE_WARNINGS)
    if (MSVC)
        add_compile_options(/W4)
    else ()
        add_compile_options(-Wall -Wextra -Wpedantic)
    endif ()
endif ()

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")

# Executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# External libraries
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)

set(YAML_CPP_DIR "/opt/homebrew/Cellar/yaml-cpp/0.8.0")

# Include directories
target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
        ${OPENGL_INCLUDE_DIR}
        ${GLEW_INCLUDE_DIRS}
        ${SDL2_INCLUDE_DIRS}
        ${YAML_CPP_DIR}/include
)

# Linking
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        OpenGL::GL
        GLEW::GLEW
        SDL2::SDL2
        ${YAML_CPP_DIR}/lib/libyaml-cpp.dylib
)

# Assets copying
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)

# Installation settings
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)

install(DIRECTORY assets/
        DESTINATION bin/assets
)

# Packaging settings
include(CPack)