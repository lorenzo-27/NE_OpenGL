# NE_OpenGL: Non-Euclidean OpenGL Engine

## Index

1.  [Introduction](#introduction)
2.  [Non-Euclidean Geometry: Basic Concepts](#non-euclidean-geometry-basic-concepts)
3.  [Prerequisites](#prerequisites)
    *   [Windows](#prerequisites-windows)
    *   [Linux](#prerequisites-linux)
    *   [macOS](#prerequisites-macos)
4.  [Setup and Usage](#setup-and-usage)
    *   [Windows](#setup-and-usage-windows)
    *   [Linux](#setup-and-usage-linux)
    *   [macOS](#setup-and-usage-macos)
5.  [Implementation Description](#implementation-description)
6.  [Key Features](#key-features)
7.  [Additional Information](#additional-information)
8.  [Reference List](#references)
9.  [License](#license)

## <a name="introduction"></a>1. Introduction

NE_OpenGL is an experimental graphics engine that explores the visualization of non-Euclidean geometries using OpenGL. The project focuses on rendering non-Euclidean portals, offering a unique and unconventional visual experience. The engine is written in C++ and uses CMake as its build system, making it cross-platform (Windows, Linux, macOS). This README file provides a comprehensive overview of the project, its dependencies, installation and usage instructions, and the theoretical concepts behind it.

## <a name="non-euclidean-geometry-basic-concepts"></a>2. Non-Euclidean Geometry: Basic Concepts

Euclidean geometry, based on Euclid's postulates, is the geometry of "flat" space to which we are accustomed.

This project implements a rendering system that allows the visualization of "portals" that connect regions of space in ways that violate Euclidean geometry. For example, it's possible to create a tunnel that, once traversed, returns the user to the ending point, but at a different scale, or a tunnel that seems very long from the outside but in reality is very short when traversed.

## <a name="prerequisites"></a>3. Prerequisites

This project has the following dependencies:

*   **CMake (>= 3.30):** Cross-platform build system.
*   **C++20 Compiler:** A compiler that supports the C++20 standard is required (e.g., GCC 10+, Clang 10+, MSVC 19.28+).
*   **OpenGL (>= 3.3):** Graphics library.
*   **GLEW:** OpenGL Extension Wrangler Library. Manages the loading of OpenGL extensions.
*   **yaml-cpp:** Library for parsing YAML files (used for level configuration).
*   **SDL2 (only for Linux and macOS):** Simple DirectMedia Layer. Provides a low-level abstraction for input, audio, and windowing (used instead of native Windows APIs).

Specific instructions for installing these dependencies vary depending on the operating system.

### <a name="prerequisites-windows"></a>3.1 Prerequisites (Windows)

1.  **CMake:** Download and install CMake from the official website: [https://cmake.org/download/](https://cmake.org/download/). Make sure to add CMake to the PATH during installation.

2.  **Visual Studio (recommended):** Visual Studio 2019 or 2022 with C++20 support is strongly recommended. During installation, make sure to select the "Desktop development with C++" workload. Visual Studio automatically includes a compatible C++ compiler.

3.  **MinGW-w64 (alternative to Visual Studio):** If you prefer to use MinGW-w64, you need to install it separately. It is recommended to use MSYS2 ([https://www.msys2.org/](https://www.msys2.org/)) to install MinGW-w64. After installing MSYS2, run the following commands in an MSYS2 terminal:

    ```bash
    pacman -S mingw-w64-x86_64-toolchain
    pacman -S mingw-w64-x86_64-cmake
    pacman -S mingw-w64-x86_64-glew
    pacman -S mingw-w64-x86_64-yaml-cpp
    ```

    Make sure that the MinGW-w64 path (e.g., `C:\msys64\mingw64\bin`) is added to the system PATH.

4.  **GLEW and yaml-cpp (if using Visual Studio):** The `CMakeLists.txt` file is configured to look for GLEW and yaml-cpp in specific paths (`C:/libs/glew-2.1.0` and `C:/libs/yaml-cpp-0.8.0`). You can download precompiled versions of these libraries and place them in these directories, or modify the `GLEW_ROOT` and `YAML_CPP_ROOT` variables in `CMakeLists.txt` to point to the correct installation directories. If the libraries are not found in these paths, CMake will attempt to use `find_package()`, which may require configuring additional environment variables. As a last resort, if `find_package()` fails, CMake will attempt to download and compile yaml-cpp from source.

### <a name="prerequisites-linux"></a>3.2 Prerequisites (Linux)

Use your distribution's package manager to install the dependencies. Here are some examples for the most common distributions:

*   **Debian/Ubuntu:**

    ```bash
    sudo apt update
    sudo apt install cmake g++ libglew-dev libsdl2-dev libyaml-cpp-dev
    ```

*   **Fedora/CentOS/RHEL:**

    ```bash
    sudo dnf install cmake gcc-c++ glew-devel SDL2-devel yaml-cpp-devel
    ```

*   **Arch Linux:**

    ```bash
    sudo pacman -S cmake gcc glew sdl2 yaml-cpp
    ```

### <a name="prerequisites-macos"></a>3.3 Prerequisites (macOS)

1.  **Homebrew:** If you don't already have Homebrew, install it by following the instructions on the official website: [https://brew.sh/](https://brew.sh/).

2.  **CMake, GLEW, SDL2, yaml-cpp:** Install the dependencies via Homebrew:

    ```bash
    brew install cmake glew sdl2 yaml-cpp
    ```
    The `CMakeLists.txt` file is configured to look for yaml-cpp in `/opt/homebrew/Cellar/yaml-cpp/0.8.0`. If your version or installation path is different, modify the `YAML_CPP_DIR`, `YAML_CPP_INCLUDE_DIRS`, and `YAML_CPP_LIBRARIES` variables in `CMakeLists.txt`.

## <a name="setup-and-usage"></a>4. Setup and Usage

### <a name="setup-and-usage-windows"></a>4.1 Setup and Usage (Windows)

1.  **Clone the repository:**

    ```bash
    git clone https://github.com/lorenzo-27/NE_OpenGL.git
    cd NE_OpenGL
    ```

2.  **Create a build directory:**

    ```bash
    mkdir build
    cd build
    ```

3.  **Generate project files with CMake (Visual Studio):**

    ```bash
    cmake .. -G "Visual Studio 16 2019" -A x64
    ```

    Replace `"Visual Studio 16 2019"` with the correct version of Visual Studio installed (e.g., `"Visual Studio 17 2022"`). The `-A x64` option specifies the 64-bit target platform (recommended).

4.  **Build the project (Visual Studio):**

    Open the `.sln` solution file generated by CMake in Visual Studio and build the project (Build -> Build Solution).

5.  **Generate project files with CMake (MinGW-w64):**

    ```bash
    cmake .. -G "MinGW Makefiles"
    ```

6.  **Build the project (MinGW-w64):**

    ```bash
    cmake --build .
    ```

7.  **Run the project:**

    The executable (`NE_OpenGL.exe`) will be created in the `build/bin` directory. You can run it directly from Visual Studio (Debug -> Start Without Debugging) or from the command line. Make sure that the working directory is the project's root directory (NE_OpenGL), so that the executable can find the resources (shaders, textures, levels) in the `assets` directory.

### <a name="setup-and-usage-linux"></a>4.2 Setup and Usage (Linux)

1.  **Clone the repository:**

    ```bash
    git clone https://github.com/lorenzo-27/NE_OpenGL.git
    cd NE_OpenGL
    ```

2.  **Create a build directory:**

    ```bash
    mkdir build
    cd build
    ```

3.  **Generate project files with CMake:**

    ```bash
    cmake ..
    ```

4.  **Build the project:**

    ```bash
    cmake --build .
    ```
    or, alternatively:
    ```
    make
    ```

5.  **Run the project:**

    The executable (`NE_OpenGL`) will be created in the `build/bin` directory. Make sure that the working directory is the project's root directory (NE_OpenGL) when running the executable.

    ```bash
    cd ..
    ./build/bin/NE_OpenGL
    ```

### <a name="setup-and-usage-macos"></a>4.3 Setup and Usage (macOS)

The steps are identical to those for Linux. Use the terminal and follow the instructions for Linux.

## <a name="implementation-description"></a>5. Implementation Description

The core of the graphics engine consists of the following main classes and components:

*   **`Engine`:** Manages the main game loop, OpenGL initialization, input handling, and coordinating scene rendering and updating.

*   **`Camera`:** Represents the virtual camera. Handles projection (both Euclidean and oblique for portals) and view-world transformation.

*   **`Object`:** Base class for all scene objects. Defines basic properties such as position, rotation, scale, and pointers to mesh, shader, and texture.

*   **`Physical`:** Subclass of `Object` that adds physical properties such as velocity, gravity, friction, and a set of collision spheres.

*   **`Player`:** Subclass of `Physical` that represents the player. Handles player input (movement and view) and player-specific logic (such as view bobbing).

*   **`Portal`:** Subclass of `Object` that represents a portal. Handles teleportation logic between portals, recursive scene rendering through portals, and creating a non-Euclidean visual effect. Portals are connected to each other via a `Warp` structure that contains the transformation matrices needed for teleportation.

*   **`Tunnel`:** Subclass of `Object` that represents a tunnel. Provides methods for automatically positioning and orienting portals at its ends. There are different types of tunnels (NORMAL, SCALE, SLOPE) to create different non-Euclidean effects.

*   **`Ground`:** Subclass of `Object` that represents the ground.

*   **`Sky`:** Represents the sky (skybox).

*   **`Mesh`:** Manages object geometry (vertices, normals, UV coordinates, indices). Loads models from OBJ files and manages OpenGL VAOs (Vertex Array Objects) and VBOs (Vertex Buffer Objects). Also includes a `Collider` system for collision detection.

*   **`Shader`:** Manages shader compilation and loading (vertex and fragment shaders) from GLSL or SPIR-V files. Provides methods for setting uniforms (such as MVP matrices). Supports shader hot-reloading.

*   **`Texture`:** Manages texture loading from files (BMP, HDR, and other formats supported by stb_image). Supports 2D textures and array textures (used for portals).

*   **`FrameBuffer`:** Manages OpenGL framebuffer objects (FBOs), used for off-screen rendering (necessary for recursive portal rendering).

*   **`LevelConfig`:** Represents the configuration of a level, loaded from a YAML file. Contains information about the objects to create, their properties, and connections between portals.

*   **`LevelManager`:** Manages level loading from YAML files.

*   **`ObjectFactory`:** Creates instances of objects (`Tunnel`, `Ground`, etc.) from the level configuration.

*   **`Input` / `InputAdapter`:** Handles user input (keyboard, mouse). `Input` provides a generic interface, while `InputAdapter` (and its subclasses `WindowsInputAdapter` and `SDLInputAdapter`) implement platform-specific input handling.

*   **`Timer`:** Provides timing functionality (for fixed timestep).

*   **`core/math/`:** Contains the `Vector3`, `Vector4`, and `Matrix4` classes for vector and matrix mathematics.

Portal rendering happens recursively. When a portal is visible, the scene is rendered to a framebuffer from the viewpoint of the "destination" portal, applying a transformation that takes into account the relative position and orientation of the two portals. This framebuffer is then used as a texture to draw the portal in the main scene. Recursion is limited by `GH_MAX_RECURSION` to avoid an infinite loop. Occlusion culling (via OpenGL queries) is used to avoid rendering portals that are not visible.

## <a name="key-features"></a>6. Key Features

*   **Non-Euclidean Geometry:** Implementation of non-Euclidean portal rendering with support for scale and slope effects.
*   **Cross-Platform:** Support for Windows, Linux, and macOS thanks to CMake and SDL2 (for Linux/macOS).
*   **Shader Hot-Reloading:** Shaders can be modified and reloaded at runtime without restarting the application, simplifying development and experimentation.
*   **Level Loading from YAML Files:** Levels are defined in YAML files, making it easy to create and modify new levels without having to recompile the code.
*   **Modern OpenGL Usage:** Use of Vertex Array Objects (VAO), Vertex Buffer Objects (VBO), Framebuffer Objects (FBO), and GLSL/SPIR-V shaders.
*   **Optimizations:** Use of SIMD (SSE2 on x86/x64 and NEON on ARM) for some operations (IDCT, resampling, YCbCr-to-RGB conversion).
*   **Occlusion Culling:** Use of occlusion queries to avoid rendering invisible portals.
*   **Open Source Code:** The code is released under MIT license (see [License](#license) section), allowing free use, modification, and distribution.
* **SPIR-V Support:** Ability to use precompiled shaders in SPIR-V format to improve performance and portability.

## <a name="additional-information"></a>7. Additional Information

*   **Controls:**
    *   `W`, `A`, `S`, `D`: Movement.
    *   Mouse: View rotation.
    *   `Q`: Quit.
    *   `R`: Reload shaders.
    *   `F`: Toggle fullscreen mode.
    *   `1`-`5`: Load levels 1 through 5.

*   **Project Structure:**
    *   `src/`: Contains C++ source code.
        *   `core/`: Core engine components (engine, camera, input, math).
        *   `game/`: Game-specific classes (objects, scenes, levels).
        *   `rendering/`: Classes for managing graphic resources (mesh, shader, texture, framebuffer).
        *   `resources/`: Support files (stb_image.h).
    *   `include/`: C++ header files.
    *   `assets/`: Contains game resources (shaders, textures, models, levels).
        *   `shaders/`: GLSL or SPIR-V shaders.
        *   `textures/`: Textures (BMP, HDR).
        *   `meshes/`: 3D models (OBJ).
        *   `levels/`: Level definitions (YAML).
    *   `CMakeLists.txt`: CMake configuration file.
 
## <a name="references"></a>8. Reference List
TBD

## <a name="license"></a>9. License
This project is licensed under the <a href="https://github.com/lorenzo-27/NE_OpenGL/blob/master/LICENSE" target="_blank">MIT</a> License.
