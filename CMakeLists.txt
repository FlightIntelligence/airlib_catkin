cmake_minimum_required(VERSION 3.0.0)
project(airlib_catkin)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(catkin REQUIRED COMPONENTS
	mavlinkcom_catkin
	rpc_catkin
	)

find_package(Eigen3 REQUIRED)
set(Eigen3_INCLUDE_DIRS ${EIGEN3_INCLUDE_DIRS})
set(Eigen3_LIBRARIES ${EIGEN3_LIBRARIES})

catkin_package(
 INCLUDE_DIRS include ${EIGEN3_INCLUDE_DIRS}
 LIBRARIES ${PROJECT_NAME}
 CATKIN_DEPENDS 
 	mavlinkcom_catkin
 	rpc_catkin
 DEPENDS Eigen3
)

include_directories(
 include
 ${catkin_INCLUDE_DIRS}
 ${Eigen3_INCLUDE_DIRS}
)

file(GLOB_RECURSE ${PROJECT_NAME}_sources 
  ./src/api/*.cpp
  ./src/common/common_utils/*.cpp
  ./src/controllers/*.cpp
  ./src/safety/*.cpp
  ./src/vehicles/car/api/*.cpp
  ./src/vehicles/multirotor/api/*.cpp
  ./src/vehicles/multirotor/controllers/*.cpp
)

add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_sources})
target_link_libraries(${PROJECT_NAME} ${catkin_LIBRARIES})