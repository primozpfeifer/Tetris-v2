#pragma once
#include "tetromino.hpp"


Tetromino::Tetromino(int shapeType, sf::Vector2i position)
{
	m_shapeType = shapeType;
	m_shapeState = 0;
	m_position = position;
	initShape(shapeType);
}


void Tetromino::initShape(int shapeType)
{
	switch (shapeType)
	{
	case 1: // I
		m_shapeStates[0] = { 0,1 };
		m_shapeStates[1] = { 1,1 };
		m_shapeStates[2] = { 2,1 };
		m_shapeStates[3] = { 3,1 };
		m_shapeStates[4] = { 2,0 };
		m_shapeStates[5] = { 2,1 };
		m_shapeStates[6] = { 2,2 };
		m_shapeStates[7] = { 2,3 };
		m_shapeStates[8] = { 0,2 };
		m_shapeStates[9] = { 1,2 };
		m_shapeStates[10] = { 2,2 };
		m_shapeStates[11] = { 3,2 };
		m_shapeStates[12] = { 1,0 };
		m_shapeStates[13] = { 1,1 };
		m_shapeStates[14] = { 1,2 };
		m_shapeStates[15] = { 1,3 };
		break;

	case 2:	// J
		m_shapeStates[0] = { 0,0 };
		m_shapeStates[1] = { 0,1 };
		m_shapeStates[2] = { 1,1 };
		m_shapeStates[3] = { 2,1 };
		m_shapeStates[4] = { 1,0 };
		m_shapeStates[5] = { 1,1 };
		m_shapeStates[6] = { 1,2 };
		m_shapeStates[7] = { 2,0 };
		m_shapeStates[8] = { 0,1 };
		m_shapeStates[9] = { 1,1 };
		m_shapeStates[10] = { 2,1 };
		m_shapeStates[11] = { 2,2 };
		m_shapeStates[12] = { 0,2 };
		m_shapeStates[13] = { 1,0 };
		m_shapeStates[14] = { 1,1 };
		m_shapeStates[15] = { 1,2 };
		break;

	case 3:	// L
		m_shapeStates[0] = { 2,0 };
		m_shapeStates[1] = { 0,1 };
		m_shapeStates[2] = { 1,1 };
		m_shapeStates[3] = { 2,1 };
		m_shapeStates[4] = { 1,0 };
		m_shapeStates[5] = { 1,1 };
		m_shapeStates[6] = { 1,2 };
		m_shapeStates[7] = { 2,2 };
		m_shapeStates[8] = { 0,1 };
		m_shapeStates[9] = { 0,2 };
		m_shapeStates[10] = { 1,1 };
		m_shapeStates[11] = { 2,1 };
		m_shapeStates[12] = { 0,0 };
		m_shapeStates[13] = { 1,0 };
		m_shapeStates[14] = { 1,1 };
		m_shapeStates[15] = { 1,2 };
		break;

	case 4:	// O
		m_shapeStates[0] = { 0,0 };
		m_shapeStates[1] = { 1,0 };
		m_shapeStates[2] = { 0,1 };
		m_shapeStates[3] = { 1,1 };
		m_shapeStates[4] = { 0,0 };
		m_shapeStates[5] = { 1,0 };
		m_shapeStates[6] = { 0,1 };
		m_shapeStates[7] = { 1,1 };
		m_shapeStates[8] = { 0,0 };
		m_shapeStates[9] = { 1,0 };
		m_shapeStates[10] = { 0,1 };
		m_shapeStates[11] = { 1,1 };
		m_shapeStates[12] = { 0,0 };
		m_shapeStates[13] = { 1,0 };
		m_shapeStates[14] = { 0,1 };
		m_shapeStates[15] = { 1,1 };
		break;

	case 5:	// S
		m_shapeStates[0] = { 1,0 };
		m_shapeStates[1] = { 2,0 };
		m_shapeStates[2] = { 0,1 };
		m_shapeStates[3] = { 1,1 };
		m_shapeStates[4] = { 1,0 };
		m_shapeStates[5] = { 1,1 };
		m_shapeStates[6] = { 2,1 };
		m_shapeStates[7] = { 2,2 };
		m_shapeStates[8] = { 0,2 };
		m_shapeStates[9] = { 1,1 };
		m_shapeStates[10] = { 1,2 };
		m_shapeStates[11] = { 2,1 };
		m_shapeStates[12] = { 0,0 };
		m_shapeStates[13] = { 0,1 };
		m_shapeStates[14] = { 1,1 };
		m_shapeStates[15] = { 1,2 };
		break;

	case 6:	// T
		m_shapeStates[0] = { 0,1 };
		m_shapeStates[1] = { 1,1 };
		m_shapeStates[2] = { 1,0 };
		m_shapeStates[3] = { 2,1 };
		m_shapeStates[4] = { 1,0 };
		m_shapeStates[5] = { 1,1 };
		m_shapeStates[6] = { 1,2 };
		m_shapeStates[7] = { 2,1 };
		m_shapeStates[8] = { 0,1 };
		m_shapeStates[9] = { 1,1 };
		m_shapeStates[10] = { 1,2 };
		m_shapeStates[11] = { 2,1 };
		m_shapeStates[12] = { 0,1 };
		m_shapeStates[13] = { 1,0 };
		m_shapeStates[14] = { 1,1 };
		m_shapeStates[15] = { 1,2 };
		break;

	case 7:	// Z
		m_shapeStates[0] = { 0,0 };
		m_shapeStates[1] = { 1,0 };
		m_shapeStates[2] = { 1,1 };
		m_shapeStates[3] = { 2,1 };
		m_shapeStates[4] = { 1,1 };
		m_shapeStates[5] = { 1,2 };
		m_shapeStates[6] = { 2,0 };
		m_shapeStates[7] = { 2,1 };
		m_shapeStates[8] = { 0,1 };
		m_shapeStates[9] = { 1,1 };
		m_shapeStates[10] = { 1,2 };
		m_shapeStates[11] = { 2,2 };
		m_shapeStates[12] = { 0,1 };
		m_shapeStates[13] = { 0,2 };
		m_shapeStates[14] = { 1,0 };
		m_shapeStates[15] = { 1,1 };
		break;
	}
}

int Tetromino::getShapeType()
{
	return m_shapeType;
}

std::array<sf::Vector2i, 4>Tetromino::getShapePositions()
{
	return
	{
		m_shapeStates[m_shapeState * 4 + 0] + m_position,
		m_shapeStates[m_shapeState * 4 + 1] + m_position,
		m_shapeStates[m_shapeState * 4 + 2] + m_position,
		m_shapeStates[m_shapeState * 4 + 3] + m_position
	};
}

sf::Vector2i Tetromino::getPosition()
{
	return m_position;
}

void Tetromino::setPosition(sf::Vector2i position)
{
	m_position = position;
}

void Tetromino::move(sf::Vector2i velocity)
{
	m_position += velocity;
}

void Tetromino::rotate(int rotation)
{
	m_shapeState += rotation;

	if (m_shapeState > 3)
	{
		m_shapeState = 0;
	}
	else if (m_shapeState < 0)
	{
		m_shapeState = 3;
	}
}
