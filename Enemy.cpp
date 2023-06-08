#include "Enemy.h"
#include <iostream>

void Enemy::initVariables()
{
	this->hpMax    = 10;
	this->hp	   = 0;
	this->damage   = 1;
	this->points   = 5;
	this->state = State::IDLE;

	this->animationTimer.restart();
}

void Enemy::initTexture()
{
	std::string file;
	// load texture from file
	switch (this->type)
	{
		case 0:
			file = "Textures/enemy_0_sheet.png";
			break;
		case 1:
			file = "Textures/enemy_1_sheet.png";
			break;
		case 2:
			file = "Textures/enemy_2_sheet.png";
			break;
		case 3:
			file = "Textures/enemy_3_sheet.png";
			break;
	
	}

	if (!this->texture.loadFromFile(file))
	{
		std::cout << "ERROR::PLAYER::INITTEXTURE:: Could not load texture file." << "\n";
	};
}

void Enemy::initSprite()
{
	this->sprite.setTexture(this->texture);

	int randomPos = rand() % 4;
	this->currentFrame = sf::IntRect(randomPos * 64, 0, 64, 64);

	this->sprite.setTextureRect(this->currentFrame);
	this->sprite.scale(0.5f, 0.5f);
}



Enemy::Enemy(float pos_x, float pos_y, int type)
{
	this->type = type;
	this->initVariables();
	this->initTexture();
	this->initSprite();
	this->sprite.setPosition(pos_x, pos_y);
}

Enemy::~Enemy()
{
}

sf::FloatRect Enemy::getBounds()
{
	return this->sprite.getGlobalBounds();
}

void Enemy::updateFlyAnimation()
{
	if (this->animationTimer.getElapsedTime().asSeconds() >= 0.5f)
	{
		this->currentFrame.left += 64.f;
		if (this->currentFrame.left > 192.f)
		{
			this->currentFrame.left = 0;
		}

		this->sprite.setTextureRect(this->currentFrame);
		this->animationTimer.restart();
	}
}

void Enemy::updateExplosionAnimation()
{
	if (this->animationTimer.getElapsedTime().asSeconds() >= 0.1f)
	{
		if (this->currentFrame.left < 256.f || this->currentFrame.top != 0.f)
		{
			// start explosion animation
			this->currentFrame.left = 256.f;
			this->currentFrame.top = 0.f;
		}
		else if (this->currentFrame.left >= 448)
		{
			// end of animation - remove enemy
			this->state = State::DEAD;
		}
		else
		{
			this->currentFrame.left += 64.f;
		}

		this->sprite.setTextureRect(this->currentFrame);
		this->animationTimer.restart();
	}
}

void Enemy::updateSpinningAnimation()
{
	// start the animation
	if (this->currentFrame.top < 64.f)
	{
		this->currentFrame.top = 64.f;
		this->currentFrame.left = 0;
		this->sprite.setTextureRect(this->currentFrame);
	}
	else
	{
		// continue the animation
		if (this->animationTimer.getElapsedTime().asSeconds() >= 0.1f)
		{
			this->currentFrame.left += 64.f;

			// set frame
			if (this->currentFrame.left > 768)
			{
				// end the animation
				this->currentFrame.left -= 64.f;
				this->state = State::ATTACKING;
			}
			this->sprite.setTextureRect(this->currentFrame);

			// set position
			float x;
			float y;
			if (this->currentFrame.left >= 448.f)
			{
				// goes down and right 6.f
				x = this->sprite.getGlobalBounds().left + 6.f;
				y = this->sprite.getGlobalBounds().top + 6.f;
			}
			else
			{
				// goes up and right 6.f
				x = this->getBounds().left + 6.f;
				y = this->getBounds().top - 6.f;
			}
			this->sprite.setPosition(x, y);

			// restart timer
			this->animationTimer.restart();
		}
	}
}

void Enemy::updateAttackAnimation()
{
	if (this->animationTimer.getElapsedTime().asSeconds() >= 0.1f)
	{
		// falls - goes down
		this->sprite.setPosition(this->sprite.getGlobalBounds().left, this->sprite.getGlobalBounds().top + 6.f);

		// restart timer
		this->animationTimer.restart();
	}
}

void Enemy::update()
{
	switch (this->state)
	{
		case State::IDLE:
			this->updateFlyAnimation();
			break;
	
		case State::EXPLODING:
			this->updateExplosionAnimation();
			break;
	
		case State::ATTACKING:
			this->updateAttackAnimation();
			break;

		case State::SPINNING:
			this->updateSpinningAnimation();
			break;
	}
}

void Enemy::render(sf::RenderTarget& target)
{
	target.draw(this->sprite);
}
