add_executable(main main.cpp)

target_include_directories(main PUBLIC ${CMAKE_SOURCE_DIR}/inc)

target_link_libraries(main PRIVATE avl_tree)
