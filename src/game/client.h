#pragma once

#include "../input/keyboard.h"
#include "net_commands.h"
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

struct ClientPeer {
    sf::CircleShape sprite;
};

class Client {
  public:
    Client();

    void run();

  private:
    void send(sf::Packet &packet);
    void handleWindowEvents();

    void connect(sf::Packet& packet);

    unsigned m_connectedClients = 0;
    std::array<ClientPeer, MAX_CONNECTIONS> m_peers;
    std::array<bool, MAX_CONNECTIONS> m_connects;

    sf::UdpSocket m_socket;

    sf::RenderWindow m_window;
    Keyboard m_keys;

    //Server info
    sf::IpAddress m_remoteAddress;
    Port_t m_remotePort;

    //Id 
    Client_t m_clientId;
    bool m_isConnected = false;
};