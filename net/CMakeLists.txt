
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

aux_source_directory(./util/ util_srcs)
aux_source_directory(./app/http http_srcs)
aux_source_directory(./app/http/parser parser_srcs)



add_library(netlib STATIC 
    ${util_srcs}
    ${http_srcs}
    ${parser_srcs}
)

target_link_libraries(netlib spdlog pthread dl)

