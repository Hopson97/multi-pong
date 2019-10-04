add_library(pong-common 
    src/net_commands.cpp
)   

target_include_directories(pong-common PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)