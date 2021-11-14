option(BUILD_SHARED_LIBS "Build shared/dynamic library." OFF)

set(PRIVATE_INCLUDES
    "Private/Image.h"
    "Private/Memory.h"
    "Private/Tensor.h")

set(SOURCES
    "Algorithm.c"
    "Assert.c"
    "Image.c"
    "ImgProc.c"
    "Tensor.c")

set(PRIVATE_SOURCES
    "Private/Image.c"
    "Private/Memory.c"
    "Private/Tensor.c")

add_library("${CBRAIN}" "${SOURCES}" "${PRIVATE_INCLUDES}" "${PRIVATE_SOURCES}")

target_include_directories("${CBRAIN}"
                           PUBLIC
                           "${PROJECT_SOURCE_DIR}/include"
                           "${PROJECT_SOURCE_DIR}/external")

target_link_libraries("${CBRAIN}" "m")

if(BUILD_SHARED_LIBS)
    install(TARGETS "${CBRAIN}"
            LIBRARY DESTINATION "lib/${CBRAIN}")
else()
    install(TARGETS "${CBRAIN}"
            ARCHIVE DESTINATION "lib/${CBRAIN}")
endif()
