# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/dmrfcoder/CLionProjects/CompilationPrincipleCourseDesgin

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/dmrfcoder/CLionProjects/CompilationPrincipleCourseDesgin/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/CompilationPrincipleCourseDesgin.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/CompilationPrincipleCourseDesgin.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/CompilationPrincipleCourseDesgin.dir/flags.make

CMakeFiles/CompilationPrincipleCourseDesgin.dir/main.cpp.o: CMakeFiles/CompilationPrincipleCourseDesgin.dir/flags.make
CMakeFiles/CompilationPrincipleCourseDesgin.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/dmrfcoder/CLionProjects/CompilationPrincipleCourseDesgin/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/CompilationPrincipleCourseDesgin.dir/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/CompilationPrincipleCourseDesgin.dir/main.cpp.o -c /Users/dmrfcoder/CLionProjects/CompilationPrincipleCourseDesgin/main.cpp

CMakeFiles/CompilationPrincipleCourseDesgin.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CompilationPrincipleCourseDesgin.dir/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/dmrfcoder/CLionProjects/CompilationPrincipleCourseDesgin/main.cpp > CMakeFiles/CompilationPrincipleCourseDesgin.dir/main.cpp.i

CMakeFiles/CompilationPrincipleCourseDesgin.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CompilationPrincipleCourseDesgin.dir/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/dmrfcoder/CLionProjects/CompilationPrincipleCourseDesgin/main.cpp -o CMakeFiles/CompilationPrincipleCourseDesgin.dir/main.cpp.s

# Object files for target CompilationPrincipleCourseDesgin
CompilationPrincipleCourseDesgin_OBJECTS = \
"CMakeFiles/CompilationPrincipleCourseDesgin.dir/main.cpp.o"

# External object files for target CompilationPrincipleCourseDesgin
CompilationPrincipleCourseDesgin_EXTERNAL_OBJECTS =

CompilationPrincipleCourseDesgin: CMakeFiles/CompilationPrincipleCourseDesgin.dir/main.cpp.o
CompilationPrincipleCourseDesgin: CMakeFiles/CompilationPrincipleCourseDesgin.dir/build.make
CompilationPrincipleCourseDesgin: CMakeFiles/CompilationPrincipleCourseDesgin.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/dmrfcoder/CLionProjects/CompilationPrincipleCourseDesgin/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable CompilationPrincipleCourseDesgin"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/CompilationPrincipleCourseDesgin.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/CompilationPrincipleCourseDesgin.dir/build: CompilationPrincipleCourseDesgin

.PHONY : CMakeFiles/CompilationPrincipleCourseDesgin.dir/build

CMakeFiles/CompilationPrincipleCourseDesgin.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/CompilationPrincipleCourseDesgin.dir/cmake_clean.cmake
.PHONY : CMakeFiles/CompilationPrincipleCourseDesgin.dir/clean

CMakeFiles/CompilationPrincipleCourseDesgin.dir/depend:
	cd /Users/dmrfcoder/CLionProjects/CompilationPrincipleCourseDesgin/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/dmrfcoder/CLionProjects/CompilationPrincipleCourseDesgin /Users/dmrfcoder/CLionProjects/CompilationPrincipleCourseDesgin /Users/dmrfcoder/CLionProjects/CompilationPrincipleCourseDesgin/cmake-build-debug /Users/dmrfcoder/CLionProjects/CompilationPrincipleCourseDesgin/cmake-build-debug /Users/dmrfcoder/CLionProjects/CompilationPrincipleCourseDesgin/cmake-build-debug/CMakeFiles/CompilationPrincipleCourseDesgin.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/CompilationPrincipleCourseDesgin.dir/depend

