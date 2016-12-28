#include "server.h"

SLOWS::SLOWS() {}

void SLOWS::Listen(const unsigned short int port) {
    Port = port;

    struct sockaddr_in sockAddr;
    int socketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socketFD == -1) {
        perror("Socket create Error");
        exit(EXIT_FAILURE);
    }
    memset(&sockAddr, 0, sizeof(sockAddr));
    sockAddr.sin_family = PF_INET;
    sockAddr.sin_port = htons(Port);
    sockAddr.sin_addr.s_addr = INADDR_ANY; // htonl(INADDR_ANY);

    if (bind(socketFD, (struct sockaddr *)&sockAddr, sizeof(sockAddr)) == -1) {
        perror("Socket bind Error");
        close(socketFD);
        exit(EXIT_FAILURE);
    }

    if (listen(socketFD, SOMAXCONN) == -1) {
        perror("Socket listen Error");
        close(socketFD);
        exit(EXIT_FAILURE);
    }

    for (;;) {
        int clientSocketFD = accept(socketFD, 0, 0);

        if (clientSocketFD < 0) {
            perror("Socket accept Error");
            close(socketFD);
            exit(EXIT_FAILURE);
        }
        printf("Someone connected.\n");
        close(clientSocketFD);
    }
}
void SLOWS::Static(std::string path) {
    HomeDir = path;
}

SLOWS::~SLOWS() {}
