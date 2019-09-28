#pragma once

#include "net_commands.h"
#include <SFML/Network.hpp>
#include <array>
#include <queue>

struct ConnectedClient {
    sf::IpAddress address;
    Port_t port;
    Client_t id;
};

class Server {
  public:
    Server();

    void run();

  private:
    void sendTo(sf::Packet &packet, Client_t client);
    int emptySlot();

    void handleConnect(const sf::Packet &packet, const sf::IpAddress &address,
                       Port_t port);

    template <typename T>
    ConnectedClient &getClient(T slot);

    sf::UdpSocket m_socket;

    unsigned m_connectedClients = 0;
    std::array<ConnectedClient, MAX_CONNECTIONS> m_clients;
    std::array<bool, MAX_CONNECTIONS> m_connects;
};

template <typename T>
ConnectedClient &Server::getClient(T slot)
{
    return m_clients[static_cast<size_t>(slot)];
}