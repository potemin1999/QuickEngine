cmake_minimum_required(VERSION 3.8)


set(GRAPHIC_SOURCE_FILES
        ../../include/glad/glad.h ../../include/glad/glad.cpp
        ../../include/glfw/glfw3.h
        ../../include/il/il.h
        ../../include/il/ilut.h
        ../../include/il/ilu.h
        #settings.h
        core/camera.h core/camera.cpp
        core/engine.h core/engine.cpp
        core/engine_utils.cpp
        core/main.h core/main.cpp
        core/material.h core/material.cpp
        core/max_model.h
        core/test_model.h
        core/mesh.h
        core/model_loader.h core/model_loader.cpp
        core/shader.h core/shader.cpp
        core/string_helper.h core/string_helper.cpp
        core/texture.h core/texture.cpp
        core/settings.h
        core/object.h
        core/basic/framebuffer.h
        defines.h
        core/basic/texture.h
        core/basic/shader.h core/basic/render_pass.h core/lighting/ssao_shader.h core/geometry/geometry_pass.h core/lighting/ssao_pass.h)

set(GPR "${PROJECT_SOURCE_DIR}/libs")


#link_directories("${GPR}include ${GPR}libs ${GPR}src")

#include_directories("${GPR}libs")


add_library(${GRAPH_PROJ} SHARED ${GRAPHIC_SOURCE_FILES})
target_include_directories(${GRAPH_PROJ} PRIVATE "core")

set_target_properties(${GRAPH_PROJ}
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${BUILD_PATH}"
        RUNTIME_OUTPUT_DIRECTORY "${BUILD_PATH}"
        )

#target_compile_options(Engine PUBLIC "-static -static-libgcc -static-libstdc++")

add_library(glfw STATIC IMPORTED)
set_target_properties(glfw PROPERTIES IMPORTED_LOCATION "${GPR}/glfw3.dll")
set_target_properties(glfw PROPERTIES IMPORTED_IMPLIB "${GPR}/glfw3dll.lib")
#set_target_properties(glfw PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}")
target_link_libraries(${GRAPH_PROJ} glfw)

add_library(ilu STATIC IMPORTED)
set_target_properties(ilu PROPERTIES IMPORTED_LOCATION "${GPR}/ilu.dll")
set_target_properties(ilu PROPERTIES IMPORTED_IMPLIB "${GPR}/ilu.lib")
#target_link_libraries(${GRAPH_PROJ} ilu)

add_library(glew STATIC IMPORTED)
set_target_properties(glew PROPERTIES IMPORTED_LOCATION "${GPR}/glew32.lib")
target_link_libraries(${GRAPH_PROJ} glew)


#add_library(ilut STATIC IMPORTED)
#set_target_properties(ilut PROPERTIES IMPORTED_LOCATION "W:\\Projects\\QuickEngine\\libs\\ilut.dll")
#target_link_libraries(${GRAPH_PROJ} ilut)

add_library(devil STATIC IMPORTED)
set_target_properties(devil PROPERTIES IMPORTED_LOCATION "${GPR}/devil.dll")
set_target_properties(devil PROPERTIES IMPORTED_IMPLIB "${GPR}/devil.lib")
target_link_libraries(${GRAPH_PROJ} devil)

configure_file(${GPR}/devil.dll ${BUILD_PATH}/DevIL.dll COPYONLY)
configure_file(${GPR}/glfw3.dll ${BUILD_PATH}/glfw3.dll COPYONLY)
configure_file(settings.ini ${BUILD_PATH}/settings.ini COPYONLY)
#target_link_libraries(Engine glfw devil)

