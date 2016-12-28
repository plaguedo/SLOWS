#ifndef SLOWS_H
#define SLOWS_H

#include "request.h"
#include "response.h"
#include <iostream> //DEL
#include <netinet/in.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>

#define METHOD_CHAR_CAP 128
#define URI_CHAR_CAP 8192
#define PROTOCOL_CHAR_CAP 128
#define LEFT_HEADER_CHAR_CAP 8192
#define RIGHT_HEADER_CHAR_CAP 8192

class SLOWS {
  private:
    unsigned short int Port;
    int SocketFD;
    std::string HomeDir;

  public:
    SLOWS();

    void Listen(const unsigned short int port = 8080);
    void Static(std::string path = "/var/www");
    void Prepare(int clientSocket);
    void ParseHTTP();
    void BreakConnection(int clientSocket, short status);

    virtual ~SLOWS();
};

#endif
