cmake_minimum_required(VERSION 4.0)
project(chess_bot)

file(GLOB_RECURSE SRC_FILES includes/*.cpp)
add_executable(chess_bot main.cpp ${SRC_FILES})
target_include_directories(chess_bot PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes)
