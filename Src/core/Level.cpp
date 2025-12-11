#include "../../include/Core/Level.hpp"
#include <iostream>

// Variables globales para texturas
sf::Texture tileTexture;
sf::Sprite tileSprite;

Level::Level() {
    tileSize = 40.0f;

    // Cargar imagen
    if (!tileTexture.loadFromFile("Assets/tile.png")) {
        std::cout << "ERROR: No se encuentra Assets/tile.png" << std::endl;
        tileTexture.create(32, 32); 
    }
    tileSprite.setTexture(tileTexture);
}

void Level::Load() {
    // Definimos el mapa aquí mismo
    mapData = {
        "#######################################################",
        "#.....................................................#",
        "#.....................................................#",
        "#.....................................................#",
        "#............................^........................#",
        "#......#####.......#.........#.......#######..........#",
        "#..................#.........#........................#",
        "#...#..............#.........#...........#####........#",
        "#...#.......#......#.........#...........#............#",
        "#####.......#......#.......###########...#......#######",
        "#...........#####..#.......#.............#............#",
        "#...^^^^^..................#.............#............#",
        "#..........................#.............#............#",
        "#......##..........#.......#.............#............#",
        "#......##....#######.......#.............#............#",
        "#######################################################"
    };
}

bool Level::IsSolid(int gridX, int gridY) {
    if (gridY < 0 || gridY >= mapData.size()) return false;
    if (gridX < 0 || gridX >= mapData[0].size()) return false;
    return mapData[gridY][gridX] == '#';
}

bool Level::IsHazard(int gridX, int gridY) {
    if (gridY < 0 || gridY >= mapData.size()) return false;
    if (gridX < 0 || gridX >= mapData[0].size()) return false;
    return mapData[gridY][gridX] == '^';
}

void Level::Render(sf::RenderWindow& window) {
    for (int y = 0; y < mapData.size(); y++) {
        for (int x = 0; x < mapData[y].size(); x++) {
            char cell = mapData[y][x];

            if (cell == '#') {
                tileSprite.setPosition(x * tileSize, y * tileSize);
                
                sf::Vector2u texSize = tileTexture.getSize();
                if (texSize.x > 0) {
                    float scaleX = tileSize / texSize.x;
                    float scaleY = tileSize / texSize.y;
                    tileSprite.setScale(scaleX, scaleY);
                }
                window.draw(tileSprite);
            }
            else if (cell == '^') {
                sf::RectangleShape spike(sf::Vector2f(tileSize, tileSize/2));
                spike.setFillColor(sf::Color::Red);
                spike.setPosition(x * tileSize, y * tileSize + tileSize/2);
                window.draw(spike);
            }
        }
    }
}