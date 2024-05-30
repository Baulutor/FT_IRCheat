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
	std::cout << "cmd: " << cmd << std::endl;
    const char *lstCmd[] = {"JOIN", "KICK", "PRIVMSG", "PING", "INVITE", "MODE", "TOPIC", "QUIT"};
    // , "NAMES", "NICK", "INVITE", "TOPIC", "PRIVMSG", "QUIT", "PART", "KICK", "MODE"
    void (*lstFunc[])(std::string, Clients&, Server&) = {Join, Kick, Privmsg, Pong, Invite, Mode, Topic, Quit};
    for (int i = 0; i < 8; i++)
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

void Server::launch(std::string av, std::string av2)
{
	int opt = 1;

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

	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = INADDR_ANY;
	_address.sin_port = htons(atoi(av.c_str()));

	if (bind(getFd(), (struct sockaddr *)&_address, sizeof(_address)) < 0)
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
	serverHandler();
}

bool Server::ClientConnexion()
{
	int newClientFd = 0;
	std::cout << "New connexion detected" << std::endl;
	Clients newClient;
	socklen_t sock_info_len = sizeof(_address);
	struct pollfd pollClienTmp;
	newClientFd = accept(getFd(), (struct sockaddr*)&_address, &sock_info_len);
	newClient.setFd(newClientFd);
	_clients.insert(std::make_pair(newClient.getFd(), newClient));
	pollClienTmp.fd = newClientFd;
	pollClienTmp.events = POLLIN;
	pollClienTmp.revents = 0;
	_lstPollFd.push_back(pollClienTmp);
	return (true);
}

void Server::ClientHandler(bool init)
{
	size_t i = 1;
	while (i < _lstPollFd.size())
	{
		if (_lstPollFd[i].revents & POLLIN)
		{
			std::map<int, Clients>::iterator itClients = getClients().find(_lstPollFd[i].fd);
			if (itClients == getClients().end())
			{
				i++;
				continue;
			}

			bzero(itClients->second.getBuffer(), 512);
			ssize_t bytes = recv(_lstPollFd[i].fd, itClients->second.getBuffer(), 511, MSG_DONTWAIT);
			if (bytes < 0)
			{
				std::cerr << "ERROR rcve !" << std::endl;
			}
			else if (bytes == 0)
			{
				std::cout << "connexion closed " << std::endl;
				close(itClients->second.getFd());
				close(_lstPollFd[i].fd);
				_clients.erase(itClients->first);
				_lstPollFd.erase(_lstPollFd.begin() + i);
				continue;
			}
			else if (itClients->second.getBuffer()[strlen(itClients->second.getBuffer())] == '\n')
			{
				itClients->second.setBufferTmp(itClients->second.getBuffer());
				itClients->second.setBuffer(NULL);
				continue ;
			}
			else if (strlen(itClients->second.getBufferTmp()) > 0)
			{
				itClients->second.setBuffer(itClients->second.getBufferTmp());
				itClients->second.setBufferTmp(NULL);
			}

			std::cout << "buffer = " << itClients->second.getBuffer() << std::endl;
			if (startWith(itClients->second.getBuffer(), "CAP LS 302") || !init)
			{
				std::cout << "init 1 = " << (init ? "true" : "false") << std::endl;
				init = itClients->second.initClients(itClients->second.getBuffer(), *this);
				std::cout << "init 2 = " << (init ? "true" : "false") << std::endl;
				std::cout << "isRegistered = " << (itClients->second.getIsRegistered() ? "true" : "false") << std::endl;
				if (init && itClients->second.getIsRegistered() == true)
				{
					std::cout << "init client" << std::endl;
					itClients->second.printInfo();
				}
				else if (init && itClients->second.getIsRegistered() == false)
				{
					std::cout << "client not registered" << std::endl;
					sendCmd("ERROR :Deconnexion", itClients->second);
					close(itClients->second.getFd());
					close(_lstPollFd[i].fd);
					std::cout << "close client fd = " << itClients->second.getFd() << std::endl;
					std::cout << "close poll fd = " << _lstPollFd[i].fd << std::endl;

					std::map<int, Clients>::iterator itNext = itClients;
					++itNext;
					_clients.erase(itClients->first);
					itClients = itNext;
					// std::vector<pollfd>::iterator pollNext = it;
					// ++pollNext;
					_lstPollFd.erase(_lstPollFd.begin() + i);
					// it = pollNext;
					i--;
					std::cout << "init 3 = " << (init ? "true" : "false") << std::endl;
					if (itClients == _clients.end())
					{
						break;
					}
				}
			}
			else
			{
				cmdHandler(itClients->second.getBuffer(), itClients->second);
			}
		}
		i++;
	}
}

void	Server::serverHandler()
{


	struct pollfd pollServ;
	pollServ.fd = getFd();
	pollServ.events = POLLIN;
	pollServ.revents = 0;
	_lstPollFd.push_back(pollServ);
	bool init = false;
	while (true)
	{
		if (poll(&_lstPollFd[0], _lstPollFd.size(), -1) >= 0)
		{
			if (_lstPollFd[0].revents & POLLIN)
				init = ClientConnexion();
			else
				ClientHandler(init);
		}
		else
			throw std::invalid_argument("lol");
		sleep(1);
	}
}

Server::~Server()
{

}