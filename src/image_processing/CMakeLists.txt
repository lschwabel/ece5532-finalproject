cmake_minimum_required(VERSION 2.8.3)
project(image_processing)

set(${PROJECT_NAME}_CATKIN_DEPS
  roscpp
  tf
  ugv_course_libs
  dynamic_reconfigure
  sensor_msgs
  std_msgs
  geometry_msgs
  image_transport
  cv_bridge
)

find_package(catkin REQUIRED COMPONENTS ${${PROJECT_NAME}_CATKIN_DEPS})
find_package(OpenCV REQUIRED)


catkin_package(
  DEPENDS OpenCV
)

include_directories(
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

add_executable(image_processing src/image_processing.cpp)
target_link_libraries(image_processing ${catkin_LIBRARIES})

target_link_libraries(image_processing
  ${catkin_LIBRARIES}
  ${OpenCV_LIBS}
)