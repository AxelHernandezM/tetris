#include "../../include/Core/Level.hpp"
#include <iostream>
#include <cmath>
#include <algorithm> 

Level::Level() {
    tileSize = 40.0f;
    currentLevelIndex = 0;

    // 1. CARGAR TILESET
    // Necesitas una imagen de 96x32: [0-32: Pasto] [32-64: Tierra] [64-96: Ladrillo]
    if (!tilesetTexture.loadFromFile("Assets/tileset.png")) {
        std::cout << "[ERROR] Falta Assets/tileset.png" << std::endl;
        tilesetTexture.create(96, 32); 
    }
    tileSprite.setTexture(tilesetTexture);

    if (!spikeTexture.loadFromFile("Assets/spike.png")) spikeTexture.create(32, 32);
    spikeSprite.setTexture(spikeTexture);

    if (!berryTexture.loadFromFile("Assets/strawberry.png")) berryTexture.create(16, 16);

    if (!flagTexture.loadFromFile("Assets/flag.png")) flagTexture.create(32, 32);
    flagSprite.setTexture(flagTexture);

    bool bgLoaded = false;
    std::vector<std::string> bgNames = { "Assets/Background.png", "Assets/background.png" };
    for (const auto& name : bgNames) {
        if (bgTexture.loadFromFile(name)) {
            bgLoaded = true; bgSprite.setColor(sf::Color::White); break;
        }
    }
    if (!bgLoaded) { bgTexture.create(100, 100); bgSprite.setColor(sf::Color(50, 50, 50)); }
    bgSprite.setTexture(bgTexture);

    // ==========================================
    //            DISEÑO DE NIVELES
    // ==========================================
    
    // NIVEL 1
    std::vector<std::string> level1 = {
        "#######################################################",
        "#.....................................................#",
        "#.....................................................#",
        "#...P.................................................#",
        "#.......BBBBB................^......................F.#",
        "#######......................#.......#######.......####",
        "#..................#.........#........................#",
        "#...#..............#.........#...........#####........#",
        "#...#.......B......#.........#...........#............#",
        "#####.......B......#.......###########...#......#######",
        "#...........BBBBB..#.......#.............#............#",
        "#...^^^^^..................#.............#............#",
        "#..........................#.............#............#",
        "#......##..........#.......#.............#.@..........#",
        "#......##....#######.......#.............#######......#",
        "#######################################################"
    };

    // NIVEL 2
    std::vector<std::string> level2 = {
        "#######################################################",
        "#F....................................................#",
        "#####.................................................#",
        "#...................................^^^...............#",
        "#...........@......................#####..............#",
        "#.........#####.......................................#",
        "#.......................BBBB..........................#",
        "#.....................................................#",
        "#......^.........^.............P......................#",
        "#####################.......#######.......#############",
        "#...................#.......#.....#.......#...........#",
        "#...................#.......#.....#.......#...........#",
        "#...................#.......#.....#.......#...........#",
        "#...@...............#.......#.....#.......#...........#",
        "#..###..............#.......#.....#.......#...........#",
        "#######################################################"
    };

    allLevels.push_back(level1);
    allLevels.push_back(level2);
}

void Level::Load(int levelIndex) {
    if (levelIndex >= 0 && levelIndex < allLevels.size()) {
        currentLevelIndex = levelIndex;
        mapData = allLevels[levelIndex]; 
    }
    strawberries.clear();
    for (int y = 0; y < mapData.size(); y++) {
        for (int x = 0; x < mapData[y].size(); x++) {
            char cell = mapData[y][x];
            float posX = x * tileSize; float posY = y * tileSize;
            if (cell == '@') {
                Strawberry s; s.sprite.setTexture(berryTexture);
                s.sprite.setPosition(posX + 10, posY + 10);
                s.hitbox = sf::FloatRect(posX + 10, posY + 10, 20, 20);
                strawberries.push_back(s);
                mapData[y][x] = '.'; 
            } else if (cell == 'F') {
                flagSprite.setPosition(posX, posY);
                float sX = tileSize / flagTexture.getSize().x; float sY = tileSize / flagTexture.getSize().y;
                flagSprite.setScale(sX, sY);
                flagHitbox = sf::FloatRect(posX, posY, tileSize, tileSize);
                mapData[y][x] = '.';
            } else if (cell == 'P') {
                playerSpawn = sf::Vector2f(posX, posY);
                mapData[y][x] = '.'; 
            }
        }
    }
}

void Level::NextLevel() { Load(currentLevelIndex + 1); }
bool Level::IsLastLevel() { return currentLevelIndex >= allLevels.size() - 1; }
bool Level::CheckWin(sf::FloatRect playerBox) { return playerBox.intersects(flagHitbox); }
bool Level::CheckCollection(sf::FloatRect playerBox) {
    bool collectedSomething = false;
    for (auto& berry : strawberries) {
        if (!berry.collected && playerBox.intersects(berry.hitbox)) {
            berry.collected = true; collectedSomething = true;
        }
    }
    return collectedSomething;
}

// --- DETECCIÓN DE SÓLIDOS ---
bool Level::IsSolid(int gridX, int gridY) {
    if (gridY < 0 || gridY >= mapData.size()) return false;
    if (gridX < 0 || gridX >= mapData[0].size()) return false;
    
    char c = mapData[gridY][gridX];
    // '#' es Tierra/Pasto, 'B' es Ladrillo
    return c == '#' || c == 'B';
}

bool Level::IsHazard(int gridX, int gridY) {
    if (gridY < 0 || gridY >= mapData.size()) return false;
    if (gridX < 0 || gridX >= mapData[0].size()) return false;
    return mapData[gridY][gridX] == '^';
}
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
void Level::SetTile(int gridX, int gridY, char tile) {
    if (gridY >= 0 && gridY < mapData.size()) {
        if (gridX >= 0 && gridX < mapData[gridY].size()) mapData[gridY][gridX] = tile;
    }
}

// --- RENDERIZADO ---
void Level::Render(sf::RenderWindow& window) {
    sf::View currentView = window.getView();
    bgSprite.setOrigin(bgTexture.getSize().x / 2.0f, bgTexture.getSize().y / 2.0f);
    bgSprite.setPosition(currentView.getCenter());
    float scale = std::max(currentView.getSize().x / bgTexture.getSize().x, currentView.getSize().y / bgTexture.getSize().y); 
    bgSprite.setScale(scale, scale);
    window.draw(bgSprite);

    for (int y = 0; y < mapData.size(); y++) {
        for (int x = 0; x < mapData[y].size(); x++) {
            char cell = mapData[y][x];
            
            // SI ES PASTO (#) O LADRILLO (B)
            if (cell == '#' || cell == 'B') {
                tileSprite.setPosition(x * tileSize, y * tileSize);
                
                // Escalar a 40px (asumiendo tiles de 32px)
                float sX = tileSize / 32.0f;
                float sY = tileSize / 32.0f;
                tileSprite.setScale(sX, sY);

                if (cell == '#') {
                    // --- LÓGICA DE PASTO/TIERRA ---
                    bool blockAbove = IsSolid(x, y - 1); 
                    if (!blockAbove) tileSprite.setTextureRect(sf::IntRect(0, 0, 32, 32)); // Pasto (0-32)
                    else tileSprite.setTextureRect(sf::IntRect(32, 0, 32, 32));            // Tierra (32-64)
                }
                else if (cell == 'B') {
                    // --- LÓGICA DE LADRILLO ---
                    // Ladrillo (64-96)
                    tileSprite.setTextureRect(sf::IntRect(64, 0, 32, 32));
                }

                window.draw(tileSprite);
            }
            // SI ES PINCHO
            else if (cell == '^') {
                spikeSprite.setPosition(x * tileSize, y * tileSize);
                spikeSprite.setScale(tileSize / spikeTexture.getSize().x, tileSize / spikeTexture.getSize().y);
                window.draw(spikeSprite);
            }
        }
    }
    
    // Dibujar objetos restantes
    window.draw(flagSprite);
    for (const auto& berry : strawberries) {
        if (!berry.collected) window.draw(berry.sprite);
    }
}