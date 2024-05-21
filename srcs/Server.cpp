#include "Server.hpp"
#include "Cmd.hpp"

int Server::getFd() const {return this->_fd;}

std::string Server::getAddrIp() const {return this->_addrIp;}

void Server::setFd(int fd) {this->_fd = fd;}

void Server::setAddrIp(std::string addrIp) {this->_addrIp = addrIp;}

std::map<std::string, Channels>& Server::getChannels() {return (_channels);}

std::map<int, Clients>& Server::getClients() {return (_clients);}

void Server::setChannels(std::map<std::string, Channels> channels) {this->_channels = channels;}

void Server::setClient(std::map<int, Clients> clients) {this->_clients = clients;}

void Server::setPassword(std::string password) {this->_password = password;}

std::string Server::getPassword() const {return (this->_password);}

bool startWith(const std::string &line, const char *cmd) {return (line.find(cmd) == 0);}

void Server::cmdHandler(std::string cmd, Clients& client)
{
	std::cout << "cmd: " << cmd << std::endl;
    const char *lstCmd[] = {"JOIN", "KICK"};
    // , "NAMES", "NICK", "INVITE", "TOPIC", "PRIVMSG", "QUIT", "PART", "KICK", "MODE"
    void (*lstFunc[])(std::string, Clients&, Server&) = {Join, Kick};
    // NAMES, NICK, INVITE, TOPIC, PRIVMSG, QUIT, PART, KICK, MODE
    for (int i = 0; i < 2; i++)
    {
        if (startWith(cmd, lstCmd[i]))
        {
            lstFunc[i](cmd, client, *this);
            return;
        }
    }
}

void Server::Pong(std::string cmd, Clients& client)
{
    std::cout << "PONG" << std::endl;
    std::vector<std::string> splited = split(cmd, ' ');
    std::string pong = "PONG IRC_test " + splited[1];
    std::cout << "pong : " << pong << std::endl;
    if (send(client.getFd(), pong.c_str(), pong.size(), 0) < 0)
        throw std::exception();
}

// void Server::setDataServer()
// {
    
// }

// Fonction pour attendre les événements sur les sockets
//int my_poll(std::vector<PollFd>& pollFds) {
//	const int nfds = pollFds.size();
//	fd_set readSet;
//	FD_ZERO(&readSet);
//
//	int maxFd = -1;
//	for (int i = 0; i < nfds; ++i) {
//		FD_SET(pollFds[i].fd, &readSet);
//		if (pollFds[i].fd > maxFd) {
//			maxFd = pollFds[i].fd;
//		}
//	}
//
//	struct timeval timeout = {0, 0}; // Pas de timeout
//	int ret = select(maxFd + 1, &readSet, NULL, NULL, &timeout);
//	if (ret == -1) {
//		return -1; // Erreur
//	}
//
//	for (int i = 0; i < nfds; ++i)
//	{
//		if (FD_ISSET(pollFds[i].fd, &readSet))
//			pollFds[i].revents = pollFds[i].events;
//		else
//			pollFds[i].revents = 0;
//	}
//	return ret; // Nombre de descripteurs prêts
//}


// ANCIEN en BAS

// void broadcast_message(const std::string &message, int sender_fd, std::vector<pollfd> &clients)
// {
// 	for (size_t i = 1; i < clients.size(); i++)
// 	{
// 		if (clients[i].fd != sender_fd)
// 			send(clients[i].fd, message.c_str(), message.size(), 0);
// 	}
// }



Server::Server(std::string av, std::string av2)
{
	struct sockaddr_in address;
	int opt = 1;
	char buffer[2048];
	// std::string servAddress = "127.0.0.1";

	// Création de la socket serveur

	setAddrIp("127.0.0.1");
	setPassword(av2);
	
	// client.setAddrIp(servAddress);
	setFd(socket(AF_INET, SOCK_STREAM, 0));
	if (getFd() < 0)
	{
		perror("Error Socket: ");
		throw std::invalid_argument("");
	}

	// Attacher la socket au port
	if (setsockopt(getFd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
	{
		perror("Error setsockopt: ");
		throw std::invalid_argument("");
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(atoi(av.c_str()));

	if (bind(getFd(), (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("Error bind: ");
		throw std::invalid_argument("");
	}

	if (listen(getFd(), 3) < 0)
	{
		perror("Error listen: ");
		throw std::invalid_argument("");
	}

	// Tableau pour les descripteurs de fichiers surveillés par poll
	
	struct pollfd pollServ;
	pollServ.fd = getFd();
	pollServ.events = POLLIN;
	_lstPollFd.push_back(pollServ);
	bool init = false;
	while (true)
	{
		if (poll(&_lstPollFd[0], _lstPollFd.size(), -1) >= 0)
		{
			if (_lstPollFd[0].revents & POLLIN)
			{
				Clients newClient;
				socklen_t sock_info_len = sizeof(address);
   				newClient.setFd(accept(getFd(), (struct sockaddr*)&address, &sock_info_len));
				struct pollfd pollClienTmp;
				pollClienTmp.fd = newClient.getFd();
				pollClienTmp.events = POLLIN;
				_lstPollFd.push_back(pollClienTmp);
				_clients.insert(std::make_pair(newClient.getFd(), newClient));
			}
			for (size_t i = 1; i < _lstPollFd.size(); i++)
			{
				if (_lstPollFd[i].revents & POLLIN)
				{
					bzero(buffer, 256);
					std::map<int, Clients>::iterator itClients = getClients().find(_lstPollFd[i].fd);
					ssize_t bytes = recv(itClients->first, buffer, 255, MSG_DONTWAIT);// MSG_DONTWAIT
					std::cout << "buffer : |" << buffer << "| BYTES: " << bytes<< std::endl;
					if (bytes < 0)
						std::cerr << "ERROR rcve !" << std::endl;
					else if ( bytes == 0)
					{

						std::cout << "connexion closed " << std::endl;
						throw std::invalid_argument("tg");
					}
					// else
					// 	buffer[bytes] = '\0';
					if (startWith(buffer, "CAP LS 302") || !init)
					{
						init = itClients->second.initClients(buffer, *this);
						if (init)
						{
							std::cout << "init client" << std::endl;
							itClients->second.printInfo();
						}
					}
					else
					{
						cmdHandler(buffer, itClients->second);
						// if (startWith(buffer, "PING"))
						// 	Pong(buffer, itClients->second);
						itClients->second.printChannels();
					}
					itClients++;
				}
			}
		}
		else // ERROR DE POLL
		{
			throw std::invalid_argument("lol");
		}

	}
}


// CELUI DE NICO !!!!

//Server::Server( std::string av ) {
//
//    //creation du socket serv
//    Clients client;
//    setFd(socket(AF_INET, SOCK_STREAM, 0));
//    if (getFd() < 0) {
//        std::cerr << "error socket" << std::endl;
//    }
//    setAddrIp("127.0.0.1");
//    client.setAddrIp(getAddrIp());
//
//    //configuration d'une struct socketaddr_in pour avoir des appels systeme reseau comme bind et connect
//    struct sockaddr_in addr;
//    addr.sin_family = AF_INET;
//    addr.sin_port = htons(atoi(av.c_str()));  //parsing du port
//    addr.sin_addr.s_addr = inet_addr(getAddrIp().c_str());
//
//    //lie un socket a une adresse ip et un port
//    if (bind(getFd(), (struct sockaddr*)&addr, sizeof(addr)) < 0)
//	{
//        std::cerr << "Error liaison socket" << std::endl;
//		throw std::invalid_argument("PAS OUF bind");
//    }
//
//    // mode ecoute du socket
//    if (listen(getFd(), 5) == -1)
//		throw std::invalid_argument("PAS OUF listen");
//
//    socklen_t sock_info_len = sizeof(addr);
//    client.setFd(accept(getFd(), (struct sockaddr*)&addr, &sock_info_len));
//    // std::cout << "fd2 = " << getFd() << std::endl;
//
//    if(getFd() < 0)
//        std::cout << "Error : accept" << std::endl;
//    else
//        std::cout << "Accept " << std::endl;
//    char buffer[2048];
//    bool init = false;
//    while (true)
//    {
//        ssize_t bytes = recv(client.getFd(), buffer, 2048, 0);
//        if (bytes < 0)
//            std::cerr << "ERROR rcve !" << std::endl;
//        else if ( bytes == 0)
//            std::cout << "connexion closed " << std::endl;
//        else
//            buffer[bytes] = '\0';
//        if (startWith(buffer, "CAP LS 302") || !init)
//        {
//            init = client.initClients(buffer);
//            if (init)
//            {
//                std::cout << "init client" << std::endl;
//                client.printInfo();
//            }
//        }
//        else
//        {
//            cmdHandler(buffer, client);
//            if (startWith(buffer, "PING"))
//                Pong(buffer, client);
//            client.printChannels();
//        }
//        sleep(1);
//    }
//    close(getFd());
//}

Server::~Server()
{

}