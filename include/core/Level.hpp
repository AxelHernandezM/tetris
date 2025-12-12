#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

struct Strawberry {
    sf::Sprite sprite;
    bool collected = false;
    sf::FloatRect hitbox;
};

class Level {
public:
    Level();

    void Load(int levelIndex);
    void NextLevel();
    bool IsLastLevel();
    int GetCurrentLevelIndex() const { return currentLevelIndex; }
    sf::Vector2f GetPlayerSpawn() const { return playerSpawn; }

    void Render(sf::RenderWindow& window);
    
    bool IsSolid(int gridX, int gridY);
    bool IsHazard(int gridX, int gridY);
    bool IsHazard(sf::FloatRect rect);
    void SetTile(int gridX, int gridY, char tile);
    bool CheckCollection(sf::FloatRect playerBox);
    bool CheckWin(sf::FloatRect playerBox);
    float GetTileSize() const { return tileSize; }

private:
    std::vector<std::string> mapData; 
    std::vector<std::vector<std::string>> allLevels;
    int currentLevelIndex;
    sf::Vector2f playerSpawn; 

    float tileSize;
    sf::Texture bgTexture; sf::Sprite bgSprite;
    std::vector<Strawberry> strawberries;

    sf::Texture flagTexture; sf::Sprite flagSprite; sf::FloatRect flagHitbox;
    
    // --- AQUÍ ESTABA EL ERROR ---
    // Antes se llamaba tileTexture, ahora debe ser tilesetTexture
    sf::Texture tilesetTexture; 
    // ----------------------------
    
    sf::Sprite tileSprite;
    sf::Texture spikeTexture; sf::Sprite spikeSprite;
    sf::Texture berryTexture;
};