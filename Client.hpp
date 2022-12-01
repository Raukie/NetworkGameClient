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
		int c = 0;
		for (int i = 0; i < 4; i++) {
			bool atNumber = false;
			int number = 0;
			bool atPoint = false;
			bool negative = false;
			while (i < 4) {
				if (buf[c] == '}') {
					c++;
					break;
				}
					
				if (std::isdigit(buf[c])) {
					if (!atPoint) {
						number = number * 10 + (buf[c] - 48);
					}
					else {
						number = number + ((-c) * 10) * (buf[c] - 48);
					}
					
				}
				else if (buf[c] == '{') {
					atNumber = true;
				}
				else if (buf[c] == '-') {
					negative = true;
				}
					
					
				if (buf[c] == '.') {
					atPoint = true;
				}
				c++;
			}
			if (negative)
				number = (-1) * number;
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