set(TEST_GAME "QeTestGame")
set(TEST_GAME_SOURCE_DIR "${SOURCE_DIR}/TestGame")
set(TEST_GAME_SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/libs/include/glad/glad.cpp
        private/Main.cpp
        private/Wrapper.cpp
        public/Crate_01.h private/Player.cpp public/Player.h public/Main.h)


SET(NANOGUI_SOURCE_FILES
        nanogui/ext/nanovg/src/nanovg.c

        nanogui/include/nanogui/glutil.h
        nanogui/include/nanogui/common.h
        nanogui/include/nanogui/widget.h
        nanogui/include/nanogui/theme.h
        nanogui/include/nanogui/layout.h
        nanogui/include/nanogui/screen.h
        nanogui/include/nanogui/label.h
        nanogui/include/nanogui/window.h
        nanogui/include/nanogui/popup.h
        nanogui/include/nanogui/checkbox.h
        nanogui/include/nanogui/button.h
        nanogui/include/nanogui/popupbutton.h
        nanogui/include/nanogui/combobox.h
        nanogui/include/nanogui/progressbar.h
        nanogui/include/nanogui/slider.h
        nanogui/include/nanogui/messagedialog.h
        nanogui/include/nanogui/textbox.h
        nanogui/include/nanogui/imagepanel.h
        nanogui/include/nanogui/imageview.h
        nanogui/include/nanogui/vscrollpanel.h
        nanogui/include/nanogui/colorwheel.h
        nanogui/include/nanogui/colorpicker.h
        nanogui/include/nanogui/graph.h
        nanogui/include/nanogui/stackedwidget.h
        nanogui/include/nanogui/tabheader.h
        nanogui/include/nanogui/tabwidget.h
        nanogui/include/nanogui/glcanvas.h
        nanogui/include/nanogui/formhelper.h
        nanogui/include/nanogui/toolbutton.h
        nanogui/include/nanogui/opengl.h
        nanogui/include/nanogui/nanogui.h
        nanogui/include/nanogui/serializer/core.h
        nanogui/include/nanogui/serializer/opengl.h
        nanogui/include/nanogui/serializer/sparse.h
        )

include_directories("nanogui/include")

add_executable(${TEST_GAME} ${NANOGUI_SOURCE_FILES} ${TEST_GAME_SOURCE_FILES})
set_target_properties(${TEST_GAME}
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${BIN_BUILD_PATH}"
        RUNTIME_OUTPUT_DIRECTORY "${BIN_BUILD_PATH}"
        )


add_library(glfw3 SHARED IMPORTED)
if (WIN32)
    set_target_properties(glfw3 PROPERTIES IMPORTED_LOCATION "${GPR}/glfw3.dll")
    set_target_properties(glfw3 PROPERTIES IMPORTED_IMPLIB "${GPR}/glfw3dll.lib")
endif (WIN32)
if (UNIX)
    set_target_properties(glfw3 PROPERTIES IMPORTED_LOCATION "${GPR}/libglfw.so")
endif (UNIX)

add_library(NanoGUI SHARED IMPORTED)
if (WIN32)
    # TODO: FIX THIS
endif (WIN32)
if (UNIX)
    set_target_properties(NanoGUI PROPERTIES IMPORTED_LOCATION "${GPR}/libnanogui.so")
endif (UNIX)


target_include_directories(${TEST_GAME} PRIVATE "${SOURCE_DIR}/QuickEngine/Core/public")
target_include_directories(${TEST_GAME} PRIVATE "${SOURCE_DIR}/QuickEngine/Engine/public")
target_include_directories(${TEST_GAME} PRIVATE "${SOURCE_DIR}/QuickEngine/Renderer/public")
target_include_directories(${TEST_GAME} PRIVATE "${TEST_GAME_SOURCE_DIR}/public")
target_link_libraries(${TEST_GAME} QeCore)
target_link_libraries(${TEST_GAME} QeEngine)
target_link_libraries(${TEST_GAME} QeRenderer)
target_link_libraries(${TEST_GAME} ${CMAKE_DL_LIBS})


target_link_libraries(${TEST_GAME} glfw3)
target_link_libraries(${TEST_GAME} NanoGUI)