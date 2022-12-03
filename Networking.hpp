
#ifndef Networking_H_INCLUDED
#define Networking_H_INCLUDED
#define SCK_VERSION2 0x0202 //version 2
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "Client.hpp"
#include <mutex>
#include <thread>
#include "customObjects.hpp"
#include <string>

struct SERVER {
public:
	WSADATA DATA;
	SOCKET ClientSocket;
	SOCKADDR_IN ServerInfo;
	int ClientId;

	SERVER(int port, std::string ip) {
		ServerInfo.sin_family = AF_INET;
		ServerInfo.sin_port = htons(port);
		std::wstring tempIp = std::wstring(ip.begin(), ip.end());
		InetPton(AF_INET, tempIp.c_str(), &ServerInfo.sin_addr.s_addr);

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
		}
		else {
			std::cout << "Socket has been initialised" << std::endl;
		}

		//connecting to the server
		if (connect(ClientSocket, (sockaddr*)&ServerInfo, sizeof(ServerInfo)) == SOCKET_ERROR) {
			std::cout << "Socket connection error: " << WSAGetLastError() << std::endl;
			closesocket(ClientSocket);
			WSACleanup();
		}
		else {
			std::cout << "Socket Connection Succefull" << std::endl;
		}

		char welcomeRes[4096];
		ZeroMemory(welcomeRes, 4096);
		int bytesRecieved = recv(ClientSocket, welcomeRes, 4096, 0);

		if (bytesRecieved > 0) {
			//recieved bytes
			std::cout << welcomeRes << std::endl;

			//grab the id
			bool atNumber = false;
			int id = 0;
			for (int i = 0; i < 4096; i++) {
				if (atNumber) {
					if (welcomeRes[i] == ';')
						break;
					if (std::isdigit(welcomeRes[i]))
						id = id * 10 + (welcomeRes[i] - '0');
				}

				if (welcomeRes[i] == ':')
					atNumber = true;
			}
			ClientId = id;
		}
	}
};

namespace NET {


	class NetworkLogic {
	public:

		
		

		NetworkLogic(SharedClientData& SharedClientData, SERVER& _server) : SharedData(SharedClientData), Server( _server ) {
			timeout.tv_usec = 10;

			FD_ZERO(&Master);
			FD_SET(Server.ClientSocket, &Master);
		}

		SERVER& Server;
		

		void ProcessData() {
			while (true) {
				fd_set copy = Master;
			
				int Activity = select(0, &copy, nullptr, nullptr, &timeout);

				//Recieving bytes
				if (Activity == 1) {
					SOCKET socket = copy.fd_array[0];

					char buffer[4096];
					ZeroMemory(buffer, 4096);
					int bytesRecieved = recv(socket, buffer, 4096, 0);
					if (bytesRecieved > 0) {
						std::vector<char>Data(buffer, buffer + 4096);
						SharedData.Lock();
						ClientData cl(Data);
						bool f = false;
						for (int i = 0; i < SharedData.IncomingClients.size(); i++) {
							if (cl.id == SharedData.IncomingClients[i].id) {
								SharedData.IncomingClients[i] = cl;
								f = true;
								break;
							}
						}
						if(f==false)
							SharedData.IncomingClients.push_back(cl);
						SharedData.Release();
					}
					

				}
			}
		}

	private:
		fd_set Master;
		SharedClientData& SharedData;
		timeval timeout;
	};

	
}



#endif