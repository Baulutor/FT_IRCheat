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

bool startWith(const std::string &line, const char *cmd)
{
    return (line.find(cmd) == 0);
}

void cmdHandler(std::string cmd, Clients& client, std::map<std::string, Channels>& channels)
{
    const char *lstCmd[] = {"JOIN"};
    // , "NAMES", "NICK", "INVITE", "TOPIC", "PRIVMSG", "QUIT", "PART", "KICK", "MODE"
    void (*lstFunc[])(std::string, Clients&, std::map<std::string, Channels>&) = {Join};
    // NAMES, NICK, INVITE, TOPIC, PRIVMSG, QUIT, PART, KICK, MODE
    for (int i = 0; i < 1; i++)
    {
        if (startWith(cmd, lstCmd[i]))
        {
            lstFunc[i](cmd, client, channels);
            return;
        }
    }
    // if (startWith(cmd, "JOIN"))
    // {
    //     std::cout << "JOIN" << std::endl;
    //     std::string reponse = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getAddrIp() + " JOIN :" + cmd.substr(6, cmd.find(' ', 6) - 6);
    //     if (send(client.getFd(), reponse.c_str(), reponse.size(), 0) == -1)
    //         throw (SendErrorExeption());
    // }
    // else if (startWith(cmd, "NAMES"))
    //     std::cout << "NAMES" << std::endl;
    // else if (startWith(cmd, "NICK"))
    //     std::cout << "NICK" << std::endl;
    // else if (startWith(cmd, "INVITE"))
    //     std::cout << "INVITE" << std::endl;
    // else if (startWith(cmd, "TOPIC"))
    //     std::cout << "TOPIC" << std::endl;
    // else if (startWith(cmd, "PRIVMSG"))
    //     std::cout << "PRIVMSG" << std::endl;
    // else if (startWith(cmd, "QUIT"))
    //     std::cout << "QUIT" << std::endl;
    // else if (startWith(cmd, "PART"))
    //     std::cout << "PART" << std::endl;
    // else if (startWith(cmd, "KICK"))
    //     std::cout << "KICK" << std::endl;
    // else if (startWith(cmd, "MODE"))
    //     std::cout << "MODE" << std::endl;
    // else
    //     std::cout << "UNKNOWN" << std::endl;
}

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
        // std::string command = "CAP LS 302";
        if (startWith(buffer, "CAP LS 302") || !init)
        {
            init = client.initClients(buffer);
            // continue ;
        }
        if (init)
        {
            client.printInfo();
            cmdHandler(buffer, client, _channels);
            // init = false;
        }
        sleep(1);
    }
    close(getFd());
}