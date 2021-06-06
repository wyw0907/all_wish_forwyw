include_directories("/usr/local/grpc/include")
link_directories("/usr/local/grpc/lib")

find_path(GRPC_INCLUDE_PATH NAMES grpc/grpc.h)
find_library(GRPC_LIB NAMES grpc)

find_program(PROTOC protoc REQUIRED PATH /usr/local/grpc/bin)
find_program(GRPC_CPP_PLUGIN grpc_cpp_plugin REQUIRED PATH /usr/local/grpc/bin)

EXECUTE_PROCESS (
    COMMAND ${PROTOC}
    -I${CMAKE_CURRENT_SOURCE_DIR} --grpc_out=${CMAKE_CURRENT_SOURCE_DIR} --plugin=protoc-gen-grpc=${GRPC_CPP_PLUGIN} ${CMAKE_CURRENT_SOURCE_DIR}/health_check.proto
)
EXECUTE_PROCESS (
    COMMAND ${PROTOC} 
    -I${CMAKE_CURRENT_SOURCE_DIR} --cpp_out=${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/health_check.proto
)

add_executable(consul_client consul_client.cpp health_check.grpc.pb.cc health_check.pb.cc)
target_link_libraries(consul_client grpc++ gpr grpc++_reflection protobuf pthread)

