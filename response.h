#ifndef SLOWS_RESPONSE
#define SLOWS_RESPONSE

#include <map>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

class SLOWSRes {
  private:
    short Status;
    std::map<std::string, std::string> Headers;

  public:
    SLOWSRes();
    void Send(int clientSocket, short status = 200);
    virtual ~SLOWSRes();
};

#endif
