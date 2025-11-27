#pragma once
#include <SFML/Graphics.hpp>
#include <chrono>
#include "../Entities/Player.hpp" // Necesitamos conocer al Player

class Game {
public:
    Game();
    ~Game();

    // Inicializa la ventana y los objetos
    void Init();

    // El bucle principal (Game Loop)
    void Run();

private:
    // Funciones internas del ciclo
    void ProcessInput();
    void Update(float dt);
    void Render();

    // Variables miembros
    sf::RenderWindow window;
    bool isRunning;
    
    // Puntero al jugador (lo crearemos dinámicamente)
    Player* player;

    // Manejo del tiempo
    std::chrono::high_resolution_clock::time_point lastFrameTime;
};