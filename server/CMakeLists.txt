set(PONG_SERVER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(pong-server
    src/server.cpp
)

setup_target_compile(pong-server)

target_include_directories(pong-server PRIVATE ${PONG_SERVER_INCLUDE_DIR})
target_include_directories(pong-server PRIVATE ${PONG_COMMON_INCLUDE_DIR})
