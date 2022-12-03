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
#include <mutex>
#include <thread>
#include "Networking.hpp"



int main() {
	//network setup
	

	


	//setting up the shared container and networking logic
	SharedClientData SharedData;
	SERVER Server(55555, "127.0.0.1");
	NET::NetworkLogic Network(std::ref(SharedData), Server);
	


	std::vector<std::unique_ptr<Player>> players;
	//game setup
	sf::RenderWindow Window(sf::VideoMode(1280, 720), "MyProgram");
	sf::View vw;
	vw.setSize(1280, 720);
	Window.setFramerateLimit(60);
	Game game(Window, vw);
	Window.setView(vw);
	sf::Clock clock;

	//fix for focussing issues
	SetForegroundWindow(Window.getSystemHandle());

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
	Player player(sf::Vector2f(100, 100), Network.Server.ClientId, txt);
	game.GameObjects.push_back(&player);
	game.follow = false;
	game.target = game.GameObjects[0];


	std::thread t1(&NET::NetworkLogic::ProcessData, &Network);


	int i = 0;
	bool focus = true;
	while (Window.isOpen()) {
		
		sf::Event Event;
		
		//game logic
		sf::Vector2f mov = sf::Vector2f(0, 0);
		while (Window.pollEvent(Event)) {
			//close window if window is closed on desktop
			if (Event.type == sf::Event::Closed) 
				Window.close();

			if (Event.type == sf::Event::LostFocus)
				focus = false;

			if (Event.type == sf::Event::GainedFocus)
				focus = true;
		}


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)&&focus) {
			mov += sf::Vector2f(-1, 0);
		}


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && focus) {
			mov += sf::Vector2f(1, 0);
		}


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && focus) {
			mov += sf::Vector2f(0, 1);
		}


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && focus) {
			mov += sf::Vector2f(0, -1);
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

		
		std::vector<OBJ::GameObject*> objects = game.GetObjectsByTag("Player");
		bool found = false;

		//thread safety
		SharedData.Lock();
		std::vector<ClientData> CopyData(SharedData.IncomingClients);
		SharedData.IncomingClients.clear();
		SharedData.Release();
		//check if gameobject already exists
		for (int x = 0; x < CopyData.size(); x++) {
			ClientData& cl = CopyData[x];
			for (int i = 0; i < objects.size(); i++) {
				Player* cur = dynamic_cast<Player*>(objects[i]);
				if (cl.id == cur->id) {
					found = true;
					cur->Pos = sf::Vector2f(cl.x, cl.y);
					cur->health = cl.health;
				}
			}
			if (found == false) {
				players.push_back(std::make_unique<Player>(Player(sf::Vector2f(cl.x, cl.y), cl.id, txt)));

				game.GameObjects.push_back(players[players.size() - 1].get());
			}
		}
		
		
		i++;
		if (i == 1) {
			i = 0;
			send(Network.Server.ClientSocket, player.NetData(), 4096, 0);
		}
		
		
	}
	return EXIT_SUCCESS;
}