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

Server::Server( std::string av ) {
    setFd(socket(AF_INET, SOCK_STREAM, 0));
    if (getFd() < 0) {
        std::cerr << "error socket" << std::endl;
    }
    setAddrIp("127.0.0.1");
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(av.c_str()));  //parsing du port
    addr.sin_addr.s_addr = inet_addr(getAddrIp().c_str());

    if (bind(getFd(), (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "Error liaison socket" << std::endl;
    }
    if (listen(getFd(), 5))
        std::cerr << "Error listen" << std::endl;
    // else
    //     std::cout << "Listen " << listen(getFd(), 5) << std::endl;

    struct sockaddr_in sock_info;
    socklen_t sock_info_len = sizeof(sock_info);
    if (getsockname(getFd(), (struct sockaddr*) &sock_info, &sock_info_len) < 0 ) {
        std::cerr << "Error obtention des info du socket" << std::endl;
    }
    std::cout << "fd = " << getFd() << std::endl;
    std::cout << "addr local : " << inet_ntoa(sock_info.sin_addr) << std::endl;
    std::cout << "port local : " << ntohs(sock_info.sin_port) << std::endl;
    socklen_t sock_info_len2 = sizeof(addr);
    void *buffer[5000];
    if (accept(getFd(), (struct sockaddr*)&addr, &sock_info_len2) < 0)
        std::cout << "Error : accept" << std::endl;
    else
        std::cout << "Accept " << std::endl;
    while (true)
    {
        recv(getFd(), buffer, 1024, 0);
        std::cout << "buffer " << *buffer << std::endl;
        sleep(1);
    }

    close(getFd());
    std::cout << "Server constructor created" << std::endl;
}