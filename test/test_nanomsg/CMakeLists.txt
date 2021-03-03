

add_executable(test_pull test_pull.cpp)

target_link_libraries(test_pull wish nanomsg)

add_executable(test_push test_push.cpp)

target_link_libraries(test_push wish nanomsg)
