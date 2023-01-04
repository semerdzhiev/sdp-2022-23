cmake_minimum_required (VERSION 3.13)

project ("Simple CMake Template" VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#
# Tell MSVC to build using multiple processes.
# This may speed up compilation time significantly.
# For more information check:
# https://learn.microsoft.com/en-us/cpp/build/reference/mp-build-with-multiple-processes?view=msvc-170
#
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)


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
		Catch2::Catch2WithMain
)

target_include_directories(unit-tests PRIVATE "src")

target_sources(
	unit-tests
	PRIVATE
		"test/TestAdditional.cpp"
		"test/TestNode.cpp"
		"test/TestSolution.cpp"
)


# Automatically register all tests
include(CTest)
include(Catch)
catch_discover_tests(unit-tests)
