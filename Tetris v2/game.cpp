#pragma once
#include "game.hpp"
#include <iostream>


Game::Game()
{
	initWindow();
	initGame();
}


void Game::run()
{
	sf::Clock frameTimer;

	while (m_running)
	{
		sf::Time dT = frameTimer.restart();
		m_dT += dT.asSeconds();

		float gameSpeed = m_gameSpeed;
		if (m_hardDrop)
		{
			gameSpeed /= 100;
		}
		else if (m_softDrop)
		{
			gameSpeed /= 10;
		}

		if (m_dT >= gameSpeed)
		{
			m_dT = 0.0f;
			m_moveMinoDown = true;
		}


		if (m_gameState == Playing)
		{
			movement();
			collision();
		}
		inputEvents();
		render();


		m_moveMinoDown = false;
	}
}

void Game::initWindow()
{
	std::string title = "Tetris";
	
	int width = m_config.cellSize * m_config.cols;
	int height = m_config.cellSize * m_config.rows + m_config.topBarHeight;

	m_window.create(
		sf::VideoMode(width, height),
		title,
		sf::Style::Titlebar | sf::Style::Close);

	m_window.setKeyRepeatEnabled(false);

	// load font from file
	if (!m_font.loadFromFile("arial.ttf"))
	{
		std::cout << "Unable to load font file!\n";
		exit(1);
	}
}

void Game::initGame()
{
	m_running = true;
	m_gameState = Playing;
	m_dT = 0.0f;
	m_gameSpeed = m_config.gameSpeed;
	m_playfield = std::make_unique<Playfield>(m_config.cols, m_config.rows);
	m_playfield->init();
	m_playfield->reset();
	m_playfield->spawnMino();
}

void Game::updateScore(int clearedRows)
{
	if (clearedRows > 0)
	{
		int points = m_score.level;

		switch (clearedRows)
		{
		case 1:
			points *= 40;
			break;
		case 2:
			points *= 100;
			break;
		case 3:
			points *= 300;
			break;
		case 4:
			points *= 1200;
			break;
		}

		m_score.points += points;
		m_score.rows += clearedRows;
		m_score.level = int(m_score.rows * 0.1f);
	}

	m_score.points += m_softDropRows - 1;
	m_score.points += m_hardDropRows * 2 - 1;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void Game::movement()
{
	switch (m_lastPressedKey)
	{
		// rotation
	case sf::Keyboard::Up:
		m_playfield->activeMino()->rotate(1);
		break;

		// horizontal movement
	case sf::Keyboard::Left:
		m_playfield->activeMino()->move(sf::Vector2i(-1, 0));
		break;

	case sf::Keyboard::Right:
		m_playfield->activeMino()->move(sf::Vector2i(1, 0));
		break;

	}

	// falling
	if (m_moveMinoDown)
	{
		m_playfield->activeMino()->move(sf::Vector2i(0, 1));
		std::cout << "Move mino down!" << std::endl;

		if (m_hardDrop)
		{
			m_hardDropRows++;
		}
		else if (m_softDrop)
		{
			m_softDropRows++;
		}
	}
}

void Game::collision()
{
	const std::array<sf::Vector2i, 4>& positions = m_playfield->activeMino()->getPositions();

	// rotation collision
	if (m_lastPressedKey == sf::Keyboard::Up)
	{
		for (int i = 0; i < 4; i++)
		{
			if (m_playfield->getCellType(positions[i]) != 0)
			{
				m_playfield->activeMino()->rotate(-1);
				std::cout << "CANNOT ROTATE!" << std::endl;
				break;
			}
		}
	}

	// horizontal collision
	if (m_lastPressedKey == sf::Keyboard::Left)
	{
		for (int i = 0; i < 4; i++)
		{
			if (m_playfield->getCellType(positions[i]) != 0)
			{
				m_playfield->activeMino()->move(sf::Vector2i(1, 0));
				std::cout << "CANNOT MOVE LEFT!" << std::endl;
				break;
			}
		}
	}
	
	if (m_lastPressedKey == sf::Keyboard::Right)
	{
		for (int i = 0; i < 4; i++)
		{
			if (m_playfield->getCellType(positions[i]) != 0)
			{
				m_playfield->activeMino()->move(sf::Vector2i(-1, 0));
				std::cout << "CANNOT MOVE RIGHT!" << std::endl;
				break;
			}
		}
	}

	// falling collision
	if (m_moveMinoDown)
	{
		for (int i = 0; i < 4; i++)
		{
			if (m_playfield->getCellType(positions[i]) != 0)
			{
				std::cout << "MINO LANDED!" << std::endl;
				
				// save mino to the playfield grid
				for (int j = 0; j < 4; j++)
				{
					m_playfield->setCellType(sf::Vector2i(positions[j].x, positions[j].y - 1), m_playfield->activeMino()->getShapeType());
				}

				// full rows and scoring
				updateScore(m_playfield->clearFullRows());
				m_softDrop = false;
				m_softDropRows = 0;
				m_hardDrop = false;
				m_hardDropRows = 0;

				// check if minos stacked to the top
				for (int j = 0; j < 4; j++)
				{
					if (positions[j].y < 2)
					{
						std::cout << "GAME OVER" << std::endl;
						// gameOver();
						exit(1);
					}
				}

				// spawn new mino
				m_playfield->spawnMino();
				break;
			}
		}
	}
}

void Game::inputEvents()
{
	m_lastPressedKey = sf::Keyboard::Unknown;
	sf::Event event;

	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_running = false;
			std::cout << "Window closed\n";
			return;
		}
		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::Escape:	m_running = false; return;
			case sf::Keyboard::Left:	m_lastPressedKey = sf::Keyboard::Left; break;
			case sf::Keyboard::Right:	m_lastPressedKey = sf::Keyboard::Right; break;
			case sf::Keyboard::Up:		m_lastPressedKey = sf::Keyboard::Up; break;
			case sf::Keyboard::Down:	m_softDrop = true; break;
			case sf::Keyboard::Space:	m_hardDrop = true; break;
			case sf::Keyboard::P:		m_gameState = Playing ? Pause : Playing; break;
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///  RENDERER  //////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::render()
{
	m_window.clear();
	drawTopBar(); //////
	drawPlayField();
	drawActiveMino();
	//drawTopBar(); //////
	m_window.display();
}

sf::Color Game::getColor(int type)
{
	return m_config.color[type];
}

void Game::drawTopBar()
{
	sf::RectangleShape cell(sf::Vector2f(m_config.cellSize * m_config.cols * 1.f, m_config.topBarHeight * 1.f));
	cell.setOutlineThickness(-2);
	cell.setOutlineColor(getColor(9));
	cell.setPosition(0 * 1.0f, 0 * 1.0f);
	cell.setFillColor(getColor(8));
	m_window.draw(cell);

	sf::Text text;
	text.setFont(m_font);
	text.setCharacterSize(24);
	text.setFillColor(sf::Color::White);
	text.setStyle(sf::Text::Bold);

	text.setString("SCORE : " + std::to_string(m_score.points));
	text.setPosition(20, 15);
	m_window.draw(text);

	text.setString("LINES : " + std::to_string(m_score.rows));
	text.setPosition(36, 55);
	m_window.draw(text);

	text.setString("LEVEL : " + std::to_string(m_score.level));
	text.setPosition(261, 55);
	m_window.draw(text);


	std::string hours = std::to_string(m_score.playTime / 3600);
	std::string minutes = std::to_string((m_score.playTime - ((m_score.playTime / 3600) * 3600)) / 60);
	std::string seconds = std::to_string(m_score.playTime - ((m_score.playTime / 3600) * 3600) - (((m_score.playTime - ((m_score.playTime / 3600) * 3600)) / 60) * 60));

	if (hours.length() == 1) hours = "0" + hours;
	if (minutes.length() == 1) minutes = "0" + minutes;
	if (seconds.length() == 1) seconds = "0" + seconds;

	text.setString("TIME : " + hours + ":" + minutes + ":" + seconds);
	text.setPosition(276, 15);
	m_window.draw(text);
}

void Game::drawPlayField()
{
	sf::RectangleShape cell(sf::Vector2f(m_config.cellSize * 1.0f, m_config.cellSize * 1.0f));
	cell.setOutlineThickness(-2);
	cell.setOutlineColor(getColor(9));

	for (int y = 0; y < m_config.rows; y++)
	{
		for (int x = 0; x < m_config.cols; x++)
		{
			cell.setPosition(m_config.cellSize * x * 1.0f, m_config.topBarHeight + m_config.cellSize * y * 1.0f);
			cell.setFillColor(getColor(m_playfield->getCellType(sf::Vector2i(x, y))));
			m_window.draw(cell);
		}
	}
}

void Game::drawActiveMino()
{
	sf::RectangleShape cell(sf::Vector2f(m_config.cellSize * 1.0f, m_config.cellSize * 1.0f));
	cell.setOutlineThickness(-2);
	cell.setOutlineColor(getColor(9));

	cell.setFillColor(getColor(m_playfield->activeMino()->getShapeType()));

	const std::array<sf::Vector2i, 4>& positions = m_playfield->activeMino()->getPositions();

	for (int i = 0; i < 4; i++)
	{
		cell.setPosition(m_config.cellSize * positions[i].x * 1.0f, m_config.topBarHeight + m_config.cellSize * positions[i].y * 1.0f);
		m_window.draw(cell);
	}
}

