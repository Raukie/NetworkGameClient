#include "SFML/Graphics.hpp"
#include <iostream>
#include <vector>
#include <WinSock2.h>
#include <WS2tcpip.h>
#define NOMINMAX
#include <windows.h>
#include <tchar.h>
#include "Game.hpp"
#include "customObjects.hpp"
#include "Game.cpp"
#include "Client.hpp"



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
	Client client = Client();
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
		client.SetId(id);
	}
	std::vector<Player> players;
	//game setup
	sf::RenderWindow Window(sf::VideoMode(1280, 720), "MyProgram");
	sf::View vw;
	vw.setSize(1280, 720);
	Window.setFramerateLimit(60);
	Game game(Window, vw);
	Window.setView(vw);
	sf::Clock clock;


	//setting up the ui font
	sf::Text txt;
	sf::Font font;
	if (!font.loadFromFile("Minecraft.ttf")) {
		return EXIT_FAILURE;
	}
	txt.setFont(font);
	txt.setPosition(sf::Vector2f(10, 10));
	txt.setCharacterSize(30);
	txt.setFillColor(sf::Color::White);
	Player player(sf::Vector2f(100, 100), client.id, txt);
	game.GameObjects.push_back(&player);
	game.follow = true;
	game.target = game.GameObjects[0];

	//network setup
	fd_set Master;
	FD_ZERO(&Master);
	FD_SET(ClientSocket, &Master);

	int i = 0;
	timeval timeout;
	timeout.tv_usec = 10;
	while (Window.isOpen()) {
		






	

		sf::Event Event;

		//game logic
		sf::Vector2f mov = sf::Vector2f(0, 0);
		while (Window.pollEvent(Event)) {
			//close window if window is closed on desktop
			if (Event.type == sf::Event::Closed) Window.close();




			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
				mov += sf::Vector2f(-1,0);
			}
				
			
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
				mov += sf::Vector2f(1, 0);
			}
				
			
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
				mov += sf::Vector2f(0, 1);
			}
				
	
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
				mov += sf::Vector2f(0, -1);
			}

		}

		if (std::abs(mov.x) + std::abs(mov.y) > 1)
			mov = sf::Vector2f(mov.x / 2, mov.y / 2);

		player.Force = sf::Vector2f(mov.x*2, mov.y*2);
		sf::Time time = clock.restart();
		float Delta = time.asMicroseconds();
		game.Update(Delta);


		/// <summary>
		/// //////////////
		/// </summary>
		/// <returns></returns>
		
		//networking logic
		fd_set copy = Master;

		int Activity = select(0, &copy, nullptr, nullptr,&timeout);

		//Recieving bytes
		if (Activity == 1) {
			SOCKET socket = copy.fd_array[0];
			
			char buffer[4096];
			ZeroMemory(buffer, 4096);
			int bytesRecieved = recv(socket, buffer, 4096, 0);
			if (bytesRecieved > 0) {
				std::vector<char>Data(buffer, buffer + 4096);
				ClientData cl(Data);
				//check if gameobject already exists
				std::vector<OBJ::GameObject*> objects = game.GetObjectsByTag("Player");
				bool found = false;
				for (int i = 0; i < objects.size(); i++) {
					Player* cur = dynamic_cast<Player*>(objects[i]);
					if (cl.id == cur->id) {
						found = true;
						cur->Pos = sf::Vector2f(cl.x, cl.y);
						cur->health = cl.health;
					}

				}
				if (found == false) {
		
					players.push_back(Player(sf::Vector2f(cl.x, cl.y), cl.id, txt));
		
					game.GameObjects.push_back(&players[players.size() - 1]);
				}
			}
			

		}
		i++;
		if (i == 1) {
			i = 0;
			send(ClientSocket, player.NetData(), 4096, 0);
		}
		
		
	}
	return EXIT_SUCCESS;
}