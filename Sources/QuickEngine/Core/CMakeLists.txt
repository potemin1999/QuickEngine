set(CORE "QeCore")
set(CORE_SOURCE_DIR "${QUICK_ENGINE_SOURCE_DIR}/Core")
set(CORE_SOURCE_FILES
        private/Core.cpp
        )

add_library(${CORE} SHARED ${CORE_SOURCE_FILES})
set_target_properties(${CORE} PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(${CORE}
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${BIN_BUILD_PATH}"
        RUNTIME_OUTPUT_DIRECTORY "${BIN_BUILD_PATH}"
        )

target_include_directories(${CORE} PRIVATE "${CORE_SOURCE_DIR}/public")