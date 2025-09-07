#pragma once
#include "playfield.hpp"


struct Config
{
	int							cols = 12;
	int							rows = 21;
	int							cellSize = 40;
	int							topBarHeight = 100;
	float						gameSpeed = 0.8f;

	std::array<sf::Color, 10>	color =
	{
		sf::Color(37, 36, 86),		// empty cell
		sf::Color(0, 255, 255),		// I
		sf::Color(0, 0, 255),		// J
		sf::Color(255, 127, 0),		// L
		sf::Color(255, 255, 0),		// O
		sf::Color(0, 255, 0),		// S
		sf::Color(128, 0, 128),		// T
		sf::Color(255, 0, 0),		// Z
		sf::Color(32, 32, 32),		// border
		sf::Color(0, 0, 0)			// spacing
	};

};

enum GameState { Init, Playing, Pause, Gameover };


struct Score
{
	int points = 0;
	int rows = 0;
	int level = 1;
	int playTime = 0;
};


class Game
{
private:

	Config						m_config;
	sf::RenderWindow			m_window;
	sf::Font					m_font;
	bool						m_running = false;
	GameState					m_gameState = Init;
	std::unique_ptr<Playfield>	m_playfield;
	float						m_dT = 0.0f;
	float						m_gameSpeed = 0.0f;
	bool						m_moveMinoDown = false;
	sf::Keyboard::Key			m_lastPressedKey = sf::Keyboard::Unknown;
	bool						m_softDrop = false;
	int							m_softDropRows = 0;
	bool						m_hardDrop = false;
	int							m_hardDropRows = 0;
	Score						m_score;


public:

	Game();

	void run();
	void initWindow();
	void initGame();

	void movement();
	void collision();
	void inputEvents();
	
	void render();
	sf::Color getColor(int type);
	void drawTopBar();
	void drawPlayField();
	void drawActiveMino();
};