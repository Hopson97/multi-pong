add_library(pong-server
    src/server.cpp
)

target_include_directories(pong-server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
