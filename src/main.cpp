#include <SFML/Graphics.hpp>
#include <iostream>

#include "game/client.h"
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
    }
    std::vector<std::string> options;
    for (int i = 0; i < argc; i++) {
        options.push_back(argv[i]);
    }

    if (options[1] == "client") {
    }
    else if (options[1] == "server") {
    }
    else {
        printOptions();
    }

    /*
        sf::RenderWindow window({1280, 720}, "SFML", sf::Style::Default);
        window.setFramerateLimit(60);
        window.setKeyRepeatEnabled(false);

        Keyboard keyboard;

        while (window.isOpen()) {
            sf::Event e;
            while (window.pollEvent(e)) {
                keyboard.update(e);
                switch (e.type) {
                    case sf::Event::Closed:
                        window.close();
                        break;
                    default:
                        break;
                }
            }
            window.clear();

            window.display();
        }
    */
}
