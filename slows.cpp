#include "server.h"

int main(int argc, char const *argv[]) {
    SLOWS server;
    server.Listen(8080);
    return 0;
}
