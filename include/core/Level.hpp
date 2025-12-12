#pragma once
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

class Level {
public:
    Level();
    void Load();
    void Render(sf::RenderWindow& window);
    
    // Verificadores básicos
    bool IsSolid(int x, int y);
    bool IsHazard(int x, int y);

    // --- NUEVO: ESTA LÍNEA FALTABA ---
    // Sobrecarga para detectar colisión con el rectángulo del jugador
    bool IsHazard(sf::FloatRect rect); 

    // Getter
    float GetTileSize() const { return tileSize; }

private:
    std::vector<std::string> mapData;
    float tileSize;

    // --- NUEVO: ESTAS VARIABLES FALTABAN ---
    sf::Texture bgTexture;
    sf::Sprite bgSprite;
};