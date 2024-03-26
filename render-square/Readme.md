# Rendering a Square with additional instructions

## Quick Start

You can utilize the Makefile in this folder to build and run the code. Meson is the build system used in this project.

Instructions overview:
    - `make install-glfw` & `make install-glm` are used to pull the library sources to this folder and to be linked by meson when building the project.
    - `make build` is used to build the project with meson.
    - `make compile` is used to compile the binary into a sub-folder called `builddir`.
    - `make run` is used to locate the binary and execute it.

There might be complications as to how this would be achieved on a different operating system as it has not been tested yet.
