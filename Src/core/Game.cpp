#include <SFML/Graphics.hpp>
#include "../../include/core/Game.hpp"
#include <iostream>

Game::Game() : isRunning(false), player(nullptr) {}

Game::~Game() {
    if (player) delete player;
}

void Game::Init() {
    window.create(sf::VideoMode(800, 600), "Celeste Clone C++");
    window.setFramerateLimit(60);

    // --- NUEVO: Configurar tamaño de la cámara ---
    // Hacemos que la cámara vea un área de 800x600 (igual que la ventana)
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
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }
}

void Game::Update(float dt) {
    if (dt > 0.05f) dt = 0.05f;

    if (player) {
        player->Update(dt, level);
    }
}

void Game::Render() {
    window.clear(sf::Color::Black);

    // --- NUEVO: Lógica de la Cámara ---
    if (player) {
        // 1. Obtenemos la posición del centro del jugador
        sf::Vector2f pos = player->GetPosition();
        
        // Ajuste opcional: Sumamos mitad del tamaño del player (8,8) para centrar exacto
        pos.x += 8.f; 
        pos.y += 8.f;

        // 2. Le decimos a la cámara "Mira aquí"
        camera.setCenter(pos);

        // 3. Aplicamos la cámara a la ventana
        window.setView(camera);
    }
    // ----------------------------------

    // Todo lo que dibujemos después de setView se verá afectado por la cámara
    level.Render(window);

    if (player) {
        player->Render(window);
    }

    window.display();
}