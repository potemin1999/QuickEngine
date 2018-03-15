cmake_minimum_required(VERSION 3.8)
project(QuickEngine)
set(CMAKE_CXX_STANDARD 17)

set(BUILD_NAME QuickEngine)
set(BUILD_PATH ${PROJECT_SOURCE_DIR})
include_directories("include")

if (MINGW)
     set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc -lstdc++ -lpthread")
     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static -static-libgcc -static-libstdc++ -lstdc++ -lpthread -Wwrite-strings")
     set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "${CMAKE_SHARED_LIBRARY_LINK_C_FLAGS} -static-libgcc -s -lstdc++ -lpthread")
     set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS} -static-libgcc -static-libstdc++ -s -lstdc++ -lpthread -Wwrite-strings")
endif()

set (EXECUTABLE_OUTPUT_PATH ${BUILD_PATH})

include_directories("${PROJECT_SOURCE_DIR}/src")

set(GRAPH_PROJ "QeGraphics")
add_subdirectory("src/graphics")

add_executable(${BUILD_NAME} src/startwrap.cpp)
#set_target_properties(${BUILD_NAME} PROPERTIES EXECUTABLE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}")

target_link_libraries(${BUILD_NAME} ${GRAPH_PROJ})



