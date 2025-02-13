cmake_minimum_required(VERSION 3.31)

project(Lab1 VERSION 1.0
 DESCRIPTION ""
 LANGUAGES CXX)


add_subdirectory(${PROJECT_SOURCE_DIR}/ParallelIntegral/)


add_executable(Lab1 src/main.cpp)

target_link_libraries(Lab1 PRIVATE integral::library)


target_include_directories(Lab1 PRIVATE "${PROJECT_SOURCE_DIR}/include/")


add_custom_command(TARGET Lab1 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy 
    $<TARGET_FILE:integral::library> ${CMAKE_BINARY_DIR}/$<CONFIG>/
)



 
 



