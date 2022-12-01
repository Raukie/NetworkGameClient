#include "sfml/Graphics.hpp"
#include <windows.h>
#include "GameObject.hpp"

#ifndef customObjects_H_INCLUDED
#define customObjects_H_INCLUDED

class Player : public OBJ::GameObject {
public:
	Player(sf::Vector2f pos, int _id) : GameObject("Player") {
		GameObject::Pos = pos;
		shape.setSize(sf::Vector2f(100,100));
		GameObject::Kinectic = true;
		id = _id;
	};
	int id;
	int health;
	sf::RectangleShape shape;

	char* NetData() {
		char buf[4096];
		ZeroMemory(buf, 4096);
		int c = 0;
		int a = 0;
		bool proc = true;
		std::vector<std::string> data;
		data.push_back(std::to_string(id));
		data.push_back(std::to_string(GameObject::Pos.x));
		data.push_back(std::to_string(GameObject::Pos.y));
		data.push_back(std::to_string(health));

		
		for (int i = 0; i < 4; i++) {
			bool f = true;
			while (true) {
				if (f) {
					buf[c] = '{';
					c++;
				}
				else {
					for (int x = 0; x < data[i].size(); x++) {
						buf[c] = data[i][x];
						c++;
					}
					buf[c] = '}';
					c++;
				}
			}
		}
		return buf;
		
	}

	void update() {
		if (GameObject::Kinectic)
			GameObject::Pos += GameObject::Force;
		shape.setPosition(GameObject::Pos);
	}

	void draw(sf::RenderWindow& Window) {
		Window.draw(shape);
	}
	
};

class UiObject : public OBJ::GameObject {
public:
	UiObject(sf::Vector2f initPos, sf::Text text) : GameObject("UI") {
		Pos = initPos;
		textObj = text;
	}

	sf::Text textObj;

	void Update(std::vector<OBJ::GameObject*>& objects, float delta) {
		textObj.setPosition(Pos);
	}

	void Draw(sf::RenderWindow& Window) {
		Window.draw(textObj);
	}
};


#endif // customObjects_H_INCLUDED