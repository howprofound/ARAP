#include "server.h"
#include <stdio.h>

Server::Server() {
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	iResult = getaddrinfo(NULL, "9999", &hints, &result);
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	freeaddrinfo(result);
	iResult = listen(ListenSocket, SOMAXCONN);
	FD_ZERO(&master); // wyczyœæ g³ówny i pomocniczy zestaw
	FD_ZERO(&read_fds);
	FD_SET(ListenSocket, &master);
	Accept();

}

void Server::Accept() {
	bool x = true;
	int fdmax = ListenSocket;
	while (x) {
		read_fds = master; // copy it
		for (int i = 0; i <= fdmax; i++) {
			if (FD_ISSET(i, &read_fds)) {
				if (i == ListenSocket) {
					ClientSocket = accept(ListenSocket, NULL, NULL);
					x = false;
				}
			}
		}
	}

}
