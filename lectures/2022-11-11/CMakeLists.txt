cmake_minimum_required (VERSION 3.13)

project ("Dynamic Array")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

target_sources(
	unit-tests
	PRIVATE
		"test/DynamicArrayTest.cpp"
		"test/FixedSizeArrayTest.cpp"
)

target_include_directories(unit-tests PRIVATE "src")

# Automatically register all tests
include(CTest)
include(Catch)
catch_discover_tests(unit-tests)
