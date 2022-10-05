#pragma once
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <filesystem>
#include <iostream>

namespace utils {
	void screenshot(sf::RenderWindow& window, int fractal, sf::RectangleShape& rect, sf::Shader& shader);
	void loadTexture(sf::Texture &blue, sf::Texture &orange, sf::Texture &rainbow, sf::Texture &blue_orange, sf::Texture& custom);
	void manageImGui(int& fractal, int& whichColor, float& r, float& n, sf::RenderWindow& window, sf::RectangleShape& rect, sf::Shader& shader, int& precision, int& width, int& height);
}