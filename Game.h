#pragma once

#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include <math.h>

#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"

class Game
{
private:
	// variables
	sf::RenderWindow* window;
	sf::Event event;
	sf::Clock animationTimer;

	// resources
	std::map<std::string, sf::Texture*> textures; // pointer here
	std::vector<Bullet*> bullets;
	std::vector<Bullet*> enemiesBullets;

	// GUI
	sf::Font font;
	sf::Text pointsText;
	sf::Text staticTextHighScore;
	sf::Text staticTextScore;
	sf::Text highestScoreTex;
	sf::Text gameOverText;
	sf::Text pauseGameTex;

	// world
	sf::Texture worldBackgroundTex;
	sf::Sprite worldBackground;
	sf::IntRect currentBackgroundFrame;

	sf::Texture backupShipTex;
	sf::Sprite backupShip;


	// player
	Player* player;
	int lives;

	// system
	int points;
	std::string highestScore;
	bool endGame;
	bool pauseGame;

	// enemies
	std::vector<Enemy*> enemies;
	std::vector<Enemy*> enemiesAttacking;
	float spawnTimer;
	float spawnTimerMax;
	int enemyAttackTimer;

	// private functions
	void initSystem();
	void initWindow();
	void initTextures();
	void initWorld();
	void initGUI();
	void initPlayer();
	void initEnemies();
	void initFirstEnemyWave();

public:
	// constructors | destructors
	Game();
	virtual ~Game();

	// public functions
	void printGameInformation();
	void run();

	void updatePollEvents();
	void handleBulletWindowCollision();
	void updateBullets();
	void updateInput();
	void spawnEnemies();
	void updateEnemies();
	void updateEnemiesAttacking();
	void updateEnemyAttack();
	void updateEnemiesBullets();
	void updateGUI();
	void updateWorld();
	void updatePlayerWindowCollision();
	void updatePlayer();
	void update();

	void renderWorld();
	void renderEnemies();
	void renderPlayer();
	void renderBullets();
	void renderGUI();
	void render();
};

