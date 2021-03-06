cmake_minimum_required (VERSION 3.6)
project(Framework VERSION 0.0.1)

set (CMAKE_CXX_STANDARD 11)

set(SOURCES ./src/launch.cpp)

set(CMAKE_BUILD_TYPE Release)

set(MY_LIBNAME Framework${CMAKE_BUILD_TYPE})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -ggdb -Wattributes ")

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMAKE")

find_library(SDL2 NAMES "SDL2")
find_library(SDL2main NAMES "SDL2main")
find_library(SDL2_image NAMES "SDL2_image")

message("SDL2 library found in ${SDL2}")

find_package(SDL2 REQUIRED)
find_package(SDL2_IMAGE REQUIRED)

Message("SDL2_FOUND: " ${SDL2_FOUND})
Message("SDL2_INCLUDE_DIR:" ${SDL2_INCLUDE_DIRS})

include_directories(${SDL2_INCLUDE_DIRS}/..)

add_library(${MY_LIBNAME} SHARED ./src/launch.cpp)
set_target_properties(${MY_LIBNAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${MY_LIBNAME} PROPERTIES PUBLIC_HEADER inc/Framework.h)

target_include_directories(${MY_LIBNAME} PRIVATE .)

#include(GNUInstallDirs)
install(TARGETS ${MY_LIBNAME}
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

target_link_libraries(${MY_LIBNAME} ${SDL2} ${SDL2main} ${SDL2_image}) # X11)

# set_target_properties(${MY_LIBNAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../..)