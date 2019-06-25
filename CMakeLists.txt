cmake_minimum_required(VERSION 3.4)

project(quizzar)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)

find_package(OpenCV REQUIRED)
find_package(OpenGL REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -lcurl")

set(EXECUTABLE_OUTPUT_PATH ./${CMAKE_BUILD_TYPE})


include_directories( ${OpenCV_INCLUDE_DIRS} ${OpenGL_INCLUDE_DIRS})
include_directories(./headers)

file(GLOB quizzar_SOURCES
        ./src/*.cpp
        ./headers/*.h)

add_executable(quizzar ${quizzar_SOURCES})
target_link_libraries (quizzar PRIVATE ${OpenCV_LIBS} curl)
