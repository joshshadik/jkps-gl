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



find_package(sdl2 REQUIRED)
find_package(glew REQUIRED)
find_package(glm REQUIRED)

add_library(tinygltf INTERFACE)
set_property(TARGET tinygltf PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${SOURCE_PATH}/thirdparty/tinygltf/include")


target_link_libraries(jkps-gl PUBLIC 
GLEW::GLEW
glm
tinygltf
)

target_link_libraries(jkps-gl PUBLIC
glu32
opengl32
)

target_include_directories(jkps-sample PUBLIC 
"${SOURCE_PATH}/src/"

)
target_link_libraries(jkps-sample
jkps-gl
SDL2::SDL2 
SDL2::SDL2main
GLEW::GLEW
glu32
opengl32
)

