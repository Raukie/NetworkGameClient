
#ifndef Networking_H_INCLUDED
#define Networking_H_INCLUDED

#include <WinSock2.h>
#include <WS2tcpip.h>
#include "Client.hpp"
#include <mutex>
#include <thread>
#include "customObjects.hpp"
namespace NET {
	class NetworkLogic {
	public:
		NetworkLogic(fd_set &master, SharedClientData &SharedClientData, SOCKET &server) : Master(master), SharedData(SharedClientData), Server(server) {
			timeout.tv_usec = 10;
		}



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
		fd_set& Master;
		SharedClientData& SharedData;
		SOCKET& Server;
		timeval timeout;
	};
}

#endif