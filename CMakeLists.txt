cmake_minimum_required(VERSION 3.5)
project(untitled1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -lcurl")

find_package( OpenCV REQUIRED PATHS /home/evo/Opencv-3.4)

include_directories(/home/evo/Opencv-3.4)
include_directories(./headers)

add_executable(untitled1 src/Db_Api.cpp src/Question.cpp src/Main.cpp src/Player.cpp src/Player_Manager.cpp src/UI_Manager.cpp src/Pose_Estimation.cpp)

target_link_libraries(untitled1 curl)
target_link_libraries( untitled1 ${OpenCV_LIBS} ) 