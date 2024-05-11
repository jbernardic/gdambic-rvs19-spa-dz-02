#include "Camera.h"

Camera::Camera(sf::Vector3f initialPosition, float speed) : position(initialPosition), speed(speed) {}

void Camera::update(float deltaTime)
{
	float speed = deltaTime * this->speed;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		position.x += -speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		position.x += speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		position.y += speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		position.y += -speed;

	}
}
