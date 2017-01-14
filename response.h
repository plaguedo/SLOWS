#ifndef SLOWS_RESPONSE
#define SLOWS_RESPONSE

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

class SLOWSRes {
  private:
    short Status = 200;
    std::string StatusDescription = "OK";
    std::map<std::string, std::string> Headers;

  public:
    SLOWSRes();
    void Send(int clientSocket);
    void Send(int clientSocket, std::string body);
    void PushHeader(std::string name, std::string value);
    void SetStatus(short status);
    virtual ~SLOWSRes();
};

#endif
