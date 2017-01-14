#include "response.h"

SLOWSRes::SLOWSRes() {
    Headers.insert({"Version", "HTTP/1.1"});
}

void SLOWSRes::Send(int clientSocket, std::string body) {
    std::stringstream response;
    response << "HTTP/1.1 " << Status << " " << StatusDescription << "\r\n";
    for (auto header : Headers)
        response << header.first << ": " << header.second << "\r\n";
    response << "Content-Length: " << body.length();
    response << "\r\n\r\n";
    response << body;

    int result =
        send(clientSocket, response.str().c_str(), response.str().length(), 0);
    if (result == -1) {
        perror("Socket send Error");
        close(clientSocket);
        exit(EXIT_FAILURE);
    }
}
void SLOWSRes::Send(int clientSocket) {
    std::stringstream response;
    response << "HTTP/1.1 " << Status << " " << StatusDescription << "\r\n";
    for (auto header : Headers)
        response << header.first << ": " << header.second << "\r\n";
    int result =
        send(clientSocket, response.str().c_str(), response.str().length(), 0);
    if (result == -1) {
        perror("Socket send Error");
        close(clientSocket);
        exit(EXIT_FAILURE);
    }
}
void SLOWSRes::PushHeader(std::string name, std::string value) {
    auto search = Headers.find(name);
    if (search != Headers.end())
        Headers.erase(search);
    Headers.insert({name, value});
}
void SLOWSRes::SetStatus(short status) {

    switch (status) {
    case 200:
        StatusDescription = "OK";
        break;
    case 201:
        StatusDescription = "Created";
        break;
    case 202:
        StatusDescription = "Accepted";
        break;
    }
}

SLOWSRes::~SLOWSRes() {}
