#pragma once

#include "struct.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment (lib, "Ws2_32.lib")

class Server
{
private:
	WSADATA wsaData;
	int iResult;

	timeval timeoutCounter;

	addrinfo *result;
	addrinfo hints;

	fd_set master; // g��wna lista deskryptor�w plik�w
	fd_set read_fds; // pomocnicza lista deskryptor�w dla select()

	SOCKET ListenSocket;
	SOCKET ClientSocket[3];
	
	char buffer[32];
	char IP[32];
	Package package;

public:
	Server();
	bool Accept(int);
	bool R(int);
	void S(int);
	void SendPlayers(int);
	Package* getPackage();
	char* getIP();
};
