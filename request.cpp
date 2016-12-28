#include "request.h"

SLOWSReq::SLOWSReq() {}
SLOWSReq::SLOWSReq(std::string method, std::string uri,
                   std::string protocolVersion)
    : Method(method), Uri(uri), ProtocolVersion(protocolVersion) {}

void SLOWSReq::setMethod(std::string method) {
    Method = method;
}
void SLOWSReq::setUri(std::string uri) {
    Uri = uri;
}
void SLOWSReq::setProtocolVersion(std::string protocolVersion) {
    ProtocolVersion = protocolVersion;
}
void SLOWSReq::pushHeader(std::string name, std::string value) {
    // chck unique
    Headers.insert({name, value});
}

SLOWSReq::~SLOWSReq() {}
