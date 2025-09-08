#pragma once
#include "tetromino.hpp"


class Playfield
{
private:

	int							m_cols = 0;
	int							m_rows = 0;
	std::vector<int>			m_playfieldGrid;
	std::shared_ptr<Tetromino>	m_activeMino;


public:

	Playfield(int cols, int rows);

	void init();
	void reset();
	void setCellType(sf::Vector2i position, int type);
	int getCellType(sf::Vector2i position);
	void spawnMino(int shapeType);
	std::shared_ptr<Tetromino> activeMino();
	int clearFullRows();
	int countFullCells(int y);
	void moveRowDown(int y, int rows);
};