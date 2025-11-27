#include "../../include/Entities/Actor.hpp"

Actor::Actor(float x, float y, float width, float height)
    : position(x, y), hitboxSize(width, height), remainderX(0), remainderY(0) {}

sf::FloatRect Actor::GetHitbox() const {
    return sf::FloatRect(position.x, position.y, hitboxSize.x, hitboxSize.y);
}

// Lógica placeholder para colisiones (luego la conectaremos al Mapa real)
bool Actor::IsSolid(sf::FloatRect box) {
    // Por ahora, digamos que hay un piso en Y = 500
    if (box.top + box.height > 500) return true; 
    return false;
}

void Actor::Render(sf::RenderWindow& window) {
    // Dibujo simple de debug (un rectángulo verde)
    sf::RectangleShape rect(hitboxSize);
    rect.setPosition(position);
    rect.setFillColor(sf::Color::Green);
    window.draw(rect);
}

// ---------------------------------------------------------
// EL ALGORITMO DE MOVIMIENTO (MoveX)
// ---------------------------------------------------------
void Actor::MoveX(float amount, std::function<void()> onCollide) {
    remainderX += amount;
    int move = std::round(remainderX); // Cuántos píxeles enteros mover

    if (move != 0) {
        remainderX -= move; // Restamos lo que vamos a mover del bolsillo
        int sign = (move > 0) ? 1 : -1; // ¿Derecha (1) o Izquierda (-1)?

        // Bucle píxel por píxel
        while (move != 0) {
            sf::FloatRect nextHitbox = GetHitbox();
            nextHitbox.left += sign; // Probamos mover 1 píxel

            if (!IsSolid(nextHitbox)) {
                // Si no hay pared, movemos al personaje
                position.x += sign;
                move -= sign;
            } else {
                // ¡Chocamos!
                if (onCollide) onCollide(); // Ejecutar lógica de choque (ej: velocidad = 0)
                break; // Dejamos de movernos
            }
        }
    }
}

// ---------------------------------------------------------
// EL ALGORITMO DE MOVIMIENTO (MoveY) - Idéntico a X
// ---------------------------------------------------------
void Actor::MoveY(float amount, std::function<void()> onCollide) {
    remainderY += amount;
    int move = std::round(remainderY);

    if (move != 0) {
        remainderY -= move;
        int sign = (move > 0) ? 1 : -1;

        while (move != 0) {
            sf::FloatRect nextHitbox = GetHitbox();
            nextHitbox.top += sign; // Probamos mover 1 píxel vertical

            if (!IsSolid(nextHitbox)) {
                position.y += sign;
                move -= sign;
            } else {
                if (onCollide) onCollide();
                break;
            }
        }
    }
}