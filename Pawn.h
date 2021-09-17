#pragma once
#include "Entity.h"
class Pawn :
    public Entity
{
private:

// Variables:

    bool selected;
    bool clickable;
    bool buttonPressed;
    sf::RectangleShape selectionFrame;

// private: Functions:

    void initVariables();
    void initAnimations();

public:

// Constructors and Destructor:

    Pawn(float x, float y, sf::Texture& texture_sheet, bool ckickable = true);
    ~Pawn();

// Accessors:

    const bool& isSelected() const;
    const Pawn* getPawn() const;

// Modifiers:

    void select();
    void unselect();

// Functions:

    void playAnimation(std::string key, const float& dt, bool priority = false);

    void updateAnimation(const float& dt);
    bool checkSelection(sf::Vector2f& mouse_position_view);
    void update(sf::Vector2f& mouse_position_view, const float& dt) override;

    void render(sf::RenderTarget* target, sf::Shader* shader = nullptr, const sf::Vector2f shader_position = sf::Vector2f(), const bool show_hitbox = false);
};

