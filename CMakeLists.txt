cmake_minimum_required(VERSION 3.10)

#Set up project
project(multi-pong VERSION 1.0)
include(cmake/CompileFeatures.cmake)

#Set module path
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

#Find libraries
find_package(Threads)
find_package(SFML REQUIRED audio network graphics window system)

add_subdirectory(common)
add_subdirectory(server)
add_subdirectory(client)
