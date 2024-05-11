#include "Renderer.h"
#include <iostream>

void Renderer::render(sf::Vector3f cameraPosition, sf::Vector2f mousePosition)
{
	shader.setUniform("camera_position", cameraPosition);
	shader.setUniform("mouse_position", mousePosition);
	window.clear();
	window.draw(background);
	window.draw(rect, &shader);
	window.draw(text);
	window.display();
}

void Renderer::update(std::vector<std::vector<bool>>& grid)
{
	int size = grid.size();
	if (size != grid[0].size())
	{
		std::cout << "ERROR: Grid size not square" << std::endl;
		return;
	}

	std::vector<sf::Uint8> pixels(size*size*4);
	for (int x = 0; x < size; x++)
	{
		for (int y = 0; y < size; y++)
		{
			int i = x * size + y;
			pixels[i * 4 + 0] = grid[x][y] ? 255 : 0;
			pixels[i * 4 + 1] = grid[x][y] ? 255 : 0;
			pixels[i * 4 + 2] = grid[x][y] ? 255 : 0;
			pixels[i * 4 + 3] = grid[x][y] ? 255 : 0;
		}
	}
	if (texture.getSize() != sf::Vector2u(size, size))
	{
		texture.create(size, size);
	}
	texture.update(pixels.data());
	shader.setUniform("grid_size", size);
}

void Renderer::updateAt(int x, int y, bool value)
{
	sf::Uint8 val = value ? 255 : 0;
	texture.update(&val, 1, 1, x, y);
}

Renderer::Renderer(sf::RenderWindow& window) : window(window), rect(sf::Vector2f(window.getSize())), background(sf::Vector2f(window.getSize()))
{

	text.setFont(font);
	text.setString("WASD - move\nScroll Wheel - zoom\nLeft Click - paint\nF - Pause\nC - Clear");
	text.setCharacterSize(20);
	text.setPosition(sf::Vector2f(10, 10));
	text.setFillColor(sf::Color::White);
	text.setOutlineColor(sf::Color::Black);
	text.setOutlineThickness(0.75);

	if (!shader.loadFromFile("vertex_shader.vert", "fragment_shader.frag"))
	{
		std::cout << "Error" << std::endl;
	}
	if (!font.loadFromFile("font.ttf"))
	{
		std::cout << "Error" << std::endl;
	}

	texture.create(128, 128);
	rect.setTexture(&texture);
	rect.setTextureRect(sf::IntRect(0, 0, 128, 128));

	backgroundTexture.loadFromFile("galaxy.jpg");
	background.setTexture(&backgroundTexture);

	shader.setUniform("grid_size", 128);
}
