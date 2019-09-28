#pragma once

#include "net_commands.h"
#include <SFML/Network.hpp>
#include <array>
#include <queue>

struct ClientEndPoint {
    sf::IpAddress address;
    Port_t port;
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
    ClientEndPoint &getClientEndPoint(T slot);

    sf::UdpSocket m_socket;

    unsigned m_connectedClients = 0;
    std::array<ClientEndPoint, MAX_CONNECTIONS> m_clientEndPoints;
    std::array<bool, MAX_CONNECTIONS> m_connects;
};

template <typename T>
ClientEndPoint &Server::getClientEndPoint(T slot)
{
    return m_clientEndPoints[static_cast<size_t>(slot)];
}