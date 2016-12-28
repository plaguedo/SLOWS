#include "server.h"

SLOWS::SLOWS() {}

void SLOWS::Listen(const unsigned short int port) {
    Port = port;

    struct sockaddr_in sockAddr;
    SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (SocketFD == -1) {
        perror("Socket create Error");
        exit(EXIT_FAILURE);
    }
    // memset(&sockAddr, 0, sizeof(sockAddr));
    sockAddr.sin_family = PF_INET;
    sockAddr.sin_port = htons(Port);
    sockAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(SocketFD, (struct sockaddr *)&sockAddr, sizeof(sockAddr)) == -1) {
        perror("Socket bind Error");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }

    if (listen(SocketFD, SOMAXCONN) == -1) {
        perror("Socket listen Error");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }

    while (true) {
        int clientSocketFD = accept(SocketFD, 0, 0);
        if (clientSocketFD < 0) {
            perror("Socket accept Error");
            close(SocketFD);
            exit(EXIT_FAILURE);
        }
        std::thread clientThread(&SLOWS::Prepare, this, clientSocketFD);
        clientThread.detach();
    }

    close(SocketFD);
}
void SLOWS::Static(std::string path) {
    HomeDir = path;
}
void SLOWS::Prepare(int clientSocket) {
    printf("Someone connected. FD: %d\n", clientSocket);
    char c;
    std::string headerName, headerValue, method, uri, protocolVersion;
    while (read(clientSocket, &c, sizeof(char)) > 0 && c != ' ') {
        method.push_back(c);
        if (method.length() == METHOD_CHAR_CAP) {
            BreakConnection(clientSocket, 400);
        }
    }
    // Method Allow Check
    while (read(clientSocket, &c, sizeof(char)) > 0 && c != ' ') {
        uri.push_back(c);
        if (method.length() == URI_CHAR_CAP) {
            BreakConnection(clientSocket, 400);
        }
    }
    while (read(clientSocket, &c, sizeof(char)) > 0 && c != '\n') {
        if (c == '\r')
            continue;
        protocolVersion.push_back(c);
        if (method.length() == PROTOCOL_CHAR_CAP) {
            BreakConnection(clientSocket, 400);
        }
    }
    if (protocolVersion != "HTTP/1.1")
        BreakConnection(clientSocket, 505);
    SLOWSReq request(method, uri, protocolVersion);
    while (read(clientSocket, &c, sizeof(char)) > 0 && c != '\n') {
        if (c == '\r')
            continue;
        headerName.push_back(c);
        while (read(clientSocket, &c, sizeof(char)) > 0 && c != ':') {
            headerName.push_back(c);
            if (headerName.length() == LEFT_HEADER_CHAR_CAP) {
                BreakConnection(clientSocket, 400);
            }
        }
        if (read(clientSocket, &c, sizeof(char)) <= 0 || c != ' ')
            BreakConnection(clientSocket, 400);
        while (read(clientSocket, &c, sizeof(char)) > 0 && c != '\n') {
            if (c == '\r')
                continue;
            headerValue.push_back(c);
            if (headerValue.length() == RIGHT_HEADER_CHAR_CAP) {
                BreakConnection(clientSocket, 400);
            }
        }
        request.pushHeader(headerName, headerValue);
        headerName = "";
        headerValue = "";
    }
    std::cout << "End Parse" << '\n'; // END
    // Read Body
    close(clientSocket);
}
void SLOWS::BreakConnection(int clientSocket, short status) {
    std::stringstream responseHeaders, responseBody;
    std::string statusDescription;
    // Add responseBody
    switch (status) {
    case 400:
        statusDescription = "Bad Request";
        break;
    case 401:
        statusDescription = "Unauthorized";
        break;
    case 403:
        statusDescription = "Forbidden";
        break;
    case 404:
        statusDescription = "Not Found";
        break;
    case 405:
        statusDescription = "Method Not Allowed";
        break;
    case 406:
        statusDescription = "Not Acceptable";
        break;
    case 407:
        statusDescription = "Proxy Authentication Required";
        break;
    case 408:
        statusDescription = "Request Timeout";
        break;
    case 409:
        statusDescription = "Conflict";
        break;
    case 410:
        statusDescription = "Gone";
        break;
    case 411:
        statusDescription = "Length Required";
        break;
    case 412:
        statusDescription = "Precondition Failed";
        break;
    case 413:
        statusDescription = "Request Entity Too Large";
        break;
    case 414:
        statusDescription = "Request-URI Too Long";
        break;
    case 415:
        statusDescription = "Unsupported Media Type";
        break;
    case 500:
        statusDescription = "Internal Server Error";
        break;
    case 501:
        statusDescription = "Not Implemented";
        break;
    case 502:
        statusDescription = "Bad Gateway";
        break;
    case 503:
        statusDescription = "Service Unavailable";
        break;
    case 504:
        statusDescription = "Gateway Timeout";
        break;
    case 505:
        statusDescription = "HTTP Version Not Supported";
        break;
    }
    responseHeaders << "HTTP/1.1 " << status << " " << statusDescription
                    << "\r\n"
                    << "Version: HTTP/1.1\r\n"
                    << "Content-Type: text/html; charset=utf-8\r\n"
                    << "Content-Length: " << responseBody.str().length()
                    << "\r\n\r\n"
                    << responseBody.str();

    int result = send(clientSocket, responseHeaders.str().c_str(),
                      responseHeaders.str().length(), 0);
    if (result == -1) {
        perror("Socket send Error");
        close(clientSocket);
        close(SocketFD);
        exit(EXIT_FAILURE);
    }
    close(clientSocket);
}

SLOWS::~SLOWS() {}
