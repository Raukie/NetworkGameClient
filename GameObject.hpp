
#include "sfml/Graphics.hpp";


#ifndef GameObject_H_INCLUDED
#define GameObject_H_INCLUDED




namespace OBJ {
	
	class GameObject {
	public:

		GameObject (std::string tag) {
			ObjectTag = tag;
			Force = sf::Vector2f(0, 0);
			ForceThisFrame = sf::Vector2f(0, 0);
		}
		
		char Type;
		bool Invalid = false;
		std::string ObjectTag;
		sf::Vector2f Force;
		sf::Vector2f Pos;
		sf::Vector2f Scale;
		sf::Vector2f ForceThisFrame;
		bool Kinectic = false;


		void MoveTo(sf::Vector2f vec2) {
			Pos = vec2;
		}

		void SetScale(int x, int y) {
			Scale = sf::Vector2f(x, y);
		}

		void SetForce(sf::Vector2f vec2) {
			Kinectic = true;
			Force = vec2;
		}

		void AddForce(sf::Vector2f vec2) {
			Kinectic = true;
			Force += vec2;
			ForceThisFrame = vec2;
		}
		
		float DistanceToVector(sf::Vector2f vec2) {
			sf::Vector2f dif = Pos - vec2;
			float d = std::abs(dif.x);
			float y = std::abs(dif.y);
			return std::sqrt(d*d + y*y);
		}

		std::vector<OBJ::GameObject*> GetObjectsByTag(std::vector<OBJ::GameObject*>& Objects, std::string tag) {
			std::vector<OBJ::GameObject*> foundObjects;
			for (int i = 0; i < Objects.size(); i++) {
				OBJ::GameObject* Object = Objects[i];
				if (Object->ObjectTag == tag) {
					
					foundObjects.push_back(Object);
				}
			}
			return foundObjects;
		}

		virtual void Update(std::vector<OBJ::GameObject*> &objects, float delta) {

		}

		virtual void Draw(sf::RenderWindow& Window) {

		}

		virtual ~GameObject() {

		}
	};

	class Scene {
	public:
		Scene(std::vector<OBJ::GameObject*> objects) {
			Objects = objects;
		}
		std::vector<GameObject*> Objects;
	};
}


#endif // GameObject_H_INCLUDED