add_executable(test test.cpp)

set(TEST_DATA_DIR ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(test PUBLIC TEST_DATA_DIR="${TEST_DATA_DIR}")
target_include_directories(test PUBLIC ${CMAKE_SOURCE_DIR}/inc)
target_link_libraries(test PRIVATE avl_tree gtest::gtest)
