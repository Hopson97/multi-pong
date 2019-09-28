#include "server.h"

#include <iostream>

Server::Server()
{
    m_socket.setBlocking(false);
    m_socket.bind(54321);
}

void Server::run()
{
    while (true) {
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

void Server::sendTo(sf::Packet &packet, Client_t clientId) {
    auto& client = getClient(clientId);
    m_socket.send(packet, client.address, client.port);
}

void Server::handleConnect(sf::Packet packet, sf::IpAddress address, Port_t port)
{
    std::cout << "Client requesting connection from " << address.toString() << std::endl;    
    (void)packet;
    (void)address;
    (void)port;
}