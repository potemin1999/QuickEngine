set(TEST_GAME "QeTestGame")
set(TEST_GAME_SOURCE_DIR "${SOURCE_DIR}/TestGame")
set(TEST_GAME_SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/libs/include/glad/glad.cpp
        private/main.cpp
        private/startwrap.cpp
        )
add_executable(${TEST_GAME} ${TEST_GAME_SOURCE_FILES})
set_target_properties(${TEST_GAME}
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${BIN_BUILD_PATH}"
        RUNTIME_OUTPUT_DIRECTORY "${BIN_BUILD_PATH}"
        )

target_include_directories(${TEST_GAME} PRIVATE "${SOURCE_DIR}/QuickEngine/Core/public")
target_include_directories(${TEST_GAME} PRIVATE "${SOURCE_DIR}/QuickEngine/Engine/public")
target_include_directories(${TEST_GAME} PRIVATE "${SOURCE_DIR}/QuickEngine/Renderer/public")
target_include_directories(${TEST_GAME} PRIVATE "${TEST_GAME_SOURCE_DIR}/public")
target_link_libraries(${TEST_GAME} QeEngine)
target_link_libraries(${TEST_GAME} ${CMAKE_DL_LIBS})


set(GPR "${PROJECT_SOURCE_DIR}/libs")

add_library(glfw3 STATIC IMPORTED)
if (WIN32)
    set_target_properties(glfw3 PROPERTIES IMPORTED_LOCATION "${GPR}/glfw3.dll")
    set_target_properties(glfw3 PROPERTIES IMPORTED_IMPLIB "${GPR}/glfw3dll.lib")
endif (WIN32)
if (UNIX)
    set_target_properties(glfw3 PROPERTIES IMPORTED_LOCATION "${GPR}/glfw3.so")
endif (UNIX)

target_link_libraries(${TEST_GAME} glfw3)
