#include <SFML/Graphics.hpp>
#include "../../include/Entities/Player.hpp"
#include <iostream>

Player::Player(float x, float y) 
    : Actor(x, y, 16.f, 16.f) 
{
    velocity = {0.f, 0.f};
    isGrounded = false;
    
    // Inicializamos variables del Dash
    hasDash = true;
    isDashing = false;
    dashTimer = 0.0f;
}

void Player::Update(float dt, Level& level) {
    // 1. Gestionar el Temporizador del Dash
    if (isDashing) {
        dashTimer -= dt;
        if (dashTimer <= 0) {
            isDashing = false; // Se acabó el tiempo, volvemos a la normalidad
            velocity.x = 0;    // Frenamos un poco al terminar (opcional)
            velocity.y = 0;
        }
    }

    // 2. Leer Inputs (Solo si no estamos en pleno dash, o para iniciarlo)
    HandleInput();

    // 3. Aplicar Gravedad (SOLO SI NO ESTAMOS HACIENDO DASH)
    if (!isDashing) {
        velocity.y += GRAVITY * dt;
    }

    // 4. Movimiento X
    MoveX(velocity.x * dt, level, [this]() {
        velocity.x = 0;
    });

    // 5. Movimiento Y
    isGrounded = false;
    MoveY(velocity.y * dt, level, [this]() {
        // Si chocamos verticalmente...
        if (velocity.y > 0) { 
            isGrounded = true; 
            hasDash = true; // <--- ¡RECARGAR DASH AL TOCAR SUELO!
            
            // Si chocamos contra el suelo mientras hacíamos dash, se cancela
            isDashing = false; 
        }
        velocity.y = 0;
    });
}

void Player::StartDash() {
    if (hasDash) {
        isDashing = true;
        hasDash = false; // Gastamos el cartucho
        dashTimer = DASH_DURATION;

        // Calculamos dirección basada en las flechas
        float dirX = 0;
        float dirY = 0;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) dirX = 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  dirX = -1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    dirY = -1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  dirY = 1;

        // Si no presionamos nada, dash hacia donde mira (o derecha por defecto)
        if (dirX == 0 && dirY == 0) dirX = 1; 

        // Normalizar vectores diagonales (para que no sea más rápido en diagonal)
        // (Simplificado para este tutorial: solo asignamos velocidad directa)
        
        velocity.x = dirX * DASH_SPEED;
        velocity.y = dirY * DASH_SPEED;
    }
}

void Player::HandleInput() {
    // Si estamos en medio de un dash, ignoramos el input de movimiento normal
    if (isDashing) return;

    velocity.x = 0;

    // Movimiento Normal
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        velocity.x = SPEED;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        velocity.x = -SPEED;
    }

    // Salto
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && isGrounded) {
        velocity.y = JUMP_FORCE;
        isGrounded = false;
    }

    // Iniciar Dash (Tecla X)
    // Usamos un "truco" para que no se repita si dejas la tecla pegada:
    // En un motor real usaríamos "InputManager::IsJustPressed", pero aquí
    // confiaremos en que el cooldown del dash nos protege un poco.
    static bool wasXPressed = false;
    bool isXPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::X);

    if (isXPressed && !wasXPressed) {
        StartDash();
    }
    wasXPressed = isXPressed;
}

void Player::Render(sf::RenderWindow& window) {
    sf::RectangleShape rect(hitboxSize);
    rect.setPosition(position);
    
    // Cambiar color: Rojo normal, Cyan si estamos haciendo Dash
    if (isDashing) {
        rect.setFillColor(sf::Color::Cyan); 
    } else if (!hasDash) {
        rect.setFillColor(sf::Color(100, 100, 100)); // Gris si no tiene dash
    } else {
        rect.setFillColor(sf::Color::Red); // Rojo si tiene dash
    }
    
    window.draw(rect);
}