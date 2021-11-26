set(SOURCES
    "Main.c")

set(TARGET "digit-recognizer")

add_executable("${TARGET}" "${SOURCES}")

target_include_directories("${TARGET}" PUBLIC "${PROJECT_SOURCE_DIR}/include")

target_link_libraries("${TARGET}" PUBLIC "${CBRAIN}")
