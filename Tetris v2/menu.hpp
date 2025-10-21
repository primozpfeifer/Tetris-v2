#pragma once
#include <SFML/Graphics.hpp>


class MenuElement
{
protected:

	sf::RectangleShape m_rectShape;
	

public:

	MenuElement() = default;
	virtual ~MenuElement() = default;
	
	virtual void setActive(bool active) = 0;
	virtual void draw(std::shared_ptr<sf::RenderWindow>& window) = 0;
};



class Menu : public MenuElement
{
private:

	int m_activeIndex = 0;
	std::vector<std::shared_ptr<MenuElement>> m_elements;


public:

	Menu(const sf::Vector2f parentSize, const sf::Vector2f size);

	void addButton(const sf::Vector2f size, const sf::Vector2f pos, const sf::Font& font, const sf::String buttonText);
	void setActive(bool active) override;
	void setActiveIndex(int index);
	int	 getActiveIndex();
	void draw(std::shared_ptr<sf::RenderWindow>& window) override;
};



class MenuButton : public MenuElement
{
private:

	sf::Text m_text;


public:

	MenuButton(const sf::Vector2f parentSize, const sf::Vector2f size, const sf::Vector2f parentPos, const sf::Vector2f position, const sf::Font& font, const sf::String buttonText);

	void setActive(bool active) override;
	void draw(std::shared_ptr<sf::RenderWindow>& window) override;
};

