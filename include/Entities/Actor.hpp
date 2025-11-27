#pragma once
#include <SFML/Graphics.hpp>
#include <functional> // Para los callbacks de colisión
#include <cmath>

class Actor {
public:
    Actor(float x, float y, float width, float height);
    virtual ~Actor() = default;

    // Métodos principales de movimiento (La magia de Celeste)
    // onCollide es una función que se ejecuta si chocamos (ej: detener velocidad)
    void MoveX(float amount, std::function<void()> onCollide);
    void MoveY(float amount, std::function<void()> onCollide);

    // Actualización y Renderizado básico
    virtual void Update(float dt) = 0; // Método puro, las hijas deben implementarlo
    virtual void Render(sf::RenderWindow& window);

    // Getters para la posición y la caja de colisión
    sf::Vector2f GetPosition() const { return position; }
    sf::FloatRect GetHitbox() const;

protected:
    sf::Vector2f position;
    sf::Vector2f hitboxSize; // Ancho y alto del personaje

private:
    // Los "bolsillos" para guardar fracciones de píxel
    float remainderX;
    float remainderY;

    // Simulación de detección de colisión con el mapa
    // En el futuro, esto consultará al archivo 'Level'
    bool IsSolid(sf::FloatRect box);
};