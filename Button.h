#pragma once

namespace gui
{
	enum class ButtonName
	{
		Start = 0,
		Settings,
		Editor,
		Exit,
		Resume,
		Save,
		Load,
		Quit,
		Apply,
		Back
	};

	enum class ButtonStates
	{
		Idle = 0,
		Hover,
		Active
	};

	//Converts a percentage value to pixels relative to current resolution in the X axis.
	//@param	float percent			The percentage value.
	//@param	sf::VideoMode& video_mode	Current video mode of the window (resolution).
	//@return	float				The calculated pixel value.
	const float percentIntoX(const float percent, const sf::VideoMode& video_mode);

	//Converts a percentage value to pixels relative to current resolution in the Y axis.
	//@param	float percent			The percentage value.
	//@param	sf::VideoMode& video_mode	Current video mode of the window (resolution).
	//@return	float				The calculated pixel value.
	const float percentIntoY(const float percent, const sf::VideoMode& video_mode);

	//Calculates the character size for text using the current resolution and a constant.
	//@param	float percent			The percentage value.
	//@param	sf::VideoMode& video_mode	Current video mode of the window (resolution).
	//@return	unsigned			The calculated character size value.
	const unsigned calculateCharSize(const float percent, const sf::VideoMode& video_mode);

	class Button
	{
	private:

		//Variables:

		ButtonStates buttonState;
		uint32_t id;

		sf::RectangleShape shape;
		sf::Font* font;
		sf::Text text;
		uint32_t fontSize;

		sf::Color outlineIdleColor;
		sf::Color outlineHoverColor;
		sf::Color outlineActiveColor;

		sf::Color buttonIdleColor;
		sf::Color buttonHoverColor;
		sf::Color buttonActiveColor;

		sf::Color textIdleColor;
		sf::Color textHoverColor;
		sf::Color textActiveColor;

		sf::Color currentColorOutline;
		sf::Color currentColorButton;
		sf::Color currentColorText;

		int32_t colorNumber;
		const static uint32_t colorMaxStages = 6;

		sf::Color idleToHoverColorOutline[colorMaxStages];
		sf::Color idleToHoverColorButton[colorMaxStages];
		sf::Color idleToHoverColorText[colorMaxStages];

		float stepOutline;
		float stepButton;
		float stepText;

		float coefficientOutline;
		float coefficientButton;
		float coefficientText;

		float buttonTimer;
		float buttonTimerMax;

		//private: Functions:

		void initVariables();

	public:

		//Constructors and Destructor:

		Button
		(
			const float x, const float y, const float width, const float height,
			sf::Font* font, const std::string& str, const uint32_t font_size,
			sf::Color button_idle_color, sf::Color button_hover_color, sf::Color button_active_color,
			sf::Color text_idle_color, sf::Color text_hover_color, sf::Color text_active_color,
			sf::Color outline_idle_color = sf::Color::Transparent,
			sf::Color outline_hover_color = sf::Color::Transparent,
			sf::Color outline_active_color = sf::Color::Transparent,
			const uint32_t id = 0
		);
		Button
		(
			const float x, const float y, const float width, const float height,
			sf::Font* font, const std::string& text, const uint32_t font_size,
			sf::Color auto_outline_hover_color = sf::Color::Transparent,
			sf::Color auto_button_hover_color = sf::Color::Cyan,
			sf::Color auto_text_hover_color = sf::Color::Black,
			const uint32_t id = 0
		);
		virtual ~Button();

		//Accessors:

		const bool isPressed() const;
		const std::string getString() const;
		const uint32_t& getId() const;

		//Modifiers:

		void setString(const std::string str);
		void setId(const uint32_t id);

		//Fucntions:

		void update(const sf::Vector2i& mouse_position, bool update_buttons = false, bool update_text = false);
		void updateButtonColor(const float& dt);
		void render(sf::RenderTarget* target);
	};
}
