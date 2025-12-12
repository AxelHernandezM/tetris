#include "../../include/Core/ParticleSystem.hpp"
#include <cstdlib> // Para números aleatorios (rand)
#include <cmath>

ParticleSystem::ParticleSystem() {
    // Configurar cómo se ve una partícula de polvo
    particleShape.setSize(sf::Vector2f(4.f, 4.f)); // Cuadradito de 4x4
    particleShape.setFillColor(sf::Color::White);
    particleShape.setOrigin(2.f, 2.f); // Pivote al centro
}

void ParticleSystem::Emit(sf::Vector2f position, int count) {
    for (int i = 0; i < count; i++) {
        Particle p;
        p.position = position;
        
        // Velocidad aleatoria: Hacia arriba y a los lados
        float angle = (rand() % 180) * 3.14159f / 180.f; // Ángulo hacia arriba
        float speed = (rand() % 50 + 20.f); // Velocidad entre 20 y 70
        p.velocity.x = std::cos(angle) * speed * ((rand() % 2 == 0) ? 1 : -1); // Izq o Der
        p.velocity.y = -std::sin(angle) * speed; // Siempre hacia arriba (negativo Y)

        // Tiempo de vida aleatorio (entre 0.3 y 0.6 segundos)
        p.lifetime = (rand() % 30 + 30) / 100.f;
        p.maxLifetime = p.lifetime;

        particles.push_back(p);
    }
}

void ParticleSystem::Update(float dt) {
    for (int i = 0; i < particles.size(); ) {
        // Mover
        particles[i].position += particles[i].velocity * dt;
        
        // Envejecer
        particles[i].lifetime -= dt;

        // Si murió, borrarla. Si no, avanzar al siguiente.
        if (particles[i].lifetime <= 0) {
            particles.erase(particles.begin() + i);
        } else {
            i++;
        }
    }
}

void ParticleSystem::Render(sf::RenderWindow& window) {
    for (auto& p : particles) {
        particleShape.setPosition(p.position);
        
        // Calcular transparencia (Alpha) basado en la vida restante
        // Empieza en 255 (blanco) y baja a 0 (transparente)
        float alphaRatio = p.lifetime / p.maxLifetime;
        sf::Color c = sf::Color::White;
        c.a = static_cast<sf::Uint8>(255 * alphaRatio);
        particleShape.setFillColor(c);

        window.draw(particleShape);
    }
}