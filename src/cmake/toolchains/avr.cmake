###################### FOR CROSS-COMPILING using the avr-g** compiler
# invoke this toolchain file using `cmake .. -D CMAKE_TOOLCHAIN_FILE=cmake/toolchains/<ARCH_TYPE>.cmake`
# this file is meant to be used generically, but will not work for all CMake projects
# this toolchain file's cmds was copied from the CMake docs then modified for better explanation and re-use

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR avr)
set(TARGET_ARCH avr) # only used in cmake/createDebianPkg.cmake
set(CMAKE_C_COMPILER /usr/bin/avr-gcc)
set(CMAKE_CXX_COMPILER /usr/bin/avr-g++)

# CMAKE_SYSROOT can only be set in a toolchain file
# set(CMAKE_SYSROOT  /usr/avr-linux-gnu) # useful when a target machine's files are available

# set the directory for searching installed headers
# add_compile_options(-I /usr/avr-gnu/include) # this may not be best practice

#[[
# CMAKE_STAGING_PREFIX is only useful for transfering a built CMake project to a target machine
set(CMAKE_STAGING_PREFIX /home/devel/stage) # use CMAKE_INSTALL_PREFIX instead (see below comments)

CMAKE_FIND_ROOT_PATH is an empty list by default (this list can be modified where applicable)
if cross-compiling a dependent lib (like MRAA - which is optional), then
set the lib's CMAKE_INSTALL_PREFIX to a value that is appended to RF24 lib's CMAKE_FIND_ROOT_PATH
example using MRAA:
(for MRAA/build dir) `cmake .. -D CMAKE_TOOLCHAIN_FILE=path/to/RF24/repo/cmake/toolchains/avr.cmake -D CMAKE_INSTALL_PREFIX:PATH=/usr/avr-gnu
(for RF24/build dir) `cmake .. -D CMAKE_TOOLCHAIN_FILE=cmake/toolchains/avr.cmake
]]
list(APPEND CMAKE_FIND_ROOT_PATH /usr/avr-gnu)
# message("CMAKE_FIND_ROOT_PATH = ${CMAKE_FIND_ROOT_PATH}")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER) # search CMAKE_SYSROOT when find_program() is called
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY) # search CMAKE_FIND_ROOT_PATH entries when find_library() is called
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY) # search CMAKE_FIND_ROOT_PATH entries when find_file() is called
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY) # search CMAKE_FIND_ROOT_PATH entries when find_package() is called
