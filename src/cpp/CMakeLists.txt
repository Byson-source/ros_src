cmake_minimum_required(VERSION 3.0.2)
project(cpp)

set(CMAKE_BUILD_TYPE Debug)
## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages

find_package(catkin REQUIRED COMPONENTS
  actionlib_msgs
  message_generation
  rospy
  roscpp
  std_msgs
  tf
  sensor_msgs
  cv_bridge
  image_transport
)
########################################################
SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

FIND_PACKAGE(RTABMap REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
FIND_PACKAGE(OpenCV REQUIRED)
find_package(PCL 1.3 REQUIRED COMPONENTS common io)

link_directories(${PCL_LIBRARIES_DIRS})
add_definitions(${PCL_DEFINITIONS})

if(POLICY CMP0020)
cmake_policy(SET CMP0020 NEW)
endif()


# fix libproj.so not found on Xenial
if(NOT "${PCL_LIBRARIES}" STREQUAL "")
  list(REMOVE_ITEM PCL_LIBRARIES "vtkproj4")
endif()

if(POLICY CMP0020)
  cmake_policy(SET CMP0020 OLD)
endif()


# include directories
SET(INCLUDE_DIRS
${RTABMap_INCLUDE_DIRS}
${OpenCV_INCLUDE_DIRS}
${PCL_INCLUDE_DIRS}
)

# libraries
SET(LIBRARIES
${RTABMap_LIBRARIES}
${OpenCV_LIBRARIES}
${PCL_LIBRARIES}
)


IF(octomap_FOUND)

SET(INCLUDE_DIRS
${INCLUDE_DIRS}
${OCTOMAP_INCLUDE_DIRS}
)
SET(LIBRARIES
${LIBRARIES}
${OCTOMAP_LIBRARIES}
)
ENDIF(octomap_FOUND)

INCLUDE_DIRECTORIES(${INCLUDE_DIRS})
##############################################################

## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)


## Uncomment this if the package has a setup.py. This macro ensures
## modules and global scripts declared therein get installed
## See http://ros.org/doc/api/catkin/html/user_guide/setup_dot_py.html
# catkin_python_setup()

################################################
## Declare ROS messages, services and actions ##
################################################

## To declare and build messages, services or actions from within this
## package, follow these steps:
## * Let MSG_DEP_SET be the set of packages whose message types you use in
##   your messages/services/actions (e.g. std_msgs, actionlib_msgs, ...).
## * In the file package.xml:
##   * add a build_depend tag for "message_generation"
##   * add a build_depend and a exec_depend tag for each package in MSG_DEP_SET
##   * If MSG_DEP_SET isn't empty the following dependency has been pulled in
##     but can be declared for certainty nonetheless:
##     * add a exec_depend tag for "message_runtime"
## * In this file (CMakeLists.txt):
##   * add "message_generation" and every package in MSG_DEP_SET to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * add "message_runtime" and every package in MSG_DEP_SET to
##     catkin_package(CATKIN_DEPENDS ...)
##   * uncomment the add_*_files sections below as needed
##     and list every .msg/.srv/.action file to be processed
##   * uncomment the generate_messages entry below
##   * add every package in MSG_DEP_SET toegenerate_messages(DEPENDENCIES ...)

## Generate messages in the 'msg' folder
add_message_files(
  FILES
  StringArray.msg
  MultiArray.msg
  FeatureArray.msg
  RO_Array.msg
  # IoTSensor.msg
)
## Generate services in the 'srv' folder
add_service_files(
  FILES
  AddThreeInts.srv
  AddTwoInts.srv
)

## Generate actions in the 'action' folder
# add_action_files(
#   DIRECTORY action
#   FILES
#   Fibonacci.action
#   Averaging.action
#   LoopClosure.action
#   RtabmapReprocess.action
# )

## Generate added messages and services with any dependencies listed here
generate_messages(
  DEPENDENCIES
  std_msgs
  # actionlib_msgs
)

################################################
## Declare ROS dynamic reconfigure parameters ##
################################################

## To declare and build dynamic reconfigure parameters within this
## package, follow these steps:
## * In the file package.xml:
##   * add a build_depend and a exec_depend tag for "dynamic_reconfigure"
## * In this file (CMakeLists.txt):
##   * add "dynamic_reconfigure" to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * uncomment the "generate_dynamic_reconfigure_options" section below
##     and list every .cfg file to be processed

## Generate dynamic reconfigure parameters in the 'cfg' folder
# generate_dynamic_reconfigure_options(
#   cfg/DynReconf1.cfg
#   cfg/DynReconf2.cfg
# )

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if your package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES cpp
 CATKIN_DEPENDS message_generation roscpp std_msgs message_runtime
#  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
src/gui/include
  ${catkin_INCLUDE_DIRS}
)

## Declare a C++ library
# add_library(${PROJECT_NAME}
#   src/${PROJECT_NAME}/cpp.cpp
# )

## Add cmake target dependencies of the library
## as an example, code may need to be generated before libraries
## either from message generation or dynamic reconfigure
# add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Declare a C++ executable
## With catkin_make all packages are built within a single CMake context
## The recommended prefix ensures that target names across packages don't collide
# add_executable(${PROJECT_NAME}_node src/cpp_node.cpp)

## Rename C++ executable without prefix
## The above recommended prefix causes long target names, the following renames the
## target back to the shorter version for ease of user use
## e.g. "rosrun someones_pkg node" instead of "rosrun someones_pkg someones_pkg_node"
# set_target_properties(${PROJECT_NAME}_node PROPERTIES OUTPUT_NAME node PREFIX "")

## Add cmake target dependencies of the executable
## same as for the library above
# add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Specify libraries to link a library or executable target against
# target_link_libraries(${PROJECT_NAME}_node
#   ${catkin_LIBRARIES}
# )

#############
## Install ##
#############

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
catkin_install_python(PROGRAMS
  scripts/gen_numbers.py scripts/any_image_extractor.py scripts/image_extractor.py
  scripts/talker.py scripts/listener.py scripts/feature_matching.py scripts/depth_test.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

## Mark executables for installation
## See http://docs.ros.org/melodic/api/catkin/html/howto/format1/building_executables.html
# install(TARGETS ${PROJECT_NAME}_node
#   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark libraries for installation
## See http://docs.ros.org/melodic/api/catkin/html/howto/format1/building_libraries.html
# install(TARGETS ${PROJECT_NAME}
#   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION}
# )

## Mark cpp header files for installation
# install(DIRECTORY include/${PROJECT_NAME}/
#   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
#   FILES_MATCHING PATTERN "*.h"
#   PATTERN ".svn" EXCLUDE
# )

## Mark other files for installation (e.g. launch and bag files, etc.)
# install(FILES
#   # myfile1
#   # myfile2
#   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
# )

#############
## Testing ##
#############

## Add gtest based cpp test target and link libraries
# catkin_add_gtest(${PROJECT_NAME}-test test/test_cpp.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)
add_executable(talker_node src/CPP_demo/talker.cpp)
target_link_libraries(talker_node ${catkin_LIBRARIES})
add_executable(listener_node src/CPP_demo/listener.cpp)
target_link_libraries(listener_node ${catkin_LIBRARIES})

#service--------------------------------------------------------------
add_executable(add_two_ints_server_node src/CPP_demo/server.cpp)
target_link_libraries(add_two_ints_server_node ${catkin_LIBRARIES})
add_dependencies(add_two_ints_server_node cpp_gencpp)

add_executable(add_two_ints_client src/CPP_demo/client.cpp)
target_link_libraries(add_two_ints_client ${catkin_LIBRARIES})
add_dependencies(add_two_ints_client cpp_gencpp)


add_executable(turtlesimNode src/CPP_demo/move_straight.cpp)
target_link_libraries(turtlesimNode ${catkin_LIBRARIES})

add_executable(turnTurtle src/CPP_demo/turn.cpp)
target_link_libraries(turnTurtle ${catkin_LIBRARIES})

add_executable(frame_a_to_frame_b_broadcaster src/CPP_demo/tf_broadcaster.cpp)
target_link_libraries(frame_a_to_frame_b_broadcaster ${catkin_LIBRARIES})


# add_executable(detection_checker loop_closure/detection_checker/LoopClosureDetection.cpp)
# target_link_libraries(detection_checker ${catkin_LIBRARIES})
# target_link_libraries(detection_checker ${LIBRARIES})

# add_executable(GUI_node
# src/gui/src/example.cpp
# src/gui/src/listener.cpp
# src/gui/src/main.cpp
# src/gui/include/example.h
# src/gui/include/listener.h
# )

# target_link_libraries(GUI_node ${catkin_LIBRARIES})
# target_link_libraries(GUI_node ${LIBRARIES})

add_executable(fibonacci_server src/CPP_demo/action_server.cpp)

target_link_libraries(
  fibonacci_server
  ${catkin_LIBRARIES}
)

add_dependencies(
  fibonacci_server
  ${cpp_EXPORTED_TARGETS}
)

add_executable(spin_study src/loop_closure/multi_thread.cpp)

target_link_libraries(
  spin_study
  ${catkin_LIBRARIES}
)

add_dependencies(
  spin_study
  ${cpp_EXPORTED_TARGETS}
)

add_executable(test_fibonacci src/CPP_demo/actionlib_client.cpp)

target_link_libraries(
  test_fibonacci
  ${catkin_LIBRARIES}
)

add_dependencies(
  test_fibonacci
  ${cpp_EXPORTED_TARGETS}
)

add_executable(averaging src/CPP_demo/averaging_server.cpp)

target_link_libraries(
  averaging
  ${catkin_LIBRARIES}
)

add_dependencies(
  averaging
  ${cpp_EXPORTED_TARGETS}
)

add_executable(test_averaging src/CPP_demo/averaging_client.cpp)

target_link_libraries(
  test_averaging
  ${catkin_LIBRARIES}
)

add_dependencies(
  test_averaging
  ${cpp_EXPORTED_TARGETS}
)

add_executable(test_fibonacci_callback src/CPP_demo/fibonacci_callback_client.cpp)

target_link_libraries(
  test_fibonacci_callback
  ${catkin_LIBRARIES}
)

add_dependencies(
  test_fibonacci_callback
  ${cpp_EXPORTED_TARGETS}
)


# add_executable(LoadDatabase_client src/load_database/client.cpp)

# target_link_libraries(
#   LoadDatabase_client 
#   ${catkin_LIBRARIES}
# )

# add_dependencies(
#   LoadDatabase_client
#   ${cpp_EXPORTED_TARGETS}
# )

# add_executable(LoadDatabase_client2 src/load_database/client2.cpp)

# target_link_libraries(
#   LoadDatabase_client2 
#   ${catkin_LIBRARIES}
# )

# add_dependencies(
#   LoadDatabase_client2
#   ${cpp_EXPORTED_TARGETS}
# )

# add_executable(loop_closure_client src/loop_closure/LoopClosure_client.cpp)

# target_link_libraries(
#   loop_closure_client
#   ${catkin_LIBRARIES}
# )

# add_dependencies(
#   loop_closure_client
#   ${cpp_EXPORTED_TARGETS}
# )

add_executable(detection_checker src/loop_closure/LoopClosure.cpp)

target_link_libraries(
  detection_checker
  ${catkin_LIBRARIES}
  ${LIBRARIES}
)

add_dependencies(
  detection_checker
  ${cpp_EXPORTED_TARGETS}
)

# add_executable(reprocess_client_node src/rtabmap-reprocess/client.cpp)

# target_link_libraries(
#   reprocess_client_node
#   ${catkin_LIBRARIES}
#   ${LIBRARIES}
# )

# add_dependencies(
#   reprocess_client_node
#   ${cpp_EXPORTED_TARGETS}
# )


# add_executable(reprocess_server_node src/rtabmap-reprocess/server.cpp)

# target_link_libraries(
#   reprocess_server_node
#   ${catkin_LIBRARIES}
#   ${LIBRARIES}
# )

# add_dependencies(
#   reprocess_server_node
#   ${cpp_EXPORTED_TARGETS}
# )

# INSTALL(TARGETS reprocess_server_node
#     RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT runtime
#     BUNDLE DESTINATION "${CMAKE_BUNDLE_LOCATION}" COMPONENT runtime)

# add_executable(GUI_node 
# src/gui/src/main.cpp
# src/gui/src/example.cpp
# src/gui/include/example.h
# )

# target_link_libraries(
#   GUI_node
#   ${catkin_LIBRARIES}
#   ${LIBRARIES}
#   Qt5::Widgets
# )

# # target_link_libraries(GUI_node PRIVATE Qt5::Widgets)

# add_dependencies(
#   GUI_node
#   ${cpp_EXPORTED_TARGETS}
# )
