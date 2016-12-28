#include "server.h"

int main(int argc, char const *argv[]) {
    SLOWS server;
    server.Listen();
    return 0;
}
