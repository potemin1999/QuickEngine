set(TEXTURE_CONVERTER "TextureConverter")
set(TEXTURE_CONVERTER_SOURCE_DIR "${UTILS_SOURCE_DIR}/TextureConverter")
set(TEXTURE_CONVERTER_SOURCE_FILES
        private/texture_converter.cpp)

add_executable(${TEXTURE_CONVERTER} ${TEXTURE_CONVERTER_SOURCE_FILES})
target_include_directories(${TEXTURE_CONVERTER} PRIVATE "${TEXTURE_CONVERTER_SOURCE_DIR}/public")