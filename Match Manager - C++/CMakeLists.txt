cmake_minimum_required(VERSION 4.0)
project(chess_bot_match_manager)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SRC_FILES includes/*.cpp)
file(GLOB_RECURSE SRC_FILES includes/*.hpp)
add_executable(chess_bot_match_manager main.cpp ${SRC_FILES})
target_include_directories(chess_bot_match_manager PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes)
