set(PONG_SERVER_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/server/include)

add_library(pong-server
    src/server.cpp
)

target_include_directories(pong-server PRIVATE ${PONG_COMMON_INCLUDE_DIR})
target_include_directories(pong-server PRIVATE ${PONG_SERVER_INCLUDE_DIR})

