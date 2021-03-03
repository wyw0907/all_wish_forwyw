
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

aux_source_directory(./ core_srcs)
aux_source_directory(./mq/ mq_srcs)
aux_source_directory(./crypt crypt_srcs)
aux_source_directory(./time time_srcs)


add_library(wish STATIC 
    ${core_srcs}
    ${mq_srcs}
    ${crypt_srcs}
    ${time_srcs}
)

target_link_libraries(wish nanomsg ssl pthread dl)

