set(ENGINE "QeEngine")
set(ENGINE_SOURCE_DIR "${QUICK_ENGINE_SOURCE_DIR}/Engine")
set(ENGINE_SOURCE_FILES
        private/actors/model_loader.cpp
        private/actors/object.cpp
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

target_include_directories(${ENGINE} PRIVATE "${QUICK_ENGINE_SOURCE_DIR}/Core/public")
target_include_directories(${ENGINE} PRIVATE "${QUICK_ENGINE_SOURCE_DIR}/Renderer/public")
target_include_directories(${ENGINE} PRIVATE "${ENGINE_SOURCE_DIR}/public")
target_link_libraries(${ENGINE} QeCore)
target_link_libraries(${ENGINE} QeRenderer)
