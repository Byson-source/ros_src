# CMake generated Testfile for 
# Source directory: /home/ayumi/catkin_ws/src/map_merge/map_merge_3d
# Build directory: /home/ayumi/catkin_ws/build/map_merge/map_merge_3d
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(_ctest_map_merge_3d_gtest_test_map_merging "/home/ayumi/catkin_ws/build/catkin_generated/env_cached.sh" "/usr/local/bin/python3" "/opt/ros/noetic/share/catkin/cmake/test/run_tests.py" "/home/ayumi/catkin_ws/build/test_results/map_merge_3d/gtest-test_map_merging.xml" "--return-code" "/home/ayumi/catkin_ws/devel/lib/map_merge_3d/test_map_merging --gtest_output=xml:/home/ayumi/catkin_ws/build/test_results/map_merge_3d/gtest-test_map_merging.xml")
set_tests_properties(_ctest_map_merge_3d_gtest_test_map_merging PROPERTIES  _BACKTRACE_TRIPLES "/opt/ros/noetic/share/catkin/cmake/test/tests.cmake;160;add_test;/opt/ros/noetic/share/catkin/cmake/test/gtest.cmake;98;catkin_run_tests_target;/opt/ros/noetic/share/catkin/cmake/test/gtest.cmake;37;_catkin_add_google_test;/home/ayumi/catkin_ws/src/map_merge/map_merge_3d/CMakeLists.txt;122;catkin_add_gtest;/home/ayumi/catkin_ws/src/map_merge/map_merge_3d/CMakeLists.txt;0;")
add_test(_ctest_map_merge_3d_roslaunch-check_launch "/home/ayumi/catkin_ws/build/catkin_generated/env_cached.sh" "/usr/local/bin/python3" "/opt/ros/noetic/share/catkin/cmake/test/run_tests.py" "/home/ayumi/catkin_ws/build/test_results/map_merge_3d/roslaunch-check_launch.xml" "--return-code" "/opt/cmake-3.21.0-linux-x86_64/bin/cmake -E make_directory /home/ayumi/catkin_ws/build/test_results/map_merge_3d" "/opt/ros/noetic/share/roslaunch/cmake/../scripts/roslaunch-check -o \"/home/ayumi/catkin_ws/build/test_results/map_merge_3d/roslaunch-check_launch.xml\" \"/home/ayumi/catkin_ws/src/map_merge/map_merge_3d/launch\" ")
set_tests_properties(_ctest_map_merge_3d_roslaunch-check_launch PROPERTIES  _BACKTRACE_TRIPLES "/opt/ros/noetic/share/catkin/cmake/test/tests.cmake;160;add_test;/opt/ros/noetic/share/roslaunch/cmake/roslaunch-extras.cmake;66;catkin_run_tests_target;/home/ayumi/catkin_ws/src/map_merge/map_merge_3d/CMakeLists.txt;126;roslaunch_add_file_check;/home/ayumi/catkin_ws/src/map_merge/map_merge_3d/CMakeLists.txt;0;")