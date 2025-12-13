#include "../../include/Core/Level.hpp"
#include <iostream>
#include <cmath>
#include <algorithm> 

Level::Level() {
    tileSize = 40.0f;
    currentLevelIndex = 0;

    // --- RUTAS ACTUALIZADAS ---
    if (!tilesetTexture.loadFromFile("Assets/imagenes/tileset.png")) {
        std::cout << "[ERROR] Falta Assets/imagenes/tileset.png" << std::endl;
        tilesetTexture.create(96, 32); 
    }
    tileSprite.setTexture(tilesetTexture);

    if (!spikeTexture.loadFromFile("Assets/imagenes/spike.png")) spikeTexture.create(32, 32);
    spikeSprite.setTexture(spikeTexture);

    if (!berryTexture.loadFromFile("Assets/imagenes/strawberry.png")) berryTexture.create(16, 16);

    if (!flagTexture.loadFromFile("Assets/imagenes/flag.png")) flagTexture.create(32, 32);
    flagSprite.setTexture(flagTexture);

    bool bgLoaded = false;
    std::vector<std::string> bgNames = { "Assets/imagenes/Background.png", "Assets/imagenes/background.png" };
    for (const auto& name : bgNames) {
        if (bgTexture.loadFromFile(name)) {
            bgLoaded = true; bgSprite.setColor(sf::Color::White); break;
        }
    }
    if (!bgLoaded) { bgTexture.create(100, 100); bgSprite.setColor(sf::Color(50, 50, 50)); }
    bgSprite.setTexture(bgTexture);

    // ==========================================
    //            DISEÑO DE 4 NIVELES
    // ==========================================
    
    // --- NIVEL 1: TUTORIAL ---
    std::vector<std::string> level1 = {
        "#################################################################",
        "#.............................##................................#",
        "#.............................##................................#",
        "#.............................##................................#",
        "#.............................##................................#",
        "#......##...............@......##......####..#..#..#..#......F...#",
        "#..P...BBBBBBBB........^BB^....###..........................######",
        "BBBBBBBB....##.....BB...........####.............................#",
        "#...........#####...................BBBB####.....................#",
        "#.............................##............##..................#",
        "#...................BB.....................####.................#",
        "#..@............BB.............................####.............#",
        "#####....##..#...BBBBBBBBBBBB.......................#...#...######",
        "#.................###########..............................######",
        "#^^^^^^^^^^^^^^^^^###########..............................######",
        "#############################......######......####........######",
        "#############################..##..######......####........######",
        "#############################^^^^^^######^^##^^####........######",
        "#################################################################"
    };

    // --- NIVEL 2: ESCALADA (Wall Jump) ---
    std::vector<std::string> level2 = {
        "#################################################################",
        "#................................................................#",
        "#...............................................................#",
        "#####.....................#####..................................#",
        "#####.....................#####............####..................#",
        "#####.........@...........#####............####..................#",
        "#...........#####.........#####............####..................#",
        "#.........................#####............####..................#",
        "#..P......................#####............####......@...........#",
        "###########......###......#####............####....#####.........#",
        "###########......###......#####............####....##.B..........#",
        "###########......###......#####............####....##BB..........#",
        "###########......###......#####^^^^####^^^^####......BB.......F..#",
        "###########^^^^^^###......#####################.......B#.....BB####",
        "####################......#####################^^^^^^^#############",
        "####################......#########################################"
    };

    // --- NIVEL 3: PELIGRO (Pinchos) ---
    std::vector<std::string> level3 = {
        "#####################################################################",
"#####################################################################",
"#####################################################################",
"#####################################################################",
"#......................................................B............#",
"#......................................................B..F.........#",
"#......................................................#####........#",
"#.........@............................................BBBBB........#",
"#.......#####.......................@..................BBBBB........#",
"#.......#####.....................#####................BBBBB........#",
"#.......#####....BB...............#BB#B...#................BBBBB....#",
"#.......#####.....................#BB#B...#.......###......BBBBB....#",
"####....#####.........###.........#BB#B...#.......#B#......BBBBB....#",
"#.......#####.........#B#.........#BB#B...#.......#B#......BBBBB....#",
"#.P.....#####.........#B#.........#BB#B...#.......#B#......BBBBB....#",
"####....#####.........#B#.........#BB#B...#.......#B#......BBBBB....#",
"####....#####^^^^##^^^#B#...##....#BB#B...B.......#B#...............#",
"####^^^^#################.........#########^^^^^^#######BBBBB########",
"####BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB####",
"####BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB####",
"####BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB####"
    };

    // --- NIVEL 4: EL CASTILLO FINAL (Largo y Complejo) ---
    std::vector<std::string> level4 = {
 "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
"#BBBBBBBBBBBBBBBBBBBBBBBB....B...................................B",
"#BBBBBBBBBBBBBBBBBBBBBBBB..............................@.........B",
"#BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB...B",
"#................................................................B",
"#................................................................B",
"#........................@.......................................B",
"#.P..............................................................B",
"#BB#B#................#BBBBB#..........................F.........B",
"##BB#.................#BBBBB#.............#B#........#B#B#.......B",
"###B#.............######BBBBB#^^##^^^.....#B#.........#B#B#......B",
"#####^^^^^^#B#........#...................#B#........#BBBBB......B",
"###########BB#........#...................#BB#............#B##B..B",
"###########BB#........#...@....############BB#^^##^^##...........B",
"###########BB#........#^^^#^^^######################BB...........B",
"####BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB####"
    };

    // AGREGAR LOS 4 NIVELES A LA LISTA
    allLevels.push_back(level1);
    allLevels.push_back(level2);
    allLevels.push_back(level3);
    allLevels.push_back(level4);
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
            } 
            else if (cell == 'F') {
                // Bandera Gigante
                float bigSize = tileSize * 2.0f; 
                float sX = bigSize / flagTexture.getSize().x;
                float sY = bigSize / flagTexture.getSize().y;
                flagSprite.setScale(sX, sY);
                // Ajuste de altura para que crezca hacia arriba
                flagSprite.setPosition(posX, posY - tileSize);
                flagHitbox = sf::FloatRect(posX, posY - tileSize, bigSize, bigSize);
                mapData[y][x] = '.';
            } 
            else if (cell == 'P') {
                // CORRECCIÓN DE ALTURA DEL JUGADOR
                // El jugador mide 64px, el bloque 40px. 
                // Subimos 24px para que no nazca enterrado.
                playerSpawn = sf::Vector2f(posX, posY - 24.0f);
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
    return c == '#' || c == 'B'; // # = Tierra, B = Ladrillo
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
            
            if (cell == '#' || cell == 'B') {
                tileSprite.setPosition(x * tileSize, y * tileSize);
                float sX = tileSize / 32.0f; float sY = tileSize / 32.0f;
                tileSprite.setScale(sX, sY);

                if (cell == '#') {
                    // Autotiling Pasto/Tierra
                    bool blockAbove = IsSolid(x, y - 1); 
                    if (!blockAbove) tileSprite.setTextureRect(sf::IntRect(0, 0, 32, 32)); 
                    else tileSprite.setTextureRect(sf::IntRect(32, 0, 32, 32));            
                }
                else if (cell == 'B') {
                    // Ladrillo (Pixel 64 en adelante)
                    tileSprite.setTextureRect(sf::IntRect(64, 0, 32, 32));
                }
                window.draw(tileSprite);
            }
            else if (cell == '^') {
                spikeSprite.setPosition(x * tileSize, y * tileSize);
                spikeSprite.setScale(tileSize / spikeTexture.getSize().x, tileSize / spikeTexture.getSize().y);
                window.draw(spikeSprite);
            }
        }
    }
    window.draw(flagSprite);
    for (const auto& berry : strawberries) {
        if (!berry.collected) window.draw(berry.sprite);
    }
}