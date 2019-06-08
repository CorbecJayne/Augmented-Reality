cmake_minimum_required(VERSION 3.14)
project(untitled1)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lcurl")

find_package( OpenCV REQUIRED PATHS /home/evo/myopencv)

include_directories(/home/evo/myopencv/include)

add_executable(untitled1 Db_Api.cpp Question.cpp Question.h main.cpp Db_Api.h Player.cpp Player_Manager.cpp)
target_link_libraries(untitled1 curl)
target_link_libraries( untitled1 ${OpenCV_LIBS} )