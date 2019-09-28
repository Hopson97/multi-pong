#pragma once

#include <SFML/Network.hpp>

using Command_t = uint8_t;
using Client_t = uint8_t;
using Port_t = uint16_t;

enum CommandsToServer : Command_t {
    // Pure
    Connect,
};

enum CommandsToClient : Command_t {
    // Pure
    RejectConnection,

    // Pure
    AcceptConnection,
};
