#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include "inkrenderer.h"
#include <ctime>
#include "Camera.h"
#include "Renderer.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Game Of Life");
	window.setFramerateLimit(60);

	srand(time(0));

	sf::Clock clock;
	sf::Time deltaTime;
	float updateTimer = 0.0;
	bool stop = false;
	std::thread thread;

	Renderer renderer(window);

	std::vector<std::vector<bool>> grid(400, std::vector<bool>(400));
	Camera camera(sf::Vector3f(grid.size() / 2, grid.size() / 2, -10), 10.0f);

	for (int i = 0; i < grid.size(); i++)
	{
		for (int j = 0; j < grid.size(); j++)
		{
			grid[i][j] = (rand() % 4 == 0);
		}
	}
	renderer.update(grid);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
				{
					for (int i = 0; i < grid.size(); i++)
					{
						for (int j = 0; j < grid.size(); j++)
						{
							grid[i][j] = 0;
						}
					}
					renderer.update(grid);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
				{
					stop = !stop;
				}
			}
			if (event.type == sf::Event::MouseWheelMoved)
			{
				camera.position.z += event.mouseWheel.delta * abs(camera.position.z / 10.0);
			}
		}

		camera.update(deltaTime.asSeconds());

		sf::Vector2f mousePosition = sf::Vector2f(sf::Mouse::getPosition(window));
		sf::Vector3f mouseDirection = sf::Vector3f(800 / 2, 600 / 2, 400.0) - sf::Vector3f(mousePosition.x, mousePosition.y, 1.0);
		float normaliziraj = sqrt(pow(mouseDirection.x, 2) + pow(mouseDirection.y, 2));
		mouseDirection = mouseDirection / normaliziraj;
		mouseDirection.x = -mouseDirection.x;

		sf::Vector3i space_mouse = sf::Vector3i(camera.position + ((-camera.position.z / mouseDirection.z)) * mouseDirection);

		if (space_mouse.y >= 0 && space_mouse.y < grid.size() &&
			space_mouse.x >= 0 && space_mouse.x < grid.size())
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !grid[space_mouse.y][space_mouse.x])
			{
				grid[space_mouse.y][space_mouse.x] = true;
				renderer.updateAt(space_mouse.x, space_mouse.y, true);
			}
		}

		if (updateTimer >= 2.0 && !stop)
		{
			if (thread.joinable())
			{
				thread.join();
			}
			updateTimer = 0.0;
			//Game of life ...
			thread = std::thread([&]() {
				auto prev = grid;

				for (int i = 0; i < prev.size(); i++)
				{
					for (int j = 0; j < prev.size(); j++)
					{
						int n_count = 0;
						for (int x = -1; x <= 1; x++)
						{
							for (int y = -1; y <= 1; y++)
							{
								if (i + x >= 0 && i + x < prev.size() && j + y >= 0 && j + y < prev.size())
								{
									if (!(x == 0 && y == 0)) n_count += prev[i + x][j + y];
								}
							}
						}
						if (n_count < 2 || n_count > 3)
						{
							grid[i][j] = false;
						}
						else if (n_count == 3)
						{
							grid[i][j] = true;
						}
					}
				}
				renderer.update(grid);
				std::cout << updateTimer << std::endl;
			});
		}
		renderer.render(camera.position, mousePosition);
		deltaTime = clock.restart();
		updateTimer += deltaTime.asSeconds();
	}
	if (thread.joinable())
		thread.detach();
	return 0;
}