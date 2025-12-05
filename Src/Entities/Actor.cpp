#include "../../include/Entities/Actor.hpp"
#include <cmath>

Actor::Actor(float x, float y, float width, float height)
    : position(x, y), hitboxSize(width, height), remainderX(0), remainderY(0) {}

sf::FloatRect Actor::GetHitbox() const {
    return sf::FloatRect(position.x, position.y, hitboxSize.x, hitboxSize.y);
}

void Actor::Render(sf::RenderWindow& window) {
    // Dibujo de debug (caja verde)
    sf::RectangleShape rect(hitboxSize);
    rect.setPosition(position);
    rect.setFillColor(sf::Color::Green);
    window.draw(rect);
}

// --- DETECCIÓN DE COLISIÓN (GRID) ---
bool Actor::CheckCollision(sf::FloatRect box, Level& level) {
    float ts = level.GetTileSize();
    
    // Convertimos pixeles a coordenadas de cuadrícula (0, 1, 2...)
    int left_tile   = static_cast<int>(box.left / ts);
    int right_tile  = static_cast<int>((box.left + box.width - 0.01f) / ts); // -0.01 para no agarrar el bloque vecino exacto
    int top_tile    = static_cast<int>(box.top / ts);
    int bottom_tile = static_cast<int>((box.top + box.height - 0.01f) / ts);

    // Revisamos cada bloque que la caja esté tocando
    for (int i = left_tile; i <= right_tile; i++) {
        for (int j = top_tile; j <= bottom_tile; j++) {
            if (level.IsSolid(i, j)) {
                return true; // ¡Chocamos!
            }
        }
    }
    return false;
}

// --- MOVIMIENTO X ---
void Actor::MoveX(float amount, Level& level, std::function<void()> onCollide) {
    remainderX += amount;
    int move = std::round(remainderX);

    if (move != 0) {
        remainderX -= move;
        int sign = (move > 0) ? 1 : -1;

        while (move != 0) {
            sf::FloatRect nextHitbox = GetHitbox();
            nextHitbox.left += sign;

            if (!CheckCollision(nextHitbox, level)) {
                position.x += sign;
                move -= sign;
            } else {
                if (onCollide) onCollide();
                break;
            }
        }
    }
}

// --- MOVIMIENTO Y ---
void Actor::MoveY(float amount, Level& level, std::function<void()> onCollide) {
    remainderY += amount;
    int move = std::round(remainderY);

    if (move != 0) {
        remainderY -= move;
        int sign = (move > 0) ? 1 : -1;

        while (move != 0) {
            sf::FloatRect nextHitbox = GetHitbox();
            nextHitbox.top += sign;

            if (!CheckCollision(nextHitbox, level)) {
                position.y += sign;
                move -= sign;
            } else {
                if (onCollide) onCollide();
                break;
            }
        }
    }
}