cmake_minimum_required( VERSION 3.9.2 FATAL_ERROR )

project( std_for_each_speed_test )

add_subdirectory( benchmark )

add_executable(
  std_for_each_speed_test
  std_for_each_speed_test.cpp)

set_target_properties(
  std_for_each_speed_test PROPERTIES
  CMAKE_CXX_STANDARD 17
  CMAKE_CXX_STANDARD_REQUIRED ON)

add_dependencies( std_for_each_speed_test benchmark )
target_link_libraries( std_for_each_speed_test benchmark )

target_link_libraries(std_for_each_speed_test
  shlwapi
)

if( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" )
  # using Clang
elseif( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
  # using GCC
elseif( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel" )
  # using Intel C++
elseif( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" )
  # using Visual Studio C++
  set_target_properties(
    std_for_each_speed_test PROPERTIES
    COMPILE_OPTIONS /std:c++17
    COMPILE_DEFINITIONS _SILENCE_PARALLEL_ALGORITHMS_EXPERIMENTAL_WARNING)
endif()