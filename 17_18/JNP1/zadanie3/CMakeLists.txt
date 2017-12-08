cmake_minimum_required(VERSION 3.8)
project(JNP1_Zadanie3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
set(CMAKE_CXX_STANDARD 17)

set(COMMON_SOURCE_FILES sejf.h sejf.cc)

# Tests
enable_testing()
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
add_executable(GTest tests.cpp ${COMMON_SOURCE_FILES})
target_link_libraries(GTest ${CMAKE_THREAD_LIBS_INIT} ${GTEST_BOTH_LIBRARIES})
