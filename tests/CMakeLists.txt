add_executable(test test.cpp)

target_include_directories(test PUBLIC ${CMAKE_SOURCE_DIR}/inc ${CMAKE_SOURCE_DIR}/tests)

target_link_libraries(test PRIVATE avl_tree)
target_link_libraries(test PRIVATE gtest::gtest)
