#include "server.h"

#include <bitset>
#include <iostream>
#include <thread>

Server::Server()
{
    m_socket.setBlocking(false);
    m_socket.bind(54321);
    m_connects.fill(false);
}

void Server::run()
{
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
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
                handleConnect(packet, address, port);
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
                   << client.position.y;
            sendTo(packet, static_cast<Client_t>(i));
        }
    }
}

void Server::handleConnect(const sf::Packet &packet,
                           const sf::IpAddress &address, Port_t port)
{
    (void)packet;
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
    if (input & Input::FOWARDS) {
        state.position.y -= 1;
    }
    if (input & Input::BACK) {
        state.position.y += 1;
    }
    if (input & Input::LEFT) {
        state.position.x -= 1;
    }
    if (input & Input::RIGHT) {
        state.position.x += 1;
    }

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