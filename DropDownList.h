#pragma once

#include "Button.h"

namespace gui
{
	class DropDownList
	{
	private:

		//Variables:

		float keyTime;
		const float keyTimeMax;
		sf::Font& font;
		gui::Button* activeElement;
		std::vector<gui::Button*> list;
		bool showList;
		std::string strWidth;
		std::string strHeight;

	public:

		//Constructors and Destructor:

		DropDownList
		(
			float x, float y, float width, float height,
			sf::Font& font, unsigned int font_size, std::string list[],
			unsigned number_of_elements, unsigned default_index = 0
		);
		DropDownList
		(
			float x, float y, float width, float height,
			sf::VideoMode video_mode,
			sf::Font& font, unsigned int font_size, std::string list[],
			unsigned number_of_elements, unsigned default_index = 0
		);
		virtual ~DropDownList();

		//Accessors:

		const bool getKeyTime();
		const uint32_t& getActiveElementId() const;

		//Fucntions:

		void updateKeyTime(const float& dt);
		void update(const sf::Vector2i& mouse_position, const float& dt);
		void render(sf::RenderTarget* target);
	};
}

