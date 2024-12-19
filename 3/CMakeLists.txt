cmake_minimum_required(VERSION 3.16)
project(SecuritySystemSimulation)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs --coverage -Wall -Wextra -fprofile-update=atomic")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs --coverage")

enable_testing()

find_package(nlohmann_json REQUIRED)

add_library(game_lib
    model/game/game.cpp
    model/modules/router.cpp
    model/modules/sensor.cpp
    model/modules/weapon.cpp
    model/objects/moving.cpp
    model/objects/objects.cpp
    model/system/ai.cpp
    model/system/environment.cpp
    model/objects/base_platform.cpp
)

add_library(view_lib
    view/graphics.cpp
)

add_library(import_lib
    import/import.cpp
)

add_library(controller_lib
    model/controller/controller.cpp
)

add_library(myvector_lib
    myvector/myvector.h
)

set_target_properties(myvector_lib PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(game_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/import
    ${CMAKE_CURRENT_SOURCE_DIR}/view
    ${CMAKE_CURRENT_SOURCE_DIR}/model
    ${CMAKE_CURRENT_SOURCE_DIR}/model/game
    ${CMAKE_CURRENT_SOURCE_DIR}/model/modules
    ${CMAKE_CURRENT_SOURCE_DIR}/model/system
    ${CMAKE_CURRENT_SOURCE_DIR}/model/objects
    ${CMAKE_CURRENT_SOURCE_DIR}/tests
    ${CMAKE_CURRENT_SOURCE_DIR}/controller
)

target_link_libraries(game_lib myvector_lib)
target_link_libraries(view_lib nlohmann_json::nlohmann_json sfml-graphics sfml-window sfml-system)
target_link_libraries(import_lib nlohmann_json::nlohmann_json sfml-graphics sfml-window sfml-system)
target_link_libraries(controller_lib game_lib view_lib import_lib)

add_compile_options(-fsanitize=thread -g)
add_link_options(-fsanitize=thread)

option(T "Запуск тестов" OFF)
option(TIMING "Запуск тестов на скорость" OFF)

if(T)
    add_executable(sss
        tests/test_main.cpp
        tests/ai_test.cpp
        tests/complex_scenarios_test.cpp
        tests/module_test.cpp
        tests/movement_test.cpp
        tests/objects_test.cpp
        tests/vector_test.cpp
        tests/multithreading_test.cpp
    )
    find_package(GTest REQUIRED)
    target_link_libraries(sss game_lib GTest::gtest GTest::gtest_main)
elseif(TIMING)
    add_executable(sss tests/timing.cpp)
    find_package(SFML 2.6.1 COMPONENTS graphics window system REQUIRED)
    target_link_libraries(sss game_lib)
else()
    add_executable(sss main.cpp)
    find_package(SFML 2.6.1 COMPONENTS graphics window system REQUIRED)
    target_link_libraries(sss controller_lib)
endif()




# rm -rf build; cmake -S . -DT=ON -B build; cmake --build build; build/sss
# lcov --directory build/CMakeFiles/game_lib.dir/ --capture --output-file coverage.info --exclude "/usr/include/*"; genhtml coverage.info --output-directory out --ignore-errors inconsistent --ignore-errors corrupt; firefox out/index.html


# doxygen; firefox doxygen/html/index.html