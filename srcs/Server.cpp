#include "Server.hpp"
#include "Cmd.hpp"

Server::Server(){}

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

std::vector<pollfd> Server::getLstPollFd() {return (this->_lstPollFd);}

bool startWith(const std::string &line, const char *cmd) {return (line.find(cmd) == 0);}

void Server::cmdHandler(std::string cmd, Clients& client)
{
	// std::string::size_type pos = cmd.find("\r\n");
	// if (pos != std::string::npos)
	// {
	// 	cmd = cmd.erase(pos);
	// }
	// std::cout << "cmd char: " << cmd[cmd.length()] + 48 << std::endl;
	std::cout << "cmd: " << cmd << std::endl;
    const char *lstCmd[] = {"JOIN", "KICK", "PRIVMSG", "PING", "INVITE"};
    // , "NAMES", "NICK", "INVITE", "TOPIC", "PRIVMSG", "QUIT", "PART", "KICK", "MODE"
    void (*lstFunc[])(std::string, Clients&, Server&) = {Join, Kick, Privmsg, Pong, Invite};
    for (int i = 0; i < 5; i++)
    {
        if (startWith(cmd, lstCmd[i]))
        {
            lstFunc[i](cmd, client, *this);
            return;
        }
    }
}


std::string intToString(int value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

void signalHandler(int signum)
{
	(void)signum; // a differencier ctrl C et ctrl D
	throw std::invalid_argument("CHELOU j'ai bien envoyer lol");
}

void	Server::serverHandler(std::string av, std::string av2)
{
	struct sockaddr_in address;
	int opt = 1;
	char buffer[512];

	// Création de la socket serveur

	setAddrIp("127.0.0.1");
	setPassword(av2);
	_fd = -1;

	setFd(socket(AF_INET, SOCK_STREAM, 0));
	if (getFd() < 0)
	{
		perror("Error Socket");
		throw std::invalid_argument("");
	}
	signal(SIGINT, signalHandler);

	if (setsockopt(getFd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
	{
		perror("Error setsockopt");
		close(_fd);
		throw std::invalid_argument("");
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(atoi(av.c_str()));

	if (bind(getFd(), (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("Error bind");
		close(_fd);
		throw std::invalid_argument("");
	}

	if (listen(getFd(), 3) < 0)
	{
		perror("Error listen: ");
		close(_fd);
		throw std::invalid_argument("");
	}

	struct pollfd pollServ;
	pollServ.fd = getFd();
	pollServ.events = POLLIN;
	pollServ.revents = 0;
	_lstPollFd.push_back(pollServ);
	bool init = false;
	int newClientFd = 0;
	while (true)
	{
		if (poll(&_lstPollFd[0], _lstPollFd.size(), -1) >= 0)
		{
			if (_lstPollFd[0].revents & POLLIN)
			{
				socklen_t sock_info_len = sizeof(address);
				struct pollfd pollClienTmp;
				newClientFd = accept(getFd(), (struct sockaddr*)&address, &sock_info_len);
   				pollClienTmp.fd = newClientFd;
				pollClienTmp.events = POLLIN;
				pollClienTmp.revents = 0;
				_lstPollFd.push_back(pollClienTmp);
			}
			size_t i = 1;
			for (std::vector<pollfd>::iterator it = _lstPollFd.begin() + 1; it < _lstPollFd.end(); it++)
			{
				if (_lstPollFd[i].revents & POLLIN)
				{
					bzero(buffer, 512);
					std::map<int, Clients>::iterator itClients = getClients().find(_lstPollFd[i].fd);
					ssize_t bytes = recv(_lstPollFd[i].fd, buffer, 511, MSG_DONTWAIT);
					if (bytes < 0)
						std::cerr << "ERROR rcve !" << std::endl;
					else if (bytes == 0)
					{
						std::cout << "connexion closed " << std::endl;
						std::map<int, Clients> buf = getClients();
//						if (buf.size() == 1)
//							throw std::invalid_argument("IL SE PASSE DES BAILS LA"); // dans le cas ou il y a plus de client dans le server meme si je pense qu'il faut que ca continu
						buf.erase(itClients);
						--itClients;
						--i;
//						throw std::invalid_argument("tg");
					}
					if (startWith(buffer, "CAP LS 302") || !init)
					{
						Clients newClient;
						newClient.setFd(_lstPollFd[i].fd);
						init = newClient.initClients(buffer, *this);
						if (init)
						{
							std::cout << "init client" << std::endl;
							newClient.printInfo();
							_clients.insert(std::make_pair(newClient.getFd(), newClient));
						}
						// else {
						// 	std::cout << "cacaboudin = " << itClients->first << std::endl;
						// 	// _clients.erase(it);
						// }
					}
					else
					{
						cmdHandler(buffer, itClients->second);
						// itClients->second.printChannels();
					}
					itClients++;
				}
				i++;
			}
		}
		else
			throw std::invalid_argument("lol");
	}
}

Server::~Server()
{

}