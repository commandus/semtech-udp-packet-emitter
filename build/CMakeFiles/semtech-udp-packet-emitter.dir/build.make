# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

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

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/andrei/src/semtech-udp-packet-emitter

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/andrei/src/semtech-udp-packet-emitter/build

# Include any dependencies generated for this target.
include CMakeFiles/semtech-udp-packet-emitter.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/semtech-udp-packet-emitter.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/semtech-udp-packet-emitter.dir/flags.make

CMakeFiles/semtech-udp-packet-emitter.dir/semtech-udp-packet-emitter.cpp.o: CMakeFiles/semtech-udp-packet-emitter.dir/flags.make
CMakeFiles/semtech-udp-packet-emitter.dir/semtech-udp-packet-emitter.cpp.o: ../semtech-udp-packet-emitter.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/andrei/src/semtech-udp-packet-emitter/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/semtech-udp-packet-emitter.dir/semtech-udp-packet-emitter.cpp.o"
	/usr/bin/g++-4.6 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/semtech-udp-packet-emitter.dir/semtech-udp-packet-emitter.cpp.o -c /home/andrei/src/semtech-udp-packet-emitter/semtech-udp-packet-emitter.cpp

CMakeFiles/semtech-udp-packet-emitter.dir/semtech-udp-packet-emitter.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/semtech-udp-packet-emitter.dir/semtech-udp-packet-emitter.cpp.i"
	/usr/bin/g++-4.6 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/andrei/src/semtech-udp-packet-emitter/semtech-udp-packet-emitter.cpp > CMakeFiles/semtech-udp-packet-emitter.dir/semtech-udp-packet-emitter.cpp.i

CMakeFiles/semtech-udp-packet-emitter.dir/semtech-udp-packet-emitter.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/semtech-udp-packet-emitter.dir/semtech-udp-packet-emitter.cpp.s"
	/usr/bin/g++-4.6 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/andrei/src/semtech-udp-packet-emitter/semtech-udp-packet-emitter.cpp -o CMakeFiles/semtech-udp-packet-emitter.dir/semtech-udp-packet-emitter.cpp.s

CMakeFiles/semtech-udp-packet-emitter.dir/errlist.cpp.o: CMakeFiles/semtech-udp-packet-emitter.dir/flags.make
CMakeFiles/semtech-udp-packet-emitter.dir/errlist.cpp.o: ../errlist.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/andrei/src/semtech-udp-packet-emitter/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/semtech-udp-packet-emitter.dir/errlist.cpp.o"
	/usr/bin/g++-4.6 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/semtech-udp-packet-emitter.dir/errlist.cpp.o -c /home/andrei/src/semtech-udp-packet-emitter/errlist.cpp

CMakeFiles/semtech-udp-packet-emitter.dir/errlist.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/semtech-udp-packet-emitter.dir/errlist.cpp.i"
	/usr/bin/g++-4.6 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/andrei/src/semtech-udp-packet-emitter/errlist.cpp > CMakeFiles/semtech-udp-packet-emitter.dir/errlist.cpp.i

CMakeFiles/semtech-udp-packet-emitter.dir/errlist.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/semtech-udp-packet-emitter.dir/errlist.cpp.s"
	/usr/bin/g++-4.6 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/andrei/src/semtech-udp-packet-emitter/errlist.cpp -o CMakeFiles/semtech-udp-packet-emitter.dir/errlist.cpp.s

CMakeFiles/semtech-udp-packet-emitter.dir/daemonize.cpp.o: CMakeFiles/semtech-udp-packet-emitter.dir/flags.make
CMakeFiles/semtech-udp-packet-emitter.dir/daemonize.cpp.o: ../daemonize.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/andrei/src/semtech-udp-packet-emitter/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/semtech-udp-packet-emitter.dir/daemonize.cpp.o"
	/usr/bin/g++-4.6 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/semtech-udp-packet-emitter.dir/daemonize.cpp.o -c /home/andrei/src/semtech-udp-packet-emitter/daemonize.cpp

CMakeFiles/semtech-udp-packet-emitter.dir/daemonize.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/semtech-udp-packet-emitter.dir/daemonize.cpp.i"
	/usr/bin/g++-4.6 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/andrei/src/semtech-udp-packet-emitter/daemonize.cpp > CMakeFiles/semtech-udp-packet-emitter.dir/daemonize.cpp.i

CMakeFiles/semtech-udp-packet-emitter.dir/daemonize.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/semtech-udp-packet-emitter.dir/daemonize.cpp.s"
	/usr/bin/g++-4.6 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/andrei/src/semtech-udp-packet-emitter/daemonize.cpp -o CMakeFiles/semtech-udp-packet-emitter.dir/daemonize.cpp.s

CMakeFiles/semtech-udp-packet-emitter.dir/utilstring.cpp.o: CMakeFiles/semtech-udp-packet-emitter.dir/flags.make
CMakeFiles/semtech-udp-packet-emitter.dir/utilstring.cpp.o: ../utilstring.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/andrei/src/semtech-udp-packet-emitter/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/semtech-udp-packet-emitter.dir/utilstring.cpp.o"
	/usr/bin/g++-4.6 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/semtech-udp-packet-emitter.dir/utilstring.cpp.o -c /home/andrei/src/semtech-udp-packet-emitter/utilstring.cpp

CMakeFiles/semtech-udp-packet-emitter.dir/utilstring.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/semtech-udp-packet-emitter.dir/utilstring.cpp.i"
	/usr/bin/g++-4.6 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/andrei/src/semtech-udp-packet-emitter/utilstring.cpp > CMakeFiles/semtech-udp-packet-emitter.dir/utilstring.cpp.i

CMakeFiles/semtech-udp-packet-emitter.dir/utilstring.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/semtech-udp-packet-emitter.dir/utilstring.cpp.s"
	/usr/bin/g++-4.6 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/andrei/src/semtech-udp-packet-emitter/utilstring.cpp -o CMakeFiles/semtech-udp-packet-emitter.dir/utilstring.cpp.s

CMakeFiles/semtech-udp-packet-emitter.dir/utillora.cpp.o: CMakeFiles/semtech-udp-packet-emitter.dir/flags.make
CMakeFiles/semtech-udp-packet-emitter.dir/utillora.cpp.o: ../utillora.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/andrei/src/semtech-udp-packet-emitter/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/semtech-udp-packet-emitter.dir/utillora.cpp.o"
	/usr/bin/g++-4.6 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/semtech-udp-packet-emitter.dir/utillora.cpp.o -c /home/andrei/src/semtech-udp-packet-emitter/utillora.cpp

CMakeFiles/semtech-udp-packet-emitter.dir/utillora.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/semtech-udp-packet-emitter.dir/utillora.cpp.i"
	/usr/bin/g++-4.6 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/andrei/src/semtech-udp-packet-emitter/utillora.cpp > CMakeFiles/semtech-udp-packet-emitter.dir/utillora.cpp.i

CMakeFiles/semtech-udp-packet-emitter.dir/utillora.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/semtech-udp-packet-emitter.dir/utillora.cpp.s"
	/usr/bin/g++-4.6 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/andrei/src/semtech-udp-packet-emitter/utillora.cpp -o CMakeFiles/semtech-udp-packet-emitter.dir/utillora.cpp.s

CMakeFiles/semtech-udp-packet-emitter.dir/aes-128.c.o: CMakeFiles/semtech-udp-packet-emitter.dir/flags.make
CMakeFiles/semtech-udp-packet-emitter.dir/aes-128.c.o: ../aes-128.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/andrei/src/semtech-udp-packet-emitter/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/semtech-udp-packet-emitter.dir/aes-128.c.o"
	/usr/bin/gcc-4.6 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/semtech-udp-packet-emitter.dir/aes-128.c.o -c /home/andrei/src/semtech-udp-packet-emitter/aes-128.c

CMakeFiles/semtech-udp-packet-emitter.dir/aes-128.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/semtech-udp-packet-emitter.dir/aes-128.c.i"
	/usr/bin/gcc-4.6 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/andrei/src/semtech-udp-packet-emitter/aes-128.c > CMakeFiles/semtech-udp-packet-emitter.dir/aes-128.c.i

CMakeFiles/semtech-udp-packet-emitter.dir/aes-128.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/semtech-udp-packet-emitter.dir/aes-128.c.s"
	/usr/bin/gcc-4.6 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/andrei/src/semtech-udp-packet-emitter/aes-128.c -o CMakeFiles/semtech-udp-packet-emitter.dir/aes-128.c.s

CMakeFiles/semtech-udp-packet-emitter.dir/third_party/argtable3/argtable3.c.o: CMakeFiles/semtech-udp-packet-emitter.dir/flags.make
CMakeFiles/semtech-udp-packet-emitter.dir/third_party/argtable3/argtable3.c.o: ../third_party/argtable3/argtable3.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/andrei/src/semtech-udp-packet-emitter/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/semtech-udp-packet-emitter.dir/third_party/argtable3/argtable3.c.o"
	/usr/bin/gcc-4.6 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/semtech-udp-packet-emitter.dir/third_party/argtable3/argtable3.c.o -c /home/andrei/src/semtech-udp-packet-emitter/third_party/argtable3/argtable3.c

CMakeFiles/semtech-udp-packet-emitter.dir/third_party/argtable3/argtable3.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/semtech-udp-packet-emitter.dir/third_party/argtable3/argtable3.c.i"
	/usr/bin/gcc-4.6 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/andrei/src/semtech-udp-packet-emitter/third_party/argtable3/argtable3.c > CMakeFiles/semtech-udp-packet-emitter.dir/third_party/argtable3/argtable3.c.i

CMakeFiles/semtech-udp-packet-emitter.dir/third_party/argtable3/argtable3.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/semtech-udp-packet-emitter.dir/third_party/argtable3/argtable3.c.s"
	/usr/bin/gcc-4.6 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/andrei/src/semtech-udp-packet-emitter/third_party/argtable3/argtable3.c -o CMakeFiles/semtech-udp-packet-emitter.dir/third_party/argtable3/argtable3.c.s

# Object files for target semtech-udp-packet-emitter
semtech__udp__packet__emitter_OBJECTS = \
"CMakeFiles/semtech-udp-packet-emitter.dir/semtech-udp-packet-emitter.cpp.o" \
"CMakeFiles/semtech-udp-packet-emitter.dir/errlist.cpp.o" \
"CMakeFiles/semtech-udp-packet-emitter.dir/daemonize.cpp.o" \
"CMakeFiles/semtech-udp-packet-emitter.dir/utilstring.cpp.o" \
"CMakeFiles/semtech-udp-packet-emitter.dir/utillora.cpp.o" \
"CMakeFiles/semtech-udp-packet-emitter.dir/aes-128.c.o" \
"CMakeFiles/semtech-udp-packet-emitter.dir/third_party/argtable3/argtable3.c.o"

# External object files for target semtech-udp-packet-emitter
semtech__udp__packet__emitter_EXTERNAL_OBJECTS =

semtech-udp-packet-emitter: CMakeFiles/semtech-udp-packet-emitter.dir/semtech-udp-packet-emitter.cpp.o
semtech-udp-packet-emitter: CMakeFiles/semtech-udp-packet-emitter.dir/errlist.cpp.o
semtech-udp-packet-emitter: CMakeFiles/semtech-udp-packet-emitter.dir/daemonize.cpp.o
semtech-udp-packet-emitter: CMakeFiles/semtech-udp-packet-emitter.dir/utilstring.cpp.o
semtech-udp-packet-emitter: CMakeFiles/semtech-udp-packet-emitter.dir/utillora.cpp.o
semtech-udp-packet-emitter: CMakeFiles/semtech-udp-packet-emitter.dir/aes-128.c.o
semtech-udp-packet-emitter: CMakeFiles/semtech-udp-packet-emitter.dir/third_party/argtable3/argtable3.c.o
semtech-udp-packet-emitter: CMakeFiles/semtech-udp-packet-emitter.dir/build.make
semtech-udp-packet-emitter: CMakeFiles/semtech-udp-packet-emitter.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/andrei/src/semtech-udp-packet-emitter/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX executable semtech-udp-packet-emitter"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/semtech-udp-packet-emitter.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/semtech-udp-packet-emitter.dir/build: semtech-udp-packet-emitter

.PHONY : CMakeFiles/semtech-udp-packet-emitter.dir/build

CMakeFiles/semtech-udp-packet-emitter.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/semtech-udp-packet-emitter.dir/cmake_clean.cmake
.PHONY : CMakeFiles/semtech-udp-packet-emitter.dir/clean

CMakeFiles/semtech-udp-packet-emitter.dir/depend:
	cd /home/andrei/src/semtech-udp-packet-emitter/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/andrei/src/semtech-udp-packet-emitter /home/andrei/src/semtech-udp-packet-emitter /home/andrei/src/semtech-udp-packet-emitter/build /home/andrei/src/semtech-udp-packet-emitter/build /home/andrei/src/semtech-udp-packet-emitter/build/CMakeFiles/semtech-udp-packet-emitter.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/semtech-udp-packet-emitter.dir/depend
