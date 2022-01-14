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
  set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
  #set the default path for built libraries to the "lib" directory
  set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

  OPTION(BUILD_TESTS "Build tests" ON)
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
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/modules/")
    find_package(Eigen REQUIRED)
    set(ADDITIONAL_INCLUDE_DIRS ${EIGEN_INCLUDE_DIRS} ${EIGEN_INCLUDE_DIR}/unsupported )
  ENDIF()

  include_directories(${ADDITIONAL_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include)

  set( OPENGV_SOURCE_FILES
    src/absolute_pose/modules/main.cpp
    src/absolute_pose/modules/gp3p/code.cpp
    src/absolute_pose/modules/gp3p/init.cpp
    src/absolute_pose/modules/gp3p/reductors.cpp
    src/absolute_pose/modules/gp3p/spolynomials.cpp
    src/absolute_pose/modules/Epnp.cpp
    src/absolute_pose/modules/gpnp1/code.cpp
    src/absolute_pose/modules/gpnp1/init.cpp
    src/absolute_pose/modules/gpnp1/reductors.cpp
    src/absolute_pose/modules/gpnp1/spolynomials.cpp
    src/absolute_pose/modules/gpnp2/code.cpp
    src/absolute_pose/modules/gpnp2/init.cpp
    src/absolute_pose/modules/gpnp2/reductors.cpp
    src/absolute_pose/modules/gpnp2/spolynomials.cpp
    src/absolute_pose/modules/gpnp3/code.cpp
    src/absolute_pose/modules/gpnp3/init.cpp
    src/absolute_pose/modules/gpnp3/reductors.cpp
    src/absolute_pose/modules/gpnp3/spolynomials.cpp
    src/absolute_pose/modules/gpnp4/code.cpp
    src/absolute_pose/modules/gpnp4/init.cpp
    src/absolute_pose/modules/gpnp4/reductors.cpp
    src/absolute_pose/modules/gpnp4/spolynomials.cpp
    src/absolute_pose/modules/gpnp5/code.cpp
    src/absolute_pose/modules/gpnp5/init.cpp
    src/absolute_pose/modules/gpnp5/reductors.cpp
    src/absolute_pose/modules/gpnp5/spolynomials.cpp
    src/absolute_pose/modules/upnp2.cpp
    src/absolute_pose/modules/upnp4.cpp
    src/absolute_pose/modules/mlpnp/mlpnp.cpp
    src/absolute_pose/modules/mlpnp/mlpnp_lm.cpp
    src/relative_pose/modules/main.cpp
    src/relative_pose/modules/fivept_nister/modules.cpp
    src/relative_pose/modules/fivept_stewenius/modules.cpp
    src/relative_pose/modules/fivept_kneip/code.cpp
    src/relative_pose/modules/fivept_kneip/init.cpp
    src/relative_pose/modules/fivept_kneip/reductors.cpp
    src/relative_pose/modules/fivept_kneip/spolynomials.cpp
    src/relative_pose/modules/sixpt/modules2.cpp
    src/relative_pose/modules/eigensolver/modules.cpp
    src/relative_pose/modules/ge/modules.cpp
    src/relative_pose/modules/sixpt_ventura/approx_relpose_generalized_fast_computeA.cpp
    src/math/cayley.cpp
    src/math/rodrigues.cpp
    src/math/quaternion.cpp
    src/math/arun.cpp
    src/math/Sturm.cpp
    src/math/roots.cpp
    src/math/gauss_jordan.cpp
    src/absolute_pose/methods.cpp
    src/absolute_pose/CentralAbsoluteAdapter.cpp
    src/absolute_pose/NoncentralAbsoluteAdapter.cpp
    src/absolute_pose/NoncentralAbsoluteMultiAdapter.cpp
    src/relative_pose/methods.cpp
    src/relative_pose/CentralRelativeAdapter.cpp
    src/relative_pose/CentralRelativeWeightingAdapter.cpp
    src/relative_pose/NoncentralRelativeAdapter.cpp
    src/relative_pose/CentralRelativeMultiAdapter.cpp
    src/relative_pose/NoncentralRelativeMultiAdapter.cpp
    src/triangulation/methods.cpp
    src/point_cloud/methods.cpp
    src/point_cloud/PointCloudAdapter.cpp
    src/sac_problems/absolute_pose/AbsolutePoseSacProblem.cpp
    src/sac_problems/absolute_pose/MultiNoncentralAbsolutePoseSacProblem.cpp
    src/sac_problems/relative_pose/CentralRelativePoseSacProblem.cpp
    src/sac_problems/relative_pose/NoncentralRelativePoseSacProblem.cpp
    src/sac_problems/relative_pose/RotationOnlySacProblem.cpp
    src/sac_problems/relative_pose/TranslationOnlySacProblem.cpp
    src/sac_problems/relative_pose/EigensolverSacProblem.cpp
    src/sac_problems/relative_pose/MultiCentralRelativePoseSacProblem.cpp
    src/sac_problems/relative_pose/MultiNoncentralRelativePoseSacProblem.cpp
    src/sac_problems/point_cloud/PointCloudSacProblem.cpp
    src/absolute_pose/MACentralAbsolute.cpp
    src/absolute_pose/MANoncentralAbsolute.cpp
    src/relative_pose/MACentralRelative.cpp
    src/relative_pose/MANoncentralRelative.cpp
    src/relative_pose/MANoncentralRelativeMulti.cpp
    src/point_cloud/MAPointCloud.cpp )

  set( OPENGV_HEADER_FILES
    include/opengv/types.hpp
    include/opengv/OptimizationFunctor.hpp
    include/opengv/absolute_pose/methods.hpp
    include/opengv/relative_pose/methods.hpp
    include/opengv/triangulation/methods.hpp
    include/opengv/point_cloud/methods.hpp
    include/opengv/math/cayley.hpp
    include/opengv/math/rodrigues.hpp
    include/opengv/math/quaternion.hpp
    include/opengv/math/arun.hpp
    include/opengv/math/Sturm.hpp
    include/opengv/math/roots.hpp
    include/opengv/math/gauss_jordan.hpp
    include/opengv/absolute_pose/AbsoluteAdapterBase.hpp
    include/opengv/absolute_pose/CentralAbsoluteAdapter.hpp
    include/opengv/absolute_pose/NoncentralAbsoluteAdapter.hpp
    include/opengv/absolute_pose/NoncentralAbsoluteMultiAdapter.hpp
    include/opengv/absolute_pose/AbsoluteMultiAdapterBase.hpp
    include/opengv/relative_pose/RelativeAdapterBase.hpp
    include/opengv/relative_pose/RelativeMultiAdapterBase.hpp
    include/opengv/relative_pose/CentralRelativeAdapter.hpp
    include/opengv/relative_pose/CentralRelativeWeightingAdapter.hpp
    include/opengv/relative_pose/NoncentralRelativeAdapter.hpp
    include/opengv/relative_pose/CentralRelativeMultiAdapter.hpp
    include/opengv/relative_pose/NoncentralRelativeMultiAdapter.hpp
    include/opengv/point_cloud/PointCloudAdapterBase.hpp
    include/opengv/point_cloud/PointCloudAdapter.hpp
    include/opengv/sac_problems/absolute_pose/AbsolutePoseSacProblem.hpp
    include/opengv/sac_problems/absolute_pose/MultiNoncentralAbsolutePoseSacProblem.hpp
    include/opengv/sac_problems/relative_pose/CentralRelativePoseSacProblem.hpp
    include/opengv/sac_problems/relative_pose/NoncentralRelativePoseSacProblem.hpp
    include/opengv/sac_problems/relative_pose/MultiCentralRelativePoseSacProblem.hpp
    include/opengv/sac_problems/relative_pose/MultiNoncentralRelativePoseSacProblem.hpp
    include/opengv/sac_problems/relative_pose/EigensolverSacProblem.hpp
    include/opengv/sac_problems/relative_pose/RotationOnlySacProblem.hpp
    include/opengv/sac_problems/relative_pose/TranslationOnlySacProblem.hpp
    include/opengv/sac_problems/point_cloud/PointCloudSacProblem.hpp
    include/opengv/absolute_pose/MACentralAbsolute.hpp
    include/opengv/absolute_pose/MANoncentralAbsolute.hpp
    include/opengv/relative_pose/MACentralRelative.hpp
    include/opengv/relative_pose/MANoncentralRelative.hpp
    include/opengv/relative_pose/MANoncentralRelativeMulti.hpp
    include/opengv/point_cloud/MAPointCloud.hpp )

  IF(WIN32)
    add_library( opengv ${OPENGV_SOURCE_FILES} ${OPENGV_HEADER_FILES} )
    add_library( random_generators test/random_generators.cpp test/random_generators.hpp test/experiment_helpers.cpp test/experiment_helpers.hpp test/time_measurement.cpp test/time_measurement.hpp )
  ELSE()
    add_library( opengv ${OPENGV_SOURCE_FILES} ${OPENGV_HEADER_FILES} )
    add_library( random_generators test/random_generators.cpp test/random_generators.hpp test/experiment_helpers.cpp test/experiment_helpers.hpp test/time_measurement.cpp test/time_measurement.hpp )
    target_link_libraries(random_generators opengv)
  ENDIF()

  if(TARGET opengv)
    message(STATUS "print_all_variables------------------------------------------}")
  endif()


  IF (BUILD_TESTS)
    enable_testing()

    add_executable( test_absolute_pose test/test_absolute_pose.cpp )
    target_link_libraries( test_absolute_pose opengv random_generators )
    add_test(NAME test_absolute_pose
      WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
      COMMAND test_absolute_pose)

    add_executable( test_absolute_pose_sac test/test_absolute_pose_sac.cpp )
    target_link_libraries( test_absolute_pose_sac opengv random_generators )
    add_test(NAME test_absolute_pose_sac
      WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
      COMMAND test_absolute_pose_sac)

    add_executable( test_noncentral_absolute_pose test/test_noncentral_absolute_pose.cpp )
    target_link_libraries( test_noncentral_absolute_pose opengv random_generators )
    add_test(NAME test_noncentral_absolute_pose
      WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
      COMMAND test_noncentral_absolute_pose)

    add_executable( test_noncentral_absolute_pose_sac test/test_noncentral_absolute_pose_sac.cpp )
    target_link_libraries( test_noncentral_absolute_pose_sac opengv random_generators )
    add_test(NAME test_noncentral_absolute_pose_sac
      WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
      COMMAND test_noncentral_absolute_pose_sac)

    add_executable( test_multi_noncentral_absolute_pose_sac test/test_multi_noncentral_absolute_pose_sac.cpp )
    target_link_libraries( test_multi_noncentral_absolute_pose_sac opengv random_generators )
    add_test(NAME test_multi_noncentral_absolute_pose_sac
      WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
      COMMAND test_multi_noncentral_absolute_pose_sac)

    add_executable( test_relative_pose test/test_relative_pose.cpp )
    target_link_libraries( test_relative_pose opengv random_generators )
    add_test(NAME test_relative_pose
      WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
      COMMAND test_relative_pose)

    add_executable( test_relative_pose_rotationOnly test/test_relative_pose_rotationOnly.cpp )
    target_link_libraries( test_relative_pose_rotationOnly opengv random_generators )
    add_test(NAME test_relative_pose_rotationOnly
      WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
      COMMAND test_relative_pose_rotationOnly)

    add_executable( test_relative_pose_rotationOnly_sac test/test_relative_pose_rotationOnly_sac.cpp )
    target_link_libraries( test_relative_pose_rotationOnly_sac opengv random_generators )
    add_test(NAME test_relative_pose_rotationOnly_sac
      WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
      COMMAND test_relative_pose_rotationOnly_sac)

    add_executable( test_relative_pose_sac test/test_relative_pose_sac.cpp )
    target_link_libraries( test_relative_pose_sac opengv random_generators )
    add_test(NAME test_relative_pose_sac
      WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
      COMMAND test_relative_pose_sac)

    add_executable( test_noncentral_relative_pose test/test_noncentral_relative_pose.cpp )
    target_link_libraries( test_noncentral_relative_pose opengv random_generators )
    add_test(NAME test_noncentral_relative_pose
      WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
      COMMAND test_noncentral_relative_pose)

    add_executable( test_noncentral_relative_pose_sac test/test_noncentral_relative_pose_sac.cpp )
    target_link_libraries( test_noncentral_relative_pose_sac opengv random_generators )
    add_test(NAME test_noncentral_relative_pose_sac
      WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
      COMMAND test_noncentral_relative_pose_sac)

    add_executable( test_multi_noncentral_relative_pose_sac test/test_multi_noncentral_relative_pose_sac.cpp )
    target_link_libraries( test_multi_noncentral_relative_pose_sac opengv random_generators )
    add_test(NAME test_multi_noncentral_relative_pose_sac
      WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
      COMMAND test_multi_noncentral_relative_pose_sac)

    add_executable( test_eigensolver_sac test/test_eigensolver_sac.cpp )
    target_link_libraries( test_eigensolver_sac opengv random_generators )
    add_test(NAME test_eigensolver_sac
      WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
      COMMAND test_eigensolver_sac)

    add_executable( test_triangulation test/test_triangulation.cpp )
    target_link_libraries( test_triangulation opengv random_generators )
    add_test(NAME test_triangulation
      WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
      COMMAND test_triangulation)

    add_executable( test_eigensolver test/test_eigensolver.cpp )
    target_link_libraries( test_eigensolver opengv random_generators )
    add_test(NAME test_eigensolver
      WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
      COMMAND test_eigensolver)

    add_executable( test_point_cloud test/test_point_cloud.cpp )
    target_link_libraries( test_point_cloud opengv random_generators )
    add_test(NAME  test_point_cloud
      WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
      COMMAND test_point_cloud)

    add_Executable( test_point_cloud_sac test/test_point_cloud_sac.cpp )
    target_link_libraries( test_point_cloud_sac opengv random_generators )
    add_test(NAME test_point_cloud_sac
      WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
      COMMAND test_point_cloud_sac)

    add_Executable( test_Sturm test/test_Sturm.cpp )
    target_link_libraries( test_Sturm opengv random_generators )
    add_test(NAME test_Sturm
      WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
      COMMAND test_Sturm)

  ENDIF()


ENDIF(OPENGV_IMPORT)
set(CMAKE_BUILD_TYPE Debug)
set(DCMAKE_BUILD_TYPE RELEASE)
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
# find_package(opengv REQUIRED)

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
# ${opengv_INCLUDE_DIRS}
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



