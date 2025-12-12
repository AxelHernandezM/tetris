#include <SFML/Graphics.hpp>
#include "../../include/core/Game.hpp"
#include <iostream>
#include <string> 

// 1. INICIALIZAR selectedTile
Game::Game() : isRunning(false), player(nullptr), editorMode(false), isClicking(false), selectedTile('#') {}

Game::~Game() {
    if (player) delete player;
}

void Game::Init() {
    window.create(sf::VideoMode(800, 600), "Celeste Clone - [JUEGO]");
    window.setFramerateLimit(60);
    camera.setSize(800.f, 600.f);
    level.Load(); 
    player = new Player(100.f, 100.f); 
    isRunning = true;
    lastFrameTime = std::chrono::high_resolution_clock::now();
}

void Game::Run() {
    Init();
    while (window.isOpen()) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> duration = currentTime - lastFrameTime;
        float deltaTime = duration.count();
        lastFrameTime = currentTime;

        ProcessInput();
        Update(deltaTime);
        Render();
    }
}

void Game::ProcessInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) window.close();

        // Toggle Modo Editor
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E) {
            editorMode = !editorMode;
            // Actualizamos el título para que sepas qué está pasando
            if (editorMode) window.setTitle("EDITOR: Pincel [" + std::string(1, selectedTile) + "]");
            else window.setTitle("Celeste Clone - [JUEGO]");
        }

        // --- NUEVO: CAMBIO DE PINCEL CON TECLAS 1, 2, 3... ---
        if (editorMode && event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Num1) {
                selectedTile = '#'; // Pared
                window.setTitle("EDITOR: Pincel [PARED #]");
            }
            if (event.key.code == sf::Keyboard::Num2) {
                selectedTile = '^'; // Pincho
                window.setTitle("EDITOR: Pincel [PINCHO ^]");
            }
            // AQUÍ PUEDES AGREGAR MÁS EN EL FUTURO:
            // if (event.key.code == sf::Keyboard::Num3) selectedTile = '@'; 
        }
    }

    // --- LÓGICA DE EDICIÓN ---
    if (editorMode) {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos, camera);

        int gridX = static_cast<int>(worldPos.x / level.GetTileSize());
        int gridY = static_cast<int>(worldPos.y / level.GetTileSize());

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            // --- NUEVO: USAMOS EL selectedTile EN LUGAR DE SIEMPRE '#' ---
            level.SetTile(gridX, gridY, selectedTile);
        }
        else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
            // Clic derecho siempre borra (Goma de borrar)
            level.SetTile(gridX, gridY, '.');
        }
    }
}

void Game::Update(float dt) {
    if (dt > 0.05f) dt = 0.05f;
    if (player) player->Update(dt, level);
}

void Game::Render() {
    window.clear(sf::Color::Black);

    if (player) {
        sf::Vector2f pos = player->GetPosition();
        camera.setCenter(pos.x + 10.f, pos.y + 10.f);
        window.setView(camera);
    }

    level.Render(window);
    if (player) player->Render(window);

    window.display();
}