#pragma once
#include <SFML/Graphics.hpp>
#include <array>


class Tetromino
{
private:

	int								m_shapeType;	// 1 = I, 2 = J, 3 = L, 4 = O, 5 = S, 6 = T, 7 = Z
	std::array<sf::Vector2i, 16>	m_shapeStates;	// 4 shape states with 4 positions each
	int								m_shapeState;	// one of 4 different states	
	sf::Vector2i					m_position;		// global position on the playfield


public:

	Tetromino(int shapeType, sf::Vector2i position);
	
	void initShape(int shapeType);
	int getShapeType();
	std::array<sf::Vector2i, 4>getShapePositions();
	sf::Vector2i getPosition();
	void setPosition(sf::Vector2i position);
	void move(sf::Vector2i velocity);
	void rotate(int rotation);
};