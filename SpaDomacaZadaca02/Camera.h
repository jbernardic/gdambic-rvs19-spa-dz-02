#pragma once
#include <SFML/Graphics.hpp>
class Camera
{
public:
	Camera(sf::Vector3f initialPosition, float speed);
	void update(float deltaTime);
	sf::Vector3f position;
	float speed;
};

