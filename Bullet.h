#pragma once

#include <SFML/Graphics.hpp>


class Bullet
{
private:
	sf::Sprite shape;
	sf::Vector2f direction;
	float movementSpeed;

public:

	// constructors | desctructors
	Bullet();
	Bullet(sf::Texture* texture, float pos_x, float pos_y, float move_speed, float dir_x, float dir_y, float angle); // underline cause these are local var, not class var
	virtual ~Bullet();

	// accessor
	const sf::FloatRect getBounds() const;

	void update();
	void render(sf::RenderTarget& target);
};

