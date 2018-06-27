# jkps-gl

This library is primarily meant to be used for creating my own small graphics projects.

To build sample project run cmake in the /sample directory

It currently depends on the following packages to build for windows:
glew
glfw3


## Building for WebAssembly

First download and install emscripten (https://kripken.github.io/emscripten-site/docs/getting_started/downloads.html)


To build the sample project, create a new build directory inside the sample directory. Then run cmake like this:

```
cmake .. -DWASM_BUILD=1 -DCMAKE_TOOLCHAIN_FILE=../../cmake/Modules/Platform/Emscripten.cmake -DEMSCRIPTEN_ROOT_PATH=PATH_TO_INSTALLED_EMSDK_GOES_HERE
```

where *PATH_TO_INSTALLED_EMSDK_GOES_HERE* is dependant on where you installed emscripten

and then run make
```
make
```
