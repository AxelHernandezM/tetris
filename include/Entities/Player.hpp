#pragma once
#include "Actor.hpp"
#include <vector> // <--- OBLIGATORIO: Sin esto, el vector no funciona

class Player : public Actor {
public:
    Player(float x, float y);
    void Update(float dt, Level& level) override;
    void Render(sf::RenderWindow& window) override;

private:
    void HandleInput();
    void StartDash();
    void CheckDeath(Level& level);
    void Respawn();

    // Física
    sf::Vector2f velocity;
    sf::Vector2f spawnPosition;
    bool isGrounded;

    // Mecánicas
    bool hasDash;
    bool isDashing;
    float dashTimer;
    int wallDir;
    float wallJumpTimer;
    float coyoteTimer;
    float jumpBufferTimer;

    // Gráficos
    sf::Texture texture;
    sf::Sprite sprite;
    int facingDir;

    // --- ESTA ES LA VARIABLE QUE TE FALTA ---
    std::vector<sf::IntRect> animationFrames; 
    // ----------------------------------------

    int currentFrame;
    float animationTimer;
    
    // Constantes
    const float SPEED = 160.0f;
    const float GRAVITY = 900.0f;
    const float JUMP_FORCE = -350.0f;
    const float DASH_SPEED = 400.0f;
    const float DASH_DURATION = 0.15f;
    const float WALL_SLIDE_SPEED = 50.0f;
    const float WALL_JUMP_FORCE_X = 140.0f;
    const float WALL_JUMP_FORCE_Y = -320.0f;
    const float WALL_JUMP_TIME = 0.15f;
    const float COYOTE_TIME = 0.1f;
    const float JUMP_BUFFER_TIME = 0.1f;
    const float ANIM_SPEED = 0.1f;
};