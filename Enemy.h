#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>

#include "utils.h"

class Enemy
{
private:
	int hp;
	int hpMax;
	int type;
	int damage;
	int points;
	int state;


	//sf::CircleShape shape;
	sf::Texture texture;
	sf::Sprite sprite;
	sf::IntRect currentFrame;
	sf::Clock animationTimer;

	void initVariables();
	//void initShape();
	void initTexture();
	void initSprite();

	public:
		Enemy(float pos_x, float pos_y, int type);
		virtual ~Enemy();

		// accessors
		sf::FloatRect getBounds();
		int getState() { return this->state; }
		void setState(State state) { this->state = state; }
		void rotate(float angle) { this->sprite.setRotation(angle); }

		void updateFlyAnimation();
		void updateExplosionAnimation();
		void updateAttackAnimation();
		void updateSpinningAnimation();
		void update();
		void render(sf::RenderTarget& target);
};


#endif // !ENEMY_H