#include "server.h"

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
            }
        }
    }
}

void Server::sendTo(sf::Packet &packet, Client_t clientId)
{
    auto &client = getClientEndPoint(clientId);
    m_socket.send(packet, client.address, client.port);
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
        packet << CommandsToClient::AcceptConnection << static_cast<Client_t>(slot);
        sendTo(packet, slot);

        m_connectedClients++;
    }
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