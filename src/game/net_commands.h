#pragma once

#include <SFML/Network.hpp>

using Command_t = uint8_t;
using Client_t = uint8_t;
using Port_t = uint16_t;
using Input_t = uint8_t;

constexpr unsigned MAX_CONNECTIONS = 4;
constexpr unsigned WIN_WIDTH = 600;
constexpr unsigned WIN_HEIGHT = 400;

enum class CommandsToServer : Command_t {
    // Pure
    Connect,

    // Enum
    // Client_t id
    Disconnect,

    /*
        Enum
        Client_t ClientID
        Input_t Input
    */
    Input,
};

enum class CommandsToClient : Command_t {
    // Enum
    // uint8_t, 0 = fail, 1 = success
    // client_t id
    ConnectRequestResult,

    // Enum
    // id
    // Data [x, y, angle]
    State,

    // Enum
    // Data [x, y]
    BallPosition,
};

enum Input : Input_t { FOWARDS = 0x1, BACK = 0x2, LEFT = 0x4, RIGHT = 0x8 };

sf::Packet &operator>>(sf::Packet &packet, CommandsToServer &command);
sf::Packet &operator>>(sf::Packet &packet, CommandsToClient &command);
sf::Packet &operator<<(sf::Packet &packet, CommandsToServer command);
sf::Packet &operator<<(sf::Packet &packet, CommandsToClient command);