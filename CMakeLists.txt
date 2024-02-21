cmake_minimum_required(VERSION 3.10)

project(happy_cube VERSION 0.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(happy_cube_VERSION_MAJOR 0)
set(happy_cube_VERSION_MINOR 0)

add_executable(happy_cube
	assemble.cc
	assemble.hh
	brick.cc
	brick.hh
	combinations.cc
	combinations.hh
	compare.hpp
	main.cc
	permutations.cc
	permutations.hh
)
