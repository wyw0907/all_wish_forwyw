

add_executable(test_thread_pool test_thread_pool.cpp)

target_link_libraries(test_thread_pool wish pthread dl)
