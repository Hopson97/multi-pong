set(PONG_COMMON_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/common/include)

add_library(pong-common 
    src/net_commands.cpp
)   

target_include_directories(pong-common PRIVATE ${PONG_COMMON_INCLUDE_DIR})