#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <chrono>
#include <string> 
#include "../Entities/Player.hpp"
#include "Level.hpp"
#include "ParticleSystem.hpp"

// Define the game states
enum class GameState {
    MENU,
    PLAYING,
    ENDING // Added ENDING state
};

class Game {
public:
    Game();
    ~Game();

    void Init();
    void Run();

private:
    void ProcessInput();
    void Update(float dt);
    void Render();
    void ResetGame(); 

    sf::RenderWindow window;
    sf::View camera;
    bool isRunning;
    bool editorMode; bool isClicking; char selectedTile;
    
    // UI Game
    sf::Font font; 
    sf::Text scoreText; 
    sf::Text winText; 
    int score;

    // UI Menu
    sf::Text titleText;       
    sf::Text instructionText; 
    GameState currentState;   
    float menuTimer;          

    // --- NEW: Ending System Variables ---
    sf::Texture badEndingTex;  // Texture for bad ending
    sf::Texture goodEndingTex; // Texture for good ending
    sf::Sprite endingSprite;   // Sprite to display the ending image
    
    // Configuration
    const int LIMONES_PARA_GANAR = 5; // Threshold for good ending
    // ---------------------------------

    bool gameWon; 

    // Audio
    sf::SoundBuffer buffJump, buffDash, buffCollect; 
    sf::Sound sndJump, sndDash, sndCollect;   
    
    // Background Music
    sf::Music music;       

    // Particles
    ParticleSystem particleSystem; 

    Player* player;
    Level level;
    std::chrono::high_resolution_clock::time_point lastFrameTime;
};