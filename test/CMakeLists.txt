
add_subdirectory(test_core)

add_subdirectory(test_asio)

if(BUILD_PYBIND)
#add_subdirectory(test_pybind11)
endif()

#add_subdirectory(test_rxcpp)

add_subdirectory(test_nanomsg)

add_subdirectory(test_zmq)
add_subdirectory(test_net)

find_library(GRPC_LIB NAMES grpc)
message ('${GRPC_LIB}')
if ( ${GRPC_LIB})
add_subdirectory(test_grpc)
add_subdirectory(test_consul)
endif()


add_subdirectory(test_lua)
