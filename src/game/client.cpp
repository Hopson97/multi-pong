#include "client.h"

#include <iostream>

Client::Client()
    : m_remoteAddress(sf::IpAddress::LocalHost)
    , m_remotePort(54321)
{
    m_connects.fill(false);
    sf::Packet packet;
    packet << CommandsToServer::Connect;
    send(packet);
    {
        sf::Packet packet;
        sf::IpAddress address;
        Port_t port;
        if (m_socket.receive(packet, address, port) == sf::Socket::Done) {
            CommandsToClient command;
            packet >> command;
            switch (command)
            {
            case CommandsToClient::AcceptConnection:
                connect(packet);
                break;

            case CommandsToClient::RejectConnection:
                std::cout << "Connection rejected." << std::endl;
                break;
            
            default:
                break;
            }
        }
    }
}

void Client::run()
{
    if (!m_isConnected) {
        return;
    }
    m_window.create({600, 400}, "Pong");
    m_window.setFramerateLimit(60);
    m_window.setKeyRepeatEnabled(false);

    while (m_window.isOpen()) {
        handleWindowEvents();

        //Render
        m_window.clear();
        for (unsigned i = 0; i < MAX_CONNECTIONS; i++) {
            if (m_connects[i]) {
                m_window.draw(m_peers[i].sprite);
            }
        }
        m_window.display();
    }
}

void Client::send(sf::Packet &packet)
{
    m_socket.send(packet, m_remoteAddress, m_remotePort);
}

void Client::connect(sf::Packet& packet) {
    std::cout << "Connection request accepted!" << std::endl;
    packet >> m_clientId;
    m_isConnected = true;

    auto slot = static_cast<std::size_t>(m_clientId);
    m_connects[slot] = true;
    m_peers[slot].sprite.setFillColor(sf::Color::Red);
    m_peers[slot].sprite.setRadius(20.0f);
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