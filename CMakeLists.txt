cmake_minimum_required(VERSION 3.28)
project(Algorithms)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(catch2 GIT_REPOSITORY https://github.com/catchorg/Catch2.git GIT_TAG v3.7.0)
FetchContent_MakeAvailable(catch2)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(BinaryTree)
add_subdirectory(List)
add_subdirectory(Vector)

enable_testing()

add_executable(BinaryTreeTests BinaryTree/unit_tests.cpp)
target_link_libraries(BinaryTreeTests PRIVATE BinaryTree Catch2::Catch2)

include(CTest)
include(Catch)
catch_discover_tests(BinaryTreeTests)
