#include <SFML/Graphics.hpp> // <--- ESTO ARREGLA EL ERROR DE sf::Keyboard
#include "../../include/Entities/Player.hpp"
#include <iostream>

Player::Player(float x, float y) 
    : Actor(x, y, 16.f, 16.f) 
{
    velocity = {0.f, 0.f};
    isGrounded = false;
}

void Player::Update(float dt) {
    HandleInput();

    // Gravedad
    velocity.y += 900.0f * dt;

    // Movimiento X
    MoveX(velocity.x * dt, [this]() {
        velocity.x = 0;
    });

    // Movimiento Y
    isGrounded = false;
    MoveY(velocity.y * dt, [this]() {
        if (velocity.y > 0) isGrounded = true; 
        velocity.y = 0;
    });
}

void Player::HandleInput() {
    velocity.x = 0;

    // Aquí usamos sf::Keyboard
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        velocity.x = 120.0f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        velocity.x = -120.0f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && isGrounded) {
        velocity.y = -350.0f;
        isGrounded = false;
    }
}

void Player::Render(sf::RenderWindow& window) {
    sf::RectangleShape rect(hitboxSize);
    rect.setPosition(position);
    rect.setFillColor(sf::Color::Red);
    window.draw(rect);
}