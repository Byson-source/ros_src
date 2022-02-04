cmake_minimum_required(VERSION 3.0.2)
project(cpp)

set(OPENGV_IMPORT 1)

IF(OPENGV_IMPORT)
  set (OPENGV_VERSION_MAJOR 1)
  set (OPENGV_VERSION_MINOR 0)

  set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/opengv/cmake)

  # ==============================================================================
  # Check and enable C++11 support if available
  # ==============================================================================

  include(CXX11)
  check_for_cxx11_compiler(CXX11_COMPILER)
  # If a C++11 compiler is available, then set the appropriate flags
  IF(CXX11_COMPILER)
    enable_cxx11()
  ENDIF(CXX11_COMPILER)

  # Set the build type.  Options are:
  #
  #  None (CMAKE_C_FLAGS or CMAKE_CXX_FLAGS used)
  #  Debug (CMAKE_C_FLAGS_DEBUG or CMAKE_CXX_FLAGS_DEBUG)
  #  Release (CMAKE_C_FLAGS_RELEASE or CMAKE_CXX_FLAGS_RELEASE)
  #  RelWithDebInfo (CMAKE_C_FLAGS_RELWITHDEBINFO or CMAKE_CXX_FLAGS_RELWITHDEBINFO
  #  MinSizeRel (CMAKE_C_FLAGS_MINSIZEREL or CMAKE_CXX_FLAGS_MINSIZEREL)

  set(CMAKE_BUILD_TYPE Release)

  #set the default path for built executables to the "bin" directory
  set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/opengv/bin)
  #set the default path for built libraries to the "lib" directory
  set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/opengv/lib)

  OPTION(BUILD_TESTS "Build tests" OFF)
  OPTION(BUILD_SHARED_LIBS "Build shared libraries" OFF)
  OPTION(BUILD_PYTHON "Build Python extension" OFF)
  OPTION(BUILD_POSITION_INDEPENDENT_CODE "Build position independent code (-fPIC)" ON)
  OPTION(INSTALL_OPENGV "Install OpenGV on the system" OFF)

  IF(WIN32)
  add_definitions (-bigobj -W0 -MP -Zm10)
  ELSE()
  add_definitions (-Wall -march=native -O3 ) 
  IF(BUILD_POSITION_INDEPENDENT_CODE)
    add_definitions( -fPIC )
  ENDIF()
  ENDIF()




  # get eigen (under windows)
  IF(WIN32)
    set(ADDITIONAL_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/third_party
                                ${PROJECT_SOURCE_DIR}/third_party/eigen3
                                ${PROJECT_SOURCE_DIR}/third_party/eigen3/unsupported )
  ELSE()
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/opengv/modules/")
    find_package(Eigen REQUIRED)
    set(ADDITIONAL_INCLUDE_DIRS ${EIGEN_INCLUDE_DIRS} ${EIGEN_INCLUDE_DIR}/unsupported )
  ENDIF()

  include_directories(${ADDITIONAL_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/opengv/include)

  set( OPENGV_SOURCE_FILES
      opengv/src/absolute_pose/modules/main.cpp
      opengv/src/absolute_pose/modules/gp3p/code.cpp
      opengv/src/absolute_pose/modules/gp3p/init.cpp
      opengv/src/absolute_pose/modules/gp3p/reductors.cpp
      opengv/src/absolute_pose/modules/gp3p/spolynomials.cpp
      opengv/src/absolute_pose/modules/Epnp.cpp
      opengv/src/absolute_pose/modules/gpnp1/code.cpp
      opengv/src/absolute_pose/modules/gpnp1/init.cpp
      opengv/src/absolute_pose/modules/gpnp1/reductors.cpp
      opengv/src/absolute_pose/modules/gpnp1/spolynomials.cpp
      opengv/src/absolute_pose/modules/gpnp2/code.cpp
      opengv/src/absolute_pose/modules/gpnp2/init.cpp
      opengv/src/absolute_pose/modules/gpnp2/reductors.cpp
      opengv/src/absolute_pose/modules/gpnp2/spolynomials.cpp
      opengv/src/absolute_pose/modules/gpnp3/code.cpp
      opengv/src/absolute_pose/modules/gpnp3/init.cpp
      opengv/src/absolute_pose/modules/gpnp3/reductors.cpp
      opengv/src/absolute_pose/modules/gpnp3/spolynomials.cpp
      opengv/src/absolute_pose/modules/gpnp4/code.cpp
      opengv/src/absolute_pose/modules/gpnp4/init.cpp
      opengv/src/absolute_pose/modules/gpnp4/reductors.cpp
      opengv/src/absolute_pose/modules/gpnp4/spolynomials.cpp
      opengv/src/absolute_pose/modules/gpnp5/code.cpp
      opengv/src/absolute_pose/modules/gpnp5/init.cpp
      opengv/src/absolute_pose/modules/gpnp5/reductors.cpp
      opengv/src/absolute_pose/modules/gpnp5/spolynomials.cpp
      opengv/src/absolute_pose/modules/upnp2.cpp
      opengv/src/absolute_pose/modules/upnp4.cpp
      opengv/src/absolute_pose/modules/mlpnp/mlpnp.cpp
      opengv/src/absolute_pose/modules/mlpnp/mlpnp_lm.cpp
      opengv/src/relative_pose/modules/main.cpp
      opengv/src/relative_pose/modules/fivept_nister/modules.cpp
      opengv/src/relative_pose/modules/fivept_stewenius/modules.cpp
      opengv/src/relative_pose/modules/fivept_kneip/code.cpp
      opengv/src/relative_pose/modules/fivept_kneip/init.cpp
      opengv/src/relative_pose/modules/fivept_kneip/reductors.cpp
      opengv/src/relative_pose/modules/fivept_kneip/spolynomials.cpp
      opengv/src/relative_pose/modules/sixpt/modules2.cpp
      opengv/src/relative_pose/modules/eigensolver/modules.cpp
      opengv/src/relative_pose/modules/ge/modules.cpp
      opengv/src/relative_pose/modules/sixpt_ventura/approx_relpose_generalized_fast_computeA.cpp
      opengv/src/math/cayley.cpp
      opengv/src/math/rodrigues.cpp
      opengv/src/math/quaternion.cpp
      opengv/src/math/arun.cpp
      opengv/src/math/Sturm.cpp
      opengv/src/math/roots.cpp
      opengv/src/math/gauss_jordan.cpp
      opengv/src/absolute_pose/methods.cpp
      opengv/src/absolute_pose/CentralAbsoluteAdapter.cpp
      opengv/src/absolute_pose/NoncentralAbsoluteAdapter.cpp
      opengv/src/absolute_pose/NoncentralAbsoluteMultiAdapter.cpp
      opengv/src/relative_pose/methods.cpp
      opengv/src/relative_pose/CentralRelativeAdapter.cpp
      opengv/src/relative_pose/CentralRelativeWeightingAdapter.cpp
      opengv/src/relative_pose/NoncentralRelativeAdapter.cpp
      opengv/src/relative_pose/CentralRelativeMultiAdapter.cpp
      opengv/src/relative_pose/NoncentralRelativeMultiAdapter.cpp
      opengv/src/triangulation/methods.cpp
      opengv/src/point_cloud/methods.cpp
      opengv/src/point_cloud/PointCloudAdapter.cpp
      opengv/src/sac_problems/absolute_pose/AbsolutePoseSacProblem.cpp
      opengv/src/sac_problems/absolute_pose/MultiNoncentralAbsolutePoseSacProblem.cpp
      opengv/src/sac_problems/relative_pose/CentralRelativePoseSacProblem.cpp
      opengv/src/sac_problems/relative_pose/NoncentralRelativePoseSacProblem.cpp
      opengv/src/sac_problems/relative_pose/RotationOnlySacProblem.cpp
      opengv/src/sac_problems/relative_pose/TranslationOnlySacProblem.cpp
      opengv/src/sac_problems/relative_pose/EigensolverSacProblem.cpp
      opengv/src/sac_problems/relative_pose/MultiCentralRelativePoseSacProblem.cpp
      opengv/src/sac_problems/relative_pose/MultiNoncentralRelativePoseSacProblem.cpp
      opengv/src/sac_problems/point_cloud/PointCloudSacProblem.cpp
      opengv/src/absolute_pose/MACentralAbsolute.cpp
      opengv/src/absolute_pose/MANoncentralAbsolute.cpp
      opengv/src/relative_pose/MACentralRelative.cpp
      opengv/src/relative_pose/MANoncentralRelative.cpp
      opengv/src/relative_pose/MANoncentralRelativeMulti.cpp
      opengv/src/point_cloud/MAPointCloud.cpp )

  set( OPENGV_HEADER_FILES
      opengv/include/opengv/types.hpp
      opengv/include/opengv/OptimizationFunctor.hpp
      opengv/include/opengv/absolute_pose/methods.hpp
      opengv/include/opengv/relative_pose/methods.hpp
      opengv/include/opengv/triangulation/methods.hpp
      opengv/include/opengv/point_cloud/methods.hpp
      opengv/include/opengv/math/cayley.hpp
      opengv/include/opengv/math/rodrigues.hpp
      opengv/include/opengv/math/quaternion.hpp
      opengv/include/opengv/math/arun.hpp
      opengv/include/opengv/math/Sturm.hpp
      opengv/include/opengv/math/roots.hpp
      opengv/include/opengv/math/gauss_jordan.hpp
      opengv/include/opengv/absolute_pose/AbsoluteAdapterBase.hpp
      opengv/include/opengv/absolute_pose/CentralAbsoluteAdapter.hpp
      opengv/include/opengv/absolute_pose/NoncentralAbsoluteAdapter.hpp
      opengv/include/opengv/absolute_pose/NoncentralAbsoluteMultiAdapter.hpp
      opengv/include/opengv/absolute_pose/AbsoluteMultiAdapterBase.hpp
      opengv/include/opengv/relative_pose/RelativeAdapterBase.hpp
      opengv/include/opengv/relative_pose/RelativeMultiAdapterBase.hpp
      opengv/include/opengv/relative_pose/CentralRelativeAdapter.hpp
      opengv/include/opengv/relative_pose/CentralRelativeWeightingAdapter.hpp
      opengv/include/opengv/relative_pose/NoncentralRelativeAdapter.hpp
      opengv/include/opengv/relative_pose/CentralRelativeMultiAdapter.hpp
      opengv/include/opengv/relative_pose/NoncentralRelativeMultiAdapter.hpp
      opengv/include/opengv/point_cloud/PointCloudAdapterBase.hpp
      opengv/include/opengv/point_cloud/PointCloudAdapter.hpp
      opengv/include/opengv/sac_problems/absolute_pose/AbsolutePoseSacProblem.hpp
      opengv/include/opengv/sac_problems/absolute_pose/MultiNoncentralAbsolutePoseSacProblem.hpp
      opengv/include/opengv/sac_problems/relative_pose/CentralRelativePoseSacProblem.hpp
      opengv/include/opengv/sac_problems/relative_pose/NoncentralRelativePoseSacProblem.hpp
      opengv/include/opengv/sac_problems/relative_pose/MultiCentralRelativePoseSacProblem.hpp
      opengv/include/opengv/sac_problems/relative_pose/MultiNoncentralRelativePoseSacProblem.hpp
      opengv/include/opengv/sac_problems/relative_pose/EigensolverSacProblem.hpp
      opengv/include/opengv/sac_problems/relative_pose/RotationOnlySacProblem.hpp
      opengv/include/opengv/sac_problems/relative_pose/TranslationOnlySacProblem.hpp
      opengv/include/opengv/sac_problems/point_cloud/PointCloudSacProblem.hpp
      opengv/include/opengv/absolute_pose/MACentralAbsolute.hpp
      opengv/include/opengv/absolute_pose/MANoncentralAbsolute.hpp
      opengv/include/opengv/relative_pose/MACentralRelative.hpp
      opengv/include/opengv/relative_pose/MANoncentralRelative.hpp
      opengv/include/opengv/relative_pose/MANoncentralRelativeMulti.hpp
      opengv/include/opengv/point_cloud/MAPointCloud.hpp )


  IF(WIN32)
    add_library( opengv ${OPENGV_SOURCE_FILES} ${OPENGV_HEADER_FILES} )
    add_library( random_generators opengv/test/random_generators.cpp 
                opengv/test/random_generators.hpp 
                opengv/test/experiment_helpers.cpp 
                opengv/test/experiment_helpers.hpp 
                opengv/test/time_measurement.cpp 
                opengv/test/time_measurement.hpp )
  ELSE()
    add_library( opengv ${OPENGV_SOURCE_FILES} ${OPENGV_HEADER_FILES} )
    add_library( random_generators 
    opengv/test/random_generators.cpp 
    opengv/test/random_generators.hpp 
    opengv/test/experiment_helpers.cpp 
    opengv/test/experiment_helpers.hpp 
    opengv/test/time_measurement.cpp 
    opengv/test/time_measurement.hpp )
    target_link_libraries(random_generators opengv)
  ENDIF()
ENDIF(OPENGV_IMPORT)
# set(CMAKE_BUILD_TYPE Debug)

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
  pcl_ros
)
########################################################
SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}")

# set(opengv_DIR "/home/ayumi/okvis/build/lib/CMake/opengv/")

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
find_package(Open3D HINTS ${CMAKE_INSTALL_PREFIX}/lib/cmake)

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
${Open3D_INCLUDE_DIRS}
)

# libraries
SET(LIBRARIES
${RTABMap_LIBRARIES}
${OpenCV_LIBRARIES}
${PCL_LIBRARIES}
# ${opengv_INCLUDE_DIRS}
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
  src/relative_pose_estimation/include/
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

add_executable(RO_estimator 
              src/relative_pose_estimation/RO.cpp
              src/relative_pose_estimation/include/calibration.h)

target_link_libraries(
  RO_estimator
  ${catkin_LIBRARIES}
  opengv
  )

add_executable(gui src/gui/src/gui_2.cpp)
target_link_libraries(gui ${catkin_LIBRARIES})
# add_dependencies(open3d_conversions_ex_sub ${open3d_conversions_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
# target_link_libraries(open3d_conversions_ex_sub
# ${catkin_LIBRARIES}
# ${Open3D_LIBRARIES})




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

# add_executable(fibonacci_server src/CPP_demo/action_server.cpp)

# target_link_libraries(
#   fibonacci_server
#   ${catkin_LIBRARIES}
# )

# add_dependencies(
#   fibonacci_server
#   ${cpp_EXPORTED_TARGETS}
# )

# add_executable(spin_study src/loop_closure/multi_thread.cpp)

# target_link_libraries(
#   spin_study
#   ${catkin_LIBRARIES}
# )

# add_dependencies(
#   spin_study
#   ${cpp_EXPORTED_TARGETS}
# )

# add_executable(test_fibonacci src/CPP_demo/actionlib_client.cpp)

# target_link_libraries(
#   test_fibonacci
#   ${catkin_LIBRARIES}
# )

# add_dependencies(
#   test_fibonacci
#   ${cpp_EXPORTED_TARGETS}
# )

# add_executable(averaging src/CPP_demo/averaging_server.cpp)

# target_link_libraries(
#   averaging
#   ${catkin_LIBRARIES}
# )

# add_dependencies(
#   averaging
#   ${cpp_EXPORTED_TARGETS}
# )

# add_executable(test_averaging src/CPP_demo/averaging_client.cpp)

# target_link_libraries(
#   test_averaging
#   ${catkin_LIBRARIES}
# )

# add_dependencies(
#   test_averaging
#   ${cpp_EXPORTED_TARGETS}
# )

# add_executable(test_fibonacci_callback src/CPP_demo/fibonacci_callback_client.cpp)

# target_link_libraries(
#   test_fibonacci_callback
#   ${catkin_LIBRARIES}
# )

# add_dependencies(
#   test_fibonacci_callback
#   ${cpp_EXPORTED_TARGETS}
# )



