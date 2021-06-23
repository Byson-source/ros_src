# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "new_ros: 1 messages, 1 services")

set(MSG_I_FLAGS "-Inew_ros:/home/ayumi/catkin_ws/src/new_ros/msg;-Istd_msgs:/opt/ros/noetic/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(new_ros_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/ayumi/catkin_ws/src/new_ros/msg/IoTSensor.msg" NAME_WE)
add_custom_target(_new_ros_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "new_ros" "/home/ayumi/catkin_ws/src/new_ros/msg/IoTSensor.msg" ""
)

get_filename_component(_filename "/home/ayumi/catkin_ws/src/new_ros/srv/AddTwoPoints.srv" NAME_WE)
add_custom_target(_new_ros_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "new_ros" "/home/ayumi/catkin_ws/src/new_ros/srv/AddTwoPoints.srv" ""
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(new_ros
  "/home/ayumi/catkin_ws/src/new_ros/msg/IoTSensor.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/new_ros
)

### Generating Services
_generate_srv_cpp(new_ros
  "/home/ayumi/catkin_ws/src/new_ros/srv/AddTwoPoints.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/new_ros
)

### Generating Module File
_generate_module_cpp(new_ros
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/new_ros
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(new_ros_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(new_ros_generate_messages new_ros_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/ayumi/catkin_ws/src/new_ros/msg/IoTSensor.msg" NAME_WE)
add_dependencies(new_ros_generate_messages_cpp _new_ros_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ayumi/catkin_ws/src/new_ros/srv/AddTwoPoints.srv" NAME_WE)
add_dependencies(new_ros_generate_messages_cpp _new_ros_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(new_ros_gencpp)
add_dependencies(new_ros_gencpp new_ros_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS new_ros_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages
_generate_msg_eus(new_ros
  "/home/ayumi/catkin_ws/src/new_ros/msg/IoTSensor.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/new_ros
)

### Generating Services
_generate_srv_eus(new_ros
  "/home/ayumi/catkin_ws/src/new_ros/srv/AddTwoPoints.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/new_ros
)

### Generating Module File
_generate_module_eus(new_ros
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/new_ros
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(new_ros_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(new_ros_generate_messages new_ros_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/ayumi/catkin_ws/src/new_ros/msg/IoTSensor.msg" NAME_WE)
add_dependencies(new_ros_generate_messages_eus _new_ros_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ayumi/catkin_ws/src/new_ros/srv/AddTwoPoints.srv" NAME_WE)
add_dependencies(new_ros_generate_messages_eus _new_ros_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(new_ros_geneus)
add_dependencies(new_ros_geneus new_ros_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS new_ros_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(new_ros
  "/home/ayumi/catkin_ws/src/new_ros/msg/IoTSensor.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/new_ros
)

### Generating Services
_generate_srv_lisp(new_ros
  "/home/ayumi/catkin_ws/src/new_ros/srv/AddTwoPoints.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/new_ros
)

### Generating Module File
_generate_module_lisp(new_ros
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/new_ros
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(new_ros_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(new_ros_generate_messages new_ros_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/ayumi/catkin_ws/src/new_ros/msg/IoTSensor.msg" NAME_WE)
add_dependencies(new_ros_generate_messages_lisp _new_ros_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ayumi/catkin_ws/src/new_ros/srv/AddTwoPoints.srv" NAME_WE)
add_dependencies(new_ros_generate_messages_lisp _new_ros_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(new_ros_genlisp)
add_dependencies(new_ros_genlisp new_ros_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS new_ros_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages
_generate_msg_nodejs(new_ros
  "/home/ayumi/catkin_ws/src/new_ros/msg/IoTSensor.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/new_ros
)

### Generating Services
_generate_srv_nodejs(new_ros
  "/home/ayumi/catkin_ws/src/new_ros/srv/AddTwoPoints.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/new_ros
)

### Generating Module File
_generate_module_nodejs(new_ros
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/new_ros
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(new_ros_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(new_ros_generate_messages new_ros_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/ayumi/catkin_ws/src/new_ros/msg/IoTSensor.msg" NAME_WE)
add_dependencies(new_ros_generate_messages_nodejs _new_ros_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ayumi/catkin_ws/src/new_ros/srv/AddTwoPoints.srv" NAME_WE)
add_dependencies(new_ros_generate_messages_nodejs _new_ros_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(new_ros_gennodejs)
add_dependencies(new_ros_gennodejs new_ros_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS new_ros_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(new_ros
  "/home/ayumi/catkin_ws/src/new_ros/msg/IoTSensor.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/new_ros
)

### Generating Services
_generate_srv_py(new_ros
  "/home/ayumi/catkin_ws/src/new_ros/srv/AddTwoPoints.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/new_ros
)

### Generating Module File
_generate_module_py(new_ros
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/new_ros
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(new_ros_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(new_ros_generate_messages new_ros_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/ayumi/catkin_ws/src/new_ros/msg/IoTSensor.msg" NAME_WE)
add_dependencies(new_ros_generate_messages_py _new_ros_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ayumi/catkin_ws/src/new_ros/srv/AddTwoPoints.srv" NAME_WE)
add_dependencies(new_ros_generate_messages_py _new_ros_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(new_ros_genpy)
add_dependencies(new_ros_genpy new_ros_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS new_ros_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/new_ros)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/new_ros
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(new_ros_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/new_ros)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/new_ros
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_eus)
  add_dependencies(new_ros_generate_messages_eus std_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/new_ros)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/new_ros
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(new_ros_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/new_ros)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/new_ros
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_nodejs)
  add_dependencies(new_ros_generate_messages_nodejs std_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/new_ros)
  install(CODE "execute_process(COMMAND \"/usr/local/bin/python3\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/new_ros\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/new_ros
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(new_ros_generate_messages_py std_msgs_generate_messages_py)
endif()
