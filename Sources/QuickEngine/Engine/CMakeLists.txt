set(ENGINE "QeEngine")
set(ENGINE_SOURCE_DIR "${QUICK_ENGINE_SOURCE_DIR}/Engine")
set(ENGINE_SOURCE_FILES
        private/actors/model_loader.cpp
        private/actors/gameobject.cpp
        private/input/input_event_manager.cpp
        private/materials/material_manager.cpp
        private/util/string_helper.cpp
        private/util/utils.cpp
        private/camera.cpp
        private/engine.cpp
        private/engine_utils.cpp
        private/world.cpp
        )

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

target_include_directories(${ENGINE} PRIVATE "${QUICK_ENGINE_SOURCE_DIR}/Core/public")
target_include_directories(${ENGINE} PRIVATE "${QUICK_ENGINE_SOURCE_DIR}/Renderer/public")
target_include_directories(${ENGINE} PRIVATE "${ENGINE_SOURCE_DIR}/public")
target_link_libraries(${ENGINE} QeCore)
target_link_libraries(${ENGINE} QeRenderer)
target_link_libraries(${ENGINE} BulletCollision)
target_link_libraries(${ENGINE} BulletDynamics)
target_link_libraries(${ENGINE} LinearMath)
