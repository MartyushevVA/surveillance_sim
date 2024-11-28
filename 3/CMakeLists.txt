cmake_minimum_required(VERSION 3.16)
project(SecuritySystemSimulation)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")

enable_testing()

find_package(SFML 2.6.1 COMPONENTS graphics window system REQUIRED)
find_package(nlohmann_json REQUIRED)

add_library(game_lib
    implts/ai.cpp
    implts/game.cpp
    implts/environment.cpp
    implts/input.cpp
    implts/moving.cpp
    implts/platform.cpp
    implts/router.cpp
    implts/sensor.cpp
    implts/suspect.cpp
    implts/weapon.cpp
)

target_include_directories(game_lib PUBLIC 
    ${CMAKE_SOURCE_DIR}/headers
)

target_link_libraries(game_lib PUBLIC
    sfml-graphics 
    sfml-window 
    sfml-system
    nlohmann_json::nlohmann_json
)

add_executable(${PROJECT_NAME} implts/main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE game_lib)




# rm -rf build
# cmake -S . -DCOMPLEX_CLASS=OFF(ON) -B build
# cmake --build build; build/test_class
# lcov --directory build/CMakeFiles/simple(complex)_class.dir/ --capture --output-file coverage.info; genhtml coverage.info --output-directory out; firefox out/index.html

# doxygen; firefox doxygen/html/index.html

# --exclude-throw-branches gcovr


# rm -rf build; cmake -S . -B build; cmake --build build; build/SecuritySystemSimulation