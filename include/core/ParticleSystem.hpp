#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

// Una sola partícula (un granito de polvo)
struct Particle {
    sf::Vector2f position;
    sf::Vector2f velocity;
    float lifetime;      // Tiempo de vida restante
    float maxLifetime;   // Tiempo de vida total (para calcular transparencia)
};

// El administrador de todas las partículas
class ParticleSystem {
public:
    ParticleSystem();

    // Función para soltar un chorro de partículas en un punto
    void Emit(sf::Vector2f position, int count);

    void Update(float dt);
    void Render(sf::RenderWindow& window);

private:
    std::vector<Particle> particles;
    sf::RectangleShape particleShape; // Usaremos un cuadradito blanco simple
};