cmake_minimum_required(VERSION 3.14)
project(PermanentSet)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
  avl_tree
  GIT_REPOSITORY https://github.com/kakov1/AVL_Tree.git
  GIT_TAG        7371e0e3da8bc781ff8a824336304ee4391b42e5
)

FetchContent_MakeAvailable(avl_tree)

find_package(GTest REQUIRED)

add_subdirectory(src)
add_subdirectory(tests)
