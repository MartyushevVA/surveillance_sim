cmake_minimum_required(VERSION 3.16)
project(lab2)

enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")

add_library(simple_class sclass.h sclass.cpp alllibs.h)
add_library(complex_class sclass.h sclass.cpp hclass.h hclass.cpp alllibs.h)

find_package(GTest REQUIRED)

option(COMPLEX_CLASS "Проверить сложный класс" OFF)

if(COMPLEX_CLASS)
    add_executable(test_class test_hclass.cpp)
    target_link_libraries(test_class GTest::gtest GTest::gtest_main complex_class)
else()
    add_executable(test_class test_sclass.cpp)
    target_link_libraries(test_class GTest::gtest GTest::gtest_main simple_class)
endif()

add_test(NAME MyTests COMMAND test_class)

# rm -rf build
# cmake -S . -DCOMPLEX_CLASS=OFF(ON) -B build
# cmake --build build; build/test_class
# lcov --directory build/CMakeFiles/simple(complex)_class.dir/ --capture --output-file coverage.info; genhtml coverage.info --output-directory out; firefox out/index.html

# doxygen; firefox doxygen/html/index.html

# --exclude-throw-branches gcovr