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

	fd_set master; // g��wna lista deskryptor�w plik�w
	fd_set read_fds; // pomocnicza lista deskryptor�w dla select()
public:
	Server();
	void Accept();


};
