#pragma once
#include "playfield.hpp"


Playfield::Playfield(int cols, int rows) : m_cols(cols), m_rows(rows)
{
	m_playfieldGrid.resize(m_cols * m_rows, 0);
}


void Playfield::reset()
{
	for (int y = 0; y < m_rows; y++)
	{
		for (int x = 0; x < m_cols; x++)
		{
			if (x == 0 || x == m_cols - 1 || y == m_rows - 1)
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

void Playfield::setCellType(sf::Vector2i position, int type)
{
	m_playfieldGrid[position.x + position.y * m_cols] = type;
}

int Playfield::getCellType(sf::Vector2i position)
{
	if (position.y < 0)
	{
		return 0;
	}
	else
	{
		return m_playfieldGrid[position.x + position.y * m_cols];
	}
}

void Playfield::spawnMino(int shapeType, sf::Vector2i position, bool shadow)
{
	m_activeMino.reset();
	m_activeMino = std::make_shared<Tetromino>(shapeType, position);

	if (shadow)
	{
		m_minoShadow.reset();
		m_minoShadow = std::make_shared<Tetromino>(shapeType, position);
	}
}

std::shared_ptr<Tetromino>& Playfield::activeMino()
{
	return m_activeMino;
}

std::shared_ptr<Tetromino>& Playfield::minoShadow()
{
	return m_minoShadow;
}

int Playfield::clearFullRows()
{
	int fullRows = 0;

	for (int y = m_rows - 2; y >= 0; y--)
	{
		int fullCells = countFullCells(y);

		// full	row found
		if (fullCells == m_cols - 2)
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

int Playfield::countFullCells(int y)
{
	int fullCells = 0;
	
	for (int x = 1; x < m_cols - 1; x++)
	{
		if (getCellType(sf::Vector2i(x, y)) != 0)
		{
			fullCells++;
		}
	}

	return fullCells;
}

void Playfield::moveRowDown(int y, int rows)
{
	for (int x = 1; x < m_cols - 1; x++)
	{
		setCellType(sf::Vector2i(x, y + rows), getCellType(sf::Vector2i(x, y)));	// copy row
		setCellType(sf::Vector2i(x, y), 0);											// clear row
	}
}