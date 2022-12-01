#ifndef Client_H_INCLUDED
#define Client_H_INCLUDED

#include "customObjects.hpp"
#include <vector>
#include <string>
class Client {
public:
	bool IsSpawned;
	Player* PlayerObject;

	void SetId(int i) {
		this->id = i;
	}
	int id;

	
};
//{id}{x}{y}{health}{fx}{fy}
class ClientData {
public:
	ClientData(std::vector<char>buf) {
		for (int i = 0; i < 4; i++) {
			bool atNumber = false;
			int number = 0;
			while (i < 4) {
				if (buf[i] == '}')
					break;
				if (std::isdigit(buf[i]))
					number * 10 + (buf[i] - 48);
				if (buf[i] == '{')
					atNumber = true;
			}
			if (i == 0) {
				id = number;
			}
			else if (i == 1) {
				x = number;
			}
			else if (i == 2) {
				y = number;
			}
			else if (i == 3) {
				health = number;
			}
		}
	}
	int x;
	int y;
	int health;
	int id;
};



#endif