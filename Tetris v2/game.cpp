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
		if (m_softDrop)
		{
			gameSpeed /= 10;
		}

		if (m_dT >= gameSpeed)
		{
			m_dT = 0.0f;
			m_moveMinoDown = true;
		}


		if (m_gameState == Playing) movement();
		collision();
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
	initPlayfield();
	resetPlayfield();
	spawnMino();
}

void Game::initPlayfield()
{
	m_playfieldGrid.resize(m_config.cols * m_config.rows, 0);
}

void Game::resetPlayfield()
{
	for (int y = 0; y < m_config.rows; y++)
	{
		for (int x = 0; x < m_config.cols; x++)
		{
			if (x == 0 || x == m_config.cols - 1 || y == m_config.rows - 1)
			{
				setCellType(sf::Vector2i(x, y), 8);
			}
			else
			{
				setCellType(sf::Vector2i(x, y), 0);
			}
		}
	}
}

void Game::setCellType(sf::Vector2i position, int type)
{
	m_playfieldGrid[position.x + position.y * m_config.cols] = type;
}

int Game::getCellType(sf::Vector2i position)
{
	if (position.y < 0)
	{
		return 0;
	}
	else
	{
		return m_playfieldGrid[position.x + position.y * m_config.cols];
	}
}

void Game::spawnMino()
{
	// shape type randomizer
	std::mt19937 seed(std::random_device{}());
	std::uniform_int_distribution dist(1, 7);
	int shapeType = dist(seed);

	// starting position
	sf::Vector2i position;
	position.x = (m_config.cols - 4) / 2;
	position.y = -2;
	if (shapeType == 4) position.x += 1;

	m_activeMino.reset();
	m_activeMino = std::make_unique<Tetromino>(shapeType, position);
}

int Game::clearFullRows()
{
	int fullRows = 0;

	for (int y = m_config.rows - 2; y >= 0; y--)
	{
		int fullCells = countFullCells(y);

		// full	row found
		if (fullCells == m_config.cols - 2)
		{
			fullRows++;
		}

		// empty row found -> cancel the scan
		else if (fullCells == 0)
		{
			break;
		}

		// move row down for the amount of full rows found so far (if any)
		else if (fullRows > 0)
		{
			moveRowDown(y, fullRows);
		}
	}
	return fullRows;
}

int Game::countFullCells(int y)
{
	int fullCells = 0;
	for (int x = 1; x < m_config.cols - 1; x++)
	{
		if (getCellType(sf::Vector2i(x, y)) != 0)
		{
			fullCells++;
		}
	}
	return fullCells;
}

void Game::moveRowDown(int y, int rows)
{
	for (int x = 1; x < m_config.cols - 1; x++)
	{
		setCellType(sf::Vector2i(x, y + rows), getCellType(sf::Vector2i(x, y)));	// copy row
		setCellType(sf::Vector2i(x, y), 0);											// clear row
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void Game::movement()
{
	// rotation
	if (m_lastPressedKey == sf::Keyboard::Up)
	{
		m_activeMino->rotate(1);
	}

	// horizontal movement
	if (m_lastPressedKey == sf::Keyboard::Left)
	{
		m_activeMino->move(sf::Vector2i(-1, 0));
	}
	if ( m_lastPressedKey == sf::Keyboard::Right)
	{
		m_activeMino->move(sf::Vector2i(1, 0));
	}

	// falling
	if (m_moveMinoDown)
	{
		m_activeMino->move(sf::Vector2i(0, 1));
		std::cout << "Move mino down!" << std::endl;

		if (m_hardDrop)
		{
			m_hardDropRows++;
		}
		if (m_softDrop)
		{
			m_softDropRows++;
		}
	}
}

void Game::collision()
{
	// rotation collision
	if (m_lastPressedKey == sf::Keyboard::Up)
	{
		const std::array<sf::Vector2i, 4>& positions = m_activeMino->getPositions();

		for (int i = 0; i < 4; i++)
		{
			if (getCellType(positions[i]) != 0)
			{
				m_activeMino->rotate(-1);
				std::cout << "CANNOT ROTATE!" << std::endl;
				break;
			}
		}
	}

	// horizontal collision
	if (m_lastPressedKey == sf::Keyboard::Left)
	{
		const std::array<sf::Vector2i, 4>& positions = m_activeMino->getPositions();

		for (int i = 0; i < 4; i++)
		{
			if (getCellType(positions[i]) != 0)
			{
				m_activeMino->move(sf::Vector2i(1, 0));
				std::cout << "CANNOT MOVE LEFT!" << std::endl;
				break;
			}
		}
	}
	
	if (m_lastPressedKey == sf::Keyboard::Right)
	{
		const std::array<sf::Vector2i, 4>& positions = m_activeMino->getPositions();

		for (int i = 0; i < 4; i++)
		{
			if (getCellType(positions[i]) != 0)
			{
				m_activeMino->move(sf::Vector2i(-1, 0));
				std::cout << "CANNOT MOVE RIGHT!" << std::endl;
				break;
			}
		}
	}

	// falling collision
	if (m_moveMinoDown)
	{
		const std::array<sf::Vector2i, 4>& positions = m_activeMino->getPositions();

		for (int i = 0; i < 4; i++)
		{
			if (getCellType(positions[i]) != 0)
			{
				std::cout << "MINO LANDED!" << std::endl;
				
				// save mino to the playfield grid
				for (int j = 0; j < 4; j++)
				{
					setCellType(sf::Vector2i(positions[j].x, positions[j].y - 1), m_activeMino->getShapeType());
				}

				// full rows and scoring
				int clearedRows = clearFullRows();
				if (clearedRows)
				{
					int score = m_score.level;

					switch (clearedRows)
					{
					case 1:
						score *= 40;
						break;
					case 2:
						score *= 100;
						break;
					case 3:
						score *= 300;
						break;
					case 4:
						score *= 1200;
						break;
					}

					m_score.score += score;
					m_score.rows += clearedRows;
					m_score.level = int(m_score.rows * 0.1f);
				}
				m_softDrop = false;
				m_score.score += m_softDropRows - 1;
				m_softDropRows = 0;
				m_hardDrop = false;
				m_score.score += m_hardDropRows * 2 - 1;
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
				spawnMino();
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

	text.setString("SCORE : " + std::to_string(m_score.score));
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
			cell.setFillColor(getColor(getCellType(sf::Vector2i(x, y))));
			m_window.draw(cell);
		}
	}
}

void Game::drawActiveMino()
{
	sf::RectangleShape cell(sf::Vector2f(m_config.cellSize * 1.0f, m_config.cellSize * 1.0f));
	cell.setOutlineThickness(-2);
	cell.setOutlineColor(getColor(9));

	cell.setFillColor(getColor(m_activeMino->getShapeType()));

	const std::array<sf::Vector2i, 4>& positions = m_activeMino->getPositions();

	for (int i = 0; i < 4; i++)
	{
		cell.setPosition(m_config.cellSize * positions[i].x * 1.0f, m_config.topBarHeight + m_config.cellSize * positions[i].y * 1.0f);
		m_window.draw(cell);
	}
}

