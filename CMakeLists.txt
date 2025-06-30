cmake_minimum_required(VERSION 3.10)
project(RideSharePlanner)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configuración de compilación
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Ejecutable principal original
add_executable(RideSharePlanner
    src/main.cpp
    src/planner_gas.cpp
    src/planner_gaso1.cpp
    src/planner_gaso2.cpp
    src/additive_tree.cpp
)

# Ejecutable de benchmark
add_executable(BenchmarkSuite
    src/main_benchmark.cpp
    src/planner_gas.cpp
    src/planner_gaso1.cpp
    src/planner_gaso2.cpp
    src/additive_tree.cpp
)

# Incluir directorios de headers
target_include_directories(RideSharePlanner PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(BenchmarkSuite PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Configuración para debugging
set_target_properties(RideSharePlanner PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

set_target_properties(BenchmarkSuite PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Opción para habilitar profiling
option(ENABLE_PROFILING "Enable profiling support" OFF)
if(ENABLE_PROFILING)
    target_compile_options(RideSharePlanner PRIVATE -pg)
    target_link_options(RideSharePlanner PRIVATE -pg)
    target_compile_options(BenchmarkSuite PRIVATE -pg)
    target_link_options(BenchmarkSuite PRIVATE -pg)
endif()

# Información de build
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
