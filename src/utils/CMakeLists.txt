cmake_minimum_required(VERSION 3.8)

set(TEXTURE_UTILS_SOURCE_FILES
        qe/utils/textures/texture_converter.cpp)

set(GPR "${PROJECT_SOURCE_DIR}/libs")
set (EXECUTABLE_OUTPUT_PATH ${BIN_BUILD_PATH}/utils)

set(TEXTURE_UTILS TextureUtils)
add_executable(${TEXTURE_UTILS} ${TEXTURE_UTILS_SOURCE_FILES})
target_include_directories(${TEXTURE_UTILS} PRIVATE "${PROJECT_SOURCE_DIR}/include/utils")