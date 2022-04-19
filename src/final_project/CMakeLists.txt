cmake_minimum_required(VERSION 2.8.3)
project(final_project)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  tf
  ugv_course_libs
  dynamic_reconfigure
  sensor_msgs
  std_msgs
  geometry_msgs
)
generate_dynamic_reconfigure_options(cfg/Final_Project.cfg)
catkin_package()

include_directories(
  ${catkin_INCLUDE_DIRS}
)

#################
## Test Script ##
#################
add_executable(DriveFP src/DriveFP.cpp)
target_link_libraries(DriveFP ${catkin_LIBRARIES} ${GTEST_LIBRARIES})
add_executable(GpsCodeFP src/GpsCodeFP.cpp)
target_link_libraries(GpsCodeFP ${catkin_LIBRARIES} ${GTEST_LIBRARIES})

add_executable(LidarCodeFP src/LidarCodeFP.cpp)

target_link_libraries(LidarCodeFP ${catkin_LIBRARIES})


 