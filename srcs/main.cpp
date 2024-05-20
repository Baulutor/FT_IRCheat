#include "Clients.hpp"
#include "Server.hpp"

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "ERROR arg" << std::endl;
        return -1;
    }
    Server server(argv[1], argv[2]);
    return 0;
}

