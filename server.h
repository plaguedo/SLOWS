#ifndef SLOWS_H
#define SLOWS_H

#include "request.h"
#include "response.h"
#include <ctime>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <netinet/in.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <thread>
#include <vector>

#define METHOD_CHAR_CAP 128
#define URI_CHAR_CAP 8192
#define PROTOCOL_CHAR_CAP 128
#define LEFT_HEADER_CHAR_CAP 8192
#define RIGHT_HEADER_CHAR_CAP 8192

class SLOWS {
  private:
    unsigned short int Port;
    int SocketFD;
    std::string HomeDir = "/var/www/.SLOWS";
    std::string IndexFile = "index.html";
    std::vector<std::string> AllowedMethods;
    bool Logging = true;

  public:
    SLOWS();

    void Listen(const unsigned short int port = 8080);
    void Static(std::string path = "~/.SLOWS");
    void Prepare(int clientSocket);
    void BreakConnection(int clientSocket, short status,
                         std::stringstream responseBody = std::stringstream());
    void MethodeController(int clientSocket, SLOWSReq *req);

    void MethodeGet(int clientSocket, SLOWSReq *req, SLOWSRes *res);
    void MethodeHead(int clientSocket, SLOWSReq *req, SLOWSRes *res);

    void Logger(std::string msg);

    virtual ~SLOWS();
};

#endif
