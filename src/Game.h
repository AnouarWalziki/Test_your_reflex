#pragma once

#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

/*
	Class that acts as the game engine
	Wrapper class.
*/
class Game
{
private:
	//Variables
	//Window
	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event ev;

	//Mouse positions
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	//Sound
	sf::SoundBuffer enemyHitSoundBuffer;
	sf::SoundBuffer gameOverSoundBuffer;
	sf::Sound sound;

	//Ressources (textures & stuff)
	sf::Font font;

	//Text
	sf::Text uiText;

	//Game logic
	bool endGame;
	float difficulty;
	int health;
	unsigned points;
	float enemySpawnTimer;
	float enemySpawnTimerMax;
	int maxEnemies;
	bool mouseHeld;


	//Game objects
	std::vector<sf::RectangleShape> enemies;
	sf::RectangleShape enemy;


	//Private functions
	void initVariables();
	void initWindow();
	void iniSounds();
	void initFonts();
	void initText();
	void initEnemies();


public:
	//Constructor / Destructor
	Game();
	~Game();

	//Accessors
	const bool running() const;
	const bool getEndGame() const;

	//Functions
	void spawnEnemies();

	void playHitSound();

	void playGameOverSound();

	void pollEvents();

	void updateGameDifficulty();

	void updateMousePositions();

	void updateEnemies();
	void updateText();
	void update();

	void renderText(sf::RenderTarget& target);
	void renderEnenmies(sf::RenderTarget& target);
	void render();


};

