//
// Created by Escap on 1/9/2023.
//

#ifndef ESCAPIST_SOCKET_H
#define ESCAPIST_SOCKET_H

#include "../General.h"
#pragma comment(lib,"ws2_32.lib")

class DatagramServer {
private:
    int hSock;

public:
    DatagramServer() {
#ifdef ESCAPIST_OS_WINDOWS
        WSAData d{};
        assert(::WSAStartup(MAKEWORD(2, 2), &d));
#endif
        hSock = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        assert(hSock);
    }

    DatagramServer(const DatagramServer &other) = delete;

    DatagramServer &Bind(const char *ipAddr, int port) {
        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = ::htons(port);
    }
};

#endif //ESCAPIST_SOCKET_H
