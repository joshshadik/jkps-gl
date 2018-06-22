cmake_minimum_required (VERSION 3.6)
project (jkps-gl)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})

set(SOURCE_PATH ${PROJECT_SOURCE_DIR})

option(WASM_BUILD OFF)

file(GLOB SRC_FILES
    "${SOURCE_PATH}/src/*.cpp"
)

file(GLOB HEADER_FILES
   "${SOURCE_PATH}/src/*.h"
)

add_library(jkps-gl 
    ${SRC_FILES}
    ${HEADER_FILES}
)

source_group(TREE "${SOURCE_PATH}/src/" PREFIX "source" FILES ${SRC_FILES})
source_group(TREE "${SOURCE_PATH}/src/" PREFIX "headers" FILES ${HEADER_FILES})


add_library(glm INTERFACE)
set_property(TARGET glm PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${SOURCE_PATH}/thirdparty/glm/include")

add_library(tinygltf INTERFACE)
set_property(TARGET tinygltf PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${SOURCE_PATH}/thirdparty/tinygltf/include")
target_compile_definitions(jkps-gl PRIVATE TINYGLTF_IMPLEMENTATION )

target_link_libraries(jkps-gl PUBLIC 
    glm
    tinygltf
)

set (TARGET jkps-gl PROPERTY CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if( WASM_BUILD )
    target_include_directories(jkps-gl PUBLIC 
        "${SOURCE_PATH}/thirdparty/gles3/include"
    )

    set(CMAKE_CXX_FLAGS "-o hello.html --preload-file resources/ -O2 -s USE_WEBGL2=1 -s USE_GLFW=3  -s ALLOW_MEMORY_GROWTH=1 -s WASM=1  -std=c++11 ${CMAKE_CXX_FLAGS} ")
    add_definitions(-Wall -g -std=c++11)
    target_compile_definitions(jkps-gl PUBLIC USE_WASM)

else()

    find_package(glew REQUIRED)
    find_package(glfw3 REQUIRED)

    target_link_libraries(jkps-gl PUBLIC 
        GLEW::GLEW
    )

    if( WIN32 )
        target_link_libraries(jkps-gl PUBLIC
        glu32
        opengl32
        )        
    endif()
endif()


