#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include "struct.h"
#include <stdio.h>

#pragma comment (lib, "Ws2_32.lib")

class Client
{
private:
	bool connected;
	WSADATA wsaData;
	Package package;
	SOCKET ConnectSocket;
	addrinfo *result;
	addrinfo hints;
	int iResult;
	fd_set master;
	timeval timeoutCounter;
	char buffer[64];

public:
	Client(char*);
	int ReceiveNumber();
	bool Connect();
	void Send();
	bool Receive();
	bool ReceivePlayers();
	bool isConnected();
	void setConnected(bool);
	Package* getPackage();
	int getIResult();
};