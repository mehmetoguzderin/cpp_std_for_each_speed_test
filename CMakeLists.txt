cmake_minimum_required( VERSION 3.9.2 FATAL_ERROR )

project( benchmark )
find_package( Threads REQUIRED )
file( GLOB SOURCE_FILES
	benchmark/src/*.cc
    benchmark/include/benchmark_cpp/*.h
    benchmark/src/*.h )
add_library( benchmark ${SOURCE_FILES} )
target_include_directories( benchmark PRIVATE benchmark/include )
target_include_directories( benchmark PRIVATE benchmark/src )
target_link_libraries( benchmark PRIVATE ${CMAKE_THREAD_LIBS_INIT} )
find_library( LIBRT rt )
if( LIBRT )
	target_link_libraries( benchmark PRIVATE ${LIBRT} )
endif()
if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
	target_link_libraries( benchmark PRIVATE Shlwapi )
endif()
if( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" )
	# using Clang
elseif( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
	# using GCC
elseif( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel" )
	# using Intel C++
elseif( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" )
	# using Visual Studio C++
	target_compile_definitions( benchmark PRIVATE HAVE_STD_REGEX _CRT_SECURE_NO_WARNINGS )
endif()

project( std_for_each_speed_test_native )
project( std_for_each_speed_test_google )

add_executable(
  std_for_each_speed_test_native
  src/native.cpp
  src/std_for_each_speed_test.h )
add_executable(
  std_for_each_speed_test_google
  src/google.cpp
  src/std_for_each_speed_test.h )

set_target_properties(
  std_for_each_speed_test_native PROPERTIES
  CMAKE_CXX_STANDARD 17
  CMAKE_CXX_STANDARD_REQUIRED ON )
set_target_properties(
  std_for_each_speed_test_google PROPERTIES
  CMAKE_CXX_STANDARD 17
  CMAKE_CXX_STANDARD_REQUIRED ON )

target_include_directories( std_for_each_speed_test_google PRIVATE benchmark/include )
add_dependencies( std_for_each_speed_test_google benchmark )
target_link_libraries( std_for_each_speed_test_google benchmark )

if( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" )
  # using Clang
elseif( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
  # using GCC
elseif( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel" )
  # using Intel C++
elseif( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" )
	# using Visual Studio C++
	set_target_properties(
		std_for_each_speed_test_native PROPERTIES
		COMPILE_OPTIONS /std:c++17
		COMPILE_DEFINITIONS _SILENCE_PARALLEL_ALGORITHMS_EXPERIMENTAL_WARNING )
	set_target_properties(
		std_for_each_speed_test_google PROPERTIES
		COMPILE_OPTIONS /std:c++17
		COMPILE_DEFINITIONS _SILENCE_PARALLEL_ALGORITHMS_EXPERIMENTAL_WARNING )
endif()