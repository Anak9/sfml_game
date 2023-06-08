#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "utils.h"


class Player
{
private:
	// variables
	sf::Texture texture;
	sf::Sprite sprite;
	sf::IntRect currentFrame;

	sf::Clock animationTimer;

	float movementSpeed;
	float cooldownAttack;
	float cooldownAttackMax;
	int lives;
	
	State state;

	// private functions
	void initVariables();
	void initTexture();
	void initSprite();

public:
	Player();
	virtual ~Player();

	// accessors
	const sf::Vector2f& getPosition() const;
	const sf::FloatRect getBounds() const;
	const int getNumLives() const { return this->lives; }

	// modifiers
	void setPosition(const float x, const float y);

	// public functions
	const bool canAttack();
	void movePlayer(const float dirX, const float dirY);
	void setState(State state) { this->state = state; }
	
	void updateAttackCooldown();
	void update();
	
	void render(sf::RenderTarget& target);
};