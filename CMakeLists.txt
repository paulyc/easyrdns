cmake_minimum_required(VERSION 3.5)

project(easyrdns LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(easyrdns SHARED libmain.c easyrdns.h easyrdns.cpp easyrdns.hpp)
add_executable(getrdns main.cpp cxxopts.hpp easyrdns)
target_link_libraries(getrdns easyrdns)
