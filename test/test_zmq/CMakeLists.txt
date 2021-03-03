

add_executable(client_req client_req.cpp)
target_link_libraries(client_req zmq)

add_executable(server_rep server_rep.cpp)
target_link_libraries(server_rep zmq)


add_executable(client_req_ipc client_req_ipc.cpp)
target_link_libraries(client_req_ipc zmq)

add_executable(server_rep_ipc server_rep_ipc.cpp)
target_link_libraries(server_rep_ipc zmq)


add_executable(client_push client_push.cpp)
target_link_libraries(client_push zmq)

add_executable(server_pull server_pull.cpp)
target_link_libraries(server_pull zmq)


add_executable(client_pull client_pull.cpp)
target_link_libraries(client_pull zmq)

add_executable(server_push server_push.cpp)
target_link_libraries(server_push zmq)

add_executable(client_sub client_sub.cpp)
target_link_libraries(client_sub zmq)

add_executable(server_pub server_pub.cpp)
target_link_libraries(server_pub zmq)

add_executable(client_dealer client_dealer.cpp)
target_link_libraries(client_dealer zmq)

add_executable(server_router server_router.cpp)
target_link_libraries(server_router zmq)