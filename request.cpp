#include "request.h"

SLOWSReq::SLOWSReq() {}
SLOWSReq::SLOWSReq(std::string method, std::string uri,
                   std::string protocolVersion)
    : Method(method), ProtocolVersion(protocolVersion) {
    std::regex backPath("\\.\\./");
    Uri = std::regex_replace(uri, backPath, "");
}

void SLOWSReq::setMethod(std::string method) {
    Method = method;
}
void SLOWSReq::setUri(std::string uri) {
    Uri = uri;
}
void SLOWSReq::setProtocolVersion(std::string protocolVersion) {
    ProtocolVersion = protocolVersion;
}

std::string SLOWSReq::getMethod() {
    return Method;
}
std::string SLOWSReq::getUri() {
    return Uri;
}
std::string SLOWSReq::getProtocolVersion() {
    return ProtocolVersion;
}
std::map<std::string, std::string> SLOWSReq::getHeaders() {
    return Headers;
}

void SLOWSReq::pushHeader(std::string name, std::string value) {
    auto search = Headers.find(name);
    if (search != Headers.end())
        Headers.erase(search);
    Headers.insert({name, value});
}

SLOWSReq::~SLOWSReq() {}
