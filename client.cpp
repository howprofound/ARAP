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
	recv(ConnectSocket, (char*)&iResult, sizeof(int), 0);
	return iResult;
}

void Client::Send() {
	send(ConnectSocket, (char*)&package, sizeof(package), 0);

}
bool Client::R() {
	FD_ZERO(&master);
	FD_SET(ConnectSocket, &master);
	select(ConnectSocket + 1, &master, NULL, NULL, &timeoutCounter);
	if (FD_ISSET(ConnectSocket, &master)) {
		recv(ConnectSocket, (char*)&package, sizeof(package), 0);
		return true;
	}
	return false;

}