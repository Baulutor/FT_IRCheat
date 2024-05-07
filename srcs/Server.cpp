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

void Server::join_chan( const std::string & chanel ) {
    std::cout << "salut c coool" << chanel << std::endl;
}

void	Server::selectCmd( std::string cmd ) {
	//int lvl_value = 0;
    std::cout << "cmd == " << cmd << std::endl;
    this->_cmd[0] = "JOIN";
	this->_cmd[1] = "SLT";
	this->_cmd[2] = "CACA";

	// while (lvl_value < 3 && cmd != this->_cmd[lvl_value])
	// 	lvl_value++;
	// switch(lvl_value) {
	// 	case 0: case 1: case 2:
	// 		while (lvl_value < 3)
	// 		{
	// 			(this->*(_function[lvl_value]))();
	// 			lvl_value++;
	// 		}
	// 		break ; 
	// 	default :
	// 		std::cout << "Error: cannot found complain" << std::endl;
	//}
}


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
    int new_socket = accept(getFd(), (struct sockaddr*)&addr, &sock_info_len);
    std::cout << "fd2 = " << new_socket << std::endl;
    
    if(new_socket < 0)
        std::cout << "Error : accept" << std::endl;
    else
        std::cout << "Accept " << std::endl;
    char buffer[1024];

    while (true)
    {
        ssize_t bytes = recv(new_socket, buffer, 1024, 0);
        if (bytes < 0)
            std::cerr << "ERROR rcve !" << std::endl;
        else if ( bytes == 0)
            std::cout << "connexion closed " << std::endl;
        selectCmd(buffer);
        // if (strncmd(buffer, "JOIN", 4) == 0)
        //     join_chan(buffer);
        // else if (strncmd(buffer, "SLT", 3) == 0)
        //     break ;
        sleep(1);
    }
    close(getFd());
    close(new_socket);
}