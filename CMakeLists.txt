cmake_minimum_required(VERSION 3.14)
project(untitled1)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lcurl")

find_package( OpenCV REQUIRED PATHS /home/evo/Opencv-3.4)

include_directories(/home/evo/Opencv-3.4)
include_directories(./headers)

add_executable(untitled1 headers/Question.h headers/Area.h headers/Db_Api.h headers/Marker_Tracking.h headers/Player.h headers/Player_Manager.h headers/PoseEstimation.h headers/Question.h headers/UI_Manager.h src/Db_Api.cpp src/Question.cpp src/Main.cpp src/Player.cpp src/Player_Manager.cpp)

target_link_libraries(untitled1 curl)
target_link_libraries( untitled1 ${OpenCV_LIBS} )