cmake_minimum_required(VERSION 3.0.2)
project(cpp)
find_package(Open3D HINTS ${CMAKE_INSTALL_PREFIX}/lib/cmake)
SET(INCLUDE_DIRS
${Open3D_INCLUDE_DIRS}
)
add_executable(test non_blocking.cpp)
target_link_libraries(test
${Open3D_LIBRARIES})