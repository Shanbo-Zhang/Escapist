//
// Created by Escap on 1/9/2023.
//

#ifndef ESCAPIST_SOCKET_H
#define ESCAPIST_SOCKET_H

#include "../General.h"

#pragma comment(lib, "ws2_32.lib")

class DatagramServer {
private:
    int hSock;

public:
    DatagramServer() {
#ifdef ESCAPIST_OS_WINDOWS
        WSAData d{};
        assert(!::WSAStartup(MAKEWORD(2, 2), &d));
#endif
        hSock = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        assert(hSock);
    }

    DatagramServer(const DatagramServer &other) = delete;

    ~DatagramServer() {
        if (hSock) {
            ::closesocket(hSock);
        }
#ifdef ESCAPIST_OS_WINDOWS
        ::WSACleanup();
#endif
    }

    DatagramServer &Bind(const char *ipAddr, int port) {
        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = ::htons(port);
        if (ipAddr) {

        } else {
            addr.sin_addr.S_un.S_addr = INADDR_ANY;
        }
        assert(::bind(hSock, reinterpret_cast<const sockaddr *>(&addr), sizeof(sockaddr_in)) != SOCKET_ERROR);
    }

    DatagramServer &Receive(unsigned char ipAddress[4], ByteArray &data) {
        sockaddr_in addr;
        char each[1024];
        int size, addrLen;
        ByteArray result;
        do {
            size = ::recvfrom(hSock, each, 1024, 0, (sockaddr *) (&addr), &addrLen);
            assert(size != SOCKET_ERROR);
            result.Append((unsigned char *) each, size);
        } while (size != 0);
        ::memcpy(ipAddress, &addr.sin_addr.S_un.S_un_w, 4);
    }

    DatagramServer &Send(unsigned char ipAddress[4], short port, unsigned char *data, int size) {
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = ::htons(port);
        ::memcpy(&addr.sin_addr.S_un.S_addr, ipAddress, 4);
        assert(::sendto(hSock, (const char *) data, size, 0, (sockaddr *) &addr, sizeof(sockaddr_in))
               != SOCKET_ERROR);
        return *this;
    }
};

class DatagramClient {
private:
    int socket;

public:
    
};

#endif //ESCAPIST_SOCKET_H
