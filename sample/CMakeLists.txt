cmake_minimum_required (VERSION 3.6)
project (jkps-sample)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/../cmake/modules ${CMAKE_MODULE_PATH})

set(SOURCE_PATH "${CMAKE_SOURCE_DIR}/../")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(${SOURCE_PATH} "${CMAKE_CURRENT_BINARY_DIR}/jkps-gl/")

option(WASM_BUILD OFF)

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

target_include_directories(jkps-sample PUBLIC 
"${SOURCE_PATH}/include/"
)

if( WASM_BUILD )
    target_link_libraries(jkps-sample
        jkps-gl
    )

    set(CMAKE_CXX_FLAGS "--preload-file resources/ -O1 -s USE_WEBGL2=1 -s USE_GLFW=3  -s ALLOW_MEMORY_GROWTH=1 -s WASM=1  -std=c++11 ${CMAKE_CXX_FLAGS} ")
    add_definitions(-Wall -g -std=c++11)
    target_compile_definitions(jkps-sample PUBLIC USE_WASM)

    file(COPY "${CMAKE_SOURCE_DIR}/wasm/" DESTINATION "${CMAKE_BINARY_DIR}/")
else()

    find_package(glew REQUIRED)
    find_package(glfw3 REQUIRED)

    target_link_libraries(jkps-sample
        jkps-gl
        glfw
    )

endif()

file(COPY "${CMAKE_SOURCE_DIR}/resources/" DESTINATION "${CMAKE_BINARY_DIR}/resources/")
set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT jkps-sample)
