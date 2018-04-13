cmake_minimum_required (VERSION 3.9)
project (jkps-gl)

set(SOURCE_PATH ${CMAKE_SOURCE_DIR})

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



file(GLOB SAMPLE_SRC_FILES
    "${SOURCE_PATH}/sample/*.cpp"
)

file(GLOB SAMPLE_HEADER_FILES
   "${SOURCE_PATH}/sample/*.h"
)

add_executable(jkps-sample 
    ${SAMPLE_SRC_FILES}
    ${SAMPLE_HEADER_FILES}
)

source_group(TREE "${SOURCE_PATH}/sample/" PREFIX "source" FILES ${SAMPLE_SRC_FILES})
source_group(TREE "${SOURCE_PATH}/sample/" PREFIX "headers" FILES ${SAMPLE_HEADER_FILES})




# set(THIRDPARTY_DIR "${SOURCE_PATH}/thirdparty")

add_library(glm INTERFACE)
set_property(TARGET glm PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${SOURCE_PATH}/thirdparty/glm/include")

add_library(tinygltf INTERFACE)
set_property(TARGET tinygltf PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${SOURCE_PATH}/thirdparty/tinygltf/include")

add_library(glew STATIC IMPORTED)
set_property(TARGET glew PROPERTY IMPORTED_LOCATION "${SOURCE_PATH}/thirdparty/glew/lib/glew32.lib")

add_library(libjpeg-turbo STATIC IMPORTED)
set_property(TARGET libjpeg-turbo PROPERTY IMPORTED_LOCATION "${SOURCE_PATH}/thirdparty/libjpeg-turbo/lib/turbojpeg.lib")


add_library(sdl2 STATIC IMPORTED)
set_property(TARGET sdl2 PROPERTY IMPORTED_LOCATION "${SOURCE_PATH}/thirdparty/sdl2/lib/SDL2.lib")

add_library(sdl2-main STATIC IMPORTED)
set_property(TARGET sdl2-main PROPERTY IMPORTED_LOCATION "${SOURCE_PATH}/thirdparty/sdl2/lib/SDL2main.lib")

include_directories("${SOURCE_PATH}/thirdparty/glew/include")
include_directories("${SOURCE_PATH}/thirdparty/libjpeg-turbo/include")
include_directories("${SOURCE_PATH}/thirdparty/sdl2/include")

target_link_libraries( jkps-gl
glm
tinygltf
glew
libjpeg-turbo
glu32.lib
opengl32.lib
)

target_include_directories(jkps-sample PUBLIC "${SOURCE_PATH}/src/")
target_link_libraries(jkps-sample
jkps-gl
sdl2
sdl2-main
glm
tinygltf
glew
libjpeg-turbo
glu32.lib
opengl32.lib
)

