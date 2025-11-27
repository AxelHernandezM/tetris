#include "../../include/Entities/Player.hpp"
#include <iostream> // Para debug

Player::Player(float x, float y) 
    : Actor(x, y, 16, 16) // Tamaño del personaje: 16x16 píxeles
{
    velocity = {0, 0};
    isGrounded = false;
}

void Player::Update(float dt) {
    // 1. INPUT: Leer teclado
    HandleInput();

    // 2. FÍSICA: Aplicar Gravedad
    // Siempre aplicamos gravedad, a menos que estemos escalando (futuro)
    velocity.y += GRAVITY * dt;

    // 3. MOVIMIENTO HORIZONTAL (Eje X)
    // Pasamos una función lambda que se ejecuta si chocamos contra una pared
    MoveX(velocity.x * dt, [this]() {
        velocity.x = 0; // Si choco con pared, mi velocidad X muere
    });

    // 4. MOVIMIENTO VERTICAL (Eje Y)
    // Aquí es donde detectamos el suelo
    isGrounded = false; // Asumimos que estamos en el aire hasta probar lo contrario
    
    MoveY(velocity.y * dt, [this]() {
        // Esta función se llama si chocamos arriba (techo) o abajo (suelo)
        
        if (velocity.y > 0) { 
            // Si nos movíamos hacia abajo y chocamos, es el suelo.
            isGrounded = true; 
        }
        
        velocity.y = 0; // Detener velocidad vertical al chocar
    });
}

void Player::HandleInput() {
    // Resetear velocidad X cada frame (para movimiento preciso/instantáneo)
    velocity.x = 0;

    // Movimiento Izquierda/Derecha
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        velocity.x = SPEED;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        velocity.x = -SPEED;
    }

    // Salto
    // Solo podemos saltar si estamos en el suelo (isGrounded)
    // NOTA: En el futuro, aquí añadiremos "Coyote Time"
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && isGrounded) {
        velocity.y = JUMP_FORCE;
        isGrounded = false; // Ya no estamos en el suelo
    }
}

void Player::Render(sf::RenderWindow& window) {
    // Dibujamos al jugador como un cuadrado rojo
    sf::RectangleShape rect(hitboxSize);
    rect.setPosition(position);
    rect.setFillColor(sf::Color::Red);
    window.draw(rect);
}