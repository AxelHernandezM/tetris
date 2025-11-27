#include "../../include/Core/Game.hpp"
#include <iostream>

Game::Game() : isRunning(false), player(nullptr) {
    // Constructor vacío, la inicialización real va en Init()
}

Game::~Game() {
    // Limpieza de memoria
    if (player) {
        delete player;
    }
}

void Game::Init() {
    // Creamos una ventana de 800x600 con título
    window.create(sf::VideoMode(800, 600), "Celeste Clone C++");
    
    // Limitamos a 60 FPS para no quemar la CPU (opcional, pero recomendado al inicio)
    window.setFramerateLimit(60);

    // Inicializamos al jugador en la posición (100, 100)
    player = new Player(100.f, 100.f);

    isRunning = true;
    lastFrameTime = std::chrono::high_resolution_clock::now();
}

void Game::Run() {
    Init();

    while (window.isOpen()) {
        // --- 1. Calcular DeltaTime ---
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> duration = currentTime - lastFrameTime;
        float deltaTime = duration.count();
        lastFrameTime = currentTime;

        // --- 2. Ciclo del Juego ---
        ProcessInput();
        Update(deltaTime);
        Render();
    }
}

void Game::ProcessInput() {
    sf::Event event;
    // SFML maneja una cola de eventos (teclado, mouse, cerrar ventana)
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        // Nota: El movimiento del jugador se maneja en Player::HandleInput(),
        // aquí solo manejamos eventos globales de la ventana.
    }
}

void Game::Update(float dt) {
    // PROTECCIÓN DE LAG:
    // Si la ventana se congela (dt muy grande), limitamos el dt a 0.05 (20 FPS).
    // Esto evita que el personaje atraviese el suelo por "teletransportarse" muy lejos.
    if (dt > 0.05f) dt = 0.05f;

    if (player) {
        player->Update(dt);
    }
}

void Game::Render() {
    // 1. Limpiar pantalla con color negro
    window.clear(sf::Color::Black);

    // 2. Dibujar DEBUG del piso
    // En Actor.cpp dijimos "if (box.top + box.height > 500) return true;"
    // Así que dibujamos un piso visual en Y=500 para ver dónde chocamos.
    sf::RectangleShape debugFloor(sf::Vector2f(800.f, 100.f));
    debugFloor.setPosition(0.f, 500.f);
    debugFloor.setFillColor(sf::Color(50, 50, 150)); // Azul oscuro
    window.draw(debugFloor);

    // 3. Dibujar al Jugador
    if (player) {
        player->Render(window);
    }

    // 4. Mostrar lo dibujado (Swap Buffers)
    window.display();
}