#include "../../include/Core/Level.hpp"
#include <iostream>
#include <cmath>
#include <algorithm> // Necesario para std::max

// Variables globales para texturas (Legacy)
sf::Texture tileTexture;
sf::Sprite tileSprite;

Level::Level() {
    tileSize = 40.0f;

    // --- 1. Cargar Tiles ---
    if (!tileTexture.loadFromFile("Assets/tile.png")) {
        if (!tileTexture.loadFromFile("Assets/tile.png.png")) {
             std::cout << "ERROR: No se encuentra Assets/tile.png" << std::endl;
             tileTexture.create(32, 32);
        }
    }
    tileSprite.setTexture(tileTexture);

    // --- 2. Cargar Fondo ---
    bool bgLoaded = false;
    // Lista de intentos por si Windows cambia el nombre
    std::vector<std::string> bgNames = {
        "Assets/Background.png",
        "Assets/background.png",
        "Assets/Background.png.png"
    };

    for (const auto& name : bgNames) {
        if (bgTexture.loadFromFile(name)) {
            bgLoaded = true;
            bgSprite.setColor(sf::Color::White); // Color normal
            break;
        }
    }

    if (!bgLoaded) {
        // Fallback visual: Si ves la pantalla ROSA, es que no encontró la imagen
        std::cout << "ADVERTENCIA: Fondo no encontrado. Usando magenta de debug." << std::endl;
        bgTexture.create(100, 100);
        bgSprite.setColor(sf::Color::Magenta); 
    }
    
    bgSprite.setTexture(bgTexture);
}

void Level::Load() {
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

// --- NUEVO: Implementación de la función que faltaba ---
bool Level::IsHazard(sf::FloatRect rect) {
    int startX = static_cast<int>(rect.left / tileSize);
    int endX = static_cast<int>((rect.left + rect.width) / tileSize);
    int startY = static_cast<int>(rect.top / tileSize);
    int endY = static_cast<int>((rect.top + rect.height) / tileSize);

    for (int y = startY; y <= endY; ++y) {
        for (int x = startX; x <= endX; ++x) {
            if (IsHazard(x, y)) {
                sf::FloatRect spikeRect(x * tileSize, y * tileSize + tileSize/2, tileSize, tileSize/2);
                if (rect.intersects(spikeRect)) return true;
            }
        }
    }
    return false;
}

void Level::Render(sf::RenderWindow& window) {
    // 1. DIBUJAR FONDO (Background)
    sf::View currentView = window.getView();
    sf::Vector2f viewCenter = currentView.getCenter();
    sf::Vector2f viewSize = currentView.getSize();

    bgSprite.setOrigin(bgTexture.getSize().x / 2.0f, bgTexture.getSize().y / 2.0f);
    bgSprite.setPosition(viewCenter);

    float scaleX = viewSize.x / bgTexture.getSize().x;
    float scaleY = viewSize.y / bgTexture.getSize().y;
    float scale = std::max(scaleX, scaleY); 
    
    bgSprite.setScale(scale, scale);
    window.draw(bgSprite);

    // 2. DIBUJAR TILES
    for (int y = 0; y < mapData.size(); y++) {
        for (int x = 0; x < mapData[y].size(); x++) {
            char cell = mapData[y][x];
            if (cell == '#') {
                tileSprite.setPosition(x * tileSize, y * tileSize);
                sf::Vector2u texSize = tileTexture.getSize();
                if (texSize.x > 0) {
                    float sX = tileSize / texSize.x;
                    float sY = tileSize / texSize.y;
                    tileSprite.setScale(sX, sY);
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