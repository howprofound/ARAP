#include "client.h"

Client::Client(char *IP)
{
	ConnectSocket = INVALID_SOCKET;
	result = NULL;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	getaddrinfo(IP, "10010", &hints, &result);
	ConnectSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	timeoutCounter.tv_sec = 0;
	timeoutCounter.tv_usec = 0;
	this->connected = false;
	package.back = 0;
	package.points[0] = 0;
	package.points[1] = 0;
	package.points[2] = 0;
	package.points[3] = 0;
}

bool Client::Connect()
{
	if (connect(ConnectSocket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR)
	{
		return false;
	}
	else
	{
		return true;
	}
}

int Client::ReceiveNumber()
{
	recv(ConnectSocket, buffer, 1, 0);
	sscanf_s(buffer, "%d", &iResult);
	return iResult;
}

void Client::Send()
{
	sprintf_s(buffer, "%d %d %d %d", package.angle, package.number, package.x, package.y);
	send(ConnectSocket, buffer, sizeof(buffer), 0);
}

bool Client::Receive()
{
	FD_ZERO(&master);
	FD_SET(ConnectSocket, &master);
	select(ConnectSocket + 1, &master, NULL, NULL, &timeoutCounter);
	if (FD_ISSET(ConnectSocket, &master))
	{
		recv(ConnectSocket, buffer, sizeof(buffer), 0);
		if (buffer[0] == 'p') // pozycja
		{
			sscanf_s(buffer + 1, "%d %d %d %d", &package.angle, &package.number, &package.x, &package.y);
		}
		else if (buffer[0] == 'c') // kolizja
		{
			sscanf_s(buffer + 1, "%d %d", &package.back, &package.predatorAngle);
		}
		else if (buffer[0] == 's') // punkty
		{
			sscanf_s(buffer + 1, "%d %d %d %d", &package.points[0], &package.points[1], &package.points[2], &package.points[3]);
		}
		return true;
	}
	return false;
}

bool Client::ReceivePlayers()
{
	FD_ZERO(&master);
	FD_SET(ConnectSocket, &master);
	select(ConnectSocket + 1, &master, NULL, NULL, &timeoutCounter);
	if (FD_ISSET(ConnectSocket, &master))
	{
		recv(ConnectSocket, buffer, 1, 0);
		sscanf_s(buffer, "%d", &iResult);
		return true;
	}
	return false;
}

bool Client::isConnected()
{
	return this->connected;
}

void Client::setConnected(bool connected)
{
	this->connected = connected;
}

Package* Client::getPackage()
{
	return &(this->package);
}

int Client::getIResult()
{
	return this->iResult;
}