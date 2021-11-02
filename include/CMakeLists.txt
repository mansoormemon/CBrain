set(INCLUDES
    "CBrain/CBrain.h"
    "CBrain/Types.h"
    "CBrain/Version.h")

set(CMAKE_INSTALL_INCLUDEDIR "include")

install(DIRECTORY "${CBRAIN_DIR}"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    FILES_MATCHING PATTERN "*.h")
