#pragma once
#include "playfield.hpp"
#include "menu.hpp"
#include <random>


enum GameState { Init = 0, Playing, Pause, Gameover };
enum Randomizer { FullRandom = 0, ShuffleBag };

struct Config
{
	int									cols = 12;
	int									rows = 21;
	int									cellSize = 40;
	int									topBarHeight = 100;
	float								gameSpeed = 0.8f;
	Randomizer							randomizer = ShuffleBag;
	bool								showMinoShadow = true;

	std::array<sf::Color, 11>			color =
	{
		sf::Color(37, 36, 86),			// empty cell
		sf::Color(0, 255, 255),			// I shape
		sf::Color(0, 0, 255),			// J shape
		sf::Color(255, 127, 0),			// L shape
		sf::Color(255, 255, 0),			// O shape
		sf::Color(0, 255, 0),			// S shape
		sf::Color(128, 0, 128),			// T shape
		sf::Color(255, 0, 0),			// Z shape
		sf::Color(32, 32, 32),			// border
		sf::Color(0, 0, 0),				// spacing
		sf::Color(255, 255, 255, 16)	// mino shadow
	};
};

struct									Score
{
	int points = 0;
	int rows = 0;
	int level = 1;
	int playTime = 0;
	int softDropRows = 0;
	int hardDropRows = 0;
};


class Game
{
private:

	Config								m_config;
	std::shared_ptr<sf::RenderWindow>	m_window;
	sf::Font							m_font;
	bool								m_running = false;
	GameState							m_gameState = Init;
	std::unique_ptr<Menu>				m_menu;
	std::unique_ptr<Playfield>			m_playfield;
	float								m_gameSpeed = 0.0f;
	float								m_dT = 0.0f;
	bool								m_moveMinoDown = false;
	sf::Keyboard::Key					m_lastPressedKey = sf::Keyboard::Unknown;
	bool								m_softDrop = false;
	bool								m_hardDrop = false;
	std::vector<int>					m_shuffleBag;
	Score								m_score;


public:

	Game();

	void run();


private:

	void initWindow();
	void initGame();
	void resetGame();
	int randomShapeType();
	void updateScore(int rows);
	void togglePause();

	void spawnMino();
	void moveMino(sf::Vector2i velocity);
	void rotateMino(int rotation);
	void updateShadowPosition();

	void movement();
	void collision();
	void inputEvents();
	
	void render();
	sf::Color getColor(int type);
	void drawTopBar();
	void drawPlayField();
	void drawActiveMino();
	void drawMino(const std::shared_ptr<Tetromino>& tetromino, sf::Color color);
};
