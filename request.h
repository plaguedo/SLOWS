#ifndef SLOWS_REQUEST
#define SLOWS_REQUEST

#include <map>
#include <string>

class SLOWSReq {
  private:
    std::map<std::string, std::string> Headers;
    std::string Body = "";
    std::string Method;
    std::string Uri;
    std::string ProtocolVersion;

  public:
    SLOWSReq();
    SLOWSReq(std::string method, std::string uri, std::string protocolVersion);

    void setMethod(std::string method);
    void setUri(std::string uri);
    void setProtocolVersion(std::string protocolVersion);

    std::string getMethod();
    std::string getUri();
    std::string getProtocolVersion();
    std::map<std::string, std::string> getHeaders();

    void pushHeader(std::string name, std::string value);

    virtual ~SLOWSReq();
};

#endif
