#ifndef SLOWS_H
#define SLOWS_H

#include "request.h"
#include "response.h"
#include <netinet/in.h>

#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>

class SLOWS {
  private:
    unsigned short int Port;
    std::string HomeDir;

  public:
    SLOWS();

    void Listen(const unsigned short int port = 80);
    void Static(std::string path = "/var/www");

    virtual ~SLOWS();
};

#endif
