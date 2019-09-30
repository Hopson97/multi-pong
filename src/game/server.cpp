#include "server.h"

#include <bitset>
#include <cmath>
#include <iostream>
#include <thread>

using Clock = std::chrono::high_resolution_clock;
using Time = std::chrono::microseconds;
using Time_t = std::uint64_t;

Server::Server()
{
    m_socket.setBlocking(false);
    m_socket.bind(54321);
    m_connects.fill(false);
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

            const float delta = 1.0f;
            if (input.forwards) {
                state.speed += delta;
            }
            if (input.backwards) {
                state.speed += -delta;
            }
            if (input.left) {
                state.angle -= 5;
            }
            if (input.right) {
                state.angle += 5;
            }

            state.speed *= 0.95f;
            if (std::abs(state.speed) < 0.05f) {
                state.speed = 0.0f;
            }
            state.position.x +=
                state.speed * std::cos((state.angle + 90) * 3.14159f / 180.0f);
            state.position.y +=
                state.speed * std::sin((state.angle + 90) * 3.14159f / 180.0f);
        }
    }
}

void Server::sendState()
{
    for (unsigned i = 0; i < MAX_CONNECTIONS; i++) {
        if (m_connects[i]) {
            auto &client = getClientState(i);
            sf::Packet packet;
            packet << CommandsToClient::State << static_cast<Client_t>(i)
                   << client.position.x << client.position.y << client.angle;

            for (unsigned i = 0; i < MAX_CONNECTIONS; i++) {
                if (m_connects[i]) {
                    sendTo(packet, static_cast<Client_t>(i));
                }
            }
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

    */
    std::cout << "Stamp: " << m_clock.getElapsedTime().asSeconds() << std::endl;
    std::cout << "Input recieved from client" << std::endl;
    std::cout << "Input: " << std::bitset<sizeof(Input_t) * 8>(input)
              << std::endl;
    std::cout << "--\n\n";
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