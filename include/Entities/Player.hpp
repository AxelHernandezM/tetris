#pragma once
#include "Actor.hpp"

class Player : public Actor {
public:
    Player(float x, float y);
    void Update(float dt, Level& level) override;
    void Render(sf::RenderWindow& window) override;

private:
    void HandleInput();
    void StartDash(); // Nueva función para iniciar el dash

    sf::Vector2f velocity;
    bool isGrounded;

    // --- VARIABLES DEL DASH ---
    bool hasDash;        // ¿Tengo energía para hacer dash? (se recarga en el suelo)
    bool isDashing;      // ¿Estoy en medio de un dash ahora mismo?
    float dashTimer;     // Contador de tiempo (cuánto dura el dash)

    // Ajustes (Tuning)
    const float SPEED = 160.0f;
    const float GRAVITY = 900.0f;
    const float JUMP_FORCE = -350.0f;
    
    const float DASH_SPEED = 400.0f;  // Velocidad explosiva
    const float DASH_DURATION = 0.15f; // Dura muy poco (150ms)
};