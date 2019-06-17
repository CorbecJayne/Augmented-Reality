cmake_minimum_required(VERSION 3.4)

# following cmake project structure guide: https://codereview.stackexchange.com/questions/176508/basic-cmake-c-project-structure

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)

find_package(OpenCV REQUIRED)
find_package(glfw3 REQUIRED)

project(quizzar)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lcurl")

set(EXECUTABLE_OUTPUT_PATH ./${CMAKE_BUILD_TYPE})


include_directories( ${OpenCV_INCLUDE_DIRS} ${glfw_INCLUDE_DIRS})

set(quizzar_SOURCES
        src/Db_Api.cpp
        src/Main.cpp
        src/Marker_Tracking.cpp
        src/Player_Manager.cpp
        src/Player.cpp
        src/Pose_Estimation.cpp
        src/Question.cpp
        src/UI_Manager.cpp)

set(quizzar_LIBS
        headers/Area.h
        headers/Db_Api.h
        headers/Marker_Tracking.h
        headers/Player_Manager.h
        headers/Player.h
        headers/Pose_Estimation.h
        headers/Question.h
        headers/UI_Manager.h)


add_executable(quizzar ${quizzar_SOURCES})
target_link_libraries (quizzar PRIVATE ${OpenCV_LIBS} $(quizzar_LIBS) curl)
