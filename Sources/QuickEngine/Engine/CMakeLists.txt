set(ENGINE "QeEngine")
set(ENGINE_SOURCE_DIR "${QUICK_ENGINE_SOURCE_DIR}/Engine")
set(ENGINE_SOURCE_FILES
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
        private/Terrain.cpp public/Terrain.h public/net/NetworkManager.cpp public/net/NetworkManager.h public/net/NetworkServer.cpp public/net/NetworkServer.h public/net/NetworkClient.cpp public/net/NetworkClient.h)

add_library(${ENGINE} SHARED ${ENGINE_SOURCE_FILES})
set_target_properties(${ENGINE} PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(${ENGINE}
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${BIN_BUILD_PATH}"
        RUNTIME_OUTPUT_DIRECTORY "${BIN_BUILD_PATH}"
        )

add_library(BulletCollision SHARED IMPORTED)
if (WIN32)
    # TODO: FIX THIS
endif (WIN32)
if (UNIX)
    set_target_properties(BulletCollision PROPERTIES IMPORTED_LOCATION "${GPR}/libBulletCollision.so")
endif (UNIX)

add_library(BulletDynamics SHARED IMPORTED)
if (WIN32)
    # TODO: FIX THIS
endif (WIN32)
if (UNIX)
    set_target_properties(BulletDynamics PROPERTIES IMPORTED_LOCATION "${GPR}/libBulletDynamics.so")
endif (UNIX)

add_library(LinearMath SHARED IMPORTED)
if (WIN32)
    # TODO: FIX THIS
endif (WIN32)
if (UNIX)
    set_target_properties(LinearMath PROPERTIES IMPORTED_LOCATION "${GPR}/libLinearMath.so")
endif (UNIX)

add_library(RakNet SHARED IMPORTED)
if (WIN32)
    # TODO: FIX THIS
endif (WIN32)
if (UNIX)
    set_target_properties(RakNet PROPERTIES IMPORTED_LOCATION "${GPR}/libRakNetDLL.so")
endif (UNIX)

target_include_directories(${ENGINE} PRIVATE "${QUICK_ENGINE_SOURCE_DIR}/Core/public")
target_include_directories(${ENGINE} PRIVATE "${QUICK_ENGINE_SOURCE_DIR}/Renderer/public")
target_include_directories(${ENGINE} PRIVATE "${ENGINE_SOURCE_DIR}/public")
target_link_libraries(${ENGINE} QeCore)
target_link_libraries(${ENGINE} QeRenderer)
target_link_libraries(${ENGINE} BulletCollision)
target_link_libraries(${ENGINE} BulletDynamics)
target_link_libraries(${ENGINE} LinearMath)
target_link_libraries(${ENGINE} RakNet)
