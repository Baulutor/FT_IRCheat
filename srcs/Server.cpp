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

void Server::join_chan() {
    std::cout << "salut tu veux quel chan ?" << std::endl;
}

void Server::slt() {
    std::cout << "salut c coool" << std::endl;
}

void Server::caca() {
    std::cout << "salut j'aime le caca" << std::endl;
}

// void	Server::selectCmd( std::string cmd ) {
// 	int lvl_value = 0;
//     std::string cmd_tmp = cmd.substr(0, cmd.find_first_of(' '));
//     //std::cout << "cmd == " << cmd << std::endl;
//     std::cout << "cmd == " << cmd_tmp << "|" << std::endl;
//     this->_cmd[0] = "JOIN";
// 	this->_cmd[1] = "SLT";
// 	this->_cmd[2] = "CACA";
//     this->_function[0] = &Server::join_chan;
//     this->_function[1] = &Server::slt;
//     this->_function[2] = &Server::caca;

// 	while (lvl_value < 3 && cmd != this->_cmd[lvl_value])
// 		lvl_value++;
// 	switch(lvl_value) {
// 		case 0: case 1: case 2:
// 			while (lvl_value < 3)
// 			{
// 				(this->*(_function[lvl_value]))();
// 				lvl_value++;
// 			}
// 			break ; 
// 		default :
// 			std::cout << "Error: cannot found cmd" << std::endl;
// 	}
// }


Server::Server( std::string av ) {

    //creation du socket serv
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
    char buffer[1024];

    while (true)
    {
        ssize_t bytes = recv(getFd(), buffer, 1024, 0);
        if (bytes < 0)
            std::cerr << "ERROR rcve !" << std::endl;
        else if ( bytes == 0)
            std::cout << "connexion closed " << std::endl;
        else
            buffer[bytes] = '\0';
        std::cout << buffer << std::endl;
        //selectCmd(buffer);
        sleep(1);
    }
    close(getFd());
}