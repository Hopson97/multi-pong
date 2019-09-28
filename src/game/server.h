
#include "net_commands.h"
#include <SFML/Network.hpp>

class Server {
  public:
    Server();

    void run();

    void sendTo(sf::Packet &packet, Client_t client);

  private:
    sf::UdpSocket m_socket;
};