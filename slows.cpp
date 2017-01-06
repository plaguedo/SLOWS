#include "server.h"
#include "stdio.h"

int main(int argc, char const *argv[]) {
    // port from console
    SLOWS server;
    unsigned short port = 0;
    if (argc == 2) {
        port = atoi(argv[1]);
        server.Listen(port);
    }
    if (port == 0) {
        server.Listen();
    }
    return 0;
}
