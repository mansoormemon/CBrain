set(EXAMPLES
    "EgImage.c")

foreach(EXAMPLE ${EXAMPLES})
    get_filename_component(EX_TARGET "${EXAMPLE}" NAME_WE)
    add_executable("${EX_TARGET}" "${EXAMPLE}")

    target_include_directories("${EX_TARGET}" PUBLIC "${PROJECT_SOURCE_DIR}/include")

    target_link_libraries("${EX_TARGET}" PUBLIC "${CBRAIN}")
endforeach()
