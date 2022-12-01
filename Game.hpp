
#include "SFML/Graphics.hpp";
#include "GameObject.hpp"

#ifndef Game_H_INCLUDED
#define Game_H_INCLUDED
class Game {
public:
	Game(sf::RenderWindow& window, sf::View& vw) : Window(window), View(vw) {

	}


	std::vector<OBJ::GameObject*> GameObjects = {
		
	};
	std::vector<OBJ::GameObject*> UiObjects = {

	};

	bool follow = false;
	OBJ::GameObject* target;

	void LoadScene(OBJ::Scene scene) {
		for (int i = 0; i < scene.Objects.size(); i++) {
			GameObjects.push_back(scene.Objects[i]);
		}
	}

	virtual void Update(float delta) {
		Window.clear();

		//All the ui ui objects
		Window.setView(Window.getDefaultView());
		for (int i = 0; i < UiObjects.size(); i++) {
			OBJ::GameObject* Object = UiObjects[i];
			Object->Draw(Window);
			Object->Update(GameObjects, delta);
		}

		//All the game objects
		Window.setView(View);
		if (follow)
			View.setCenter(target->Pos);
		for (int i = 0; i < GameObjects.size(); i++) {
			
			OBJ::GameObject* Object = GameObjects[i];
			Object->Draw(Window);
			Object->Update(GameObjects, delta);
			
			
		}
		Window.display();
		
	}

	//get all objects with tag return as reference for direct editing and moving of object
	std::vector<OBJ::GameObject*> GetObjectsByTag(std::string tag) {
		std::vector<OBJ::GameObject*> foundObjects;
		for (int i = 0; i < GameObjects.size(); i++) {
			OBJ::GameObject* Object = GameObjects[i];
			if (Object->ObjectTag == tag) {
				foundObjects.push_back(Object);
			}
		}
		return foundObjects;
	}

private:
	sf::RenderWindow& Window;
	sf::View& View;
};


#endif