#include "client.h"

Client::Client()
    : m_remoteAddress(sf::IpAddress::LocalHost)
    , m_remotePort(54321)
{
    sf::Packet packet;
    packet << CommandsToServer::Connect;
    send(packet);
}

void Client::run()
{
    m_window.create({600, 400}, "Pong");
    m_window.setFramerateLimit(60);
    m_window.setKeyRepeatEnabled(false);

    while (m_window.isOpen()) {
        handleWindowEvents();
    }
}

void Client::send(sf::Packet &packet)
{
    m_socket.send(packet, m_remoteAddress, m_remotePort);
}

void Client::handleWindowEvents()
{
    sf::Event e;
    while (m_window.pollEvent(e)) {
        m_keys.update(e);
        switch (e.type) {
            case sf::Event::Closed:
                m_window.close();
                break;

            default:
                break;
        }
    }
}