#include "server.h"
#include <stdio.h>

Server::Server() {
	char x[50];
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	gethostname(x,50);
	getaddrinfo(x, "9999", &hints, &result);
	IP = inet_ntoa(((struct sockaddr_in *)result->ai_addr)->sin_addr);
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	freeaddrinfo(result);
	listen(ListenSocket, SOMAXCONN);
	FD_ZERO(&master); // wyczyœæ g³ówny i pomocniczy zestaw
	FD_ZERO(&read_fds);
	FD_SET(ListenSocket, &master);
	timeoutCounter.tv_sec = 0;
	timeoutCounter.tv_usec = 0;
	//Accept();

}

bool Server::Accept(int number) {
	
	int fdmax = ListenSocket;
	read_fds = master; // copy it
	select(fdmax + 1, &read_fds, NULL, NULL, &timeoutCounter);
	if (FD_ISSET(ListenSocket, &read_fds)) {
		ClientSocket = accept(ListenSocket, NULL, NULL);
		send(ClientSocket, (char*)&number, sizeof(int), 0);
		return true;
	}
	return false;

}
bool Server::R() {
	FD_ZERO(&master);
	FD_SET(ClientSocket, &master);
	select(ClientSocket+1, &master, NULL, NULL, &timeoutCounter);
	if (FD_ISSET(ClientSocket, &master)) {
		recv(ClientSocket, (char*)&package, sizeof(package), 0);
		return true;
	}
	return false;
}
void Server::S() {
	send(ClientSocket, (char*)&package, sizeof(package), 0);
}