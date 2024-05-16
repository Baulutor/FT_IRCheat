#include "Server.hpp"
#include "Cmd.hpp"

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

std::map<std::string, Channels>& Server::getChannels() {return (_channels);}

bool startWith(const std::string &line, const char *cmd)
{
    return (line.find(cmd) == 0);
}

void Server::cmdHandler(std::string cmd, Clients& client)
{
    const char *lstCmd[] = {"JOIN", "KICK"};
    // , "NAMES", "NICK", "INVITE", "TOPIC", "PRIVMSG", "QUIT", "PART", "KICK", "MODE"

    void (*lstFunc[])(std::string, Clients&, Server&) = {Join};
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
    std::string pong = "PONG " + cmd.substr(6) + "\r\n";
    std::cout << "pong : " << pong << std::endl;
    if (send(client.getFd(), pong.c_str(), pong.size(), 0) < 0)
        throw std::exception();
}

// void Server::setDataServer()
// {
    
// }

int my_poll(std::vector<PollFd>& pollFds) {
	const int nfds = pollFds.size();
	fd_set readSet;
	FD_ZERO(&readSet);

	int maxFd = -1;
	for (int i = 0; i < nfds; ++i) {
		FD_SET(pollFds[i].fd, &readSet);
		if (pollFds[i].fd > maxFd) {
			maxFd = pollFds[i].fd;
		}
	}

	struct timeval timeout = {0, 0}; // Pas de timeout
	int ret = select(maxFd + 1, &readSet, NULL, NULL, &timeout);
	if (ret == -1) {
		return -1; // Erreur
	}

	for (int i = 0; i < nfds; ++i) {
		if (FD_ISSET(pollFds[i].fd, &readSet)) {
			pollFds[i].revents = pollFds[i].events;
		} else {
			pollFds[i].revents = 0;
		}
	}

	return ret; // Nombre de descripteurs prêts
}


int Server::my_poll(std::vector<PollFd>& pollFds)
{
	const int nfds = pollFds.size();
	fd_set readSet;
	FD_ZERO(&readSet);

	int maxFd = -1;
	for (int i = 0; i < nfds; ++i) {
		FD_SET(pollFds[i].fd, &readSet);
		if (pollFds[i].fd > maxFd) {
			maxFd = pollFds[i].fd;
		}
	}

	struct timeval timeout = {0, 0}; // Pas de timeout
	int ret = select(maxFd + 1, &readSet, NULL, NULL, &timeout);
	if (ret == -1) {
		return -1; // Erreur
	}

	for (int i = 0; i < nfds; ++i) {
		if (FD_ISSET(pollFds[i].fd, &readSet)) {
			pollFds[i].revents = pollFds[i].events;
		} else {
			pollFds[i].revents = 0;
		}
	}

	return ret; // Nombre de descripteurs prêts
}

Server::Server(std::string av)
{
	int serverFd, clientFd;
	struct sockaddr_in serverAddr, clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);
	std::vector<PollFd> pollFds(1); // Créer un vecteur de descripteurs de fichiers à surveiller pour poll

	// Création du socket
	serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverFd < 0)
	{
		std::cerr << "Error creating socket" << std::endl;

	}

	// Configuration de l'adresse du serveur
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(atoi(av.c_str())); // Utilisation du port passé en argument

	// Liaison du socket
	if (bind(serverFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
		std::cerr << "Error binding socket" << std::endl;

	}

	// Mise en écoute
	if (listen(serverFd, 5) == -1) {
		std::cerr << "Error listening" << std::endl;
	}

	// Initialisation de la structure PollFd pour surveiller le socket d'écoute
	pollFds[0].fd = serverFd;
	pollFds[0].events = POLLIN; // Surveiller les événements d'entrée

	// Boucle principale pour accepter les nouvelles connexions et gérer les clients
	while (true) {
		int ret = my_poll(pollFds); // Attendre les événements

		if (ret == -1) {
			std::cerr << "Error in poll" << std::endl;
			break;
		}

		if (pollFds[0].revents & POLLIN)
		{ // Nouvelle connexion sur le socket d'écoute
			// Accepter la nouvelle connexion
			clientFd = accept(serverFd, (struct sockaddr*)&clientAddr, &clientAddrLen);
			if (clientFd < 0) {
				std::cerr << "Error accepting connection" << std::endl;
				continue;
			}
			std::cout << "Accepted connection" << std::endl;

			// Ajouter le nouveau descripteur de fichier à surveiller
			PollFd newClientFd;
			newClientFd.fd = clientFd;
			newClientFd.events = POLLIN;
			pollFds.push_back(newClientFd);
		}

		// Traitement des événements pour les clients existants
		for (size_t i = 1; i < pollFds.size(); ++i) {
			if (pollFds[i].revents & POLLIN) {
				// Gérer la lecture depuis le client
				// Exemple : ssize_t bytes = recv(pollFds[i].fd, buffer, sizeof(buffer), 0);
				// Traitement des données lues...
			}
		}
	}

	// Fermeture des sockets
	close(serverFd);
	for (size_t i = 1; i < pollFds.size(); ++i) {
		close(pollFds[i].fd);
	}

}

//Server::Server( std::string av )
//{
//
//	// je dois changer tout ca lol ! en gros tout doit rentrer dans le while true ??
//
//
//    //creation du socket serv
//
//
//
//    Clients client;
//    setFd(socket(AF_INET, SOCK_STREAM, 0));
//    if (getFd() < 0) {
//        std::cerr << "error socket" << std::endl;
//    }
//    std::cout << "fd = " << getFd() << std::endl;
//    setAddrIp("127.0.0.1");
//    client.setAddrIp(getAddrIp());
//
//    //configuration d'une struct socketaddr_in pour avoir des appels systeme reseau comme bind et connect
//	struct sockaddr_in addr;
//    addr.sin_family = AF_INET;
//    addr.sin_port = htons(atoi(av.c_str()));  //parsing du port
//    addr.sin_addr.s_addr = inet_addr(getAddrIp().c_str());
//
//    //lie un socket a une adresse ip et un port
//    if (bind(getFd(), (struct sockaddr*)&addr, sizeof(addr)) < 0) {
//        std::cerr << "Error liaison socket" << std::endl;
//    }
//
//    // mode ecoute du socket
//    if (listen(getFd(), 5) == -1)
//        std::cerr << "Error listen" << std::endl;
//
//    socklen_t sock_info_len = sizeof(addr);
//    client.setFd(accept(getFd(), (struct sockaddr*)&addr, &sock_info_len));
//    // std::cout << "fd2 = " << getFd() << std::endl;
//
//    if(getFd() < 0)
//        std::cout << "Error : accept" << std::endl;
//    else
//        std::cout << "Accept " << std::endl;
//
//    char buffer[2048];
//    bool init = false;
//
//struct pollfd fds[1];
//	fds[0].fd = _fd;
//	fds[0].events = POLLIN;
//
//	std::cout << fds[0].fd << std::endl;
//
//    while (true)
//    {
//		std::cout << "test" << std::endl;
//		int ret = poll(fds, 0, 1500);
//		std::cout << "22222" << std::endl;
//		socklen_t clientLen;
//		int clientSocket;
//
//		if (ret == -1)
//		{
//			perror("Error: ");
//			exit(EXIT_FAILURE);
//		}
//		std::cout << "33333333333" << std::endl;
//		if (fds[0].revents & POLLOUT) // ne rentre jamais dans cette boucle apres norml je sais pas comment faire un autre user donc je peux pas savoir si ca maecge ou pas
//		{ // Nouvelle connexion sur le socket serveur
//			if ((clientSocket = accept(_fd, (struct sockaddr *) &_addrIp, &clientLen)) == -1)
//			{
//				perror("Erreur lors de l'acceptation");
//				continue;
//			}
//			Clients clients;
//			setFd(socket(AF_INET, SOCK_STREAM, 0));
//			if (getFd() < 0) {
//				std::cerr << "error socket" << std::endl;
//			}
//			std::cout << "fd = " << getFd() << std::endl;
//			setAddrIp("127.0.0.1");
//			clients.setAddrIp(getAddrIp());
//
//			//configuration d'une struct socketaddr_in pour avoir des appels systeme reseau comme bind et connect
//			struct sockaddr_in addr;
//			addr.sin_family = AF_INET;
//			addr.sin_port = htons(atoi(av.c_str()));  //parsing du port
//			addr.sin_addr.s_addr = inet_addr(getAddrIp().c_str());
//
//			//lie un socket a une adresse ip et un port
//			if (bind(getFd(), (struct sockaddr*)&addr, sizeof(addr)) < 0) {
//				std::cerr << "Error liaison socket" << std::endl;
//			}
//
//			// mode ecoute du socket
//			if (listen(getFd(), 5) == -1)
//				std::cerr << "Error listen" << std::endl;
//
//			socklen_t sock_info_len = sizeof(addr);
//			clients.setFd(accept(getFd(), (struct sockaddr*)&addr, &sock_info_len));
//			// std::cout << "fd2 = " << getFd() << std::endl;
//
//			if(getFd() < 0)
//				std::cout << "Error : accept" << std::endl;
//			else
//				std::cout << "Accept " << std::endl;
//
//			std::cout << "test la team: " << clientSocket << std::endl;
//		}
//		std::cout << "444444444" << std::endl;
//		ssize_t bytes = recv(client.getFd(), buffer, 2048, 0);
//		std::cout << "CHELOU " << std::endl;
//        if (bytes < 0)
//            std::cerr << "ERROR rcve !" << std::endl;
//        else if ( bytes == 0)
//            std::cout << "connexion closed " << std::endl;
//        else
//            buffer[bytes] = '\0';
//        std::cout << "buffer : |" << buffer << "|" << std::endl;
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