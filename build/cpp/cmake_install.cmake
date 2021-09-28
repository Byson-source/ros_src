# Install script for directory: /home/ayumi/catkin_ws/src/cpp

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/ayumi/catkin_ws/install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cpp/msg" TYPE FILE FILES "/home/ayumi/catkin_ws/src/cpp/msg/StringArray.msg")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cpp/srv" TYPE FILE FILES "/home/ayumi/catkin_ws/src/cpp/srv/AddThreeInts.srv")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cpp/action" TYPE FILE FILES
    "/home/ayumi/catkin_ws/src/cpp/action/Fibonacci.action"
    "/home/ayumi/catkin_ws/src/cpp/action/Averaging.action"
    "/home/ayumi/catkin_ws/src/cpp/action/LoopClosure.action"
    "/home/ayumi/catkin_ws/src/cpp/action/RtabmapReprocess.action"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cpp/msg" TYPE FILE FILES
    "/home/ayumi/catkin_ws/devel/share/cpp/msg/FibonacciAction.msg"
    "/home/ayumi/catkin_ws/devel/share/cpp/msg/FibonacciActionGoal.msg"
    "/home/ayumi/catkin_ws/devel/share/cpp/msg/FibonacciActionResult.msg"
    "/home/ayumi/catkin_ws/devel/share/cpp/msg/FibonacciActionFeedback.msg"
    "/home/ayumi/catkin_ws/devel/share/cpp/msg/FibonacciGoal.msg"
    "/home/ayumi/catkin_ws/devel/share/cpp/msg/FibonacciResult.msg"
    "/home/ayumi/catkin_ws/devel/share/cpp/msg/FibonacciFeedback.msg"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cpp/msg" TYPE FILE FILES
    "/home/ayumi/catkin_ws/devel/share/cpp/msg/AveragingAction.msg"
    "/home/ayumi/catkin_ws/devel/share/cpp/msg/AveragingActionGoal.msg"
    "/home/ayumi/catkin_ws/devel/share/cpp/msg/AveragingActionResult.msg"
    "/home/ayumi/catkin_ws/devel/share/cpp/msg/AveragingActionFeedback.msg"
    "/home/ayumi/catkin_ws/devel/share/cpp/msg/AveragingGoal.msg"
    "/home/ayumi/catkin_ws/devel/share/cpp/msg/AveragingResult.msg"
    "/home/ayumi/catkin_ws/devel/share/cpp/msg/AveragingFeedback.msg"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cpp/msg" TYPE FILE FILES
    "/home/ayumi/catkin_ws/devel/share/cpp/msg/LoopClosureAction.msg"
    "/home/ayumi/catkin_ws/devel/share/cpp/msg/LoopClosureActionGoal.msg"
    "/home/ayumi/catkin_ws/devel/share/cpp/msg/LoopClosureActionResult.msg"
    "/home/ayumi/catkin_ws/devel/share/cpp/msg/LoopClosureActionFeedback.msg"
    "/home/ayumi/catkin_ws/devel/share/cpp/msg/LoopClosureGoal.msg"
    "/home/ayumi/catkin_ws/devel/share/cpp/msg/LoopClosureResult.msg"
    "/home/ayumi/catkin_ws/devel/share/cpp/msg/LoopClosureFeedback.msg"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cpp/msg" TYPE FILE FILES
    "/home/ayumi/catkin_ws/devel/share/cpp/msg/RtabmapReprocessAction.msg"
    "/home/ayumi/catkin_ws/devel/share/cpp/msg/RtabmapReprocessActionGoal.msg"
    "/home/ayumi/catkin_ws/devel/share/cpp/msg/RtabmapReprocessActionResult.msg"
    "/home/ayumi/catkin_ws/devel/share/cpp/msg/RtabmapReprocessActionFeedback.msg"
    "/home/ayumi/catkin_ws/devel/share/cpp/msg/RtabmapReprocessGoal.msg"
    "/home/ayumi/catkin_ws/devel/share/cpp/msg/RtabmapReprocessResult.msg"
    "/home/ayumi/catkin_ws/devel/share/cpp/msg/RtabmapReprocessFeedback.msg"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cpp/cmake" TYPE FILE FILES "/home/ayumi/catkin_ws/build/cpp/catkin_generated/installspace/cpp-msg-paths.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "/home/ayumi/catkin_ws/devel/include/cpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/roseus/ros" TYPE DIRECTORY FILES "/home/ayumi/catkin_ws/devel/share/roseus/ros/cpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/common-lisp/ros" TYPE DIRECTORY FILES "/home/ayumi/catkin_ws/devel/share/common-lisp/ros/cpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/gennodejs/ros" TYPE DIRECTORY FILES "/home/ayumi/catkin_ws/devel/share/gennodejs/ros/cpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  execute_process(COMMAND "/usr/local/bin/python3" -m compileall "/home/ayumi/catkin_ws/devel/lib/python3/dist-packages/cpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/python3/dist-packages" TYPE DIRECTORY FILES "/home/ayumi/catkin_ws/devel/lib/python3/dist-packages/cpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/ayumi/catkin_ws/build/cpp/catkin_generated/installspace/cpp.pc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cpp/cmake" TYPE FILE FILES "/home/ayumi/catkin_ws/build/cpp/catkin_generated/installspace/cpp-msg-extras.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cpp/cmake" TYPE FILE FILES
    "/home/ayumi/catkin_ws/build/cpp/catkin_generated/installspace/cppConfig.cmake"
    "/home/ayumi/catkin_ws/build/cpp/catkin_generated/installspace/cppConfig-version.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cpp" TYPE FILE FILES "/home/ayumi/catkin_ws/src/cpp/package.xml")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cpp" TYPE PROGRAM FILES "/home/ayumi/catkin_ws/build/cpp/catkin_generated/installspace/gen_numbers.py")
endif()

