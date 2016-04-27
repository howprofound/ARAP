#include "client.h"

Client::Client(char *IP) {
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	getaddrinfo(IP, "9999", &hints, &result);
	ConnectSocket = socket(result->ai_family, result->ai_socktype,
		result->ai_protocol);
	timeoutCounter.tv_sec = 0;
	timeoutCounter.tv_usec = 0;
	

}
bool Client::Connect() {
	if (connect(ConnectSocket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR)
		return false;
	else
		return true;
}

int Client::RecieveNumber() {
	recv(ConnectSocket, buffer, 1, 0);
	sscanf(buffer, "%d", &iResult);
	return iResult;
}

void Client::Send() {
	sprintf(buffer, "%d %d %d %d", package.angle, package.number, package.x, package.y);
	send(ConnectSocket, buffer, 15, 0);

}
bool Client::R() {
	FD_ZERO(&master);
	FD_SET(ConnectSocket, &master);
	select(ConnectSocket + 1, &master, NULL, NULL, &timeoutCounter);
	if (FD_ISSET(ConnectSocket, &master)) {
		recv(ConnectSocket, buffer, 15, 0);
		sscanf(buffer, "%d %d %d %d", &package.angle, &package.number, &package.x, &package.y);
		return true;
	}
	return false;

}