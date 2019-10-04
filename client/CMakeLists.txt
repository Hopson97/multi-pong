
add_executable(${PROJECT_NAME} 
    main.cpp
    client.cpp
    input/keyboard.cpp
)

setup_target_compile(${PROJECT_NAME})

#Finally
target_link_libraries(${PROJECT_NAME}
    pong-common
    pong-server
    Threads::Threads 
    ${SFML_LIBRARIES} 
    ${SFML_DEPENDENCIES}
)
