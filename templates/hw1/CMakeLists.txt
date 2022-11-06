cmake_minimum_required (VERSION 3.13)

project ("Homework 1: Bracket Checking")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Library target for the solution
add_library(solution STATIC)

target_sources(
	solution
	PRIVATE
		"src/solution.cpp"
		"src/solution.h"
)


# Executable target for the application
add_executable(checker)

target_link_libraries(
	checker
	PRIVATE
		solution
		Catch2::Catch2WithMain
)

target_sources(
	checker
	PRIVATE
		"src/application.cpp"
)


#
# Make Catch2 available. For more information check
# https://github.com/catchorg/Catch2/blob/devel/docs/cmake-integration.md
#
Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.1.1)

FetchContent_MakeAvailable(Catch2)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)


# Executable target for the unit tests
add_executable(unit-tests)

target_link_libraries(
	unit-tests
	PRIVATE
		solution
		Catch2::Catch2WithMain
)

target_include_directories(unit-tests PRIVATE "src")

target_sources(
	unit-tests
	PRIVATE
		"test/solution.cpp"
)


# Automatically register all tests
include(CTest)
include(Catch)
catch_discover_tests(unit-tests)
