#include "stdafx.h"
#include "Button.h"

const float gui::percentIntoX(const float percent, const sf::VideoMode& video_mode)
{
	return std::floor(static_cast<float>(video_mode.width) * (percent / 100.f));
}

const float gui::percentIntoY(const float percent, const sf::VideoMode& video_mode)
{
	return std::floor(static_cast<float>(video_mode.height) * (percent / 100.f));
}

const unsigned gui::calculateCharSize(const float percent, const sf::VideoMode& video_mode)
{
	if (video_mode.width > video_mode.height)
	{
		return static_cast<unsigned>(std::floor(static_cast<float>(video_mode.height) * (percent / 100.f)));
	}

	return static_cast<unsigned>(std::floor(static_cast<float>(video_mode.width) * (percent / 100.f)));
}

// private: Functions:

void gui::Button::initVariables()
{
	this->fontSize = 20;

	this->colorNumber = 0;

	this->stepOutline = 0.034f;
	this->stepButton = 0.1f;
	this->stepText = 0.034f;

	this->coefficientOutline = 0.8f;
	this->coefficientButton = 0.4f;
	this->coefficientText = 0.8f;

	this->buttonTimer = 0.f;
	this->buttonTimerMax = 4.f;
}


// Constructors and Destructor:

gui::Button::Button
(
	const float x, const float y, const float width, const float height,
	sf::Font* font, const std::string& str, const uint32_t font_size,
	sf::Color button_idle_color, sf::Color button_hover_color, sf::Color button_active_color,
	sf::Color text_idle_color, sf::Color text_hover_color, sf::Color text_active_color,
	sf::Color outline_idle_color,
	sf::Color outline_hover_color,
	sf::Color outline_active_color,
	const uint32_t id
)
{
	this->initVariables();

	this->buttonState = gui::ButtonStates::Idle;
	this->id = id;

	this->shape.setPosition(sf::Vector2f(x, y));
	this->shape.setSize(sf::Vector2f(width, height));
	this->shape.setOutlineThickness(-1.f);

	this->font = font;
	this->text.setFont(*this->font);
	this->text.setString(str);
	this->text.setFillColor(sf::Color::White);
	this->text.setCharacterSize(font_size);
	this->text.setPosition
	(
		(this->shape.getPosition().x + (this->shape.getGlobalBounds().width / 2.f)) - (this->text.getGlobalBounds().width / 2.f),
		this->shape.getPosition().y// + (this->shape.getGlobalBounds().height / 20.f))// - (this->text.getGlobalBounds().height / 2.f)
	);

	// Outline colors
	this->outlineIdleColor = outline_idle_color;
	this->outlineHoverColor = outline_hover_color;
	this->outlineActiveColor = outline_active_color;

	// Button colors
	this->buttonIdleColor = button_idle_color;
	this->buttonHoverColor = button_hover_color;
	this->buttonActiveColor = button_active_color;

	// Text colors
	this->textIdleColor = text_idle_color;
	this->textHoverColor = text_hover_color;
	this->textActiveColor = text_active_color;

	// Outline idle to hover color
	this->idleToHoverColorOutline[0] = this->outlineIdleColor;
	this->coefficientOutline += this->stepOutline;
	for (unsigned short i = 1; i < this->colorMaxStages; i++)
	{
		this->idleToHoverColorOutline[i].r = static_cast<unsigned>(static_cast<float>(outline_hover_color.r) * this->coefficientOutline);
		this->idleToHoverColorOutline[i].g = static_cast<unsigned>(static_cast<float>(outline_hover_color.g) * this->coefficientOutline);
		this->idleToHoverColorOutline[i].b = static_cast<unsigned>(static_cast<float>(outline_hover_color.b) * this->coefficientOutline);
		this->coefficientOutline += this->stepOutline;
	}

	//Button idle to hover color
	this->idleToHoverColorButton[0] = this->buttonIdleColor;
	this->coefficientButton += this->stepButton;
	for (unsigned short i = 1; i < this->colorMaxStages; i++)
	{
		this->idleToHoverColorButton[i].r = static_cast<unsigned>(static_cast<float>(button_hover_color.r) * this->coefficientButton);
		this->idleToHoverColorButton[i].g = static_cast<unsigned>(static_cast<float>(button_hover_color.g) * this->coefficientButton);
		this->idleToHoverColorButton[i].b = static_cast<unsigned>(static_cast<float>(button_hover_color.b) * this->coefficientButton);
		this->coefficientButton += this->stepButton;
	}

	//Text idle to hover color
	this->idleToHoverColorText[0] = this->textIdleColor;
	this->coefficientText += this->stepText;
	for (unsigned short i = 1; i < this->colorMaxStages; i++)
	{
		this->idleToHoverColorText[i].r = static_cast<unsigned>(static_cast<float>(text_hover_color.r) * this->coefficientText);
		this->idleToHoverColorText[i].g = static_cast<unsigned>(static_cast<float>(text_hover_color.g) * this->coefficientText);
		this->idleToHoverColorText[i].b = static_cast<unsigned>(static_cast<float>(text_hover_color.b) * this->coefficientText);
		this->coefficientText += this->stepText;
	}

	// Outline
	this->shape.setOutlineColor(this->outlineIdleColor);

	// Button
	this->shape.setFillColor(this->buttonIdleColor);

	// Text
	this->text.setFillColor(this->textIdleColor);
}

gui::Button::Button
(
	const float x, const float y, const float width, const float height,
	sf::Font* font, const std::string& text, const uint32_t font_size,
	sf::Color auto_outline_hover_color,
	sf::Color auto_button_hover_color,
	sf::Color auto_text_hover_color,
	const uint32_t id
)
{
	this->initVariables();

	this->buttonState = gui::ButtonStates::Idle;
	this->id = id;

	this->shape.setPosition(sf::Vector2f(x, y));
	this->shape.setSize(sf::Vector2f(width, height));
	this->shape.setOutlineThickness(-1.f);

	this->font = font;
	this->text.setFont(*this->font);
	this->text.setString(text);
	this->text.setFillColor(sf::Color::White);
	this->text.setCharacterSize(font_size);
	this->text.setPosition
	(
		(this->shape.getPosition().x + (this->shape.getGlobalBounds().width / 2.f)) - (this->text.getGlobalBounds().width / 2.f),
		(this->shape.getPosition().y + 1000.f)// + (this->shape.getGlobalBounds().height / 20.f))// - (this->text.getGlobalBounds().height / 2.f)
	);

	// Colors check:

	//Button
	if (auto_button_hover_color.r < 60 && auto_button_hover_color.g < 60 && auto_button_hover_color.b < 60)
	{
		auto_button_hover_color.r += 130;
		auto_button_hover_color.g += 130;
		auto_button_hover_color.b += 130;
	}

	// Outline colors:

	// Idle color
	this->outlineIdleColor.r = static_cast<uint32_t>(static_cast<float>(auto_outline_hover_color.r) * 0.7f);
	this->outlineIdleColor.g = static_cast<uint32_t>(static_cast<float>(auto_outline_hover_color.g) * 0.7f);
	this->outlineIdleColor.b = static_cast<uint32_t>(static_cast<float>(auto_outline_hover_color.b) * 0.7f);
	this->outlineIdleColor.a = 200;

	// Hover color
	this->outlineHoverColor = auto_outline_hover_color;
	this->outlineHoverColor.a = 255;

	// Active color
	this->outlineActiveColor.r = static_cast<uint32_t>(static_cast<float>(auto_outline_hover_color.r) * 0.4f);
	this->outlineActiveColor.g = static_cast<uint32_t>(static_cast<float>(auto_outline_hover_color.g) * 0.4f);
	this->outlineActiveColor.b = static_cast<uint32_t>(static_cast<float>(auto_outline_hover_color.b) * 0.4f);
	this->outlineActiveColor.a = 200;

	// Button colors:

	// Idle color
	this->buttonIdleColor.r = static_cast<uint32_t>(static_cast<float>(auto_button_hover_color.r) * 0.4f);
	this->buttonIdleColor.g = static_cast<uint32_t>(static_cast<float>(auto_button_hover_color.g) * 0.4f);
	this->buttonIdleColor.b = static_cast<uint32_t>(static_cast<float>(auto_button_hover_color.b) * 0.4f);
	this->buttonIdleColor.a = 200;

	// Hover color
	this->buttonHoverColor = auto_button_hover_color;
	this->buttonHoverColor.a = 255;

	// Active color
	this->buttonActiveColor.r = static_cast<uint32_t>(static_cast<float>(auto_button_hover_color.r) * 0.1f);
	this->buttonActiveColor.g = static_cast<uint32_t>(static_cast<float>(auto_button_hover_color.g) * 0.1f);
	this->buttonActiveColor.b = static_cast<uint32_t>(static_cast<float>(auto_button_hover_color.b) * 0.1f);
	this->buttonActiveColor.a = 200;

	// Text colors:

	// Idle color
	this->textIdleColor.r = static_cast<uint32_t>(static_cast<float>(auto_text_hover_color.r) * 0.7f);
	this->textIdleColor.g = static_cast<uint32_t>(static_cast<float>(auto_text_hover_color.g) * 0.7f);
	this->textIdleColor.b = static_cast<uint32_t>(static_cast<float>(auto_text_hover_color.b) * 0.7f);
	this->textIdleColor.a = 255;

	// Hover color
	this->textHoverColor = auto_text_hover_color;
	this->textHoverColor.a = 255;

	// Active color
	this->textActiveColor.r = static_cast<unsigned>(static_cast<float>(auto_text_hover_color.r) * 0.4f);
	this->textActiveColor.g = static_cast<unsigned>(static_cast<float>(auto_text_hover_color.g) * 0.4f);
	this->textActiveColor.b = static_cast<unsigned>(static_cast<float>(auto_text_hover_color.b) * 0.4f);
	this->textActiveColor.a = 255;

	// Outline idle to hover color
	this->idleToHoverColorOutline[0] = this->outlineIdleColor;
	this->coefficientOutline += this->stepOutline;
	for (unsigned short i = 1; i < this->colorMaxStages; i++)
	{
		this->idleToHoverColorOutline[i].r = static_cast<unsigned>(static_cast<float>(auto_outline_hover_color.r) * this->coefficientOutline);
		this->idleToHoverColorOutline[i].g = static_cast<unsigned>(static_cast<float>(auto_outline_hover_color.g) * this->coefficientOutline);
		this->idleToHoverColorOutline[i].b = static_cast<unsigned>(static_cast<float>(auto_outline_hover_color.b) * this->coefficientOutline);
		this->coefficientOutline += this->stepOutline;
	}

	// Button idle to hover color
	this->idleToHoverColorButton[0] = this->buttonIdleColor;
	this->coefficientButton += this->stepText;
	for (unsigned short i = 1; i < this->colorMaxStages; i++)
	{
		this->idleToHoverColorButton[i].r = static_cast<unsigned>(static_cast<float>(auto_button_hover_color.r) * this->coefficientButton);
		this->idleToHoverColorButton[i].g = static_cast<unsigned>(static_cast<float>(auto_button_hover_color.g) * this->coefficientButton);
		this->idleToHoverColorButton[i].b = static_cast<unsigned>(static_cast<float>(auto_button_hover_color.b) * this->coefficientButton);
		this->coefficientButton += this->stepButton;
	}

	// Text idle to hover color
	this->idleToHoverColorText[0] = this->textIdleColor;
	this->coefficientText += this->stepText;
	for (unsigned short i = 1; i < this->colorMaxStages; i++)
	{
		this->idleToHoverColorText[i].r = static_cast<unsigned>(static_cast<float>(auto_text_hover_color.r) * this->coefficientText);
		this->idleToHoverColorText[i].g = static_cast<unsigned>(static_cast<float>(auto_text_hover_color.g) * this->coefficientText);
		this->idleToHoverColorText[i].b = static_cast<unsigned>(static_cast<float>(auto_text_hover_color.b) * this->coefficientText);
		this->coefficientText += this->stepText;
	}

	// Outline
	this->shape.setOutlineColor(this->outlineIdleColor);

	// Button
	this->shape.setFillColor(this->buttonIdleColor);

	// Text
	this->text.setFillColor(this->textIdleColor);
}


gui::Button::~Button()
{

}


// Accessors:

const bool gui::Button::isPressed() const
{
	if (this->buttonState == gui::ButtonStates::Active)
	{
		return true;
	}

	return false;
}

const std::string gui::Button::getString() const
{
	return this->text.getString();
}

const uint32_t& gui::Button::getId() const
{
	return this->id;
}


// Modifiers:

void gui::Button::setString(const std::string text)
{
	this->text.setString(text);
}

void gui::Button::setId(const uint32_t id)
{
	this->id = id;
}


// Functions:

// Updates the booleans for hover and pressed.
void gui::Button::update(const sf::Vector2i& mouse_position, bool update_buttons, bool update_text)
{
	// Hover
	if (this->shape.getGlobalBounds().contains(sf::Vector2f(mouse_position)))
	{
		this->buttonState = gui::ButtonStates::Hover;

		// Pressed
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			this->buttonState = gui::ButtonStates::Active;
		}
	}
	else
	{
		// Idle
		this->buttonState = gui::ButtonStates::Idle;
	}

	switch (this->buttonState)
	{
	case gui::ButtonStates::Idle:

		if (update_buttons)
		{
			this->shape.setOutlineColor(this->currentColorOutline);
			this->shape.setFillColor(this->currentColorButton);
		}
		else
		{
			this->shape.setOutlineColor(this->outlineIdleColor);
			this->shape.setFillColor(this->buttonIdleColor);
		}

		if (update_text)
		{
			this->text.setFillColor(this->currentColorText);
		}
		else
		{
			this->text.setFillColor(this->textIdleColor);
		}

		break;

	case gui::ButtonStates::Hover:

		if (update_buttons)
		{
			this->shape.setOutlineColor(this->currentColorOutline);
			this->shape.setFillColor(this->currentColorButton);
		}
		else
		{
			this->shape.setOutlineColor(this->outlineHoverColor);
			this->shape.setFillColor(this->buttonHoverColor);
		}

		if (update_text)
		{
			this->text.setFillColor(this->currentColorText);
		}
		else
		{
			this->text.setFillColor(this->textHoverColor);
		}

		break;

	case gui::ButtonStates::Active:

		this->shape.setOutlineColor(this->outlineActiveColor);
		this->shape.setFillColor(this->buttonActiveColor);
		this->text.setFillColor(this->textActiveColor);

		break;

	default:

		this->shape.setOutlineColor(sf::Color::Green);
		this->shape.setFillColor(sf::Color::Red);
		this->text.setFillColor(sf::Color::Blue);

		break;
	}
}

void gui::Button::updateButtonColor(const float& dt)
{
	if (this->buttonState == gui::ButtonStates::Hover)
	{
		this->buttonTimer += 100.f * dt;
		if (this->buttonTimer >= this->buttonTimerMax)
		{
			this->buttonTimer = 0.f;
			this->currentColorOutline = this->idleToHoverColorOutline[static_cast<uint32_t>(this->colorNumber)];
			this->currentColorButton = this->idleToHoverColorButton[static_cast<uint32_t>(this->colorNumber)];
			this->currentColorText = this->idleToHoverColorText[static_cast<uint32_t>(this->colorNumber)];

			if (static_cast<uint32_t>(this->colorNumber) < (this->colorMaxStages - 1))
			{
				++this->colorNumber;
			}
		}
	}
	else if (this->buttonState == gui::ButtonStates::Idle)
	{
		this->buttonTimer += 100.f * dt;
		if (this->buttonTimer >= this->buttonTimerMax)
		{
			this->buttonTimer = 0.f;
			this->currentColorOutline = this->idleToHoverColorOutline[static_cast<uint32_t>(this->colorNumber)];
			this->currentColorButton = this->idleToHoverColorButton[static_cast<uint32_t>(this->colorNumber)];
			this->currentColorText = this->idleToHoverColorText[static_cast<uint32_t>(this->colorNumber)];

			if (this->colorNumber > 0)
			{
				--this->colorNumber;
			}
		}
	}
}

void gui::Button::render(sf::RenderTarget* target)
{
	target->draw(this->shape);
	target->draw(this->text);
}