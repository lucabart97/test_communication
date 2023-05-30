find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIR})
protobuf_generate_cpp(PROTO_SRC PROTO_HEADER command.proto position.proto config.proto grid.proto)
add_library(message ${PROTO_HEADER} ${PROTO_SRC})
target_link_libraries(message PUBLIC
    ${Protobuf_LIBRARIES}
)
target_include_directories(message PUBLIC
    ${PROTOBUF_INCLUDE_DIR}
)