project(gazebo_bachelor_plugins)

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

find_package(gazebo REQUIRED)
include_directories(${GAZEBO_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS})
list(APPEND CMAKE_CXX_FLAGS "${GAZEBO_CXX_FLAGS}")


find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_library(ProjectionMatrixPlugin SHARED ProjectionMatrixPlugin.cc)
target_link_libraries(ProjectionMatrixPlugin ${OpenCV_LIBRARIES} ${GAZEBO_LIBRARIES} CameraPlugin)

add_library(ExtrinsicCalibrationPlugin SHARED ExtrinsicCalibrationPlugin.cc)
target_link_libraries(ExtrinsicCalibrationPlugin ${OpenCV_LIBRARIES} ${GAZEBO_LIBRARIES})
