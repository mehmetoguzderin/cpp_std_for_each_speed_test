cmake_minimum_required( VERSION 3.9.2 FATAL_ERROR )

project( std_for_each_speed_test )

add_executable(
  std_for_each_speed_test
  std_for_each_speed_test.cpp)

# detect bitness
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  add_definitions(-DBIT_64)
else()
  add_definitions(-DBIT_32)
endif()

set_target_properties(
  std_for_each_speed_test PROPERTIES
  CMAKE_CXX_STANDARD 17
  CMAKE_CXX_STANDARD_REQUIRED ON)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  # using Clang
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # using GCC
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  # using Intel C++
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # using Visual Studio C++
  set_target_properties(
    std_for_each_speed_test PROPERTIES
    COMPILE_OPTIONS $<$<CXX_COMPILER_ID:MSVC>:/std:c++17>
    COMPILE_DEFINITIONS $<$<CXX_COMPILER_ID:MSVC>:_SILENCE_PARALLEL_ALGORITHMS_EXPERIMENTAL_WARNING>)
endif()