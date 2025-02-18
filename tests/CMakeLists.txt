add_executable(test test.cpp)

file(COPY ${CMAKE_SOURCE_DIR}/tests/in/ ${CMAKE_SOURCE_DIR}/tests/out/ DESTINATION ${CMAKE_BINARY_DIR}/tests/)

target_include_directories(test PUBLIC ${CMAKE_SOURCE_DIR}/inc)

target_link_libraries(test PRIVATE avl_tree gtest::gtest)
