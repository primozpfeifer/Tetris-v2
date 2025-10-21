#pragma once
#include "menu.hpp"


Menu::Menu(const sf::Vector2f parentSize, const sf::Vector2f size)
{
	sf::Vector2f menuSize = sf::Vector2f(parentSize.x * size.x, parentSize.y * size.y);
	m_rectShape.setSize(menuSize);
	m_rectShape.setOrigin(menuSize * 0.5f);
	m_rectShape.setPosition(parentSize * 0.5f);
	m_rectShape.setFillColor(sf::Color(32, 32, 32, 240));
}


void Menu::addButton(const sf::Vector2f size, const sf::Vector2f pos, const sf::Font& font, const sf::String buttonText)
{
	m_elements.emplace_back(std::make_shared<MenuButton>(m_rectShape.getSize(), size, m_rectShape.getPosition(), pos, font, buttonText));
}


void Menu::setActive(bool active) {}

void Menu::setActiveIndex(int index)
{
	if (index < 0 || index > 2)
		return;

	m_activeIndex = index;
}

int	Menu::getActiveIndex()
{
	return m_activeIndex;
}

void Menu::draw(std::shared_ptr<sf::RenderWindow>& window)
{
	window->draw(m_rectShape);
	for (auto& element : m_elements)
	{
		element->draw(window);
		element->setActive(false);
	}
	m_elements[m_activeIndex]->setActive(true);
}



MenuButton::MenuButton(const sf::Vector2f parentSize, const sf::Vector2f size, const sf::Vector2f parentPos, const sf::Vector2f pos, const sf::Font& font, const sf::String buttonText)
{
	sf::Vector2f buttonSize = sf::Vector2f(parentSize.x * size.x, parentSize.y * size.y);
	m_rectShape.setSize(buttonSize);
	m_rectShape.setOrigin(buttonSize * 0.5f);
	m_rectShape.setPosition(parentPos.x * pos.x, parentPos.y + parentSize.y * pos.y);
	m_rectShape.setFillColor(sf::Color(255, 255, 255, 10));

	// text
	m_text.setFont(font);
	m_text.setCharacterSize(static_cast<unsigned int>(buttonSize.y * 0.5f));
	m_text.setFillColor(sf::Color(255, 255, 255));
	m_text.setStyle(sf::Text::Regular);
	m_text.setString(buttonText);
	m_text.setOrigin(m_text.getLocalBounds().width * 0.5f, m_text.getLocalBounds().height * 0.8f);
	m_text.setPosition(m_rectShape.getPosition());
}


void MenuButton::setActive(bool active)
{
	if (active)
	{
		m_rectShape.setFillColor(sf::Color(255, 255, 255, 10));
		m_text.setStyle(sf::Text::Bold);
	}
	else
	{
		m_rectShape.setFillColor(sf::Color(255, 255, 255, 0));
		m_text.setStyle(sf::Text::Regular);
	}
}

void MenuButton::draw(std::shared_ptr<sf::RenderWindow>& window)
{
	window->draw(m_rectShape);
	window->draw(m_text);
}