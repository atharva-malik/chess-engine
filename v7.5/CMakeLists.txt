cmake_minimum_required(VERSION 4.0)
project(chess_bot)

# set(CMAKE_C_COMPILER "C:\\msys64\\mingw64\\bin\\gcc.exe")
# set(CMAKE_CXX_COMPILER "C:\\msys64\\mingw64\\bin\\g++.exe")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# if(CMAKE_BUILD_TYPE EQUAL "Debug")
#     target_compile_options(my_executable PRIVATE -g)
# elseif(CMAKE_BUILD_TYPE EQUAL "Release")
# endif()

file(GLOB_RECURSE SRC_FILES includes/*.cpp)
file(GLOB_RECURSE SRC_FILES includes/*.hpp)
add_executable(chess_bot main.cpp ${SRC_FILES})

# target_compile_options(chess_bot PRIVATE /O2)

target_include_directories(chess_bot PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes)
