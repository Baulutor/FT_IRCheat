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

std::map<std::string, Clients>& Server::getClients() {return (_clients);}

void Server::setChannels(std::map<std::string, Channels> channels) {this->_channels = channels;}

void Server::setClient(std::map<std::string, Clients> clients) {this->_clients = clients;}

bool startWith(const std::string &line, const char *cmd)
{
    return (line.find(cmd) == 0);
}

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
    std::string pong = "PONG :" + splited[1];
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
//
//
//Server::Server(std::string av)
//{
//	int serverFd;
//	int	clientFd;
//	struct sockaddr_in serverAddr, clientAddr;
//	socklen_t clientAddrLen = sizeof(clientAddr);
//	std::vector<PollFd> pollFds(5); // Créer un vecteur de descripteurs de fichiers à surveiller pour poll
//
//	// Création du socket
//	serverFd = socket(AF_INET, SOCK_STREAM, 0);
//	if (serverFd < 0) {
//		std::cerr << "Error creating socket" << std::endl;
//		throw (std::invalid_argument("Pas ouf la relou"));
//	}
//
//	// Configuration de l'adresse du serveur
//	serverAddr.sin_family = AF_INET;
//	serverAddr.sin_addr.s_addr = INADDR_ANY;
//	serverAddr.sin_port = htons(atoi(av.c_str())); // Utilisation du port passé en argument
//
////	std::cout << "Valeur de server Addr: " <<  serverAddr.sin_zero << " valeur de size of addr: " << sizeof(serverAddr) << " valeur de serverFd: " << serverFd << std::endl;
//	// Liaison du socket
//	if (bind(serverFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
//	{
//		std::cerr << "Error binding socket" << std::endl;
//		close(serverFd);
//		for (size_t i = 1; i < pollFds.size(); ++i)
//			close(pollFds[i].fd);
//		throw (std::invalid_argument("Pas ouf la relou bind"));
//	}
//
//	// Mise en écoute
//	if (listen(serverFd, 5) == -1) {
//		std::cerr << "Error listening" << std::endl;
//		throw (std::invalid_argument("Pas ouf la relou listen"));
//	}
//
//	// Initialisation de la structure PollFd pour surveiller le socket d'écoute
//	pollFds[0].fd = serverFd;
//	pollFds[0].events = POLLIN; // Surveiller les événements d'entrée
//
//	// Boucle principale pour accepter les nouvelles connexions et gérer les clients
//	while (true) {
//		int ret = my_poll(pollFds); // Attendre les événements
//
//		if (ret == -1) {
//			std::cerr << "Error in poll" << std::endl;
//			break;
//		}
//
//		if (pollFds[0].revents & POLLIN) { // Nouvelle connexion sur le socket d'écoute
//			// Accepter la nouvelle connexion
//			clientFd = accept(serverFd, (struct sockaddr*)&clientAddr, &clientAddrLen);
//			if (clientFd < 0) {
//				std::cerr << "Error accepting connection" << std::endl;
//				continue;
//			}
//			std::cout << "Accepted connection" << std::endl;
//
//			// Ajouter le nouveau descripteur de fichier à surveiller
//			PollFd newClientFd;
//			newClientFd.fd = clientFd;
//			newClientFd.events = POLLIN;
//			pollFds.push_back(newClientFd);
//		}
//
//		// Traitement des événements pour les clients existants
//		for (size_t i = 1; i < pollFds.size(); ++i) {
//			if (pollFds[i].revents & POLLIN) {
//				// Gérer la lecture depuis le client
//				// Exemple : ssize_t bytes = recv(pollFds[i].fd, buffer, sizeof(buffer), 0);
//				// Traitement des données lues...
//			}
//		}
//	}
//
//	// Fermeture des sockets
//	close(serverFd);
//	for (size_t i = 1; i < pollFds.size(); ++i) {
//		close(pollFds[i].fd);
//	}
//}

// ANCIEN en BAS

void broadcast_message(const std::string &message, int sender_fd, std::vector<pollfd> &clients)
{
	for (size_t i = 1; i < clients.size(); i++)
	{
		if (clients[i].fd != sender_fd)
			send(clients[i].fd, message.c_str(), message.size(), 0);
	}
}


Server::Server(std::string av)
{
	Clients client;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[2048];

	// Création de la socket serveur
	setAddrIp("127.0.0.1");
	client.setAddrIp(getAddrIp());
	setFd(socket(AF_INET, SOCK_STREAM, 0));
	if (getFd() < 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Attacher la socket au port
	if (setsockopt(getFd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(atoi(av.c_str()));

	if (bind(getFd(), (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	if (listen(getFd(), 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	// Tableau pour les descripteurs de fichiers surveillés par poll
	std::vector<pollfd> vecClient;
	pollfd server_pollfd = { getFd(), POLLIN, 0 };
	vecClient.push_back(server_pollfd);


	while (true)
	{
		int poll_count = poll(&vecClient[0], vecClient.size(), -1);

		if (poll_count == -1)
		{
			perror("poll");
			exit(EXIT_FAILURE);
		}

		// Vérifier les événements sur chaque descripteur
		for (size_t i = 0; i < vecClient.size(); i++)
		{
			if (vecClient[i].revents & POLLIN)
			{
				std::cout << vecClient[i].revents << ": vecClient[i].revents" << std::endl;
				if (vecClient[i].fd == getFd())
				{
					// Nouvelle connexion entrante
					client.setFd(accept(vecClient[i].fd, (struct sockaddr *)&address, (socklen_t*)&addrlen));
					if (getFd() < 0)
					{
						perror("accept");
						exit(EXIT_FAILURE);
					}
					pollfd new_client = { client.getFd(), POLLIN, 0 };
					vecClient.push_back(new_client);
					std::cout << "New connection, socket fd is " << client.getFd() << std::endl;
				}
				else
				{
					// Message entrant d'un client existant
					std::cout << "dans LE ELSE" << std::endl;
					ssize_t valread = recv(vecClient[i].fd, buffer, 2048, 0);
					if (valread < 0)
					{
						std::cout << "CHELOU!!!!" << std::endl;
					}
					else if (valread == 0)
					{
						// Déconnexion du client
						std::cout << "Client disconnected, socket fd is " << vecClient[i].fd << std::endl;
						close(vecClient[i].fd);
						vecClient.erase(vecClient.begin() + i);
						--i;
					}
					else
					{
						buffer[valread] = '\0';
						std::string message(buffer);
						if (client.initClients(buffer))
							client.printInfo();
						std::cout << "Received message: " << message << std::endl;
						broadcast_message(message, vecClient[i].fd, vecClient);
						cmdHandler(buffer, client);
						if (startWith(buffer, "PING"))
							Pong(buffer, client);
						client.printChannels();
					}
				}
			}
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