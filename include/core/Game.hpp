#pragma once
#include <SFML/Graphics.hpp>
#include <chrono>
#include "../Entities/Player.hpp"
#include "Level.hpp"

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

    sf::RenderWindow window;
    
    // --- NUEVO: La Cámara ---
    sf::View camera;
    // ------------------------

    bool isRunning;
    
    Player* player;
    Level level;
    
    std::chrono::high_resolution_clock::time_point lastFrameTime;
};