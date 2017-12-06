cmake_minimum_required( VERSION 3.9.2 FATAL_ERROR )

project( std_for_each_speed_test )

add_executable(
	std_for_each_speed_test
	std_for_each_speed_test.cpp
	)

set_target_properties(
	std_for_each_speed_test PROPERTIES
	CMAKE_CXX_STANDARD 17
	CMAKE_CXX_STANDARD_REQUIRED ON
	COMPILE_OPTIONS $<$<CXX_COMPILER_ID:MSVC>:/std:c++17>
	COMPILE_DEFINITIONS $<$<CXX_COMPILER_ID:MSVC>:_SILENCE_PARALLEL_ALGORITHMS_EXPERIMENTAL_WARNING>)