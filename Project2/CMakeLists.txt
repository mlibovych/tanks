cmake_minimum_required(VERSION 3.18.2)
project(proj)

set(TARGET tanks)


SET(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pipe -Wno-inconsistent-missing-override -Wall -Wextra -Werror -Wall -Wpedantic -Wno-unused-parameter")

set(SOURCES src/game.cpp src/my_framework.cpp src/objects.cpp
            src/animation.cpp src/tanks.cpp) 

include_directories(inc)
include_directories(../Framework/inc)

add_executable(${TARGET} ${SOURCES})

target_link_libraries(${TARGET} FrameworkRelease)
set_target_properties(${TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../..)