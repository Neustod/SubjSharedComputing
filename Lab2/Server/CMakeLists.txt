cmake_minimum_required(VERSION 3.31)

project(Server VERSION 1.0
 DESCRIPTION ""
 LANGUAGES CXX)
 
add_executable(Server src/main.cpp
	src/CoreService.cpp
	src/Session.cpp)

target_include_directories(Server PRIVATE "${PROJECT_SOURCE_DIR}/include/")
