set(RENDERER "QeRenderer")
set(RENDERER_SOURCE_DIR "${QUICK_ENGINE_SOURCE_DIR}/Renderer")
set(RENDERER_SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/libs/include/glad/glad.cpp
        private/basic/framebuffer.cpp
        private/basic/shader.cpp
        private/basic/texture.cpp
        private/effects/post_pass.cpp
        private/geometry/geometry_pass.cpp
        private/lighting/deferred_lighting_pass.cpp
        private/postprocess/ssao_pass.cpp
        private/util/render_data_storage.cpp
        private/util/shader_manager.cpp
        private/util/texture_manager.cpp
        private/renderer.cpp
        )

add_library(${RENDERER} SHARED ${RENDERER_SOURCE_FILES})
set_target_properties(${RENDERER} PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(${RENDERER}
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${BIN_BUILD_PATH}"
        RUNTIME_OUTPUT_DIRECTORY "${BIN_BUILD_PATH}"
        )

target_include_directories(${RENDERER} PRIVATE "${QUICK_ENGINE_SOURCE_DIR}/Core/public")
target_include_directories(${RENDERER} PUBLIC "${QUICK_ENGINE_SOURCE_DIR}/Engine/public")
target_include_directories(${RENDERER} PRIVATE "${RENDERER_SOURCE_DIR}/public")
target_include_directories(${RENDERER} PRIVATE "${RENDERER_SOURCE_DIR}/private")

add_library(glfw3 STATIC IMPORTED)
if (WIN32)
    set_target_properties(glfw3 PROPERTIES IMPORTED_LOCATION "${GPR}/glfw3.dll")
    set_target_properties(glfw3 PROPERTIES IMPORTED_IMPLIB "${GPR}/glfw3dll.lib")
endif (WIN32)
if (UNIX)
    set_target_properties(glfw3 PROPERTIES IMPORTED_LOCATION "${GPR}/libglfw.so")
endif (UNIX)

add_library(ilu STATIC IMPORTED)
if (WIN32)
    set_target_properties(ilu PROPERTIES IMPORTED_LOCATION "${GPR}/ilu.dll")
    set_target_properties(ilu PROPERTIES IMPORTED_IMPLIB "${GPR}/ilu.lib")
endif (WIN32)
if (UNIX)
    set_target_properties(ilu PROPERTIES IMPORTED_LOCATION "${GPR}/libILU.so")
endif (UNIX)

if (UNIX)
    find_package(OpenGL REQUIRED)
endif (UNIX)

add_library(devil STATIC IMPORTED)
if (WIN32)
    set_target_properties(devil PROPERTIES IMPORTED_LOCATION "${GPR}/devil.dll")
    set_target_properties(devil PROPERTIES IMPORTED_IMPLIB "${GPR}/devil.lib")
endif (WIN32)
if (UNIX)
    set_target_properties(devil PROPERTIES IMPORTED_LOCATION "${GPR}/libIL.so")
endif (UNIX)

target_link_libraries(${RENDERER} glfw3)
target_link_libraries(${RENDERER} ilu)
target_link_libraries(${RENDERER} devil)