#pragma once

#include "net_commands.h"
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <array>
#include <queue>

struct ClientEndPoint {
    sf::Time lastHeard;
    sf::IpAddress address;
    Port_t port;
};

struct ClientState {
    sf::Vector2f position;
    float speed;
    float angle;
};

struct ClientInput {
    int32_t up;
    int32_t down;
    int32_t left;
    int32_t right;
};

class Server {
  public:
    Server();

    void run();

  private:
    void recievePackets();
    void sendState();

    void sendTo(sf::Packet &packet, Client_t client);
    int emptySlot();

    void handleConnect(const sf::Packet &packet, const sf::IpAddress &address,
                       Port_t port);
    void handleInput(sf::Packet &packet);

    template <typename T>
    ClientEndPoint &getClientEndPoint(T slot);

    template <typename T>
    ClientState &getClientState(T slot);

    sf::UdpSocket m_socket;
    sf::Clock m_clock;

    unsigned m_connectedClients = 0;
    std::array<ClientEndPoint, MAX_CONNECTIONS> m_clientEndPoints;
    std::array<ClientState, MAX_CONNECTIONS> m_clientStates;
    std::array<ClientInput, MAX_CONNECTIONS> m_clientInputs;
    std::array<bool, MAX_CONNECTIONS> m_connects;
};

template <typename T>
ClientEndPoint &Server::getClientEndPoint(T slot)
{
    return m_clientEndPoints[static_cast<size_t>(slot)];
}

template <typename T>
ClientState &Server::getClientState(T slot)
{
    return m_clientStates[static_cast<size_t>(slot)];
}