cmake_minimum_required(VERSION 2.8.3)
project(was_serial)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  serial
  std_msgs
)

catkin_package(
  CATKIN_DEPENDS
    serial
    std_msgs
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(was_serial src/was_serial.cpp)

target_link_libraries(was_serial
  ${catkin_LIBRARIES}
)

