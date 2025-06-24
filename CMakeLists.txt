cmake_minimum_required(VERSION 3.10)
project(RideSharePlanner)

set(CMAKE_CXX_STANDARD 17)

add_executable(RideSharePlanner
    src/main.cpp
    src/planner_gas.cpp
    src/planner_gaso1.cpp
    src/planner_gaso2.cpp
    src/additive_tree.cpp
)