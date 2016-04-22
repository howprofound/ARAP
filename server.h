#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")

class Server
{
private:
	WSADATA wsaData;
	int iResult;

	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL;
	struct addrinfo hints;

	int iSendResult;
	char recvbuf[512];
	int recvbuflen = 512;

	fd_set master; // g³ówna lista deskryptorów plików
	fd_set read_fds; // pomocnicza lista deskryptorów dla select()
public:
	Server();
	void Accept();


};
