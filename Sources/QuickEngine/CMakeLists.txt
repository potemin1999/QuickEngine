set(ENGINE "QeEngine")
set(ENGINE_SOURCE_DIR "${SOURCE_DIR}/QuickEngine")

set(ENGINE_SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/libs/include/glad/glad.cpp
        private/actors/ModelLoader.cpp
        private/actors/GameObject.cpp
        private/input/InputEventManager.cpp
        private/materials/MaterialManager.cpp
        private/util/StringHelper.cpp
        private/util/Utils.cpp
        private/Camera.cpp
        private/Engine.cpp
        private/EngineUtils.cpp
        private/World.cpp
        private/Terrain.cpp
        private/net/NetworkManager.cpp
        private/net/NetworkServer.cpp
        private/net/NetworkClient.cpp
        private/basic/Framebuffer.cpp
        private/basic/Shader.cpp
        private/basic/Texture.cpp
        private/effects/PostPass.cpp
        private/geometry/GeometryPass.cpp
        private/lighting/DeferredLightingPass.cpp
        private/postprocess/SSAOPass.cpp
        private/util/RenderDataStorage.cpp
        private/util/ShaderManager.cpp
        private/util/TextureManager.cpp
        private/Renderer.cpp
        private/Model.cpp public/Model.h)

add_library(${ENGINE} STATIC ${ENGINE_SOURCE_FILES})

include_directories(${ENGINE} "${ENGINE_SOURCE_DIR}/public")
include_directories("${PROJECT_SOURCE_DIR}/libs/includeRakNet")
include_directories("${PROJECT_SOURCE_DIR}/libs/include")
include_directories("${PROJECT_SOURCE_DIR}/libs/bullet")

set_target_properties(${ENGINE}
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${BIN_BUILD_PATH}"
        RUNTIME_OUTPUT_DIRECTORY "${BIN_BUILD_PATH}"
        )


if (WIN32)
    add_library(BulletDynamics STATIC IMPORTED)
    set_target_properties(BulletDynamics PROPERTIES IMPORTED_LOCATION "${LIBS_DIR}/libBulletDynamics.a")
endif (WIN32)
if (UNIX)
    add_library(BulletDynamics SHARED IMPORTED)
    set_target_properties(BulletDynamics PROPERTIES IMPORTED_LOCATION "${LIBS_DIR}/libBulletDynamics.so")
endif (UNIX)


if (WIN32)
    add_library(BulletCollision STATIC IMPORTED)
    set_target_properties(BulletCollision PROPERTIES IMPORTED_LOCATION "${LIBS_DIR}/libBulletCollision.a")
endif (WIN32)
if (UNIX)
    add_library(BulletCollision SHARED IMPORTED)
    set_target_properties(BulletCollision PROPERTIES IMPORTED_LOCATION "${LIBS_DIR}/libBulletCollision.so")
endif (UNIX)


if (WIN32)
    add_library(LinearMath STATIC IMPORTED)
    set_target_properties(LinearMath PROPERTIES IMPORTED_LOCATION "${LIBS_DIR}/libLinearMath.a")
endif (WIN32)
if (UNIX)
    add_library(LinearMath SHARED IMPORTED)
    set_target_properties(LinearMath PROPERTIES IMPORTED_LOCATION "${LIBS_DIR}/libLinearMath.so")
endif (UNIX)


add_library(RakNet SHARED IMPORTED)
if (WIN32)
    set_target_properties(RakNet PROPERTIES IMPORTED_LOCATION "${LIBS_DIR}/RakNet.dll")
    set_target_properties(RakNet PROPERTIES IMPORTED_IMPLIB "${LIBS_DIR}/RakNet.lib")
endif (WIN32)
if (UNIX)
    set_target_properties(RakNet PROPERTIES IMPORTED_LOCATION "${LIBS_DIR}/libRakNetDLL.so")
endif (UNIX)


add_library(glfw3 STATIC IMPORTED)
if (WIN32)
    set_target_properties(glfw3 PROPERTIES IMPORTED_LOCATION "${LIBS_DIR}/glfw3.dll")
    set_target_properties(glfw3 PROPERTIES IMPORTED_IMPLIB "${LIBS_DIR}/glfw3dll.lib")
endif (WIN32)
if (UNIX)
    set_target_properties(glfw3 PROPERTIES IMPORTED_LOCATION "${LIBS_DIR}/libglfw.so")
endif (UNIX)


add_library(ilu STATIC IMPORTED)
if (WIN32)
    set_target_properties(ilu PROPERTIES IMPORTED_LOCATION "${LIBS_DIR}/ilu.dll")
    set_target_properties(ilu PROPERTIES IMPORTED_IMPLIB "${LIBS_DIR}/ilu.lib")
endif (WIN32)
if (UNIX)
    set_target_properties(ilu PROPERTIES IMPORTED_LOCATION "${LIBS_DIR}/libILU.so")
endif (UNIX)


if (UNIX)
    find_package(OpenGL REQUIRED)
endif (UNIX)


add_library(devil STATIC IMPORTED)
if (WIN32)
    set_target_properties(devil PROPERTIES IMPORTED_LOCATION "${LIBS_DIR}/devil.dll")
    set_target_properties(devil PROPERTIES IMPORTED_IMPLIB "${LIBS_DIR}/devil.lib")
endif (WIN32)
if (UNIX)
    set_target_properties(devil PROPERTIES IMPORTED_LOCATION "${LIBS_DIR}/libIL.so")
endif (UNIX)


# bullet libs
target_link_libraries(${ENGINE} BulletDynamics)
target_link_libraries(${ENGINE} BulletCollision)
target_link_libraries(${ENGINE} LinearMath)
# networking libs
target_link_libraries(${ENGINE} RakNet)
# render libs
target_link_libraries(${ENGINE} glfw3)
target_link_libraries(${ENGINE} ilu)
target_link_libraries(${ENGINE} devil)
