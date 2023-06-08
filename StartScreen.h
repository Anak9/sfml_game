#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class StartScreen
{
private:
	sf::Text creditsTex;
	sf::Text startTex;

public:
	void initGUI();
};