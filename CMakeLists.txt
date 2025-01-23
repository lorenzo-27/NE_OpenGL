cmake_minimum_required(VERSION 3.30)
project(NE_OpenGL)

set(CMAKE_CXX_STANDARD 11)

set(SOURCES
        Camera.cpp
        Collider.cpp
        Engine.cpp
        Engine_SDL2.cpp
        Engine_Win32.cpp
        FrameBuffer.cpp
        Input.cpp
        Level1.cpp
        Level2.cpp
        Level3.cpp
        Level4.cpp
        Level5.cpp
        Level6.cpp
        Main.cpp
        Mesh.cpp
        Object.cpp
        Physical.cpp
        Player.cpp
        Portal.cpp
        Resources.cpp
        Shader.cpp
        Texture.cpp
)

set(HEADERS
        Camera.h
        Collider.h
        Engine.h
        Floorplan.h
        FrameBuffer.h
        GameHeader.h
        Ground.h
        House.h
        Input.h
        Level1.h
        Level2.h
        Level3.h
        Level4.h
        Level5.h
        Level6.h
        Mesh.h
        Object.h
        Physical.h
        Pillar.h
        PillarRoom.h
        Player.h
        Portal.h
        Resources.h
        Scene.h
        Shader.h
        Sky.h
        Sphere.h
        Statue.h
        Texture.h
        Timer.h
        Tunnel.h
        Vector.h
)

add_executable(NE_OpenGL ${SOURCES} ${HEADERS})

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)

target_include_directories(NE_OpenGL PRIVATE ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS})
target_link_libraries(NE_OpenGL ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${SDL2_LIBRARIES})
