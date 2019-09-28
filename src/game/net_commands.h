#pragma once

#include <SFML/Network.hpp>

using Command_t = uint8_t;
using Client_t = uint8_t;
using Port_t = uint16_t;

constexpr unsigned MAX_CONNECTIONS = 4;

enum class CommandsToServer : Command_t {
    // Pure
    Connect,
};

enum class CommandsToClient : Command_t {
    // Pure
    RejectConnection,

    // Pure
    AcceptConnection,
};

enum Input : uint16_t {
    FOWARDS = 0x1,
    BACK    = 0x2,
    LEFT    = 0x4,
    RIGHT   = 0x8
};

sf::Packet &operator>>(sf::Packet &packet, CommandsToServer &command);
sf::Packet &operator>>(sf::Packet &packet, CommandsToClient &command);
sf::Packet &operator<<(sf::Packet &packet, CommandsToServer command);
sf::Packet &operator<<(sf::Packet &packet, CommandsToClient command);