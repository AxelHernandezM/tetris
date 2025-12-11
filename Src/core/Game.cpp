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

    // Configurar cámara
    camera.setSize(800.f, 600.f);

    // --- CORRECCIÓN AQUÍ ---
    // Ya no usamos LoadFromFile("...") porque el mapa está escrito en el código.
    // Solo llamamos a Load() sin parámetros.
    level.Load(); 
    // -----------------------

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

    // Lógica de la Cámara
    if (player) {
        sf::Vector2f pos = player->GetPosition();
        camera.setCenter(pos.x + 10.f, pos.y + 10.f);
        window.setView(camera);
    }

    // Dibujar
    level.Render(window);

    if (player) {
        player->Render(window);
    }

    window.display();
}