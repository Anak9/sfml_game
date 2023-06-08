#include "Player.h"

void Player::initVariables()
{
	this->movementSpeed = 1.f;

	this->cooldownAttackMax = 10.f;
	this->cooldownAttack = this->cooldownAttackMax;

	this->lives = 3;

	this->state = State::IDLE;
}

void Player::initTexture()
{
	// load texture from file
	if (!this->texture.loadFromFile("Textures/player.png")) 
	{
		std::cout << "ERROR::PLAYER::INITTEXTURE:: Could not load texture file."<< "\n";
	}
}

void Player::initSprite()
{
	this->sprite.setTexture(this->texture);
	this->currentFrame = sf::IntRect(0, 0, 128, 128);
	this->sprite.setTextureRect(this->currentFrame);
	this->sprite.scale(0.5f, 0.5f);
	this->sprite.setPosition(315, 570);
}

Player::Player()
{
	this->initVariables();
	this->initTexture();
	this->initSprite();
}

Player::~Player()
{
}

const sf::Vector2f& Player::getPosition() const
{
	return this->sprite.getPosition();
}

const sf::FloatRect Player::getBounds() const
{
	return this->sprite.getGlobalBounds();
}

void Player::setPosition(const float x, const float y)
{
	this->sprite.setPosition(x, y);
}

const bool Player::canAttack()
{
	if (this->cooldownAttack >= this->cooldownAttackMax) {
		this->cooldownAttack = 0.f;
		return true;
	}
	return false;
}

void Player::movePlayer(const float dirX, const float dirY)
{
	this->sprite.move(this->movementSpeed * dirX, this->movementSpeed * dirY);
}

void Player::updateAttackCooldown()
{
	/**
		@return void

		Defines period of time between shooting bullets
		Sets pause between shots
	*/

	if (this->cooldownAttack < this->cooldownAttackMax)
		this->cooldownAttack += 0.5f;
}



void Player::update()
{
	if (this->state == State::EXPLODING)
	{
		if (this->currentFrame.left == 0)
		{
			// start explosion animation
			this->animationTimer.restart();
			this->currentFrame.left = 128;
		}
		else 
		{
			// continue animation
			if (this->animationTimer.getElapsedTime().asSeconds() >= 0.3f)
			{
				this->currentFrame.left += 128;
				// stop animation
				if (this->currentFrame.left > 1408)
				{
					if (this->lives != 0) 
					{
						this->lives -= 1;
						this->state = State::IDLE;
						this->currentFrame.left = 0;
					}
					if (this->lives == 0)
					{
						this->currentFrame.left = 768.f;
					}
				
				}
				this->animationTimer.restart();
			}
		}
		this->sprite.setTextureRect(this->currentFrame);
	}
	else {
		// allows shooting
		this->updateAttackCooldown();
	}
}

void Player::render(sf::RenderTarget& target)
{
	target.draw(this->sprite);
}
