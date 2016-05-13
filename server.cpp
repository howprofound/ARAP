#include "server.h"


Server::Server()
{
	char x[50];
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	gethostname(x, 50);
	getaddrinfo(x, "10010", &hints, &result);
	inet_ntop(hints.ai_family, &((sockaddr_in *)result->ai_addr)->sin_addr, (PSTR)IP, sizeof(IP));
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	freeaddrinfo(result);
	listen(ListenSocket, SOMAXCONN);
	FD_ZERO(&master); // wyczysc glowny i pomocniczy zestaw
	FD_ZERO(&read_fds);
	FD_SET(ListenSocket, &master);
	timeoutCounter.tv_sec = 0;
	timeoutCounter.tv_usec = 0;
	for (int i = 0; i < 3; i++)
	{
		ClientSocket[i] = INVALID_SOCKET;
	}
	package.back = 0;
	package.points[0] = 0;
	package.points[1] = 0;
	package.points[2] = 0;
	package.points[3] = 0;
	//Accept();
}

bool Server::Accept(int number) 
{
	int fdmax = ListenSocket;
	read_fds = master; // copy it
	select(fdmax + 1, &read_fds, NULL, NULL, &timeoutCounter);
	if (FD_ISSET(ListenSocket, &read_fds))
	{
		ClientSocket[number-1] = accept(ListenSocket, NULL, NULL);
		sprintf_s(buffer, "%d", number);
		send(ClientSocket[number-1], buffer, 1, 0);
		return true;
	}
	return false;

}
bool Server::Receive(int number)
{
	FD_ZERO(&master);
	FD_SET(ClientSocket[number-1], &master);
	select(ClientSocket[number-1] + 1, &master, NULL, NULL, &timeoutCounter);
	if (FD_ISSET(ClientSocket[number-1], &master))
	{
		recv(ClientSocket[number-1], buffer, sizeof(buffer), 0);
		sscanf_s(buffer, "%d %d %d %d", &package.angle, &package.number, &package.x, &package.y);
		return true;
	}
	return false;
}

void Server::SendPosition(int number) 
{
	sprintf_s(buffer, "p%d %d %d %d", package.angle, package.number, package.x, package.y);
	send(ClientSocket[number-1], buffer, sizeof(buffer), 0);
}

void Server::SendCollision(int number)
{
	sprintf_s(buffer, "c%d %d", package.back, package.predatorAngle);
	send(ClientSocket[number - 1], buffer, sizeof(buffer), 0);
}

void Server::SendScore(int number)
{
	sprintf_s(buffer, "s%d %d %d %d", package.points[0], package.points[1], package.points[2], package.points[3]);
	send(ClientSocket[number - 1], buffer, sizeof(buffer), 0);
}

void Server::SendPlayers(int number)
{
	sprintf_s(buffer, "%d", number);
	for (int i = 0; i < number - 1; i++)
	{
		send(ClientSocket[i], buffer, 1, 0);
	}
}

Package* Server::getPackage()
{
	return &(this->package);
}

char* Server::getIP()
{
	return this->IP;
}