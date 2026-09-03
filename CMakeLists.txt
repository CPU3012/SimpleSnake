# CMakeLists.txt
cmake_minimum_required(VERSION 3.50)

project(SimpleSnake CXX)

# Dependencies
include(FetchContent)

# Fetch Raylib
set(RAYLIB_VERSION 6.0)
FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    FIND_PACKAGE_ARGS ${RAYLIB_VERSION}
)
SET(RAYLIB_BUILD_EXAMPLES OFF CACHE INTERNAL "")
FetchContent_MakeAvailable(raylib)

# Variable containing all source files that will be compiled
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
    src/ui/ui.cpp
    src/ui/playing_ui.cpp
    src/ui/menu_ui.cpp
    src/app.rc
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

target_link_libraries(${PROJECT_NAME} raylib)

target_include_directories(${PROJECT_NAME} PRIVATE src)



# copy the assets folder
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/src/assets"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
)

if (MINGW)
    target_link_options(${PROJECT_NAME} PRIVATE -static-libgcc -static-libstdc++ -static)
endif()

# MacOS Stuff
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")

elseif (WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
    
endif()