# This CMake file may serve as a kind of template for future cmake files of mine (still learning cmake lmao)
cmake_minimum_required(VERSION 3.20)

project(SimpleSnake CXX)

# Dependencies
include(FetchContent)

# Raylib Stuff
set(RAYLIB_VERSION 4.5.0)
FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    FIND_PACKAGE_ARGS ${RAYLIB_VERSION}
)
SET(RAYLIB_BUILD_EXAMPLES OFF CACHE INTERNAL "")
FetchContent_MakeAvailable(raylib)

# Variable containing source files for refrerence later
set(SOURCES
    src/main.cpp
    src/game.cpp
    src/snake.cpp
    src/utilities/utilities.cpp
    src/states/game_state.cpp
    src/states/main_menu_state.cpp
    src/states/playing_state.cpp
    src/states/pause_menu_state.cpp
    src/states/game_over_state.cpp
    src/app.rc
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

target_link_libraries(${PROJECT_NAME} raylib)

target_include_directories(${PROJECT_NAME} PRIVATE src)

# Might add console window disabling for later
#---------------

# MacOS Stuff
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()