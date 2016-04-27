#pragma once

#include "struct.h"
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")

class Server {
private:
	WSADATA wsaData;
	int iResult;

	timeval timeoutCounter;

	struct addrinfo *result;
	struct addrinfo hints;

	fd_set master; // g³ówna lista deskryptorów plików
	fd_set read_fds; // pomocnicza lista deskryptorów dla select()
public:
	Server();
	bool Accept(int);
	char *IP;
	bool R();
	void S();
	SOCKET ListenSocket;
	SOCKET ClientSocket;
	Package package;
};
