cmake_minimum_required(VERSION 3.14)
project(PermanentSet)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
  avl_tree
  GIT_REPOSITORY https://github.com/kakov1/AVL_Tree.git
  GIT_TAG        a89c8cfbeaba926b38bf08e6f9a0da09bb561cb9
)

FetchContent_MakeAvailable(avl_tree)

find_package(GTest REQUIRED)

add_subdirectory(src)
