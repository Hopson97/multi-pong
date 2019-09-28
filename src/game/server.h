#pragma once

#include "net_commands.h"
#include <SFML/Network.hpp>
#include <array>

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

    void handleConnect(const sf::Packet &packet, const sf::IpAddress &address,
                       Port_t port);

    template <typename T> ConnectedClient &getClient(T slot)
    {
        return m_clients[static_cast<size_t>(slot)];
    }

    sf::UdpSocket m_socket;

    int connectedClients = 0;
    std::array<ConnectedClient, CONNECTIONS> m_clients;
};