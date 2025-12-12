#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <chrono>
#include <string> 
#include "../Entities/Player.hpp"
#include "Level.hpp"
#include "ParticleSystem.hpp"

// ESTADOS DEL JUEGO
enum class GameState {
    MENU,
    PLAYING
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
    
    // UI JUEGO
    sf::Font font; sf::Text scoreText; sf::Text winText; int score;

    // UI MENU
    sf::Text titleText;       
    sf::Text instructionText; 
    GameState currentState;   
    float menuTimer;          

    bool gameWon; 
    sf::SoundBuffer buffJump, buffDash, buffCollect; 
    sf::Sound sndJump, sndDash, sndCollect;       
    sf::Music music;   
    ParticleSystem particleSystem; 
    Player* player;
    Level level;
    std::chrono::high_resolution_clock::time_point lastFrameTime;
};