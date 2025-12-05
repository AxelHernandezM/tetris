#include "../../include/Core/Level.hpp"

Level::Level() {
    tileSize = 40.0f; // Cada bloque mide 40 pixeles
}

void Level::Load() {
    // MAPA EXTENDIDO
    // He creado un túnel largo para probar la cámara.
    // 55 columnas de ancho.
    mapData = {
        "#######################################################",
        "#.....................................................#",
        "#.....................................................#",
        "#.....................................................#",
        "#......#####.......#.........#######..................#",
        "#..................#..................................#",
        "#...#..............#.....................#####........#",
        "#...#.......#......#.....................#............#",
        "#####.......#......#.......###########...#......#######",
        "#...........#####..#.......#.............#............#",
        "#..........................#.............#............#",
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

void Level::Render(sf::RenderWindow& window) {
    sf::RectangleShape block(sf::Vector2f(tileSize, tileSize));
    block.setFillColor(sf::Color::Blue);

    for (int y = 0; y < mapData.size(); y++) {
        for (int x = 0; x < mapData[y].size(); x++) {
            if (mapData[y][x] == '#') {
                block.setPosition(x * tileSize, y * tileSize);
                window.draw(block);
            }
        }
    }
}