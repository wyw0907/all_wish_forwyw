include_directories("/usr/local/grpc/include")
link_directories("/usr/local/grpc/lib")

find_path(GRPC_INCLUDE_PATH NAMES grpc/grpc.h)
find_library(GRPC_LIB NAMES grpc)

find_program(PROTOC protoc REQUIRED PATH /usr/local/grpc/bin)
find_program(GRPC_CPP_PLUGIN grpc_cpp_plugin REQUIRED PATH /usr/local/grpc/bin)

FILE(GLOB PROTO_ALL_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.proto")
FOREACH(PROTO_FILE ${PROTO_ALL_FILES})
    EXECUTE_PROCESS (
        COMMAND ${PROTOC} 
        -I${CMAKE_CURRENT_SOURCE_DIR} --cpp_out=${CMAKE_CURRENT_SOURCE_DIR} ${PROTO_FILE}
    )
    EXECUTE_PROCESS (
        COMMAND ${PROTOC}
        -I${CMAKE_CURRENT_SOURCE_DIR} --grpc_out=${CMAKE_CURRENT_SOURCE_DIR} --plugin=protoc-gen-grpc=${GRPC_CPP_PLUGIN} ${PROTO_FILE}
    )
ENDFOREACH()


#ADD_CUSTOM_COMMAND(
#    OUTPUT  ${CMAKE_CURRENT_SOURCE_DIR}/test.pb.h ${CMAKE_CURRENT_SOURCE_DIR}/test.pb.cc
#    COMMAND ${PROTOC}
#    ARGS    -I${CMAKE_CURRENT_SOURCE_DIR} --cpp_out=${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/test.proto
#)

#ADD_CUSTOM_COMMAND(
#    OUTPUT  ${CMAKE_CURRENT_SOURCE_DIR}/test.grpc.pb.h ${CMAKE_CURRENT_SOURCE_DIR}/test.grpc.pb.cc
#    COMMAND ${PROTOC}
#    ARGS    -I${CMAKE_CURRENT_SOURCE_DIR} --grpc_out=${CMAKE_CURRENT_SOURCE_DIR} --plugin=protoc-gen-grpc=${GRPC_CPP_PLUGIN} ${CMAKE_CURRENT_SOURCE_DIR}/test.proto
#)

add_executable(greeter_client greeter_client.cpp test.grpc.pb.cc test.pb.cc health_check.grpc.pb.cc health_check.pb.cc)
target_link_libraries(greeter_client grpc++ gpr grpc++_reflection protobuf pthread)

add_executable(greeter_server greeter_server.cpp test.grpc.pb.cc test.pb.cc health_check.grpc.pb.cc health_check.pb.cc)
target_link_libraries(greeter_server grpc++ gpr grpc++_reflection protobuf pthread)
