set(TEST_GAME "QeTestGame")
set(TEST_GAME_SOURCE_DIR "${SOURCE_DIR}/TestGame")
set(TEST_GAME_SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/libs/include/glad/glad.cpp
        private/Main.cpp
        private/Wrapper.cpp
        public/Crate_01.h private/Player.cpp public/Player.h public/Main.h)
add_executable(${TEST_GAME} ${TEST_GAME_SOURCE_FILES})
set_target_properties(${TEST_GAME}
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${BIN_BUILD_PATH}"
        RUNTIME_OUTPUT_DIRECTORY "${BIN_BUILD_PATH}"
        )

add_library(BulletCollision STATIC IMPORTED)
if (WIN32)
    # TODO: FIX THIS
endif (WIN32)
if (UNIX)
    set_target_properties(BulletCollision PROPERTIES IMPORTED_LOCATION "${GPR}/libBulletCollision.so")
endif (UNIX)

add_library(BulletDynamics STATIC IMPORTED)
if (WIN32)
    # TODO: FIX THIS
endif (WIN32)
if (UNIX)
    set_target_properties(BulletCollision PROPERTIES IMPORTED_LOCATION "${GPR}/libBulletDynamics.so")
endif (UNIX)

add_library(LinearMath STATIC IMPORTED)
if (WIN32)
    # TODO: FIX THIS
endif (WIN32)
if (UNIX)
    set_target_properties(BulletCollision PROPERTIES IMPORTED_LOCATION "${GPR}/libLinearMath.so")
endif (UNIX)

target_include_directories(${TEST_GAME} PRIVATE "${SOURCE_DIR}/QuickEngine/Core/public")
target_include_directories(${TEST_GAME} PRIVATE "${SOURCE_DIR}/QuickEngine/Engine/public")
target_include_directories(${TEST_GAME} PRIVATE "${SOURCE_DIR}/QuickEngine/Renderer/public")
target_include_directories(${TEST_GAME} PRIVATE "${TEST_GAME_SOURCE_DIR}/public")
target_link_libraries(${TEST_GAME} QeEngine)
target_link_libraries(${TEST_GAME} ${CMAKE_DL_LIBS})


add_library(glfw3 STATIC IMPORTED)
if (WIN32)
    set_target_properties(glfw3 PROPERTIES IMPORTED_LOCATION "${GPR}/glfw3.dll")
    set_target_properties(glfw3 PROPERTIES IMPORTED_IMPLIB "${GPR}/glfw3dll.lib")
endif (WIN32)
if (UNIX)
    set_target_properties(glfw3 PROPERTIES IMPORTED_LOCATION "${GPR}/libglfw.so")
endif (UNIX)


target_link_libraries(${TEST_GAME} glfw3)
target_link_libraries(${ENGINE} BulletCollision)
target_link_libraries(${ENGINE} BulletDynamics)
target_link_libraries(${ENGINE} LinearMath)
