cmake_minimum_required(VERSION 3.8)

set(CORE_SOURCE_FILES
        ../../include/glad/glad.h ../../include/glad/glad.cpp
        ../../include/glfw/glfw3.h
        ../../include/il/il.h
        ../../include/il/ilut.h
        ../../include/il/ilu.h
        #settings.h
        qe/core/graphics/basic/camera.h
        qe/core/graphics/basic/camera.cpp
        qe/core/engine.h
        qe/core/engine.cpp
        qe/core/engine_utils.cpp
        qe/core/main.h
        qe/core/main.cpp
        qe/core/graphics/materials/material_manager.h
        qe/core/graphics/materials/material_manager.cpp
        qe/core/max_model.h
        qe/core/test_model.h
        qe/core/mesh.h
        qe/core/model_loader.h
        qe/core/model_loader.cpp
        qe/core/graphics/util/shader_manager.h
        qe/core/graphics/util/shader_manager.cpp
        qe/core/util/string_helper.h
        qe/core/util/string_helper.cpp
        qe/core/settings.h
        qe/core/object.h
        qe/core/graphics/renderer.h
        qe/core/graphics/renderer.cpp
        qe/core/graphics/defines.h
        qe/core/graphics/basic/framebuffer.h
        qe/core/graphics/basic/texture.h
        qe/core/graphics/basic/shader.h
        qe/core/graphics/basic/render_pass.h
        qe/core/graphics/lighting/ssao_shader.h
        qe/core/graphics/geometry/geometry_pass.h
        qe/core/graphics/lighting/ssao_pass.h
        qe/core/graphics/effects/post_pass.h
        qe/core/graphics/lighting/deferred_lighing_pass.h
        qe/core/graphics/lighting/light_source.h
        qe/core/graphics/lighting/point_light_source.h
        qe/core/graphics/lighting/directional_light_source.h
        qe/core/graphics/util/texture_manager.h
        qe/core/graphics/util/texture_manager.cpp
        qe/core/actors/sky_hemisphere.h
        qe/core/actors/actor.h
        boot.h
        qe/core/input/input_event.h
        qe/core/input/input_receiver.h
        qe/core/input/input_event_manager.h
        qe/core/input/input_event_manager.cpp
        qe/core/graphics/basic/material.h
        qe/core/util/random.h
        qe/core/world/world.cpp
        qe/core/world/world.h
        qe/core/input/input_processing_result.h
        qe/core/graphics/renderer_input.cpp
        qe/core/input/input_key.h
        qe/core/util/log.h
        qe/core/engine_context.h
        )

set(GPR "${PROJECT_SOURCE_DIR}/libs")

add_library(${CORE_PROJ} SHARED ${CORE_SOURCE_FILES})
target_include_directories(${CORE_PROJ} PRIVATE "${PROJECT_SOURCE_DIR}/src/main")

set_target_properties(${CORE_PROJ}
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${BUILD_PATH}"
        RUNTIME_OUTPUT_DIRECTORY "${BUILD_PATH}"
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
    configure_file(${GPR}/devil.dll ${BUILD_PATH}/libDevIL.dll COPYONLY)
    configure_file(${GPR}/glfw3.dll ${BUILD_PATH}/libGLFW3.dll COPYONLY)
endif (WIN32)
if (UNIX)
    configure_file(${GPR}/devil.so ${BUILD_PATH}/libDevIL.so COPYONLY)
    configure_file(${GPR}/glfw3.so ${BUILD_PATH}/libGLFW3.so COPYONLY)
endif (UNIX)
configure_file(qe/settings.ini ${BUILD_PATH}/settings.ini COPYONLY)

