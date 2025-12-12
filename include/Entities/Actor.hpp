#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include "../Core/Level.hpp"

class Actor {
public:
    Actor(float x, float y, float width, float height);
    virtual void Update(float dt, Level& level) = 0;
    virtual void Render(sf::RenderWindow& window);

    sf::FloatRect GetHitbox() const;
    
    // Necesario para que Game.cpp no falle
    sf::Vector2f GetPosition() const { return position; }

    void MoveX(float amount, Level& level, std::function<void()> onCollide);
    void MoveY(float amount, Level& level, std::function<void()> onCollide);
    bool CheckCollision(sf::FloatRect rect, Level& level);

protected:
    sf::Vector2f position;
    sf::Vector2f hitboxSize;
    
    // Visualización de la hitbox
    sf::RectangleShape hitboxDebug;

    // Variables de movimiento sub-pixel
    float remainderX;
    float remainderY;
};