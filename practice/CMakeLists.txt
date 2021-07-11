add_executable(enum enum.cpp)


target_link_libraries(enum)


add_executable(huiwen huiwen.cpp)

add_executable(decompress decompress.cpp)

add_executable(dalou dalou.cpp)

add_executable(test_closure test_closure.cpp)

add_executable(test_mem_order memory_order.cpp)
target_link_libraries(test_mem_order pthread)

add_executable(test_tree tree.cpp)

add_executable(test_rbtree rbtree.cpp)

add_executable(test_heap_sort heap_sort.cpp)
