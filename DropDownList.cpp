#include "stdafx.h"
#include "DropDownList.h"

//Constructors and Destructor:

gui::DropDownList::DropDownList
(
	float x, float y, float width, float height,
	sf::Font& font, unsigned int font_size, std::string list[],
	unsigned number_of_elements, unsigned default_index
)
	: font(font), showList(false), keyTimeMax(30.f), keyTime(0.f)
{
	//unsigned numberOfElements = sizeof(list) / sizeof(std::string);

	this->activeElement = new gui::Button
	(
		x, y, width, height,
		&this->font, list[default_index], font_size,
		sf::Color(80, 80, 0, 200), sf::Color(255, 255, 0, 230), sf::Color(0, 0, 0, 200),
		sf::Color::Yellow, sf::Color::Black, sf::Color::White,
		sf::Color(102, 102, 0, 255), sf::Color(255, 255, 0, 255), sf::Color(0, 0, 0, 255)
	);

	for (unsigned i = 0; i < number_of_elements; i++)
	{
		this->list.push_back
		(
			new gui::Button
			(
				x, y + ((i + 1) * height), width, height,
				&this->font, list[i], font_size,
				sf::Color(80, 80, 0, 200), sf::Color(255, 255, 0, 230), sf::Color(0, 0, 0, 200),
				sf::Color::Yellow, sf::Color::Black, sf::Color::White,
				sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent,
				i
			)
		);
	}
}

gui::DropDownList::DropDownList
(
	float x, float y, float width, float height,
	sf::VideoMode video_mode,
	sf::Font& font, unsigned int font_size, std::string list[],
	unsigned number_of_elements, unsigned default_index
)
	: font(font), showList(false), keyTimeMax(30.f), keyTime(0.f)
{
	//unsigned numberOfElements = sizeof(list) / sizeof(std::string);

	this->activeElement = new gui::Button
	(
		x, y, width, height,
		&this->font, list[default_index], font_size,
		sf::Color(80, 80, 0, 200), sf::Color(255, 255, 0, 230), sf::Color(0, 0, 0, 200),
		sf::Color::Yellow, sf::Color::Black, sf::Color::White,
		sf::Color(102, 102, 0, 255), sf::Color(255, 255, 0, 255), sf::Color(0, 0, 0, 255)
	);

	this->strWidth = std::to_string(video_mode.width);
	this->strHeight = std::to_string(video_mode.height);

	this->activeElement->setString(this->strWidth + " x " + this->strHeight);

	for (unsigned i = 0; i < number_of_elements; i++)
	{
		this->list.push_back
		(
			new gui::Button
			(
				x, y + ((i + 1) * height), width, height,
				&this->font, list[i], font_size,
				sf::Color(80, 80, 0, 200), sf::Color(255, 255, 0, 230), sf::Color(0, 0, 0, 200),
				sf::Color::Yellow, sf::Color::Black, sf::Color::White,
				sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent,
				i
			)
		);
	}
}

gui::DropDownList::~DropDownList()
{
	delete this->activeElement;

	for (auto*& i : this->list)
	{
		delete i;
	}
}

//Accessors:

const bool gui::DropDownList::getKeyTime()
{
	if (this->keyTime >= this->keyTimeMax)
	{
		this->keyTime = 0;
		return true;
	}

	return false;
}

const uint32_t& gui::DropDownList::getActiveElementId() const
{
	return this->activeElement->getId();
}

//Functions:

void gui::DropDownList::updateKeyTime(const float& dt)
{
	if (this->keyTime < this->keyTimeMax)
	{
		this->keyTime += 100.f * dt;
	}
}

void gui::DropDownList::update(const sf::Vector2i& mouse_position, const float& dt)
{
	this->updateKeyTime(dt);

	//Dropdown list's active element
	this->activeElement->update(mouse_position, true);
	this->activeElement->updateButtonColor(dt);

	//Showing and hiding the dropdown list
	if (this->activeElement->isPressed() && this->getKeyTime())
	{
		if (this->showList)
		{
			this->showList = false;
		}
		else
		{
			this->showList = true;
		}
	}

	//When the dropdown list is showing up
	if (this->showList)
	{
		for (auto& i : this->list)
		{
			i->update(mouse_position, true);
			i->updateButtonColor(dt);

			if (i->isPressed() && this->getKeyTime())
			{
				this->showList = false;

				// Pointless because recreating the window recreates all this too
				this->activeElement->setString(i->getString());
				this->activeElement->setId(i->getId());
			}
		}
	}
}

void gui::DropDownList::render(sf::RenderTarget* target)
{
	this->activeElement->render(target);

	if (this->showList)
	{
		for (auto& i : this->list)
		{
			i->render(target);
		}
	}
}