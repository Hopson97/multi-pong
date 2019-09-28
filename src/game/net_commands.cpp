#include "net_commands.h"

sf::Packet &operator>>(sf::Packet &packet, CommandsToServer &command)
{
    Command_t commandId;
    packet >> commandId;
    command = static_cast<CommandsToServer>(commandId);
    return packet;
}

sf::Packet &operator>>(sf::Packet &packet, CommandsToClient &command)
{
    Command_t commandId;
    packet >> commandId;
    command = static_cast<CommandsToClient>(commandId);
    return packet;
}

sf::Packet &operator<<(sf::Packet &packet, CommandsToServer command)
{
    packet << static_cast<Command_t>(command);;
    return packet;
}

sf::Packet &operator<<(sf::Packet &packet, CommandsToClient command)
{
    packet << static_cast<Command_t>(command);;
    return packet;
}
