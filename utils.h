#pragma once
#include <iostream>
enum State { IDLE = 0, EXPLODING, SPINNING, ATTACKING, DEAD };

static void startFirstPage(sf::RenderWindow& window)
{
	// load fonts
	sf::Font font;
	sf::Text credits;

	if (!font.loadFromFile("Fonts/PixellettersFull.ttf"))
	{
		std::cout << "ERROR::GAME:: Failed to load font\n";
	}

	credits.setFont(font);
	credits.setCharacterSize(32);
	credits.setFillColor(sf::Color::White);
	credits.setString("Trabalho Pratico I  -  GALAXIAN");
	credits.setPosition(window.getSize().x / 2.f - credits.getGlobalBounds().width / 2.f, 40);
	window.draw(credits);

	credits.setString("Computacao Grafica 2023 / 01");
	credits.setPosition(window.getSize().x / 2.f - credits.getGlobalBounds().width / 2.f, 80);
	window.draw(credits);

	credits.setString("Sarah Oliveira Elias");
	credits.setPosition(window.getSize().x / 2.f - credits.getGlobalBounds().width / 2.f, 120);
	window.draw(credits);

	credits.setString("PUSH START BUTTON");
	credits.setCharacterSize(40);
	credits.setFillColor(sf::Color::Blue);
	credits.setPosition(window.getSize().x / 2.f - credits.getGlobalBounds().width / 2.f, 350);
	window.draw(credits);
	
	window.display();

	bool nextPAge = false;
	while (!nextPAge)
	{
		sf::Event e;
		while (window.pollEvent(e))
		{
			// close game
			if ((e.Event::type == sf::Event::KeyPressed && e.Event::key.code == sf::Keyboard::Enter))
				nextPAge = true;
				break;
		}
	}
}