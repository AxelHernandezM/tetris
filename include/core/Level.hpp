#pragma once
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

class Level {
public:
    Level();
    void Load();
    void Render(sf::RenderWindow& window);
    
    // Verificadores
    bool IsSolid(int x, int y);
    bool IsHazard(int x, int y);
    bool IsHazard(sf::FloatRect rect); 

    // --- NUEVA FUNCIÓN QUE FALTABA ---
    void SetTile(int x, int y, char tile);
    // ---------------------------------

    float GetTileSize() const { return tileSize; }

private:
    std::vector<std::string> mapData;
    float tileSize;

    sf::Texture bgTexture;
    sf::Sprite bgSprite;
};