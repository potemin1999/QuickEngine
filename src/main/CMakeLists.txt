cmake_minimum_required(VERSION 3.8)

set(CORE_SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/libs/include/glad/glad.cpp
        qe/core/graphics/basic/camera.cpp
        qe/core/engine.cpp
        qe/core/engine_utils.cpp
        qe/core/main.cpp
        qe/core/graphics/materials/material_manager.cpp
        qe/core/actors/model_loader.cpp
        qe/core/graphics/util/shader_manager.cpp
        qe/core/util/string_helper.cpp
        qe/core/graphics/renderer.cpp
        qe/core/graphics/util/texture_manager.cpp
        qe/core/input/input_event_manager.cpp
        qe/core/world/world.cpp
        qe/core/graphics/effects/post_pass.cpp
        qe/core/graphics/basic/shader.cpp
        qe/core/graphics/basic/framebuffer.cpp
        qe/core/graphics/basic/texture.cpp
        qe/core/graphics/geometry/geometry_pass.cpp
        qe/core/graphics/lighting/deferred_lighting_pass.cpp
        qe/core/graphics/lighting/ssao_pass.cpp
        qe/core/graphics/util/render_data_storage.cpp
        qe/core/util/utils.cpp
        qe/core/util/utils.h
        qe/core/actors/object.cpp
        )

set(GPR "${PROJECT_SOURCE_DIR}/libs")
set(EXECUTABLE_OUTPUT_PATH ${BIN_BUILD_PATH})

add_library(${CORE_PROJ} SHARED ${CORE_SOURCE_FILES})
target_include_directories(${CORE_PROJ} PRIVATE "${PROJECT_SOURCE_DIR}/src/main")
target_include_directories(${CORE_PROJ} PRIVATE "${PROJECT_SOURCE_DIR}/include/main")

set_target_properties(${CORE_PROJ}
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${BIN_BUILD_PATH}"
        RUNTIME_OUTPUT_DIRECTORY "${BIN_BUILD_PATH}"
        )

#target_compile_options(Engine PUBLIC "-static -static-libgcc -static-libstdc++")

add_library(glfw3 STATIC IMPORTED)
if (WIN32)
    set_target_properties(glfw3 PROPERTIES IMPORTED_LOCATION "${GPR}/glfw3.dll")
    set_target_properties(glfw3 PROPERTIES IMPORTED_IMPLIB "${GPR}/glfw3dll.lib")
endif (WIN32)
if (UNIX)
    set_target_properties(glfw3 PROPERTIES IMPORTED_LOCATION "${GPR}/glfw3.so")
endif (UNIX)
target_link_libraries(${CORE_PROJ} glfw3)


if (WIN32)
    add_library(ilu STATIC IMPORTED)
    set_target_properties(ilu PROPERTIES IMPORTED_LOCATION "${GPR}/ilu.dll")
    set_target_properties(ilu PROPERTIES IMPORTED_IMPLIB "${GPR}/ilu.lib")
endif (WIN32)
if (UNIX)
    find_package(OpenGL REQUIRED)
endif (UNIX)
target_link_libraries(${CORE_PROJ} ilu)


add_library(devil STATIC IMPORTED)
if (WIN32)
    set_target_properties(devil PROPERTIES IMPORTED_LOCATION "${GPR}/devil.dll")
    set_target_properties(devil PROPERTIES IMPORTED_IMPLIB "${GPR}/devil.lib")
endif (WIN32)
if (UNIX)
    set_target_properties(devil PROPERTIES IMPORTED_LOCATION "${GPR}/devil.so")
endif (UNIX)
target_link_libraries(${CORE_PROJ} devil)

#

if (WIN32)
    configure_file(${GPR}/devil.dll ${BIN_BUILD_PATH}/DevIL.dll COPYONLY)
    configure_file(${GPR}/glfw3.dll ${BIN_BUILD_PATH}/glfw3.dll COPYONLY)
endif (WIN32)
if (UNIX)
    configure_file(${GPR}/devil.so ${BIN_BUILD_PATH}/libDevIL.so COPYONLY)
    configure_file(${GPR}/glfw3.so ${BIN_BUILD_PATH}/libGLFW3.so COPYONLY)
endif (UNIX)
configure_file(qe/settings.ini ${BIN_BUILD_PATH}/settings.ini COPYONLY)

file(COPY ${PROJECT_SOURCE_DIR}/materials/ DESTINATION ${BUILD_PATH}/materials/)
file(COPY ${PROJECT_SOURCE_DIR}/textures/ DESTINATION ${BUILD_PATH}/textures/)
file(COPY ${PROJECT_SOURCE_DIR}/models/ DESTINATION ${BUILD_PATH}/models/)
file(COPY ${PROJECT_SOURCE_DIR}/shaders/ DESTINATION ${BUILD_PATH}/shaders/)

