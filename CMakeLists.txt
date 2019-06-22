cmake_minimum_required(VERSION 3.5)
project(quizzar)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -lcurl")

find_package( OpenCV REQUIRED PATHS /home/evo/Opencv-3.4)

include_directories(/home/evo/Opencv-3.4)
include_directories(./headers)

file(GLOB quizzar_SOURCES ./src/*.cpp)

add_executable(quizzar ${quizzar_SOURCES})

target_link_libraries(quizzar curl)
target_link_libraries( quizzar ${OpenCV_LIBS} ) 