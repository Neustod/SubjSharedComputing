cmake_minimum_required(VERSION 3.31)

project(integral_library VERSION 1.0
 DESCRIPTION ""
 LANGUAGES CXX)

add_library(integral_library SHARED
	src/Integrator.cpp
	src/CalculationThread.cpp
	src/ThreadManager.cpp)
 
add_library(integral::library ALIAS integral_library)
 
target_include_directories(integral_library PUBLIC "${PROJECT_SOURCE_DIR}/include")