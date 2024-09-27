cmake_minimum_required(VERSION 3.16)
project(lab2)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -lgtest -lgtest_main -pthread")


add_library(simple_class sclass.h sclass.cpp alllibs.h)
add_library(complex_class sclass.h sclass.cpp hclass.h hclass.cpp alllibs.h)

option(COMPLEX_CLASS "Проверить сложный класс" OFF)
if(COMPLEX_CLASS)
    link_libraries(complex_class)
    add_executable(test_class test_hclass.cpp)
else()
    link_libraries(simple_class)
    add_executable(test_class test_sclass.cpp)
endif()