set(INCLUDES
    "CBrain/Algorithm.h"
    "CBrain/Assert.h"
    "CBrain/CBrain.h"
    "CBrain/Image.h"
    "CBrain/ImgProc.h"
    "CBrain/Tensor.h"
    "CBrain/Types.h"
    "CBrain/Version.h")

set(CMAKE_INSTALL_INCLUDEDIR "include")

install(DIRECTORY "${CBRAIN_DIR}"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
        FILES_MATCHING PATTERN "*.h")
