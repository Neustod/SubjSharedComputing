cmake_minimum_required(VERSION 3.31)

project(Client VERSION 1.0
 DESCRIPTION ""
 LANGUAGES CXX)
 
add_executable(Client src/main.cpp
	src/EndpointService.cpp
	src/Integrator.cpp)

target_include_directories(Client PRIVATE "${PROJECT_SOURCE_DIR}/include/")
target_include_directories(Client PRIVATE "D:/Vs_projs/boost_1_87_0/")
#target_include_directories(Client PRIVATE "/run/media/ischwarz/DATA/Vs_projs/boost_1_87_0/")
