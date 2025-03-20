cmake_minimum_required(VERSION 3.31)

project(Lab1 VERSION 1.0
 DESCRIPTION ""
 LANGUAGES CXX)


add_executable(Lab1 src/main.cpp src/Integrator.cpp)

target_include_directories(Lab1 PRIVATE "${PROJECT_SOURCE_DIR}/include/")

