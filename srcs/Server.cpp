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

void Server::setChannels(std::map<std::string, Channels> channels) {this->_channels = channels;}

void Server::setClient(std::map<std::string, Clients> clients) {this->_clients = clients;}

bool startWith(const std::string &line, const char *cmd)
{
    return (line.find(cmd) == 0);
}

void Server::cmdHandler(std::string cmd, Clients& client)
{
    const char *lstCmd[] = {"JOIN"};
    // , "NAMES", "NICK", "INVITE", "TOPIC", "PRIVMSG", "QUIT", "PART", "KICK", "MODE"
    void (*lstFunc[])(std::string, Clients&, Server&) = {Join};
    // NAMES, NICK, INVITE, TOPIC, PRIVMSG, QUIT, PART, KICK, MODE
    for (int i = 0; i < 1; i++)
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

Server::Server( std::string av ) {

    //creation du socket serv
    Clients client;
    setFd(socket(AF_INET, SOCK_STREAM, 0));
    if (getFd() < 0) {
        std::cerr << "error socket" << std::endl;
    }
    std::cout << "fd = " << getFd() << std::endl;
    setAddrIp("127.0.0.1");
    client.setAddrIp(getAddrIp());

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
    client.setFd(accept(getFd(), (struct sockaddr*)&addr, &sock_info_len));
    // std::cout << "fd2 = " << getFd() << std::endl;
    
    if(getFd() < 0)
        std::cout << "Error : accept" << std::endl;
    else
        std::cout << "Accept " << std::endl;
    char buffer[2048];
    bool init = false;
    while (true)
    {
        ssize_t bytes = recv(client.getFd(), buffer, 2048, 0);
        if (bytes < 0)
            std::cerr << "ERROR rcve !" << std::endl;
        else if ( bytes == 0)
            std::cout << "connexion closed " << std::endl;
        else
            buffer[bytes] = '\0';
        std::cout << "buffer : |" << buffer << "|" << std::endl;
        if (startWith(buffer, "CAP LS 302") || !init)
        {
            init = client.initClients(buffer);
            if (init)
            {
                std::cout << "init client" << std::endl;
                client.printInfo();
            }
        }
        else
        {
            cmdHandler(buffer, client);
            if (startWith(buffer, "PING"))
                Pong(buffer, client);
            client.printChannels();
        }
        sleep(1);
    }
    close(getFd());
}