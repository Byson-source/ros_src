cmake_minimum_required(VERSION 2.8)
project( findEssential )

find_package(OpenCV REQUIRED)

SET(INCLUDE_DIRS
    ${OpenCV_INCLUDE_DIRS}
    
)

SET(LIBRARIES
    ${OpenCV_LIBRARIES}
)

add_executable(feature RO_estimator.cpp )
target_include_directories(feature PUBLIC ${INCLUDE_DIRS})
target_link_libraries(feature ${LIBRARIES})

