#ifndef SLOWS_REQUEST
#define SLOWS_REQUEST

#include <string>

class SLOWSReq {
  private:
    std::string Body = "";
    std::string Url = "/";

  public:
    SLOWSReq();
    virtual ~SLOWSReq();
};

#endif
