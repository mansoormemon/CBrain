if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "
FATAL: In-source builds are not allowed.
       Create a separate directory for build files.")
endif()

cmake_minimum_required(VERSION 3.16)

if("${CMAKE_VERSION}" VERSION_LESS 3.21)
    cmake_policy(VERSION "${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}")
else()
    cmake_policy(VERSION 3.21)
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "DEBUG")
endif()

project(CBrain
        VERSION 0.0.1
        DESCRIPTION "A library for working with neural networks."
        HOMEPAGE_URL "https://github.com/mansoormemon/CBrain"
        LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_definitions("-Wall -Wextra")
elseif(CMAKE_C_COMPILER_ID MATCHES "MSVC")
    add_definitions("/W4")
endif()

set(CBRAIN "cbrain")
set(CBRAIN_DIR "CBrain")

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    option(CBRAIN_BUILD_DOCS "Build documentation" ON)
    option(CBRAIN_BUILD_EXAMPLES "Build examples" ON)
else()
    option(CBRAIN_BUILD_DOCS "Build documentation" OFF)
    option(CBRAIN_BUILD_EXAMPLES "Build examples" OFF)
endif()

add_subdirectory("include")
add_subdirectory("src")

if(CBRAIN_BUILD_DOCS)
    find_package(Doxygen)

    if(DOXYGEN_FOUND)
        add_subdirectory("docs")
    else()
        message("
NOTE: Doxygen not found - Cannot build the documentation.
      However, You can safely continue with the build process.")
        set(CBRAIN_BUILD_DOCS OFF)
    endif()
endif()

if(CBRAIN_BUILD_EXAMPLES)
    add_subdirectory("examples")
endif()

add_subdirectory("external")

message("
--------------------------------
CBRAIN BUILD SUMMARY:
--------------------------------
PLATFORM:       ${CMAKE_SYSTEM_NAME}
CMAKE_VERSION:  ${CMAKE_VERSION}
PROJECT:        ${CMAKE_PROJECT_NAME} (v${CMAKE_PROJECT_VERSION})
LANGUAGE:       C${CMAKE_C_STANDARD}
BUILD_TYPE:     ${CMAKE_BUILD_TYPE}
INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}
COMPILER:       ${CMAKE_C_COMPILER_ID}
MAKE_PROGRAM:   ${CMAKE_MAKE_PROGRAM}
---
CBRAIN_BUILD_EXAMPLES: ${CBRAIN_BUILD_EXAMPLES}
CBRAIN_BUILD_DOCS: ${CBRAIN_BUILD_DOCS}
--------------------------------
")
