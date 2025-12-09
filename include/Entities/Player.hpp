#pragma once
#include "Actor.hpp"

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

    sf::Vector2f velocity;
    sf::Vector2f spawnPosition;
    bool isGrounded;

    // Variables Dash
    bool hasDash;
    bool isDashing;
    float dashTimer;

    // Variables Wall Jump
    int wallDir;
    float wallJumpTimer;

    // Game Juice
    float coyoteTimer;
    float jumpBufferTimer;

    // --- NUEVO: GRÁFICOS ---
   sf::Texture texture;
    sf::Sprite sprite;
    int facingDir; 

    // Variables de Animación
    int currentFrame;       // ¿En qué cuadro vamos? (0, 1, 2...)
    float animationTimer;   // Cronómetro para cambiar de cuadro
    int numFrames;          // Cuántos cuadros tiene la animación total
    int frameWidth;         // Ancho de un solo cuadro (ej. 32px)
    int frameHeight;        // Alto de un solo cuadro (ej. 32px)
    const float ANIM_SPEED = 0.1f; // Velocidad (cambia cada 0.1 segundos)

    // Ajustes
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
};