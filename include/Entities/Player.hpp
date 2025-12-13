#pragma once
#include <SFML/Graphics.hpp>
#include "Actor.hpp"
#include <vector>

class Level;

class Player : public Actor {
public:
    Player(float x, float y);
    
    void Update(float dt, Level& level);
    void Render(sf::RenderWindow& window);

    void SetSpawnPoint(sf::Vector2f newSpawn) {
        spawnPosition = newSpawn;
        position = spawnPosition;
        velocity = {0, 0};
    }

    // --- BANDERAS DE EVENTOS ---
    bool eventJumped = false;
    bool eventDashed = false;
    bool eventLanded = false; 
    bool eventDied = false; // <--- ESTA FALTABA
    // ---------------------------

private:
    void HandleInput();
    void StartDash();
    void CheckDeath(Level& level);
    void Respawn();

    sf::Vector2f velocity;
    bool isGrounded;
    
    bool hasDash;
    bool isDashing;
    float dashTimer;

    int wallDir;
    float wallJumpTimer;
    float coyoteTimer;
    float jumpBufferTimer;

    sf::Texture texture;
    sf::Sprite sprite;
    std::vector<sf::IntRect> animationFrames;
    int currentFrame;
    float animationTimer;
    int facingDir;

    sf::Vector2f spawnPosition;
};