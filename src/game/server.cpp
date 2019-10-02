#include "server.h"

#include <bitset>
#include <cmath>
#include <iostream>
#include <thread>

#include "pong.h"

using Clock = std::chrono::high_resolution_clock;
using Time = std::chrono::microseconds;
using Time_t = std::uint64_t;

Server::Server()
{
    m_socket.setBlocking(false);
    m_socket.bind(54321);
    m_connects.fill(false);

    m_ball.bounds.left = WIN_WIDTH / 2.0f;
    m_ball.bounds.top = WIN_HEIGHT / 2.0f;
    m_ball.speedX = 200;
    m_ball.speedY = 50;

    for (auto &state : m_clientStates) {
        state.bounds.width = PADDLE_WIDTH;
        state.bounds.height = PADDLE_HEIGHT;
    }
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

            const float delta = 12.5f;
            if (input.up) {
                state.speedY -= delta;
            }
            if (input.down) {
                state.speedY += delta;
            }

            state.speedX *= 0.9f;
            state.speedY *= 0.9f;
            if (std::abs(state.speedX) < 0.05f) {
                state.speedX = 0.0f;
            }
            if (std::abs(state.speedY) < 0.05f) {
                state.speedY = 0.0f;
            }

            state.bounds.left += state.speedX * (1 / 60.0f);
            state.bounds.top += state.speedY * (1 / 60.0f);

            if (state.bounds.top > WIN_HEIGHT) {
                state.bounds.top = WIN_HEIGHT - 1;
                state.speedY = 0;
            }
            else if (state.bounds.top < 0) {
                state.bounds.top = 1;
                state.speedY = 0;
            }
        }
    }

    m_ball.bounds.top += m_ball.speedY * (1 / 60.0f);

    float BALL_SIZE = 10;
    if (m_ball.bounds.left + BALL_SIZE > WIN_WIDTH || m_ball.bounds.left <= 0) {
        m_ball.speedX *= -1;
    }

    if (m_ball.bounds.top + BALL_SIZE > WIN_HEIGHT || m_ball.bounds.top <= 0) {
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
            packet << static_cast<Client_t>(i) << client.bounds.left
                   << client.bounds.top;
        }
    }
    sf::Packet ballPacket;
    ballPacket << CommandsToClient::BallPosition << m_ball.bounds.left
               << m_ball.bounds.top;

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

        auto &bounds = m_clientStates[slot].bounds;
        if (slot == 0) {
            bounds.left = 10;
            bounds.top = 10;
        }
        else {
            bounds.left = WIN_HEIGHT - PADDLE_WIDTH - 10;
            bounds.top = 10;
        }

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
    inputState.up = input & Input::UP;
    inputState.down = input & Input::DOWN;

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