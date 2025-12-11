#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Level {
public:
    Level();
    void Load();
    void Render(sf::RenderWindow& window);
    
    bool IsSolid(int gridX, int gridY);
    bool IsHazard(int gridX, int gridY);
    float GetTileSize() const { return tileSize; }

private:
    // --- ESTO ES LO QUE TE FALTABA ---
    std::vector<std::string> mapData;
    // ---------------------------------
    
    float tileSize;
};