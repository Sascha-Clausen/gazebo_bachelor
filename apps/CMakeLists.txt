project(gazebo_bachelor_apps)

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

find_package(gazebo REQUIRED)
include_directories(${GAZEBO_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS})
list(APPEND CMAKE_CXX_FLAGS "${GAZEBO_CXX_FLAGS}")


find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(SendVelocityMessage SendVelocityMessage.cc)
target_link_libraries(SendVelocityMessage ${OpenCV_LIBRARIES} ${GAZEBO_LIBRARIES})

#add_library(CamLidarHandler STATIC CamLidarHandler.cc)
#target_link_libraries(CamLidarHandler ${OpenCV_LIBRARIES} ${GAZEBO_LIBRARIES})

add_executable(ObstacleFollowerApp CamLidarHandler.cc ObstacleFollower.cc ObstacleFollowerMain.cc)
target_link_libraries(ObstacleFollowerApp ${OpenCV_LIBRARIES} ${GAZEBO_LIBRARIES})
