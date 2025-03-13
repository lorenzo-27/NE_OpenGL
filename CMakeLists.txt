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

# Windows-specific libraries setup
if (WIN32)
    # =============== GLEW Configuration ===============
    option(GLEW_STATIC "Use static GLEW library" ON)
    if (GLEW_STATIC)
        add_definitions(-DGLEW_STATIC)
    endif()

    # Set default GLEW_ROOT if not provided
    if (NOT DEFINED GLEW_ROOT)
        set(GLEW_ROOT "C:/libs/glew-2.1.0" CACHE PATH "Path to GLEW installation")
    endif()

    if (EXISTS "${GLEW_ROOT}")
        set(GLEW_INCLUDE_DIRS "${GLEW_ROOT}/include")

        if (GLEW_STATIC)
            if (MSVC)
                set(GLEW_LIBRARIES "${GLEW_ROOT}/lib/Release/x64/glew32s.lib")
                if (NOT EXISTS "${GLEW_LIBRARIES}")
                    set(GLEW_LIBRARIES "${GLEW_ROOT}/lib/Release/Win32/glew32s.lib")
                endif()
            else()
                foreach(path
                        "${GLEW_ROOT}/lib/Release/x64/glew32s.lib"
                        "${GLEW_ROOT}/lib/Release/Win32/glew32s.lib"
                        "${GLEW_ROOT}/lib/glew32s.lib"
                        "${GLEW_ROOT}/build/libyaml-cpp.a"
                        "${GLEW_ROOT}/lib/libyaml-cpp.a")
                    if(EXISTS "${path}")
                        set(GLEW_LIBRARIES "${path}")
                        break()
                    endif()
                endforeach()
            endif()
        else()
            if (MSVC)
                set(GLEW_LIBRARIES "${GLEW_ROOT}/lib/Release/x64/glew32.lib")
                if (NOT EXISTS "${GLEW_LIBRARIES}")
                    set(GLEW_LIBRARIES "${GLEW_ROOT}/lib/Release/Win32/glew32.lib")
                endif()
            else()
                set(GLEW_LIBRARIES "${GLEW_ROOT}/lib/glew32.dll.a")
            endif()
        endif()

        message(STATUS "Using GLEW from ${GLEW_ROOT}")
        message(STATUS "GLEW include: ${GLEW_INCLUDE_DIRS}")
        message(STATUS "GLEW lib: ${GLEW_LIBRARIES}")
    else()
        message(WARNING "GLEW_ROOT at ${GLEW_ROOT} does not exist. Trying find_package...")
        find_package(GLEW REQUIRED)
    endif()

    # =============== yaml-cpp Configuration for Windows ===============
    if (NOT DEFINED YAML_CPP_ROOT)
        set(YAML_CPP_ROOT "C:/libs/yaml-cpp-0.8.0" CACHE PATH "Path to yaml-cpp installation")
    endif()

    if (EXISTS "${YAML_CPP_ROOT}")
        set(YAML_CPP_INCLUDE_DIRS "${YAML_CPP_ROOT}/include")

        # Try to find the library in many more possible locations
        if (MSVC)
            # Try to find the library in multiple possible locations for MSVC
            foreach(path
                    "${YAML_CPP_ROOT}/build/Release/yaml-cpp.lib"
                    "${YAML_CPP_ROOT}/build/yaml-cpp.lib"
                    "${YAML_CPP_ROOT}/lib/yaml-cpp.lib"
                    "${YAML_CPP_ROOT}/lib/Release/yaml-cpp.lib"
                    "${YAML_CPP_ROOT}/yaml-cpp.lib"
                    "${YAML_CPP_ROOT}/build/Debug/yaml-cpp.lib"
                    "${YAML_CPP_ROOT}/build/yaml-cppd.lib"
                    "${YAML_CPP_ROOT}/lib/yaml-cppd.lib"
                    "${YAML_CPP_ROOT}/yaml-cppd.lib")
                if(EXISTS "${path}")
                    set(YAML_CPP_LIBRARIES "${path}")
                    break()
                endif()
            endforeach()
        else()
            # For MinGW
            foreach(path
                    "${YAML_CPP_ROOT}/build/libyaml-cpp.a"
                    "${YAML_CPP_ROOT}/lib/libyaml-cpp.a"
                    "${YAML_CPP_ROOT}/libyaml-cpp.a"
                    "${YAML_CPP_ROOT}/build/libyaml-cpp.dll.a"
                    "${YAML_CPP_ROOT}/lib/libyaml-cpp.dll.a"
                    "${YAML_CPP_ROOT}/libyaml-cpp.dll.a")
                if(EXISTS "${path}")
                    set(YAML_CPP_LIBRARIES "${path}")
                    break()
                endif()
            endforeach()
        endif()

        if (NOT DEFINED YAML_CPP_LIBRARIES OR "${YAML_CPP_LIBRARIES}" STREQUAL "")
            # In caso estremo, cerca in tutta la gerarchia di directory
            message(STATUS "Searching for yaml-cpp library files in ${YAML_CPP_ROOT}...")

            if (MSVC)
                file(GLOB_RECURSE YAML_CPP_LIB_CANDIDATES
                        "${YAML_CPP_ROOT}/**/yaml-cpp*.lib")
            else()
                file(GLOB_RECURSE YAML_CPP_LIB_CANDIDATES
                        "${YAML_CPP_ROOT}/**/libyaml-cpp*.a"
                        "${YAML_CPP_ROOT}/**/libyaml-cpp*.dll.a")
            endif()

            if (YAML_CPP_LIB_CANDIDATES)
                list(GET YAML_CPP_LIB_CANDIDATES 0 YAML_CPP_LIBRARIES)
                message(STATUS "Found yaml-cpp library using recursive search: ${YAML_CPP_LIBRARIES}")
            else()
                # Se ancora non trovato, creiamo una libreria fittizia
                message(STATUS "No yaml-cpp library found. Creating a minimal yaml-cpp.")

                # Imposta l'header only
                set(YAML_CPP_INCLUDE_ONLY TRUE)

                # Aggiunge un'opzione per scegliere se compilare yaml-cpp
                option(BUILD_YAML_CPP "Build yaml-cpp from source" ON)

                if (BUILD_YAML_CPP)
                    # Prepara a compilare yaml-cpp
                    message(STATUS "Will attempt to build yaml-cpp from source")

                    include(ExternalProject)
                    ExternalProject_Add(
                            yaml-cpp-ext
                            GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
                            GIT_TAG 0.8.0
                            PREFIX ${CMAKE_BINARY_DIR}/yaml-cpp
                            CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/yaml-cpp/install
                            -DYAML_CPP_BUILD_TESTS=OFF
                            -DYAML_CPP_BUILD_TOOLS=OFF
                            -DYAML_CPP_BUILD_CONTRIB=OFF
                    )

                    set(YAML_CPP_INCLUDE_DIRS "${CMAKE_BINARY_DIR}/yaml-cpp/install/include")
                    if (MSVC)
                        set(YAML_CPP_LIBRARIES "${CMAKE_BINARY_DIR}/yaml-cpp/install/lib/yaml-cpp.lib")
                    else()
                        set(YAML_CPP_LIBRARIES "${CMAKE_BINARY_DIR}/yaml-cpp/install/lib/libyaml-cpp.a")
                    endif()

                    add_dependencies(${PROJECT_NAME} yaml-cpp-ext)
                endif()
            endif()
        endif()

        message(STATUS "Using yaml-cpp from ${YAML_CPP_ROOT}")
        message(STATUS "yaml-cpp include: ${YAML_CPP_INCLUDE_DIRS}")
        if (NOT YAML_CPP_INCLUDE_ONLY)
            message(STATUS "yaml-cpp lib: ${YAML_CPP_LIBRARIES}")
        else()
            message(STATUS "yaml-cpp: header-only mode or building from source")
        endif()
    else()
        message(WARNING "YAML_CPP_ROOT at ${YAML_CPP_ROOT} does not exist. Trying find_package...")
        find_package(yaml-cpp QUIET)
        if (NOT yaml-cpp_FOUND)
            # Se non trovato, compiliamo da fonte
            message(STATUS "Will build yaml-cpp from source")

            include(ExternalProject)
            ExternalProject_Add(
                    yaml-cpp-ext
                    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
                    GIT_TAG yaml-cpp-0.8.0
                    PREFIX ${CMAKE_BINARY_DIR}/yaml-cpp
                    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/yaml-cpp/install
                    -DYAML_CPP_BUILD_TESTS=OFF
                    -DYAML_CPP_BUILD_TOOLS=OFF
                    -DYAML_CPP_BUILD_CONTRIB=OFF
            )

            set(YAML_CPP_INCLUDE_DIRS "${CMAKE_BINARY_DIR}/yaml-cpp/install/include")
            if (MSVC)
                set(YAML_CPP_LIBRARIES "${CMAKE_BINARY_DIR}/yaml-cpp/install/lib/yaml-cpp.lib")
            else()
                set(YAML_CPP_LIBRARIES "${CMAKE_BINARY_DIR}/yaml-cpp/install/lib/libyaml-cpp.a")
            endif()

            set(YAML_CPP_INCLUDE_ONLY TRUE)
            add_dependencies(${PROJECT_NAME} yaml-cpp-ext)
        endif()
    endif()

    # Add Windows-specific libraries
    set(WINDOWS_LIBRARIES "winmm.lib" "user32.lib" "gdi32.lib" "shell32.lib")
else()
    # Linux and macOS use standard find_package
    find_package(GLEW REQUIRED)
    find_package(SDL2 REQUIRED)

    # Platform specific yaml-cpp configuration
    if (APPLE)
        set(YAML_CPP_DIR "/opt/homebrew/Cellar/yaml-cpp/0.8.0")
        set(YAML_CPP_INCLUDE_DIRS "${YAML_CPP_DIR}/include")
        set(YAML_CPP_LIBRARIES "${YAML_CPP_DIR}/lib/libyaml-cpp.dylib")
    else()
        find_package(yaml-cpp REQUIRED)
    endif()
endif()

# Include directories
target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
        ${OPENGL_INCLUDE_DIR}
        ${GLEW_INCLUDE_DIRS}
        ${YAML_CPP_INCLUDE_DIRS}
)

# Add SDL2 include directories only for non-Windows platforms
if (NOT WIN32)
    target_include_directories(${PROJECT_NAME}
            PRIVATE
            ${SDL2_INCLUDE_DIRS}
    )
endif()

# Platform-specific linking
if (WIN32)
    target_link_libraries(${PROJECT_NAME}
            PRIVATE
            OpenGL::GL
            ${GLEW_LIBRARIES}
            ${WINDOWS_LIBRARIES}
    )

    if (NOT YAML_CPP_INCLUDE_ONLY)
        target_link_libraries(${PROJECT_NAME} PRIVATE ${YAML_CPP_LIBRARIES})
    endif()
else()
    target_link_libraries(${PROJECT_NAME}
            PRIVATE
            OpenGL::GL
            GLEW::GLEW
            ${YAML_CPP_LIBRARIES}
            SDL2::SDL2
    )
endif()

# Windows-specific: Copy GLEW DLLs if using dynamic GLEW
if (WIN32 AND NOT GLEW_STATIC AND EXISTS "${GLEW_ROOT}/bin")
    file(GLOB GLEW_DLLS "${GLEW_ROOT}/bin/glew32.dll")
    if (GLEW_DLLS)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${GLEW_DLLS} $<TARGET_FILE_DIR:${PROJECT_NAME}>
                COMMENT "Copying GLEW DLLs to output directory"
        )
    endif()
endif()

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