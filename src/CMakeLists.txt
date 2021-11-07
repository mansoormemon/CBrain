option(BUILD_SHARED_LIBS "Build shared/dynamic library." OFF)

set(PRIVATE_INCLUDES
    "Private/Image.h"
    "Private/Memory.h")

set(SOURCES
    "Image.c")

set(PRIVATE_SOURCES
    "Private/Image.c"
    "Private/Memory.c")

add_library("${CBRAIN}" "${SOURCES}" "${PRIVATE_INCLUDES}" "${PRIVATE_SOURCES}")

target_include_directories("${CBRAIN}"
    PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/external")

target_link_libraries("${CBRAIN}" "m")
