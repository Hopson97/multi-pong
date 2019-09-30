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

void Server::sendState()
{
    for (unsigned i = 0; i < MAX_CONNECTIONS; i++) {
        if (m_connects[i]) {
            auto &client = getClientState(i);
            sf::Packet packet;
            packet << CommandsToClient::State << client.position.x
                   << client.position.y << client.angle;
            sendTo(packet, static_cast<Client_t>(i));
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

    auto &state = getClientState(id);
    const float delta = 3.5f;
    state.speed = 0;
    if (input & Input::FOWARDS) {
        state.speed = delta;
    }
    if (input & Input::BACK) {
        state.speed = -delta;
    }
    if (input & Input::LEFT) {
        state.angle -= 5;
    }
    if (input & Input::RIGHT) {
        state.angle += 5;
    }

    state.position.x +=
        state.speed * std::cos((state.angle + 90) * 3.14159f / 180.0f);
    state.position.y +=
        state.speed * std::sin((state.angle + 90) * 3.14159f / 180.0f);

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