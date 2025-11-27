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
    if (player) player->Update(dt);
}

void Game::Render() {
    window.clear(sf::Color::Black);
    
    // Piso de prueba
    sf::RectangleShape debugFloor(sf::Vector2f(800.f, 100.f));
    debugFloor.setPosition(0.f, 500.f);
    debugFloor.setFillColor(sf::Color(50, 50, 150));
    window.draw(debugFloor);

    if (player) player->Render(window);

    window.display();
}