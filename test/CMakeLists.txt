
add_subdirectory(test_core)

add_subdirectory(test_asio)

if(BUILD_PYBIND)
#add_subdirectory(test_pybind11)
endif()

#add_subdirectory(test_rxcpp)

add_subdirectory(test_nanomsg)

add_subdirectory(test_zmq)
add_subdirectory(test_net)
add_subdirectory(test_grpc)
add_subdirectory(test_consul)