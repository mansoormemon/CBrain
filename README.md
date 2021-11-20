![](res/images/cover.png)

## Overview

A library for working with neural networks.

### Links

- [Repository](https://github.com/mansoormemon/CBrain)
- [Documentation](https://mansoormemon.github.io/CBrain)

## Author

Mansoor Ahmed

- [github.com/mansoormemon](https://github.com/mansoormemon)

## Getting Started

### Build Instructions

#### 0. Prerequisites

Before moving on to the build process, ensure the following prerequisites:

- [Git](https://git-scm.com/) (Optional)
- [CMake](https://cmake.org/) (Required)
- [Doxygen](https://www.doxygen.nl/index.html) (Optional)

#### 1. Download sources

There are two methods for getting `CBrain` sources:

- Download an archive of the repository from [GitHub](https://github.com/mansoormemon/CBrain) and extract it.

  or

- Clone the repository to your local maching using `git`.

  ```bash
  git clone https://github.com/mansoormemon/CBrain.git
  ```

Now, `cd` to the `CBrain` project directory:

```bash
cd CBrain
```

#### 2. Configure and Build

The following options are available for controlling the build process:

1. `CMAKE_INSTALL_PREFIX` - tells `CMake` to use this as the installation root directory.
    - **Default:**
        - `/usr/local/` - for Unix based platforms (including macOS).
        - `C:/Program Files (x86)/CBrain` - for Windows platforms (both x86 and x64).

2. `CMAKE_BUILD_TYPE`
    - **Default:** `DEBUG`.

3. `BUILD_SHARED_LIBS` - tells `CMake` to create a shared library.
    - **Default:** `OFF`.

4. `CBRAIN_BUILD_EXAMPLES` - tells `CMake` to build ***examples***.
    - **Default:**
        - `ON` - when configured as a standalone project.
        - `OFF` - when configured as a dependency or a sub-project.
    
5. `CBRAIN_BUILD_DOCS` - tells `CMake` to build ***docs***.
    - **Default:**
      - `ON` - when configured as a standalone project.
      - `OFF` - when configured as a dependency or a sub-project.

These options can be configured as deemed fit.

Now, to build the project, run:

```bash
cmake -Bbuild -DCMAKE_BUILD_TYPE="RELEASE"
cmake --build build
```

#### 3. Installation

The installation process only copies files to predefined destinations as follows:

| Target         | Location                                 |
| -------------- | ---------------------------------------- |
| Public Headers | `${CMAKE_INSTALL_PREFIX}/include/CBrain` |
| Library        | `${CMAKE_INSTALL_PREFIX}/lib/CBrain`     |

Finally, to install the project, run:

```bash
cmake --install build
```

## License

This product is distributed under the [Apache License, Version 2.0](https://www.apache.org/licenses/LICENSE-2.0).
