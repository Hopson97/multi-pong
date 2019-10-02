#include "client.h"

#include <bitset>
#include <iostream>

#include "pong.h"

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
            uint8_t result;
            packet >> command >> result;
            if (result) {
                connect(packet);
            }
        }
    }

    for (unsigned i = 0; i < MAX_CONNECTIONS; i++) {
        m_peers[i].sprite.setFillColor(sf::Color{100, 50, 154});
        m_peers[i].sprite.setSize({PADDLE_WIDTH, PADDLE_HEIGHT});
    }

    m_ball.setSize({10, 10});
}

void Client::run()
{
    if (!m_isConnected) {
        return;
    }
    m_window.create({WIN_WIDTH, WIN_HEIGHT}, "Pong");
    m_window.setFramerateLimit(60);
    m_window.setKeyRepeatEnabled(false);

    while (m_window.isOpen()) {
        handleWindowEvents();

        // Input
        Input_t input = 0;
        if (m_keys.isKeyDown(sf::Keyboard::W))
            input |= Input::UP;

        if (m_keys.isKeyDown(sf::Keyboard::S))
            input |= Input::DOWN;

        // if (input) {
        sf::Packet clientInput;
        clientInput << CommandsToServer::Input << m_clientId << input;
        send(clientInput);
        //   }

        // Receive packets
        {
            sf::Packet packet;
            sf::IpAddress address;
            Port_t port;
            while (m_socket.receive(packet, address, port) ==
                   sf::Socket::Done) {
                CommandsToClient command;
                packet >> command;
                switch (command) {
                    case CommandsToClient::ConnectRequestResult:
                        break;

                    case CommandsToClient::State:
                        handleStateRecieve(packet);
                        break;

                    case CommandsToClient::BallPosition:
                        handleBallPosition(packet);
                        break;
                }
            }
        }

        // Render
        m_window.clear();
        for (unsigned i = 0; i < MAX_CONNECTIONS; i++) {
            if (m_connects[i]) {
                m_window.draw(m_peers[i].sprite);
            }
        }

        m_window.draw(m_ball);
        m_window.display();
    }
}

void Client::send(sf::Packet &packet)
{
    m_socket.send(packet, m_remoteAddress, m_remotePort);
}

void Client::connect(sf::Packet &packet)
{
    std::cout << "Connection request accepted!" << std::endl;
    packet >> m_clientId;
    m_isConnected = true;

    auto slot = static_cast<std::size_t>(m_clientId);
    m_connects[slot] = true;
    m_peers[slot].sprite.setOutlineColor(sf::Color::White);
    m_peers[slot].sprite.setOutlineThickness(2);
    std::cout << "Connection is able to be made!" << std::endl;

    m_socket.setBlocking(false);
}

void Client::handleStateRecieve(sf::Packet &packet)
{
    Client_t id;
    float x;
    float y;
    while (packet >> id) {
        packet >> x >> y;
        std::cout << x << " " << y << std::endl;
        m_connects[id] = true;
        m_peers[id].sprite.setPosition(x, y);
    }
}

void Client::handleBallPosition(sf::Packet &packet)
{
    float x;
    float y;
    packet >> x >> y;
    m_ball.setPosition(x, y);
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