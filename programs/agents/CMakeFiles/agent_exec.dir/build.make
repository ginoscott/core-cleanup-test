# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/spjy/cosmos/source/core/programs/agents

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/spjy/cosmos/source/core/programs/agents

# Include any dependencies generated for this target.
include CMakeFiles/agent_exec.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/agent_exec.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/agent_exec.dir/flags.make

CMakeFiles/agent_exec.dir/agent_exec.cpp.o: CMakeFiles/agent_exec.dir/flags.make
CMakeFiles/agent_exec.dir/agent_exec.cpp.o: agent_exec.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/spjy/cosmos/source/core/programs/agents/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/agent_exec.dir/agent_exec.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/agent_exec.dir/agent_exec.cpp.o -c /home/spjy/cosmos/source/core/programs/agents/agent_exec.cpp

CMakeFiles/agent_exec.dir/agent_exec.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/agent_exec.dir/agent_exec.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/spjy/cosmos/source/core/programs/agents/agent_exec.cpp > CMakeFiles/agent_exec.dir/agent_exec.cpp.i

CMakeFiles/agent_exec.dir/agent_exec.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/agent_exec.dir/agent_exec.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/spjy/cosmos/source/core/programs/agents/agent_exec.cpp -o CMakeFiles/agent_exec.dir/agent_exec.cpp.s

CMakeFiles/agent_exec.dir/agent_exec.cpp.o.requires:

.PHONY : CMakeFiles/agent_exec.dir/agent_exec.cpp.o.requires

CMakeFiles/agent_exec.dir/agent_exec.cpp.o.provides: CMakeFiles/agent_exec.dir/agent_exec.cpp.o.requires
	$(MAKE) -f CMakeFiles/agent_exec.dir/build.make CMakeFiles/agent_exec.dir/agent_exec.cpp.o.provides.build
.PHONY : CMakeFiles/agent_exec.dir/agent_exec.cpp.o.provides

CMakeFiles/agent_exec.dir/agent_exec.cpp.o.provides.build: CMakeFiles/agent_exec.dir/agent_exec.cpp.o


# Object files for target agent_exec
agent_exec_OBJECTS = \
"CMakeFiles/agent_exec.dir/agent_exec.cpp.o"

# External object files for target agent_exec
agent_exec_EXTERNAL_OBJECTS =

agent_exec: CMakeFiles/agent_exec.dir/agent_exec.cpp.o
agent_exec: CMakeFiles/agent_exec.dir/build.make
agent_exec: CMakeFiles/agent_exec.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/spjy/cosmos/source/core/programs/agents/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable agent_exec"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/agent_exec.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/agent_exec.dir/build: agent_exec

.PHONY : CMakeFiles/agent_exec.dir/build

CMakeFiles/agent_exec.dir/requires: CMakeFiles/agent_exec.dir/agent_exec.cpp.o.requires

.PHONY : CMakeFiles/agent_exec.dir/requires

CMakeFiles/agent_exec.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/agent_exec.dir/cmake_clean.cmake
.PHONY : CMakeFiles/agent_exec.dir/clean

CMakeFiles/agent_exec.dir/depend:
	cd /home/spjy/cosmos/source/core/programs/agents && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/spjy/cosmos/source/core/programs/agents /home/spjy/cosmos/source/core/programs/agents /home/spjy/cosmos/source/core/programs/agents /home/spjy/cosmos/source/core/programs/agents /home/spjy/cosmos/source/core/programs/agents/CMakeFiles/agent_exec.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/agent_exec.dir/depend

