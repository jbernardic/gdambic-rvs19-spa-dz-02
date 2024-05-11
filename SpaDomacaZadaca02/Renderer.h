#pragma once
#include <SFML/Graphics.hpp>
#include <array>
class Renderer
{
public:
	Renderer(sf::RenderWindow& window);
	void render(sf::Vector3f camera_position, sf::Vector2f mousePosition);
	void update(std::vector<std::vector<bool>>& grid);
	void updateAt(int x, int y, bool value);
private:
	sf::RenderWindow& window;
	sf::Shader shader;
	sf::RectangleShape rect;
	sf::RectangleShape background;
	sf::Texture texture;
	sf::Texture backgroundTexture;
	sf::Font font;
	sf::Text text;
};

