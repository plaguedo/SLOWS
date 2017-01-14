#include "server.h"
#include "stdio.h"
#include <string>
#include <unistd.h>

int main(int argc, char const *argv[]) {
    SLOWS server;
    unsigned short port = 0;
    if (argc == 2) {
        port = atoi(argv[1]);
        server.Listen(port);
    } else if (argc == 3) {
        port = atoi(argv[1]);
        std::string mode = argv[2];
        if (mode == "D" || mode == "daemon")
            daemon(0, 1);
        server.Listen(port);
    }
    if (port == 0) {
        server.Listen();
    }
    return 0;
}
