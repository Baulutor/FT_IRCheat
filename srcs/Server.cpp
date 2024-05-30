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
    const char *lstCmd[] = {"JOIN", "KICK", "PRIVMSG", "PING", "INVITE", "MODE", "TOPIC", "NICK", "USER"};
    // , "NAMES", "NICK", "INVITE", "TOPIC", "PRIVMSG", "QUIT", "PART", "KICK", "MODE"
    void (*lstFunc[])(std::string, Clients&, Server&) = {Join, Kick, Privmsg, Pong, Invite, Mode, Topic, Nick, User};
    for (int i = 0; i < 9; i++)
    {
        if (startWith(cmd, lstCmd[i]))
        {
            lstFunc[i](cmd, client, *this);
            return;
        }
    }
}

void	Server::serverHandler(std::string av, std::string av2)
{
	struct sockaddr_in address;
	int opt = 1;

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
	int newClientFd = 0;
	bool init = false;
	while (true)
	{
		if (poll(&_lstPollFd[0], _lstPollFd.size(), -1) >= 0)
		{
			if (_lstPollFd[0].revents & POLLIN)
			{
				Clients newClient;
				socklen_t sock_info_len = sizeof(address);
				struct pollfd pollClienTmp;
				newClientFd = accept(getFd(), (struct sockaddr*)&address, &sock_info_len);
				// test
				std::cerr << "New connexion detected" << std::endl;
				// Clients newClient;
				newClient.setFd(newClientFd);
				std::cout << "newClient.getFd() = " << newClient.getFd() << std::endl;
				_clients.insert(std::make_pair(newClient.getFd(), newClient));
				// test
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
					// bzero(buffer, 512);
					std::map<int, Clients>::iterator itClients = getClients().find(_lstPollFd[i].fd);
					bzero(itClients->second.getBuffer(), 512);
					ssize_t bytes = recv(_lstPollFd[i].fd, itClients->second.getBuffer(), 511, MSG_DONTWAIT);
					if (bytes < 0)
						std::cerr << "ERROR rcve !" << std::endl;
					else if (bytes == 0)
					{
						std::cout << "connexion closed " << std::endl;
						// throw std::invalid_argument("tg");
					}
					else if (itClients->second.getBuffer()[strlen(itClients->second.getBuffer())] == '\n')
					{
						std::cout << "In the else if check, itClients->second.getBuffer() = " << itClients->second.getBuffer() << std::endl;
						itClients->second.setBufferTmp(itClients->second.getBuffer());
						itClients->second.setBuffer(NULL);
						continue ;
					}
					else if (strlen(itClients->second.getBufferTmp()) > 0)
					{
						std::cout << "In the else check, itClients->second.getBuffer() = |" << itClients->second.getBuffer() << "|" << std::endl;
						std::cout << "In the else check, itClients->second.getBufferTmp() = |" << itClients->second.getBufferTmp() << "|" << std::endl;
						itClients->second.setBuffer(itClients->second.getBufferTmp());
						itClients->second.setBufferTmp(NULL);
					}
					if (startWith(itClients->second.getBuffer(), "CAP LS 302") || !init)
					{
						init = itClients->second.initClients(itClients->second.getBuffer(), *this);
						if (init && itClients->second.getIsRegistered() == true)
						{
							std::cout << "init client" << std::endl;
						}
						else if (init && itClients->second.getIsRegistered() == false)
						{
							std::map<int, Clients>::iterator itNext = itClients;
							++itNext;
							_clients.erase(itClients);
							itClients = itNext;

						}
					}
					else
					{
						cmdHandler(itClients->second.getBuffer(), itClients->second);
					}
					itClients++;
				}
				i++;
			}
		}
		else
			throw std::invalid_argument("lol");
		sleep(1);
	}
}

Server::~Server()
{

}