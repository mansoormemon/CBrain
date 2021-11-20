set(CBRAIN_DOCS "cbrain_docs")

set(CBRAIN_DOXYGEN_PROJECT_LOGO "${PROJECT_SOURCE_DIR}/res/images/logo.png")
set(CBRAIN_DOXYGEN_STRIP_FROM_PATH "${PROJECT_SOURCE_DIR}/include")

set(CBRAIN_DOXYGEN_IMAGE_PATH "${PROJECT_SOURCE_DIR}/res/images")

set(CBRAIN_DOXYGEN_STYLESHEET_EXTRA
    "${PROJECT_SOURCE_DIR}/external/doxygen-awesome-css/doxygen-awesome.css")

set(CBRAIN_DOXYGEN_CUSTOM_STYLESHEET "${CMAKE_CURRENT_SOURCE_DIR}/custom/custom.css")
set(CBRAIN_DOXYGEN_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/custom/header.html")
set(CBRAIN_DOXYGEN_FOOTER "${CMAKE_CURRENT_SOURCE_DIR}/custom/footer.html")
set(CBRAIN_DOXYGEN_MAIN_README "${PROJECT_SOURCE_DIR}/README.md")
set(CBRAIN_DOXYGEN_INPUTS "${CBRAIN_DOXYGEN_MAIN_README} \
                           ${PROJECT_SOURCE_DIR}/include/${CBRAIN_DIR} \
                           ${PROJECT_SOURCE_DIR}/src/")

set(CBRAIN_DOXYGEN_EXTRAS "${CMAKE_CURRENT_SOURCE_DIR}/custom/favicon.ico")

set(CBRAIN_DOXYGEN_IGNORE_PREFIX "")

set(DOXYFILE_IN "Doxyfile.in")
set(DOXYFILE "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")

configure_file(${DOXYFILE_IN} ${DOXYFILE} @ONLY)

add_custom_target(${CBRAIN_DOCS} ALL
                  COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE}
                  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                  COMMENT "Generating API documentation with Doxygen")
