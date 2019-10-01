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

struct ObjectState {
    sf::Vector2f position;
    float speedX = 0;
    float speedY = 0;
    float angle = 0;
};

struct ClientInput {
    int32_t forwards = 0;
    int32_t backwards = 0;
    int32_t left = 0;
    int32_t right = 0;
};

class Server {
  public:
    Server();

    void run();

  private:
    void recievePackets();
    void updateState();
    void sendState();

    void sendTo(sf::Packet &packet, Client_t client);
    int emptySlot();

    void handleConnect(const sf::IpAddress &address, Port_t port);
    void handleInput(sf::Packet &packet);

    template <typename T>
    ClientEndPoint &getClientEndPoint(T slot);

    template <typename T>
    ObjectState &getClientState(T slot);

    sf::UdpSocket m_socket;
    sf::Clock m_clock;

    unsigned m_connectedClients = 0;
    std::array<ClientEndPoint, MAX_CONNECTIONS> m_clientEndPoints;
    std::array<ObjectState, MAX_CONNECTIONS> m_clientStates;
    std::array<ClientInput, MAX_CONNECTIONS> m_clientInputs;
    std::array<uint8_t, MAX_CONNECTIONS> m_connects;

    ObjectState m_ball;
};

template <typename T>
ClientEndPoint &Server::getClientEndPoint(T slot)
{
    return m_clientEndPoints[static_cast<size_t>(slot)];
}

template <typename T>
ObjectState &Server::getClientState(T slot)
{
    return m_clientStates[static_cast<size_t>(slot)];
}