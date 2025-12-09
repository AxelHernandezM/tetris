#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Level {
public:
    Level();

    void Load();
    void Render(sf::RenderWindow& window);
    
    bool IsSolid(int gridX, int gridY); // Para paredes (#)
    bool IsHazard(int gridX, int gridY); // NUEVO: Para pinchos (^)

    float GetTileSize() const { return tileSize; }

private:
    std::vector<std::string> mapData;
    float tileSize;
};