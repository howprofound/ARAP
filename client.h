#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include "struct.h"
#include <stdio.h>

#pragma comment (lib, "Ws2_32.lib")

class Client {
public:
	WSADATA wsaData;
	Package package;
	SOCKET ConnectSocket = INVALID_SOCKET;
	addrinfo *result = NULL, hints;
	int iResult;
	Client(char*);
	int RecieveNumber();
	bool Connect();
	void Send();
	bool R();
	fd_set master;
	timeval timeoutCounter;
	char buffer[512];
};