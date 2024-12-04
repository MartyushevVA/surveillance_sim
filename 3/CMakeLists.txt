cmake_minimum_required(VERSION 3.16)
project(SecuritySystemSimulation)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage -Wall -Wextra")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")

enable_testing()

find_package(SFML 2.6.1 COMPONENTS graphics window system REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(GTest REQUIRED)

add_library(game_lib
    import/import.cpp
    view/graphics.cpp
    model/game/game.cpp
    model/modules/router.cpp
    model/modules/sensor.cpp
    model/modules/weapon.cpp
    model/objects/moving.cpp
    model/objects/suspect.cpp
    model/system/ai.cpp
    model/system/environment.cpp
    model/objects/base_platform.cpp
)

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

)

target_link_libraries(game_lib
    sfml-graphics 
    sfml-window 
    sfml-system
    nlohmann_json::nlohmann_json
    GTest::gtest
    GTest::gtest_main
)

#add_executable(sss main.cpp)

#target_link_libraries(sss PRIVATE game_lib)

#target_include_directories(sss PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(unit_tests
    tests/test_main.cpp
    tests/environment_test.cpp
    tests/platform_test.cpp
    tests/module_test.cpp
    tests/movement_test.cpp
    tests/ai_test.cpp
    tests/suspect_test.cpp
)

target_link_libraries(unit_tests
    game_lib
    GTest::gtest
    GTest::gtest_main)




# rm -rf build; cmake -S . -B build; cmake --build build; build/unit_tests
# lcov --directory build/CMakeFiles/game_lib.dir/ --capture --output-file coverage.info --exclude "/usr/include/*"
# genhtml coverage.info --output-directory out --ignore-errors inconsistent --ignore-errors corrupt
# firefox out/index.html


# doxygen; firefox doxygen/html/index.html