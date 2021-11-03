option(BUILD_SHARED_LIBS "Build shared/dynamic library." OFF)

set(PRIVATE_INCLUDES
    "Private/Memory.h")

set(SOURCES
    "Image.c")

add_library("${CBRAIN}" "${SOURCES}" "${PRIVATE_INCLUDES}")

target_include_directories("${CBRAIN}"
    PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/external")

target_link_libraries("${CBRAIN}" "m")
