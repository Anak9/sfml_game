#include "Game.h"

void Game::initSystem()
{
	this->points = 0;

	std::ifstream file("Resources/highscore.txt");
	if (file.is_open())
	{
		file >> this->highestScore;
	}
	file.close();

	this->endGame = false;
	this->pauseGame = false;
}

// private functions
void Game::initWindow()
{
	/**
		@return void

	*/

	this->window = new sf::RenderWindow(sf::VideoMode(630, 700), "Galaxian", sf::Style::Close | sf::Style::Titlebar);
	this->window->setFramerateLimit(144);
	this->window->setVerticalSyncEnabled(false);

	this->animationTimer.restart();
}

void Game::initTextures()
{
	/**
		@return void

	*/
	this->textures["BULLET"] = new sf::Texture;
	this->textures["BULLET"]->loadFromFile("Textures/bullet.png");
}

void Game::initWorld()
{
	// init background
	if (!this->worldBackgroundTex.loadFromFile("Textures/background_sheet.png"))
	{
		std::cout << "ERROR::GAME:: Failed to load background file\n";
	}
	else
	{
		this->worldBackground.setTexture(this->worldBackgroundTex);
		//this->worldBackground.scale(1.5f, 1.5f);
		this->currentBackgroundFrame = sf::IntRect(0, 0, 630, 700);
		this->worldBackground.setTextureRect(this->currentBackgroundFrame);
	}

	// init backup ships
	if (!this->backupShipTex.loadFromFile("Textures/backup.png"))
	{
		std::cout << "ERROR::GAME:: Failed to load backupship image\n";
	}

	this->backupShip.setTexture(this->backupShipTex);
	this->backupShip.scale(0.5f, 0.5f);
}

void Game::initGUI()
{
	// load fonts
	if (!this->font.loadFromFile("Fonts/PixellettersFull.ttf"))
	{
		std::cout << "ERROR::GAME:: Failed to load font\n";
	}

	// init static text
	this->staticTextHighScore.setFont(this->font);
	this->staticTextHighScore.setCharacterSize(32);
	this->staticTextHighScore.setFillColor(sf::Color::Red);
	this->staticTextHighScore.setString("HIGH SCORE");
	this->staticTextHighScore.setPosition(this->window->getSize().x / 2.f - this->staticTextHighScore.getGlobalBounds().width / 2.f, 5);

	this->staticTextScore.setFont(this->font);
	this->staticTextScore.setCharacterSize(32);
	this->staticTextScore.setFillColor(sf::Color::Red);
	this->staticTextScore.setString("SCORE");
	this->staticTextScore.setPosition(50, 5);

	this->pauseGameTex.setFont(this->font);
	this->pauseGameTex.setCharacterSize(32);
	this->pauseGameTex.setFillColor(sf::Color::Red);
	this->pauseGameTex.setString("PAUSE");
	this->pauseGameTex.setPosition(
		this->window->getSize().x / 2.f - this->pauseGameTex.getGlobalBounds().width / 2.f,
		this->window->getSize().y / 2.f - this->pauseGameTex.getGlobalBounds().height / 2.f);

	// high score
	this->highestScoreTex.setFont(this->font);
	this->highestScoreTex.setCharacterSize(32);
	this->highestScoreTex.setFillColor(sf::Color::White);
	this->highestScoreTex.setString(this->highestScore);
	this->highestScoreTex.setPosition(this->window->getSize().x / 2.f - this->highestScoreTex.getGlobalBounds().width / 2.f, 30);

	// game over text
	this->gameOverText.setFont(this->font);
	this->gameOverText.setCharacterSize(64);
	this->gameOverText.setFillColor(sf::Color::Red);
	this->gameOverText.setString("Game Over");
	// center in middle of screen
	this->gameOverText.setPosition(
		this->window->getSize().x / 2.f - this->gameOverText.getGlobalBounds().width / 2.f, 
		this->window->getSize().y / 2.f - this->gameOverText.getGlobalBounds().height / 2.f);

	// init points text
	this->pointsText.setFont(this->font);
	this->pointsText.setCharacterSize(32);
	this->pointsText.setFillColor(sf::Color::White);

	std::stringstream ss;
	ss >> this->points;
	this->pointsText.setString(ss.str());
	this->pointsText.setPosition(50, 30);
}

void Game::initPlayer()
{
	this->player = new Player();
}

void Game::initEnemies()
{
	this->spawnTimerMax = 100.f;
	this->spawnTimer = this->spawnTimerMax;
	this->enemyAttackTimer = 1900;
}

void Game::initFirstEnemyWave()
{
	float x = 145;
	float y = 201;

	// create 30 enemies type 0
	for (unsigned int j = 0; j < 3; j++)
	{
		for (unsigned int i = 0; i < 10; i++)
		{
			this->enemies.push_back(new Enemy(x, y, 0));
			x += 32;
		}
		x = 145;
		y += 32;
	}

	// create 8 enemies type 1
	x = 177;
	y = 169;
	for (unsigned int j = 0; j < 8; j++)
	{
		this->enemies.push_back(new Enemy(x, y, 1));
		x += 32;
	}

	// create 6 enemies type 2
	x = 209;
	y = 137;
	for (unsigned int j = 0; j < 6; j++)
	{
		this->enemies.push_back(new Enemy(x, y, 2));
		x += 32;
	}

	// create 4 enemies type 3
	x = 241;
	y = 105;
	for (unsigned int j = 0; j < 4; j++)
	{
		this->enemies.push_back(new Enemy(x, y, 3));
		x += 32;
	}
}

// Con / Des
Game::Game()
{
	/**
		@return void

	*/
	this->initWindow();
	this->initSystem();
	this->initTextures();
	this->initPlayer();
	this->initEnemies();
	this->initGUI();
	this->initWorld();

	this->initFirstEnemyWave();
}

Game::~Game()
{
	/**
		@return void

	*/
	delete this->window;
	delete this->player;

	// remove textures
	for (auto& tex : this->textures)
		delete tex.second;

	// remove bullets
	for (auto* bullet : this->bullets)
		delete bullet;

	for (auto* bullet : this->enemiesBullets)
		delete bullet;

	// remove enemies
	for (auto* enemy : this->enemies)
		delete enemy;

	for (auto* enemy : this->enemiesAttacking)
		delete enemy;
}

// public functions

void Game::printGameInformation()
{
	std::cout << "PLAYER\n";
	std::cout << "Position:  ( " << this->player->getBounds().left << ", " << this->player->getBounds().top << " )\n";
	std::cout << "Velocity vector:  speed = 1.f      |      direction = (3.f, 0)\n";



	std::cout << "\nPLAYER BULLETS:\n";
	for (auto& bullet : this->bullets)
	{
		std::cout << "Position:  ( " << bullet->getBounds().left << ", " << bullet->getBounds().top << " )";
		std::cout << "Velocity vector: -- \n";
	}




	std::cout << "\n\nENEMIES:\n";
	for (auto& enemy : this->enemies)
	{	
		std::cout << "Position:  ( " << enemy->getBounds().left << ", " << enemy->getBounds().top << " )";
		std::cout << "Velocity vector: -- \n";
	}
	for (auto& enemy : this->enemiesAttacking)
	{
		std::cout << "Position:  ( " << enemy->getBounds().left << ", " << enemy->getBounds().top << " )";
		std::cout << "Velocity vector:  speed (-) = 1.f      |      direction = (3.f, 0)\n";
	}




	std::cout << "\n\nENEMIES BULLETS:\n";
	for (auto& bullet : this->enemiesBullets)
	{
		std::cout << "Position:  ( " << bullet->getBounds().left << ", " << bullet->getBounds().top << " )";
		std::cout << "Velocity vector: -- \n";
	}
	
}

void Game::updatePollEvents()
{	
	/**
		@return void

	*/

	// close window
	sf::Event e;
	while (this->window->pollEvent(e))
	{
		// close game
		if ((e.Event::type == sf::Event::Closed) || (e.Event::type == sf::Event::KeyPressed && e.Event::key.code == sf::Keyboard::Escape))
			this->window->close();

		// pause game
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
			if (this->pauseGame == false)
				this->pauseGame = true;
			else
				this->pauseGame = false;
		
		// pause and debug game
		if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
		{
			this->pauseGame = true;

			printGameInformation();
		}
	}
}

void Game::handleBulletWindowCollision()
{
	/**
		@return void

		Removes bullets that are not inside window anymore
	*/

	unsigned couter = 0;
	for (auto* bullet : this->bullets)
	{
		// check if bullet left the screen
		if (bullet->getBounds().top + bullet->getBounds().height < 0.f)
		{
			// remove bullet
			delete this->bullets.at(couter);
			this->bullets.erase(this->bullets.begin() + couter);
			--couter;
		}
		++couter;
	}

	// enemies bullets
	couter = 0;
	for (auto* bullet : this->enemiesBullets)
	{
		// check if bullet left the screen
		if (bullet->getBounds().top > this->window->getSize().y)
		{
			// remove bullet
			delete this->enemiesBullets.at(couter);
			this->enemiesBullets.erase(this->enemiesBullets.begin() + couter);
			--couter;
		}
		++couter;
	}
}

void Game::updateBullets()
{
	for (auto* bullet : this->bullets)
	{
		bullet->update();
	}

	for (auto* bullet : this->enemiesBullets)
	{
		bullet->update();
	}

	this->handleBulletWindowCollision();
}

void Game::updateInput()
{	/**
		@return void

		Moves player and shoots bullets
	*/

	// move player using W-A-S-D
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		this->player->movePlayer(-1.f, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		this->player->movePlayer(1.f, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		this->player->movePlayer(0.f, -1.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		this->player->movePlayer(0.f, 1.f);

	// shoot bullet when clicking left mouse button
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->player->canAttack())
	{
		// create bullet and add it to bullets vector
		this->bullets.push_back(
			new Bullet(this->textures["BULLET"], 
				this->player->getPosition().x + 15.f, 
				this->player->getPosition().y - 10.f, 
				3.f, 0.f, -1.f, 0
		));
	}
}

void Game::spawnEnemies()
{
	if (this->spawnTimer >= this->spawnTimerMax)
	{
		// reset timer
		this->spawnTimer = 0.f;

		// create new enemy
		float pos_x = 100;
		float pos_y = 100;

		this->enemies.push_back(new Enemy(pos_x, pos_y, 0));
	}
	else 
	{
		// increase timer
		this->spawnTimer += 1.f;
	}
}

void Game::updateEnemyAttack()
{
	if (this->enemyAttackTimer >= 1100)
	{
		// select one random enemy(ies) to attack
		int enemyIndex = rand() % this->enemies.size();
		this->enemies[enemyIndex]->setState(State::SPINNING);

		// change enemy to attacking vector
		Enemy* enemyPtr = this->enemies[enemyIndex];
		this->enemies.erase(this->enemies.begin() + enemyIndex);
		this->enemiesAttacking.push_back(enemyPtr);

		// reset timer
		this->enemyAttackTimer = 0;
	}
	else
	{
		// increase timer
		this->enemyAttackTimer += 1;
	}

	// handle enemy bullets
	if (this->enemyAttackTimer % 200 == 0)
	{
		for (auto& enemy : this->enemiesAttacking)
		{
			if (enemy->getBounds().top < this->window->getSize().y - 200 && enemy->getBounds().top > 100 )
			{
				// CREATE BULLET
				float speed = 3.f;

				float dirX = this->player->getBounds().left - enemy->getBounds().left;
				float dirY = this->player->getBounds().top - enemy->getBounds().top;

				float angle = atan(dirX / dirY) * (180.0 / 3.14159265359) + 90;
				if (angle > 45)
				{
					angle = dirX < 0 ? 20 : -20;
				}

				this->enemiesBullets.push_back(new Bullet(
					this->textures["BULLET"],
					enemy->getBounds().left + 15.f,
					enemy->getBounds().top + 15.f,
					speed, dirX / speed / 400, dirY / speed / 400, angle)
				);
			}
		}
	}
}

void Game::updateEnemies()
{
	// check bullet collision
	for (unsigned int i = 0; i < this->enemies.size(); i++)
	{
		// remove enemy if it is dead (explosion animation finished)
		if (this->enemies[i]->getState() == State::DEAD)
		{
			delete this->enemies[i];
			this->enemies.erase(this->enemies.begin() + i);
		}
		else
		{
			bool bulletRemoved = false;
			// check bullet - enemy collision
			for (size_t j = 0; j < this->bullets.size() && !bulletRemoved; j++)
			{
				if (this->bullets[j]->getBounds().intersects(this->enemies[i]->getBounds()))
				{

					this->points += 50;

					this->enemies[i]->setState(State::EXPLODING);

					// remove bullet
					delete this->bullets[j];
					this->bullets.erase(this->bullets.begin() + j);
					bulletRemoved = true;
				}
			}

			// update
			this->enemies[i]->update();
		}
	}
}

void Game::updateEnemiesAttacking()
{
	for (unsigned int i = 0; i < this->enemiesAttacking.size(); i++)
	{
		// KILL ENEMY - remove enemy if it is dead (explosion animation finished)
		if (this->enemiesAttacking[i]->getState() == State::DEAD)
		{
			delete this->enemiesAttacking[i];
			this->enemiesAttacking.erase(this->enemiesAttacking.begin() + i);
		}
		else
		{
			// check BULLET COLLISION
			bool removed = false;
			for (size_t j = 0; j < this->bullets.size() && !removed; j++)
			{
				if (this->bullets[j]->getBounds().intersects(this->enemiesAttacking[i]->getBounds()))
				{
					this->points += 50;

					this->enemiesAttacking[i]->setState(State::EXPLODING);

					// remove bullet
					delete this->bullets[j];
					this->bullets.erase(this->bullets.begin() + j);
					removed = true;
				}
			}

			// UPDATE
			this->enemiesAttacking[i]->update();

			// rotation
			//float angle = this->player->getBounds().left - this->enemiesAttacking[i]->getBounds().left > 0 ? -10.f : 10.f;
			//this->enemiesAttacking[i]->rotate(angle);

			// check PLAYER COLLISION
			if (this->enemiesAttacking[i]->getBounds().intersects(this->player->getBounds()))
			{
				this->enemiesAttacking[i]->setState(State::EXPLODING);

				// remove enemy from enemiesAttacking vector and puts it back in enemies vector
				this->enemies.push_back(this->enemiesAttacking[i]);
				this->enemiesAttacking.erase(this->enemiesAttacking.begin() + i);
				removed = true;

				this->player->setState(State::EXPLODING);
			}

			// WINDOW COLLISION - remove enemy if it reached the bottom of screen 
			if (!removed && this->enemiesAttacking[i]->getBounds().top > this->window->getSize().y)
			{
				this->enemiesAttacking.erase(this->enemiesAttacking.begin() + i);
				std::cout << "number of enemies: " << this->enemiesAttacking.size() << "\n";
			}

		}
	}
}

void Game::updateEnemiesBullets()
{
	// check PLAYER ENEMY BULLET COLLISION
	bool removed = false;
	for (size_t j = 0; j < this->enemiesBullets.size() && !removed; j++)
	{
		if (this->enemiesBullets[j]->getBounds().intersects(this->player->getBounds()))
		{
			this->player->setState(State::EXPLODING);

			// remove bullet
			delete this->enemiesBullets[j];
			this->enemiesBullets.erase(this->enemiesBullets.begin() + j);
			removed = true;
			break;
		}
	}
}

void Game::updateGUI()
{
	std::stringstream ss;
	ss << this->points;
	this->pointsText.setString(ss.str());

	if (this->points >= std::stoi(this->highestScore)) 
	{
		this->highestScoreTex.setString(ss.str());
	}
}

void Game::updateWorld()
{
	// background
	if (this->animationTimer.getElapsedTime().asSeconds() >= 0.5f)
	{
		this->currentBackgroundFrame.left += 630;
		if (this->currentBackgroundFrame.left > 3150.f)
		{
			this->currentBackgroundFrame.left = 0;
		}

		this->worldBackground.setTextureRect(this->currentBackgroundFrame);
		this->animationTimer.restart();
	}
}

// not allow player to leave window
void Game::updatePlayerWindowCollision()
{
	// horizontal
	if (this->player->getBounds().left < 0.f)
	{
		this->player->setPosition(0.f, this->player->getBounds().top);
	}
	else if (this->player->getBounds().left > this->window->getSize().x - this->player->getBounds().width)
	{
		this->player->setPosition(this->window->getSize().x - this->player->getBounds().width, this->player->getBounds().top);
	}

	// vertical
	if (this->player->getBounds().top < 0.f)
	{
		this->player->setPosition(this->player->getBounds().left, 0.f);
	}
	else if (this->player->getBounds().top > this->window->getSize().y - this->player->getBounds().height)
	{
		this->player->setPosition(this->player->getBounds().left, this->window->getSize().y - this->player->getBounds().height);
	}
}

void Game::updatePlayer()
{
	this->player->update();
	this->updatePlayerWindowCollision();

	if (this->player->getNumLives() == 0)
		this->endGame = true;
}

void Game::update()
{
	/**
		@return void

	*/

	this->updateWorld();
	this->updateInput();
	this->updatePlayer();
	this->updateBullets();
	this->updateEnemies();
	this->updateEnemyAttack();
	this->updateEnemiesAttacking();
	this->updateEnemiesBullets();
	this->updateGUI();

	//this->spawnEnemies();
}

void Game::renderWorld()
{
	this->window->draw(this->worldBackground);

	if (this->player->getNumLives() > 1)
	{
		// backup ships
		this->backupShip.setPosition(90.f, 630.f);
		for (int i = 0; i < this->player->getNumLives() - 1; i++)
		{
			// render backup ships
			this->window->draw(this->backupShip);
			this->backupShip.setPosition(130.f, 630.f);
		}
	}
}

void Game::renderEnemies()
{
	for (auto* enemy : this->enemies)
	{
		enemy->render(*this->window);
	}

	for (auto* enemy : this->enemiesAttacking)
	{
		enemy->render(*this->window);
	}
}

void Game::renderPlayer()
{
	
}

void Game::renderBullets()
{
	for (auto* bullet : this->bullets)
	{
		bullet->render(*this->window);
	}

	for (auto* bullet : this->enemiesBullets)
	{
		bullet->render(*this->window);
	}
}

void Game::renderGUI()
{
	this->window->draw(this->pointsText);
	this->window->draw(this->staticTextHighScore);
	this->window->draw(this->staticTextScore);
	this->window->draw(this->highestScoreTex);

	if (this->player->getNumLives() == 0)
		this->window->draw(this->gameOverText);

	if (this->pauseGame)
		this->window->draw(this->pauseGameTex);
}

void Game::render()
{
	/**
		@return void

		- clear old frame
		- render objects
		- display frame in window

		Render the game's objects
	 */

	this->window->clear();

	// draw everything
	this->renderWorld();


	this->player->render(*this->window);
	this->renderEnemies();
	this->renderBullets();
	this->renderGUI();
	
	this->window->display();
	
}

void Game::run()
{
	/**
		@return void

		Game loop
	*/

	startFirstPage(*this->window);

	while (this->window->isOpen())
	{

		if (!this->endGame && !this->pauseGame)
		{
			this->update();
		}

		// window options, like closing button
		this->updatePollEvents();

		this->render();

		if (this->endGame)
		{
			if (this->points > std::stoi(this->highestScore))
			{
				std::ofstream file("Resources/highscore.txt");
				if (file.is_open())
				{
					file << this->points;
				}

				file.close();
			}
		}
	}
}