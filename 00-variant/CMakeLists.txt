cmake_minimum_required(VERSION 3.7.2)
project(00-variant)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_EXTENSIONS on)

file(GLOB_RECURSE PROJECT_SOURCES src/*.cpp)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
