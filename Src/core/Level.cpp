#include "../../include/Core/Level.hpp"
#include <iostream>

// Variables para la textura del suelo
// (Las ponemos aquí para no modificar el archivo .hpp y hacerlo más rápido)
sf::Texture tileTexture;
sf::Sprite tileSprite;

Level::Level() {
    tileSize = 40.0f; // Cada bloque mide 40x40 pixeles

    // --- CARGAR TEXTURA DEL SUELO ---
    // Asegúrate de haber guardado el recorte como "Assets/tile.png"
    if (!tileTexture.loadFromFile("Assets/tile.png")) {
        std::cout << "Error: No se encontró Assets/tile.png" << std::endl;
        // Si falla, el sprite se verá blanco, pero el juego no se cerrará.
    }
    
    tileSprite.setTexture(tileTexture);
}

void Level::Load() {
    // Mapa Extendido con Pinchos (^) y Paredes (#)
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
    
    // Solo '#' es pared sólida
    return mapData[gridY][gridX] == '#';
}

bool Level::IsHazard(int gridX, int gridY) {
    if (gridY < 0 || gridY >= mapData.size()) return false;
    if (gridX < 0 || gridX >= mapData[0].size()) return false;
    
    // '^' es pincho mortal
    return mapData[gridY][gridX] == '^';
}

void Level::Render(sf::RenderWindow& window) {
    // Recorremos todo el mapa
    for (int y = 0; y < mapData.size(); y++) {
        for (int x = 0; x < mapData[y].size(); x++) {
            char cell = mapData[y][x];

            if (cell == '#') {
                // --- DIBUJAR SUELO CON TEXTURA ---
                
                // 1. Posicionar
                tileSprite.setPosition(x * tileSize, y * tileSize);
                
                // 2. Calcular Escala (Stretch)
                // Esto hace que tu imagen (sea 16x16 o 32x32) se estire para llenar el bloque de 40x40
                sf::Vector2u textureSize = tileTexture.getSize();
                float scaleX = tileSize / (float)textureSize.x;
                float scaleY = tileSize / (float)textureSize.y;
                tileSprite.setScale(scaleX, scaleY);

                // 3. Dibujar
                window.draw(tileSprite);
            }
            else if (cell == '^') {
                // --- DIBUJAR PINCHOS (Rojos) ---
                sf::RectangleShape spike(sf::Vector2f(tileSize, tileSize / 2));
                spike.setFillColor(sf::Color::Red);
                
                // Los ponemos en la mitad de abajo del bloque
                spike.setPosition(x * tileSize, y * tileSize + (tileSize / 2));
                
                window.draw(spike);
            }
        }
    }
}