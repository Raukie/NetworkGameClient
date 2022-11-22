#include <iostream>
#include <vector>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <tchar.h>
#include <windows.h>
//wsa version
#define SCK_VERSION2 0x0202 //version 2
WSADATA DATA;
SOCKET ClientSocket;
SOCKADDR_IN ServerInfo;

int main() {

	ServerInfo.sin_family = AF_INET;
	ServerInfo.sin_port = htons(55555);
	InetPton(AF_INET, _T("127.0.0.1"), &ServerInfo.sin_addr.s_addr);

	//iniatilsing wsa
	long SUCCESSFULL = WSAStartup(SCK_VERSION2, &DATA);

	if (SUCCESSFULL != 0) {
		std::cout << "WSA Exited with error: " << SUCCESSFULL;
	}
	else {
		std::cout << "WSA Initialised" << std::endl;
	}

	ClientSocket = INVALID_SOCKET;
	ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ClientSocket == INVALID_SOCKET) {
		std::cout << "Socket error: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return EXIT_SUCCESS;
	}
	else {
		std::cout << "Socket has been initialised" << std::endl;
	}

	//connecting to the server
	if (connect(ClientSocket, (sockaddr*)&ServerInfo, sizeof(ServerInfo))== SOCKET_ERROR) {
		std::cout << "Socket connection error: " << WSAGetLastError() << std::endl;
		closesocket(ClientSocket);
		WSACleanup();
	} else {
		std::cout << "Socket Connection Succefull" << std::endl;
	}


	char buffer[2024];
	int bytes = recv(ClientSocket, buffer, sizeof(buffer)-1,0);
	buffer[bytes] = 0;
	std::cout << buffer;

	return EXIT_SUCCESS;
}