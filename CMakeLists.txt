cmake_minimum_required(VERSION 3.8)
project(QuickEngine)
set(CMAKE_CXX_STANDARD 17)

set(BUILD_NAME QuickEngine)
set(BUILD_PATH ${PROJECT_SOURCE_DIR}/build)
set(BIN_BUILD_PATH ${PROJECT_SOURCE_DIR}/build/bin)
include_directories("${PROJECT_SOURCE_DIR}/libs/include")

if (MINGW)
     set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc -lstdc++ -lpthread")
     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static -static-libgcc -static-libstdc++ -lstdc++ -lpthread -Wwrite-strings")
     set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "${CMAKE_SHARED_LIBRARY_LINK_C_FLAGS} -static-libgcc -s -lstdc++ -lpthread")
     set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS} -static-libgcc -static-libstdc++ -s -lstdc++ -lpthread -Wwrite-strings")
endif()

set(CORE_PROJ "QeCore")

add_subdirectory("src/main")
add_subdirectory("src/utils")

set(EXECUTABLE_OUTPUT_PATH ${BIN_BUILD_PATH})
add_executable(${BUILD_NAME} src/startwrap.cpp)
target_include_directories(${BUILD_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src/main/)
target_link_libraries(${BUILD_NAME} ${CORE_PROJ})



