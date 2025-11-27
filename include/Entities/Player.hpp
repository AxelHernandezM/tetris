#pragma once
#include "Actor.hpp"

class Player : public Actor {
public:
    Player(float x, float y);
    
    // Sobrescribimos Update para poner la lógica específica del héroe
    void Update(float dt) override;
    
    // Sobrescribimos Render para dibujarlo de un color diferente (Rojo)
    void Render(sf::RenderWindow& window) override;

private:
    // --- Ajustes de Física (Tuning) ---
    // Jugar con estos valores cambia totalmente el "Game Feel"
    const float SPEED = 120.0f;       // Velocidad horizontal
    const float GRAVITY = 900.0f;     // Gravedad fuerte para saltos rápidos
    const float JUMP_FORCE = -350.0f; // Fuerza negativa (hacia arriba)

    // --- Estado ---
    sf::Vector2f velocity; // Velocidad actual (x, y)
    bool isGrounded;       // ¿Estamos tocando el suelo?

    // Métodos internos
    void HandleInput();
};