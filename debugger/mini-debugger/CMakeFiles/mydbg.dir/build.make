# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /home/flno/Desktop/C++/mini-debugger

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/flno/Desktop/C++/mini-debugger

# Include any dependencies generated for this target.
include CMakeFiles/mydbg.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/mydbg.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/mydbg.dir/flags.make

CMakeFiles/mydbg.dir/src/mydbg.cpp.o: CMakeFiles/mydbg.dir/flags.make
CMakeFiles/mydbg.dir/src/mydbg.cpp.o: src/mydbg.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/flno/Desktop/C++/mini-debugger/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/mydbg.dir/src/mydbg.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mydbg.dir/src/mydbg.cpp.o -c /home/flno/Desktop/C++/mini-debugger/src/mydbg.cpp

CMakeFiles/mydbg.dir/src/mydbg.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mydbg.dir/src/mydbg.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/flno/Desktop/C++/mini-debugger/src/mydbg.cpp > CMakeFiles/mydbg.dir/src/mydbg.cpp.i

CMakeFiles/mydbg.dir/src/mydbg.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mydbg.dir/src/mydbg.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/flno/Desktop/C++/mini-debugger/src/mydbg.cpp -o CMakeFiles/mydbg.dir/src/mydbg.cpp.s

CMakeFiles/mydbg.dir/src/mydbg.cpp.o.requires:

.PHONY : CMakeFiles/mydbg.dir/src/mydbg.cpp.o.requires

CMakeFiles/mydbg.dir/src/mydbg.cpp.o.provides: CMakeFiles/mydbg.dir/src/mydbg.cpp.o.requires
	$(MAKE) -f CMakeFiles/mydbg.dir/build.make CMakeFiles/mydbg.dir/src/mydbg.cpp.o.provides.build
.PHONY : CMakeFiles/mydbg.dir/src/mydbg.cpp.o.provides

CMakeFiles/mydbg.dir/src/mydbg.cpp.o.provides.build: CMakeFiles/mydbg.dir/src/mydbg.cpp.o


CMakeFiles/mydbg.dir/ext/linenoise/linenoise.c.o: CMakeFiles/mydbg.dir/flags.make
CMakeFiles/mydbg.dir/ext/linenoise/linenoise.c.o: ext/linenoise/linenoise.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/flno/Desktop/C++/mini-debugger/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/mydbg.dir/ext/linenoise/linenoise.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/mydbg.dir/ext/linenoise/linenoise.c.o   -c /home/flno/Desktop/C++/mini-debugger/ext/linenoise/linenoise.c

CMakeFiles/mydbg.dir/ext/linenoise/linenoise.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/mydbg.dir/ext/linenoise/linenoise.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/flno/Desktop/C++/mini-debugger/ext/linenoise/linenoise.c > CMakeFiles/mydbg.dir/ext/linenoise/linenoise.c.i

CMakeFiles/mydbg.dir/ext/linenoise/linenoise.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/mydbg.dir/ext/linenoise/linenoise.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/flno/Desktop/C++/mini-debugger/ext/linenoise/linenoise.c -o CMakeFiles/mydbg.dir/ext/linenoise/linenoise.c.s

CMakeFiles/mydbg.dir/ext/linenoise/linenoise.c.o.requires:

.PHONY : CMakeFiles/mydbg.dir/ext/linenoise/linenoise.c.o.requires

CMakeFiles/mydbg.dir/ext/linenoise/linenoise.c.o.provides: CMakeFiles/mydbg.dir/ext/linenoise/linenoise.c.o.requires
	$(MAKE) -f CMakeFiles/mydbg.dir/build.make CMakeFiles/mydbg.dir/ext/linenoise/linenoise.c.o.provides.build
.PHONY : CMakeFiles/mydbg.dir/ext/linenoise/linenoise.c.o.provides

CMakeFiles/mydbg.dir/ext/linenoise/linenoise.c.o.provides.build: CMakeFiles/mydbg.dir/ext/linenoise/linenoise.c.o


# Object files for target mydbg
mydbg_OBJECTS = \
"CMakeFiles/mydbg.dir/src/mydbg.cpp.o" \
"CMakeFiles/mydbg.dir/ext/linenoise/linenoise.c.o"

# External object files for target mydbg
mydbg_EXTERNAL_OBJECTS =

mydbg: CMakeFiles/mydbg.dir/src/mydbg.cpp.o
mydbg: CMakeFiles/mydbg.dir/ext/linenoise/linenoise.c.o
mydbg: CMakeFiles/mydbg.dir/build.make
mydbg: ext/libelfin/dwarf/libdwarf++.so
mydbg: ext/libelfin/elf/libelf++.so
mydbg: CMakeFiles/mydbg.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/flno/Desktop/C++/mini-debugger/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable mydbg"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mydbg.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/mydbg.dir/build: mydbg

.PHONY : CMakeFiles/mydbg.dir/build

CMakeFiles/mydbg.dir/requires: CMakeFiles/mydbg.dir/src/mydbg.cpp.o.requires
CMakeFiles/mydbg.dir/requires: CMakeFiles/mydbg.dir/ext/linenoise/linenoise.c.o.requires

.PHONY : CMakeFiles/mydbg.dir/requires

CMakeFiles/mydbg.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/mydbg.dir/cmake_clean.cmake
.PHONY : CMakeFiles/mydbg.dir/clean

CMakeFiles/mydbg.dir/depend:
	cd /home/flno/Desktop/C++/mini-debugger && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/flno/Desktop/C++/mini-debugger /home/flno/Desktop/C++/mini-debugger /home/flno/Desktop/C++/mini-debugger /home/flno/Desktop/C++/mini-debugger /home/flno/Desktop/C++/mini-debugger/CMakeFiles/mydbg.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/mydbg.dir/depend

