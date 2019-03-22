set(TEST_GAME "QeTestGame")
set(TEST_GAME_SOURCE_DIR "${SOURCE_DIR}/TestGame")
set(TEST_GAME_SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/libs/include/glad/glad.cpp
        private/Main.cpp
        private/Wrapper.cpp
        private/Player.cpp
        )

# include game headers
include_directories("${TEST_GAME_SOURCE_DIR}/public")
# include engine headers
include_directories("${PROJECT_SOURCE_DIR}/Sources/QuickEngine/public")
# include libraries
include_directories("${PROJECT_SOURCE_DIR}/libs/includeRakNet")
include_directories("${PROJECT_SOURCE_DIR}/libs/include")
include_directories("${PROJECT_SOURCE_DIR}/libs/bullet")
# include nanoVG
include_directories("Sources/TestGame/nanogui/include/nanovg")
# include nanogui dependencies
include_directories("nanogui/ext/coro")
include_directories("nanogui/ext/eigen")
include_directories("nanogui/ext/nanovg/src")
include_directories("nanogui/include")

add_executable(${TEST_GAME} ${TEST_GAME_SOURCE_FILES})
set_target_properties(${TEST_GAME}
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${BIN_BUILD_PATH}"
        RUNTIME_OUTPUT_DIRECTORY "${BIN_BUILD_PATH}"
        )


add_library(glfw3 SHARED IMPORTED)
if (WIN32)
    set_target_properties(glfw3 PROPERTIES IMPORTED_LOCATION "${LIBS_DIR}/glfw3.dll")
    set_target_properties(glfw3 PROPERTIES IMPORTED_IMPLIB "${LIBS_DIR}/glfw3dll.lib")
endif (WIN32)
if (UNIX)
    set_target_properties(glfw3 PROPERTIES IMPORTED_LOCATION "${LIBS_DIR}/libglfw.so")
endif (UNIX)

add_library(NanoGUI SHARED IMPORTED)
if (WIN32)
    set_target_properties(NanoGUI PROPERTIES IMPORTED_LOCATION "${LIBS_DIR}/libnanogui.dll")
    set_target_properties(NanoGUI PROPERTIES IMPORTED_IMPLIB "${LIBS_DIR}/libnanogui.dll.a")
endif (WIN32)
if (UNIX)
    set_target_properties(NanoGUI PROPERTIES IMPORTED_LOCATION "${LIBS_DIR}/libnanogui.so")
endif (UNIX)


if (WIN32)
    add_library(BulletDynamics STATIC IMPORTED)
    set_target_properties(BulletDynamics PROPERTIES IMPORTED_LOCATION "${LIBS_DIR}/libBulletDynamics.a")
endif (WIN32)
if (UNIX)
    add_library(BulletDynamics SHARED IMPORTED)
    set_target_properties(BulletDynamics PROPERTIES IMPORTED_LOCATION "${LIBS_DIR}/libBulletDynamics.so")
endif (UNIX)


if (WIN32)
    add_library(BulletCollision STATIC IMPORTED)
    set_target_properties(BulletCollision PROPERTIES IMPORTED_LOCATION "${LIBS_DIR}/libBulletCollision.a")
endif (WIN32)
if (UNIX)
    add_library(BulletCollision SHARED IMPORTED)
    set_target_properties(BulletCollision PROPERTIES IMPORTED_LOCATION "${LIBS_DIR}/libBulletCollision.so")
endif (UNIX)


if (WIN32)
    add_library(LinearMath STATIC IMPORTED)
    set_target_properties(LinearMath PROPERTIES IMPORTED_LOCATION "${LIBS_DIR}/libLinearMath.a")
endif (WIN32)
if (UNIX)
    add_library(LinearMath SHARED IMPORTED)
    set_target_properties(LinearMath PROPERTIES IMPORTED_LOCATION "${LIBS_DIR}/libLinearMath.so")
endif (UNIX)


target_link_libraries(${TEST_GAME} BulletDynamics BulletCollision LinearMath)
target_link_libraries(${TEST_GAME} QeEngine)
target_link_libraries(${TEST_GAME} ${CMAKE_DL_LIBS})
target_link_libraries(${TEST_GAME} glfw3)
target_link_libraries(${TEST_GAME} NanoGUI)