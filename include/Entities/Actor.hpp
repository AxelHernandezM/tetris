#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <cmath>
#include "../Core/Level.hpp"

class Actor {
public:
    Actor(float x, float y, float width, float height);
    virtual ~Actor() = default;

    virtual void Update(float dt, Level& level) = 0; 
    
    // --- CAMBIO AQUÍ: Agregamos 'virtual' ---
    virtual void Render(sf::RenderWindow& window);

    sf::Vector2f GetPosition() const { return position; }
    sf::FloatRect GetHitbox() const;

protected:
  protected:
    sf::Vector2f position;
    sf::Vector2f hitboxSize;
    float remainderX;
    float remainderY;

    void MoveX(float amount, Level& level, std::function<void()> onCollide);
    void MoveY(float amount, Level& level, std::function<void()> onCollide);

    // --- CAMBIO: MOVIDO DE PRIVATE A PROTECTED ---
    bool CheckCollision(sf::FloatRect box, Level& level);
};