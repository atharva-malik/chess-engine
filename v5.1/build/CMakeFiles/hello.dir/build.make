# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 4.0

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "C:\Atharva\Programming\Python\Python Scripts\chess-engine\v5.1"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "C:\Atharva\Programming\Python\Python Scripts\chess-engine\v5.1\build"

# Include any dependencies generated for this target.
include CMakeFiles/hello.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/hello.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/hello.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/hello.dir/flags.make

CMakeFiles/hello.dir/codegen:
.PHONY : CMakeFiles/hello.dir/codegen

CMakeFiles/hello.dir/includes/bot.cpp.obj: CMakeFiles/hello.dir/flags.make
CMakeFiles/hello.dir/includes/bot.cpp.obj: C:/Atharva/Programming/Python/Python\ Scripts/chess-engine/v5.1/includes/bot.cpp
CMakeFiles/hello.dir/includes/bot.cpp.obj: CMakeFiles/hello.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="C:\Atharva\Programming\Python\Python Scripts\chess-engine\v5.1\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/hello.dir/includes/bot.cpp.obj"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/hello.dir/includes/bot.cpp.obj -MF CMakeFiles\hello.dir\includes\bot.cpp.obj.d -o CMakeFiles\hello.dir\includes\bot.cpp.obj -c "C:\Atharva\Programming\Python\Python Scripts\chess-engine\v5.1\includes\bot.cpp"

CMakeFiles/hello.dir/includes/bot.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/hello.dir/includes/bot.cpp.i"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "C:\Atharva\Programming\Python\Python Scripts\chess-engine\v5.1\includes\bot.cpp" > CMakeFiles\hello.dir\includes\bot.cpp.i

CMakeFiles/hello.dir/includes/bot.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/hello.dir/includes/bot.cpp.s"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "C:\Atharva\Programming\Python\Python Scripts\chess-engine\v5.1\includes\bot.cpp" -o CMakeFiles\hello.dir\includes\bot.cpp.s

CMakeFiles/hello.dir/includes/chess.cpp.obj: CMakeFiles/hello.dir/flags.make
CMakeFiles/hello.dir/includes/chess.cpp.obj: C:/Atharva/Programming/Python/Python\ Scripts/chess-engine/v5.1/includes/chess.cpp
CMakeFiles/hello.dir/includes/chess.cpp.obj: CMakeFiles/hello.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="C:\Atharva\Programming\Python\Python Scripts\chess-engine\v5.1\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/hello.dir/includes/chess.cpp.obj"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/hello.dir/includes/chess.cpp.obj -MF CMakeFiles\hello.dir\includes\chess.cpp.obj.d -o CMakeFiles\hello.dir\includes\chess.cpp.obj -c "C:\Atharva\Programming\Python\Python Scripts\chess-engine\v5.1\includes\chess.cpp"

CMakeFiles/hello.dir/includes/chess.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/hello.dir/includes/chess.cpp.i"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "C:\Atharva\Programming\Python\Python Scripts\chess-engine\v5.1\includes\chess.cpp" > CMakeFiles\hello.dir\includes\chess.cpp.i

CMakeFiles/hello.dir/includes/chess.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/hello.dir/includes/chess.cpp.s"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "C:\Atharva\Programming\Python\Python Scripts\chess-engine\v5.1\includes\chess.cpp" -o CMakeFiles\hello.dir\includes\chess.cpp.s

# Object files for target hello
hello_OBJECTS = \
"CMakeFiles/hello.dir/includes/bot.cpp.obj" \
"CMakeFiles/hello.dir/includes/chess.cpp.obj"

# External object files for target hello
hello_EXTERNAL_OBJECTS =

hello.exe: CMakeFiles/hello.dir/includes/bot.cpp.obj
hello.exe: CMakeFiles/hello.dir/includes/chess.cpp.obj
hello.exe: CMakeFiles/hello.dir/build.make
hello.exe: CMakeFiles/hello.dir/linkLibs.rsp
hello.exe: CMakeFiles/hello.dir/objects1.rsp
hello.exe: CMakeFiles/hello.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir="C:\Atharva\Programming\Python\Python Scripts\chess-engine\v5.1\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable hello.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\hello.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/hello.dir/build: hello.exe
.PHONY : CMakeFiles/hello.dir/build

CMakeFiles/hello.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\hello.dir\cmake_clean.cmake
.PHONY : CMakeFiles/hello.dir/clean

CMakeFiles/hello.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "C:\Atharva\Programming\Python\Python Scripts\chess-engine\v5.1" "C:\Atharva\Programming\Python\Python Scripts\chess-engine\v5.1" "C:\Atharva\Programming\Python\Python Scripts\chess-engine\v5.1\build" "C:\Atharva\Programming\Python\Python Scripts\chess-engine\v5.1\build" "C:\Atharva\Programming\Python\Python Scripts\chess-engine\v5.1\build\CMakeFiles\hello.dir\DependInfo.cmake" "--color=$(COLOR)"
.PHONY : CMakeFiles/hello.dir/depend

