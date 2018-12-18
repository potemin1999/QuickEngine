cmake_minimum_required(VERSION 3.8)
project(QuickEngine)
set(CMAKE_CXX_STANDARD 17)

set(OpenGL_GL_PREFERENCE GLVND)

set(LIBS_DIR "${PROJECT_SOURCE_DIR}/libs")

set(BUILD_NAME QuickEngine)
set(BUILD_PATH ${PROJECT_SOURCE_DIR}/build)
set(BIN_BUILD_PATH ${PROJECT_SOURCE_DIR}/build/bin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lstdc++")

if (MINGW)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc -lstdc++ -lpthread")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static -static-libgcc -static-libstdc++ -lstdc++ -lpthread -Wwrite-strings")
    set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "${CMAKE_SHARED_LIBRARY_LINK_C_FLAGS} -static-libgcc -s -lstdc++ -lpthread")
    set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS} -static-libgcc -static-libstdc++ -s -lstdc++ -lpthread -Wwrite-strings")
endif ()

set(CORE_PROJ "QeCore")

add_subdirectory("Sources")

set(EXECUTABLE_OUTPUT_PATH ${BIN_BUILD_PATH})
