#pragma once

#include "../input/keyboard.h"
#include "net_commands.h"
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

class Client {
  public:
    Client();

    void run();

  private:
    void send(sf::Packet &packet);
    void handleWindowEvents();

    sf::UdpSocket m_socket;

    sf::RenderWindow m_window;
    Keyboard m_keys;

    sf::IpAddress m_remoteAddress;
    Port_t m_remotePort;
};