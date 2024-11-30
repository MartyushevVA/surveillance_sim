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

)

target_link_libraries(game_lib PUBLIC
    sfml-graphics 
    sfml-window 
    sfml-system
    nlohmann_json::nlohmann_json
)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE game_lib)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/headers
    ${CMAKE_CURRENT_SOURCE_DIR}/import
    ${CMAKE_CURRENT_SOURCE_DIR}/inoutstream
)




# rm -rf build
# cmake -S . -DCOMPLEX_CLASS=OFF(ON) -B build
# cmake --build build; build/test_class
# lcov --directory build/CMakeFiles/simple(complex)_class.dir/ --capture --output-file coverage.info; genhtml coverage.info --output-directory out; firefox out/index.html

# doxygen; firefox doxygen/html/index.html

# --exclude-throw-branches gcovr


# rm -rf build; cmake -S . -B build; cmake --build build; build/SecuritySystemSimulation