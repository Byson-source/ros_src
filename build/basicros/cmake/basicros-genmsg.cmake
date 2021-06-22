# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "basicros: 1 messages, 1 services")

set(MSG_I_FLAGS "-Ibasicros:/home/ayumi/catkin_ws/src/basicros/msg;-Istd_msgs:/opt/ros/noetic/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(basicros_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/ayumi/catkin_ws/src/basicros/msg/IoTSensor.msg" NAME_WE)
add_custom_target(_basicros_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "basicros" "/home/ayumi/catkin_ws/src/basicros/msg/IoTSensor.msg" ""
)

get_filename_component(_filename "/home/ayumi/catkin_ws/src/basicros/srv/AddTwoPoints.srv" NAME_WE)
add_custom_target(_basicros_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "basicros" "/home/ayumi/catkin_ws/src/basicros/srv/AddTwoPoints.srv" ""
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(basicros
  "/home/ayumi/catkin_ws/src/basicros/msg/IoTSensor.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/basicros
)

### Generating Services
_generate_srv_cpp(basicros
  "/home/ayumi/catkin_ws/src/basicros/srv/AddTwoPoints.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/basicros
)

### Generating Module File
_generate_module_cpp(basicros
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/basicros
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(basicros_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(basicros_generate_messages basicros_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/ayumi/catkin_ws/src/basicros/msg/IoTSensor.msg" NAME_WE)
add_dependencies(basicros_generate_messages_cpp _basicros_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ayumi/catkin_ws/src/basicros/srv/AddTwoPoints.srv" NAME_WE)
add_dependencies(basicros_generate_messages_cpp _basicros_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(basicros_gencpp)
add_dependencies(basicros_gencpp basicros_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS basicros_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages
_generate_msg_eus(basicros
  "/home/ayumi/catkin_ws/src/basicros/msg/IoTSensor.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/basicros
)

### Generating Services
_generate_srv_eus(basicros
  "/home/ayumi/catkin_ws/src/basicros/srv/AddTwoPoints.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/basicros
)

### Generating Module File
_generate_module_eus(basicros
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/basicros
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(basicros_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(basicros_generate_messages basicros_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/ayumi/catkin_ws/src/basicros/msg/IoTSensor.msg" NAME_WE)
add_dependencies(basicros_generate_messages_eus _basicros_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ayumi/catkin_ws/src/basicros/srv/AddTwoPoints.srv" NAME_WE)
add_dependencies(basicros_generate_messages_eus _basicros_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(basicros_geneus)
add_dependencies(basicros_geneus basicros_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS basicros_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(basicros
  "/home/ayumi/catkin_ws/src/basicros/msg/IoTSensor.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/basicros
)

### Generating Services
_generate_srv_lisp(basicros
  "/home/ayumi/catkin_ws/src/basicros/srv/AddTwoPoints.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/basicros
)

### Generating Module File
_generate_module_lisp(basicros
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/basicros
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(basicros_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(basicros_generate_messages basicros_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/ayumi/catkin_ws/src/basicros/msg/IoTSensor.msg" NAME_WE)
add_dependencies(basicros_generate_messages_lisp _basicros_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ayumi/catkin_ws/src/basicros/srv/AddTwoPoints.srv" NAME_WE)
add_dependencies(basicros_generate_messages_lisp _basicros_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(basicros_genlisp)
add_dependencies(basicros_genlisp basicros_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS basicros_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages
_generate_msg_nodejs(basicros
  "/home/ayumi/catkin_ws/src/basicros/msg/IoTSensor.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/basicros
)

### Generating Services
_generate_srv_nodejs(basicros
  "/home/ayumi/catkin_ws/src/basicros/srv/AddTwoPoints.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/basicros
)

### Generating Module File
_generate_module_nodejs(basicros
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/basicros
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(basicros_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(basicros_generate_messages basicros_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/ayumi/catkin_ws/src/basicros/msg/IoTSensor.msg" NAME_WE)
add_dependencies(basicros_generate_messages_nodejs _basicros_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ayumi/catkin_ws/src/basicros/srv/AddTwoPoints.srv" NAME_WE)
add_dependencies(basicros_generate_messages_nodejs _basicros_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(basicros_gennodejs)
add_dependencies(basicros_gennodejs basicros_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS basicros_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(basicros
  "/home/ayumi/catkin_ws/src/basicros/msg/IoTSensor.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/basicros
)

### Generating Services
_generate_srv_py(basicros
  "/home/ayumi/catkin_ws/src/basicros/srv/AddTwoPoints.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/basicros
)

### Generating Module File
_generate_module_py(basicros
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/basicros
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(basicros_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(basicros_generate_messages basicros_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/ayumi/catkin_ws/src/basicros/msg/IoTSensor.msg" NAME_WE)
add_dependencies(basicros_generate_messages_py _basicros_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ayumi/catkin_ws/src/basicros/srv/AddTwoPoints.srv" NAME_WE)
add_dependencies(basicros_generate_messages_py _basicros_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(basicros_genpy)
add_dependencies(basicros_genpy basicros_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS basicros_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/basicros)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/basicros
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(basicros_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/basicros)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/basicros
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_eus)
  add_dependencies(basicros_generate_messages_eus std_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/basicros)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/basicros
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(basicros_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/basicros)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/basicros
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_nodejs)
  add_dependencies(basicros_generate_messages_nodejs std_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/basicros)
  install(CODE "execute_process(COMMAND \"/usr/local/bin/python3\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/basicros\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/basicros
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(basicros_generate_messages_py std_msgs_generate_messages_py)
endif()
