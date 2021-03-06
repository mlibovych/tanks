cmake_minimum_required(VERSION 3.18.2)

project(app LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED on)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe -Wall -Wextra -Werror -Wall -Wpedantic -Wno-unused-parameter")

set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(Framework)
add_subdirectory(Project2)
