
add_executable(${PROJECT_NAME} 
    main.cpp
    client.cpp
    input/keyboard.cpp
)

setup_target_compile(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE ${PONG_SERVER_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${PONG_COMMON_INCLUDE_DIR})

#Finally
target_link_libraries(${PROJECT_NAME}
    pong-common
    pong-server
    Threads::Threads 
    ${SFML_LIBRARIES} 
    ${SFML_DEPENDENCIES}
)
