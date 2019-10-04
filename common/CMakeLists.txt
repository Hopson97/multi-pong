
set(PONG_COMMON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(pong-common 
    src/net_commands.cpp
)   

target_include_directories(pong-common PRIVATE ${PONG_COMMON_INCLUDE_DIR})