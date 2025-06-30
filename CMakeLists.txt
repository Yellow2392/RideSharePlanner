cmake_minimum_required(VERSION 3.10)
project(RideSharePlanner)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(SFML 3.0 COMPONENTS Graphics Window Audio System CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(RideSharePlanner
    src/visual.cpp
    src/planner_gas.cpp
    src/planner_gaso1.cpp
    src/planner_gaso2.cpp
    src/additive_tree.cpp
    src/gui_visual.cpp
    src/json_parsing.cpp
)

target_link_libraries(RideSharePlanner PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Audio nlohmann_json::nlohmann_json)

set(FONT_SOURCE_PATH "${CMAKE_SOURCE_DIR}/arial.ttf")
if (NOT EXISTS "${FONT_SOURCE_PATH}")
    message(WARNING "Font file not found at ${FONT_SOURCE_PATH}. Please ensure arial.ttf is in the correct location or update FONT_SOURCE_PATH in CMakeLists.txt.")
endif()

add_custom_command(
    TARGET RideSharePlanner
    POST_BUILD            
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${FONT_SOURCE_PATH}"                  
            "$<TARGET_FILE_DIR:RideSharePlanner>/arial.ttf"
    COMMENT "Copying arial.ttf to build directory"
)

set(DATA_JSON_SOURCE_PATH "${CMAKE_SOURCE_DIR}/backend/data/data.json")
if (NOT EXISTS "${DATA_JSON_SOURCE_PATH}")
    message(WARNING "Data JSON file not found at ${DATA_JSON_SOURCE_PATH}. Please ensure datos.json is in the correct location or update DATA_JSON_SOURCE_PATH in CMakeLists.txt.")
endif()

add_custom_command(
    TARGET RideSharePlanner
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${DATA_JSON_SOURCE_PATH}"
            "$<TARGET_FILE_DIR:RideSharePlanner>/data.json"
    COMMENT "Copying data.json to build directory"
)
