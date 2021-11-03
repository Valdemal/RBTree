cmake_minimum_required(VERSION 3.17)
project(RBTree)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(libs/treelib)

add_executable(RBTree main.cpp)
target_link_libraries(RBTree libtree)