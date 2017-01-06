#include "server.h"

SLOWS::SLOWS() {
    AllowedMethods = {"GET", "HEAD"};
}

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
            return;
        }
    }
    bool allow = false;
    for (auto Method : AllowedMethods)
        if (method == Method)
            allow = true;
    if (!allow) {
        BreakConnection(clientSocket, 405);
        return;
    }
    while (read(clientSocket, &c, sizeof(char)) > 0 && c != ' ') {
        uri.push_back(c);
        if (method.length() == URI_CHAR_CAP) {
            BreakConnection(clientSocket, 400);
            return;
        }
    }
    while (read(clientSocket, &c, sizeof(char)) > 0 && c != '\n') {
        if (c == '\r')
            continue;
        protocolVersion.push_back(c);
        if (method.length() == PROTOCOL_CHAR_CAP) {
            BreakConnection(clientSocket, 400);
            return;
        }
    }
    if (protocolVersion != "HTTP/1.1") {
        BreakConnection(clientSocket, 505);
        return;
    }
    SLOWSReq *request = new SLOWSReq(method, uri, protocolVersion);
    while (read(clientSocket, &c, sizeof(char)) > 0 && c != '\n') {
        if (c == '\r')
            continue;
        headerName.push_back(c);
        while (read(clientSocket, &c, sizeof(char)) > 0 && c != ':') {
            headerName.push_back(c);
            if (headerName.length() == LEFT_HEADER_CHAR_CAP) {
                BreakConnection(clientSocket, 400);
                delete request;
                return;
            }
        }
        if (read(clientSocket, &c, sizeof(char)) <= 0 || c != ' ') {
            BreakConnection(clientSocket, 400);
            delete request;
            return;
        }
        while (read(clientSocket, &c, sizeof(char)) > 0 && c != '\n') {
            if (c == '\r')
                continue;
            headerValue.push_back(c);
            if (headerValue.length() == RIGHT_HEADER_CHAR_CAP) {
                BreakConnection(clientSocket, 400);
                delete request;
                return;
            }
        }
        request->pushHeader(headerName, headerValue);
        headerName = "";
        headerValue = "";
    }
    std::cout << "End Parse" << '\n'; // DEL
    MethodeController(clientSocket, request);
    close(clientSocket);
    delete request;
}
void SLOWS::MethodeController(int clientSocket, SLOWSReq *req) {
    std::string method = req->getMethod();
    if (method == "GET") {
        MethodeGet(clientSocket, req);
    } else if (method == "HEAD") {
        MethodeHead(clientSocket, req);
    } else {
        BreakConnection(clientSocket, 501);
        delete req;
    }
}

void SLOWS::MethodeGet(int clientSocket, SLOWSReq *req) {
    // SLOWSRes *res = new SLOWSRes();
    std::string uri = req->getUri();
    std::string path = req->getUri();
    if (access((HomeDir + path).data(), 3) != 0) {
        BreakConnection(clientSocket, 404);
        return;
    }
    /*std::ofstream file;
    file.open(path);
    std::stringstream responseBody;
    responseBody << file;
    BreakConnection(clientSocket, 404, responseBody);*/

    int resource = open(path.data(), O_RDONLY);
    if (resource < 0) {
        BreakConnection(clientSocket, 404);
        perror("Open File Error");
        close(clientSocket);
        return;
    }
    BreakConnection(clientSocket, 200);
    // res->Send(clientSocket);
    // delete res;
}
void SLOWS::MethodeHead(int clientSocket, SLOWSReq *req) {}

void SLOWS::BreakConnection(int clientSocket, short status,
                            std::stringstream responseBody) {
    std::stringstream response;
    std::string statusDescription;
    // Add responseBody
    switch (status) {
    case 200:
        statusDescription = "OK";
        break;
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
    response << "HTTP/1.1 " << status << " " << statusDescription << "\r\n";
    response << "Version: HTTP/1.1\r\n"
             << "Content-Type: text/html; charset=utf-8\r\n";
    response << "Content-Length: " << responseBody.str().length();
    response << "\r\n\r\n";
    response << responseBody.str();

    int result =
        send(clientSocket, response.str().c_str(), response.str().length(), 0);
    if (result == -1) {
        perror("Socket send Error");
        close(clientSocket);
        close(SocketFD);
        exit(EXIT_FAILURE);
    }
    close(clientSocket);
}

SLOWS::~SLOWS() {}
