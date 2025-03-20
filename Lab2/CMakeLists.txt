cmake_minimum_required(VERSION 3.31)

project(Lab2 VERSION 1.0
 DESCRIPTION ""
 LANGUAGES CXX)

add_subdirectory(${PROJECT_SOURCE_DIR}/Server/)
add_subdirectory(${PROJECT_SOURCE_DIR}/Client/)

