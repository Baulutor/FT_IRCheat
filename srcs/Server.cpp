#include "Server.hpp"

int Server::getFd() const {
    return this->_fd;
}

std::string Server::getAddrIp() const {
    return this->_addrIp;
}

void Server::setFd(int fd) {
    this->_fd = fd;
}

void Server::setAddrIp(std::string addrIp) {
    this->_addrIp = addrIp;
}

bool startWith(const std::string &line, std::string &cmd)
{
    return (line.find(cmd) == 0);
}


Server::Server( std::string av ) {

    //creation du socket serv
    Clients client;
    setFd(socket(AF_INET, SOCK_STREAM, 0));
    if (getFd() < 0) {
        std::cerr << "error socket" << std::endl;
    }
    std::cout << "fd = " << getFd() << std::endl;
    setAddrIp("127.0.0.1");

    //configuration d'une struct socketaddr_in pour avoir des appels systeme reseau comme bind et connect
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(av.c_str()));  //parsing du port
    addr.sin_addr.s_addr = inet_addr(getAddrIp().c_str());

    //lie un socket a une adresse ip et un port
    if (bind(getFd(), (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "Error liaison socket" << std::endl;
    }

    // mode ecoute du socket
    if (listen(getFd(), 5))
        std::cerr << "Error listen" << std::endl;

    socklen_t sock_info_len = sizeof(addr);
    setFd(accept(getFd(), (struct sockaddr*)&addr, &sock_info_len));
    std::cout << "fd2 = " << getFd() << std::endl;
    
    if(getFd() < 0)
        std::cout << "Error : accept" << std::endl;
    else
        std::cout << "Accept " << std::endl;
    char buffer[2048];
    bool init = false;
    while (true)
    {
        ssize_t bytes = recv(getFd(), buffer, 2048, 0);
        if (bytes < 0)
            std::cerr << "ERROR rcve !" << std::endl;
        else if ( bytes == 0)
            std::cout << "connexion closed " << std::endl;
        else
            buffer[bytes] = '\0';
        std::cout << "buffer : |" << buffer << "|" << std::endl;
        std::string command = "CAP LS 302";
        if (startWith(buffer, command) || !init)
        {
            init = client.initClients(buffer);
        }
        // selectCmd(buffer);
        sleep(1);
    }
    close(getFd());
}