#include "response.h"

SLOWSRes::SLOWSRes() {}

void SLOWSRes::Send(int clientSocket, short status) {
    std::stringstream response, responseBody;
    std::string statusDescription;
    switch (status) {
    case 200:
        statusDescription = "OK";
        break;
    }

    response << "HTTP/1.1 " << status << " " << statusDescription << "\r\n";
    for (auto header : Headers)
        response << header.first << ": " << header.second << "\r\n";
    response << "Content-Length: " << responseBody.str().length();
    response << "\r\n\r\n";
    response << responseBody.str();

    int result =
        send(clientSocket, response.str().c_str(), response.str().length(), 0);
    if (result == -1) {
        perror("Socket send Error");
        close(clientSocket);
        exit(EXIT_FAILURE);
    }
}

SLOWSRes::~SLOWSRes() {}
