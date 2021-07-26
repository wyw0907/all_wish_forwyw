

add_executable(test_thread_pool test_thread_pool.cpp)

target_link_libraries(test_thread_pool wish pthread dl)


add_executable(test_thread_daemon test_thread_daemon.cpp)

target_link_libraries(test_thread_daemon wish spdlog pthread dl)

add_executable(test_safe_thread test_safe_thread_pool.cpp)

target_link_libraries(test_safe_thread wish pthread dl)


add_executable(test_thread_lockfree test_thread_lockfree.cpp)

target_link_libraries(test_thread_lockfree wish pthread dl)
