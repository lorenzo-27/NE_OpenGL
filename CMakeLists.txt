cmake_minimum_required(VERSION 3.30)
project(NE_OpenGL)

set(CMAKE_CXX_STANDARD 11)

set(SOURCES
        src/core/camera/Camera.cpp
        src/core/math/Collider.cpp
        src/core/engine/Engine.cpp
        src/core/engine/Engine_SDL2.cpp
        src/core/engine/Engine_Win32.cpp
        src/rendering/FrameBuffer.cpp
        src/core/input/Input.cpp
        src/game/levels/Level1.cpp
        src/game/levels/Level2.cpp
        src/game/levels/Level3.cpp
        src/game/levels/Level4.cpp
        src/game/levels/Level5.cpp
        src/game/levels/Level6.cpp
        Main.cpp
        src/rendering/Mesh.cpp
        src/game/objects/base/Object.cpp
        src/game/objects/base/Physical.cpp
        src/game/objects/interactive/Player.cpp
        src/game/objects/interactive/Portal.cpp
        src/resources/Resources.cpp
        src/rendering/Shader.cpp
        src/rendering/Texture.cpp
)

set(HEADERS
        include/core/camera/Camera.h
        include/core/math/Collider.h
        include/core/engine/Engine.h
        include/game/objects/environment/Floorplan.h
        include/rendering/FrameBuffer.h
        include/core/engine/GameHeader.h
        include/game/objects/environment/Ground.h
        include/game/objects/environment/House.h
        include/core/input/Input.h
        include/game/levels/Level1.h
        include/game/levels/Level2.h
        include/game/levels/Level3.h
        include/game/levels/Level4.h
        include/game/levels/Level5.h
        include/game/levels/Level6.h
        include/rendering/Mesh.h
        include/game/objects/base/Object.h
        include/game/objects/base/Physical.h
        include/game/objects/props/Pillar.h
        include/game/objects/environment/PillarRoom.h
        include/game/objects/interactive/Player.h
        include/game/objects/interactive/Portal.h
        include/resources/Resources.h
        include/game/Scene.h
        include/rendering/Shader.h
        include/game/objects/environment/Sky.h
        include/game/objects/props/Sphere.h
        include/game/objects/props/Statue.h
        include/rendering/Texture.h
        include/core/engine/Timer.h
        include/game/objects/props/Tunnel.h
        include/core/math/Vector.h
)

add_executable(NE_OpenGL ${SOURCES} ${HEADERS})

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)

target_include_directories(NE_OpenGL PRIVATE ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS})
target_link_libraries(NE_OpenGL ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${SDL2_LIBRARIES})
