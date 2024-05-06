#include "Clients.hpp"

int Clients::getFd() const {
    return this->_fd;
}

std::string Clients::getAddrIp() const {
    return this->_addrIp;
}

void Clients::setFd(int fd) {
    this->_fd = fd;
}

void Clients::setAddrIp(std::string addrIp) {
    this->_addrIp = addrIp;
}

Clients::Clients() {
    std::cout << "clients constructor created" << std::endl;
}