cmake_minimum_required(VERSION 3.10)
project(RideSharePlanner)

set(CMAKE_CXX_STANDARD 17)

add_executable(RideSharePlanner
    main.cpp
    planner_gas.cpp
    planner_gaso1.cpp
    planner_gaso2.cpp
    additive_tree.cpp
)