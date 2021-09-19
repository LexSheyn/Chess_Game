#pragma once

#include "State.h"
#include "Selector.h"
#include "PauseMenu.h"
#include "FadeScreen.h"
#include "TileMap.h"

enum Team : uint32_t
{
    White = 0,
    Black,
    None
};

class GameState :
	public State
{
private:

    sf::View view;
    sf::Vector2i viewGridPosition;
    sf::RenderTexture renderTexture;
    sf::Sprite renderSprite;
    sf::Font font;
    bool buttonPressed;
    bool gameOver;

    // GUI
    gui::Selector selector;
    PauseMenu* pauseMenu;
    gui::FadeScreen fadeScreen;

    // Shader
    sf::Shader coreShader;

    // Figures
    static const uint32_t teams = 2;
    static const uint32_t figures = 9;
    static const size_t positions = 4;
    Pawn* pawn[teams][figures];    
    Pawn* selectedPawn[teams];
    uint32_t lastSelectedIndex[teams];
    sf::Vector2f chosenPosition[teams];    
    std::vector<sf::Vector2f> allowedPositions[teams];
    sf::Vector2f direction[teams];
    sf::Vector2f directionNormalized[teams];

    // Turn
    uint32_t turn;
    uint32_t turnLast;
    sf::Clock timer;
    float timerMax;

    // Map
    TileMap* tileMap;
    static const uint32_t tiles = 9;
    std::map<const Team, Tile*[tiles]> teamTile;

// FPS counter:

    sf::Text fps;

// private: Functions:

    void initVariables();
    void initDefferedRender();
    void initView();
    void initPauseMenu();
    void initShaders();
    void initFigures();
    void initTileMap();
    void initGui();

public:

// Constructor and Destructor:

    GameState(StateData* state_data);
    virtual ~GameState();

// Functions:

    void endTurn();
    void switchTurn();

    void updateView(const float& dt);
    void updateInput(const float& dt);
    void updatePlayerInput(const float& dt);
    void updateInputAI(const float& dt);
    void updateTimer();
    void updatePauseMenuButtons();
    void uodateTileMap(const float& dt);
    void updateAllowedMovement(const float& dt);
    void updateAllowedMovementAI(const float& dt);
    void updateGui(const float& dt);
    void updateMovement(const float& dt);
    void updateMovementAI(const float& dt);
    void updateFigures(const float& dt);
    void updateGameOver(const float& dt);
    void update(const float& dt);

    void renderFigures();
    void render(sf::RenderTarget* target = nullptr);
};

