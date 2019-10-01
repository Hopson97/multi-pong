#include "server.h"

#include <bitset>
#include <cmath>
#include <iostream>
#include <thread>

using Clock = std::chrono::high_resolution_clock;
using Time = std::chrono::microseconds;
using Time_t = std::uint64_t;

namespace {
    void testWinBounds(sf::Vector2f &position, float &speedX, float& speedY)
    {
        if (position.x > WIN_WIDTH) {
            position.x = WIN_WIDTH - 1;
            speedX = 0;
        }
        else if (position.x < 0) {
            position.x = WINT_WIDTH - 1;
            speedX = 0;
        }
        if (position.y > WIN_HEIGHT) {
            position.y = WIN_HEIGHT - 1;
            speedY = 0;
        }
        else if (position.y < 0) {
            position.y = 1;
            speedY = 0;
        }
    }
} // namespace

Server::Server()
{
    m_socket.setBlocking(false);
    m_socket.bind(54321);
    m_connects.fill(false);

    m_ball.position = {100, 100};
    m_ball.speedX = 200;
    m_ball.speedY = 50;
}

void Server::run()
{
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
        recievePackets();
        updateState();
        sendState();
    }
}

void Server::sendTo(sf::Packet &packet, Client_t clientId)
{
    auto &client = getClientEndPoint(clientId);
    m_socket.send(packet, client.address, client.port);
}

void Server::recievePackets()
{
    sf::Packet packet;
    sf::IpAddress address;
    Port_t port;
    while (m_socket.receive(packet, address, port) == sf::Socket::Done) {
        CommandsToServer command;
        packet >> command;
        switch (command) {
            case CommandsToServer::Connect:
                handleConnect(address, port);
                break;

            case CommandsToServer::Disconnect:
                break;

            case CommandsToServer::Input:
                handleInput(packet);
                break;
        }
    }
}

void Server::updateState()
{
    for (unsigned i = 0; i < MAX_CONNECTIONS; i++) {
        if (m_connects[i]) {
            auto &input = m_clientInputs[i];
            auto &state = m_clientStates[i];

            const float delta = 17.5f;
            if (input.forwards) {
                state.speedX +=
                    std::cos((state.angle + 90) * 3.14159f / 180.0f) * delta;
                state.speedY +=
                    std::sin((state.angle + 90) * 3.14159f / 180.0f) * delta;
            }
            if (input.backwards) {
                state.speedX -=
                    std::cos((state.angle + 90) * 3.14159f / 180.0f) * delta;
                state.speedY -=
                    std::sin((state.angle + 90) * 3.14159f / 180.0f) * delta;
            }
            if (input.left) {
                state.angle -= 5;
            }
            if (input.right) {
                state.angle += 5;
            }

            state.speedX *= 0.9f;
            state.speedY *= 0.9f;
            if (std::abs(state.speedX) < 0.05f) {
                state.speedX = 0.0f;
            }
            if (std::abs(state.speedY) < 0.05f) {
                state.speedY = 0.0f;
            }

            state.position.x += state.speedX * (1 / 60.0f);
            state.position.y += state.speedY * (1 / 60.0f);

            testWinBounds(state.position, state.speedX, state.speedY);
        }
    }

    m_ball.position.x += m_ball.speedX * (1 / 60.0f);
    m_ball.position.y += m_ball.speedY * (1 / 60.0f);

    float BALL_SIZE = 10;
    if (m_ball.position.x + BALL_SIZE > WIN_WIDTH || m_ball.position.x <= 0) {
        m_ball.speedX *= -1;
    }

    if (m_ball.position.y + BALL_SIZE > WIN_HEIGHT || m_ball.position.y <= 0) {
        m_ball.speedY *= -1;
    }
}

void Server::sendState()
{
    sf::Packet packet;
    packet << CommandsToClient::State;
    for (unsigned i = 0; i < MAX_CONNECTIONS; i++) {
        if (m_connects[i]) {
            auto &client = getClientState(i);
            packet << static_cast<Client_t>(i) << client.position.x
                   << client.position.y << client.angle;
        }
    }
    sf::Packet ballPacket;
    ballPacket << CommandsToClient::BallPosition << m_ball.position.x
               << m_ball.position.y;

    for (unsigned i = 0; i < MAX_CONNECTIONS; i++) {
        if (m_connects[i]) {
            sendTo(packet, static_cast<Client_t>(i));
            sendTo(ballPacket, static_cast<Client_t>(i));
        }
    }
}

void Server::handleConnect(const sf::IpAddress &address, Port_t port)
{
    std::cout << "Client requesting connection\n"
              << "From IP: " << address.toString() << '\n'
              << "From port: " << (int)port << std::endl;

    if (m_connectedClients < MAX_CONNECTIONS) {
        std::cout << "Connection is able to be made!" << std::endl;

        auto slot = emptySlot();
        auto &client = getClientEndPoint(slot);
        client.address = address;
        client.port = port;

        sf::Packet packet;
        packet << CommandsToClient::ConnectRequestResult << (uint8_t)1
               << static_cast<Client_t>(slot);
        sendTo(packet, static_cast<Client_t>(slot));

        m_clientStates[slot].position = {WIN_WIDTH / 2, WIN_HEIGHT / 2};
        m_connects[slot] = true;
        m_connectedClients++;
    }
    std::cout << "--\n\n";
}

void Server::handleInput(sf::Packet &packet)
{
    Client_t id;
    Input_t input;
    packet >> id >> input;

    auto &inputState = m_clientInputs[id];
    inputState.forwards = input & Input::FOWARDS;
    inputState.backwards = input & Input::BACK;
    inputState.left = input & Input::LEFT;
    inputState.right = input & Input::RIGHT;

    /*
    std::cout << "Stamp: " << m_clock.getElapsedTime().asSeconds() << std::endl;
    std::cout << "Input recieved from client" << std::endl;
    std::cout << "Input: " << std::bitset<sizeof(Input_t) * 8>(input)
              << std::endl;
    std::cout << "--\n\n";
    */
}

int Server::emptySlot()
{
    for (unsigned i = 0; i < MAX_CONNECTIONS; i++) {
        if (!m_connects[i]) {
            return i;
        }
    }
    return -1;
}