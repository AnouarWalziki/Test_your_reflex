#include "Game.h"

//Private Functions
void Game::initVariables()
{
    this->window = nullptr;
    this->endGame = false;
    this->difficulty = 3000.f;
    this->points = 0;
    this->enemySpawnTimerMax = 20.f;
    this->enemySpawnTimer = this->enemySpawnTimerMax;
    this->maxEnemies = 5;
    this->mouseHeld = false;
    this->health = 100;
    

}

void Game::initWindow()
{
	this->videoMode.height = 720;
	this->videoMode.width = 1280;

	this->window = new sf::RenderWindow(this->videoMode, "Test your reflex", sf::Style::Titlebar | sf::Style::Close);

    this->window->setFramerateLimit(60);
}

void Game::iniSounds()
{
    if (!this->enemyHitSoundBuffer.loadFromFile("Sounds/enemyHit.wav"))
    {
        std::cout << "ERROR::GAME::INITSOUNDS::Failed to load sound's files!" << std::endl;
    }
    if (!this->gameOverSoundBuffer.loadFromFile("Sounds/gameOver.wav"))
    {
        std::cout << "ERROR::GAME::INITSOUNDS::Failed to load sound's files!" << std::endl;
    }
    
}

void Game::initFonts()
{
    if (!this->font.loadFromFile("Fonts/Game_Of_Squids.ttf"))
    {
        std::cout << "ERROR::GAME::INITFRONTS::Failed to load font!" << std::endl;
    }
}

void Game::initText()
{
    this->uiText.setFont(this->font);
    this->uiText.setCharacterSize(24);
    this->uiText.setFillColor(sf::Color::White);
    this->uiText.setString("NONE");
}

void Game::initEnemies()
{
    this->enemy.setPosition(10.f,10.f);
    this->enemy.setSize(sf::Vector2f(100.f, 100.f));
    this->enemy.setScale(sf::Vector2f(0.5f, 0.5f));
    this->enemy.setFillColor(sf::Color::Cyan);
    this->enemy.setOutlineColor(sf::Color::Red);
    this->enemy.setOutlineThickness(5.f);

}

// Constructor / Destructor
Game::Game()
{
    this->initVariables();
    this->initWindow();
    this->iniSounds();
    this->initFonts();
    this->initText();
    this->initEnemies();
}

Game::~Game() 
{
	delete this->window;
}

//Accessors
const bool Game::running() const
{
	return this->window->isOpen();
}

const bool Game::getEndGame() const
{
    return this->endGame;
}

void Game::spawnEnemies()
{
    /*
        @return void

        Spawns enemies and sets their colors and positions.
        -Sets a random position.
        -Sets a random color.
        -Adds enemy to the vector.
    */

    this->enemy.setPosition(
        static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
        0.f);

    this->enemy.setFillColor(sf::Color::Green);
    //Spawn the enemy
    this->enemies.push_back(this->enemy);
}

void Game::playHitSound()
{
    /*
        @return void

        Plays a sound when an enemy is hit
    */

    this->sound.resetBuffer();
    this->sound.setBuffer(enemyHitSoundBuffer);
    this->sound.play();
    
}

void Game::playGameOverSound()
{
    /*
        Plays game over sound
    */
    this->sound.resetBuffer();
    this->sound.setBuffer(gameOverSoundBuffer);
    this->sound.play();
    
}

void Game::updateGameDifficulty()
{
    /*
        @return void

        Increases the game timer.
    */

    this->difficulty += 1;
}

//Functions
void Game::pollEvents()
{
    //Event polling
    while (this->window->pollEvent(this->ev)) {

        switch (this->ev.type)
        {
        case sf::Event::Closed:
            this->window->close();
            break;
        case sf::Event::KeyPressed:
            if (this->ev.key.code == sf::Keyboard::Escape)
               this->window->close();
        }
    }
}

void Game::updateMousePositions() {
    /*
    @ return void

    Updates the mouse position
        * Mouse position relative to window (Vector2i)
    */
    this->mousePosWindow = sf::Mouse::getPosition(*this->window);
    this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void Game::updateEnemies()
{
    /*
        @return void

        Update the enemy spawn timers and spawns enemies
        when the total amout of enemies is maller than the maximum.
        Moves the nemies downwards.
        removes the enemy at the edges of the screen. //TODO
    */
    //Updating the timers for enemy spawning
    if (this->enemies.size() < this->maxEnemies)
    {
        if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
        {
            //Spawn the enemy and reset the timer
            this->spawnEnemies();
            this->enemySpawnTimer = 0.f;
        }
        else
        {
            this->enemySpawnTimer += 1.f;
        }

    }
    

    //Moving and updating the enemies
  
    for (int i =0; i< this->enemies.size(); i++)
    {
        bool deleted = false;
        this->enemies[i].move(0.f, this->difficulty *0.001);

        //Check if enemy passed the bottom of the  screen
        if (this->enemies[i].getPosition().y > this->window->getSize().y)
        {
            this->enemies.erase(this->enemies.begin() + i);
            this->health -= 10;
            if (this->health == 0) {
                playGameOverSound();
            }
            std::cout << "Health : " << this->health << std::endl;
        }
    }

    //Check if click upon
    
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        if (this->mouseHeld == false)
        {
            this->mouseHeld = true;
            bool deleted = false;
            for (int i = 0; i < this->enemies.size() && deleted == false; i++)
            {
                if (this->enemies[i].getGlobalBounds().contains(this->mousePosView))
                {
                    //Delete the enemy
                    deleted = true;
                    //Play hit sound
                    playHitSound();
                    this->enemies.erase(this->enemies.begin() + i);
                    //Gain points
                    this->points += 10 + difficulty*0.01;
                    std::cout << "Points : " << this->points << std::endl;
                    
                }
            }
        }        
    }
    else
    {
        this->mouseHeld = false;
    }

        
}

void Game::updateText()
{
    std::stringstream ss;

    ss << "Points: " << this->points << "\n"
        << "Health: " << this->health;
    this->uiText.setString(ss.str());
}

void Game::renderText(sf::RenderTarget& target)
{
    target.draw(this->uiText);
}

void Game::renderEnenmies(sf::RenderTarget& target)
{
    for (auto& e : this->enemies)
    {
        target.draw(e);
    }
}


void Game::update()
{
    
    this->pollEvents();
    
    if (!this->endGame)
    {
        this->updateGameDifficulty();

        this->updateMousePositions();

        this->updateText();

        this->updateEnemies();
    }

    if (this->health < 0)
    {
        this->endGame = true;
    }
    
}

void Game::render()
{
    /*
        @return void

        - clear old frame
        - render objects
        - display frame in window

        Renders the game objects.
    */
    this->window->clear(sf::Color(0, 0, 0, 255));

    //Draw game objects
    this->renderEnenmies(*this->window);

    this->renderText(*this->window);

    this->window->display();
}

