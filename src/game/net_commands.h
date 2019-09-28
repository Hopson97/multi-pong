#pragma once

#include <SFML/Network.hpp>

using Command_t = uint8_t;
using Client_t = uint8_t;
using Port_t = uint16_t;

constexpr unsigned CONNECTIONS = 4;

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

sf::Packet &operator>>(sf::Packet &packet, CommandsToServer &command);
sf::Packet &operator>>(sf::Packet &packet, CommandsToClient &command);
sf::Packet &operator<<(sf::Packet &packet, CommandsToServer command);
sf::Packet &operator<<(sf::Packet &packet, CommandsToClient command);