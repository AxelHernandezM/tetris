#pragma once
#include <SFML/Graphics.hpp>
#include "Actor.hpp"
#include <vector>

// Forward declaration (Avísale que Level existe)
class Level;

class Player : public Actor {
public:
    Player(float x, float y);
    
    void Update(float dt, Level& level);
    void Render(sf::RenderWindow& window);

    // Mover al jugador (útil al cambiar de nivel)
    void SetSpawnPoint(sf::Vector2f newSpawn) {
        spawnPosition = newSpawn;
        position = spawnPosition;
        velocity = {0, 0};
    }

    // --- BANDERAS DE EVENTOS (Públicas para que Game.cpp las lea) ---
    bool eventJumped = false;
    bool eventDashed = false;
    bool eventLanded = false; 
    // ---------------------------------------------------------------

private:
    void HandleInput();
    void StartDash();
    void CheckDeath(Level& level);
    void Respawn();

    // Variables de movimiento
    sf::Vector2f velocity;
    bool isGrounded;
    
    // Dash
    bool hasDash;
    bool isDashing;
    float dashTimer;

    // Físicas Avanzadas
    int wallDir;
    float wallJumpTimer;
    float coyoteTimer;
    float jumpBufferTimer;

    // Animación
    sf::Texture texture;
    sf::Sprite sprite;
    std::vector<sf::IntRect> animationFrames;
    int currentFrame;
    float animationTimer;
    int facingDir;

    sf::Vector2f spawnPosition;
};