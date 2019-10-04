#include <SFML/Graphics.hpp>
#include <iostream>

#include "client.h"
#include "game/server.h"

void printOptions()
{
    std::cout << "Please enter options. Usage:\n";
    std::cout << "./pong client\n";
    std::cout << "./pong server\n";
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        std::cout << "Please enter options. Usage:\n";
        std::cout << "./pong client\n";
        std::cout << "./pong server\n";
        return EXIT_SUCCESS;
    }
    std::vector<std::string> options;
    for (int i = 0; i < argc; i++) {
        options.push_back(argv[i]);
    }

    if (options[1] == "client") {
        Client client;
        client.run();
    }
    else if (options[1] == "server") {
        Server server;
        server.run();
    }
    else {
        printOptions();
    }
}
