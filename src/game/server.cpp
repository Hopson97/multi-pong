#include "server.h"

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
        while (m_socket.receive(packet, address, port)) {
        }
    }
}