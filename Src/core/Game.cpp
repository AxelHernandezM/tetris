#include "../../include/core/Game.hpp"
#include <iostream>
#include <string> 
#include <cstdlib> 
#include <ctime>   
#include <cmath> 

Game::Game() : isRunning(false), player(nullptr), editorMode(false), isClicking(false), selectedTile('#'), score(0), gameWon(false) {}

Game::~Game() { if (player) delete player; }

void Game::Init() {
    std::srand(static_cast<unsigned>(std::time(nullptr))); 

    window.create(sf::VideoMode(800, 600), "Celeste Clone - [JUEGO]");
    window.setFramerateLimit(60);
    camera.setSize(800.f, 600.f);
    
    currentState = GameState::MENU; 
    menuTimer = 0.0f;

    if (!font.loadFromFile("Assets/font.ttf")) std::cout << "[ERROR] Falta font.ttf" << std::endl;
    
    // UI
    scoreText.setFont(font); scoreText.setString("Fresas: 0"); scoreText.setCharacterSize(30);
    scoreText.setFillColor(sf::Color::White); scoreText.setOutlineColor(sf::Color::Black); scoreText.setOutlineThickness(2.0f);
    scoreText.setPosition(20.f, 20.f);

    winText.setFont(font); winText.setString("GANASTE\nPresiona R"); winText.setCharacterSize(50);
    winText.setFillColor(sf::Color::Yellow); winText.setOutlineColor(sf::Color::Black); winText.setOutlineThickness(4.0f);

    // MENU UI
    titleText.setFont(font); titleText.setString("CELESTE CLONE"); titleText.setCharacterSize(60);
    titleText.setFillColor(sf::Color::Cyan); titleText.setOutlineColor(sf::Color::Black); titleText.setOutlineThickness(4.0f);
    sf::FloatRect tr = titleText.getLocalBounds(); titleText.setOrigin(tr.width/2, tr.height/2); titleText.setPosition(400, 200);

    instructionText.setFont(font); instructionText.setString("Presiona ENTER para Jugar"); instructionText.setCharacterSize(30);
    instructionText.setFillColor(sf::Color::White);
    sf::FloatRect ir = instructionText.getLocalBounds(); instructionText.setOrigin(ir.width/2, ir.height/2); instructionText.setPosition(400, 400);

    // Audio
    if (buffJump.loadFromFile("Assets/jump.wav")) sndJump.setBuffer(buffJump);
    if (buffDash.loadFromFile("Assets/dash.wav")) sndDash.setBuffer(buffDash);
    if (buffCollect.loadFromFile("Assets/collect.wav")) sndCollect.setBuffer(buffCollect);
    
    sndJump.setVolume(50); sndDash.setVolume(60); sndCollect.setVolume(70);

    // Musica
    if (!music.openFromFile("Assets/music.ogg")) std::cout << "[ERROR] Falta music.ogg" << std::endl;
    else { music.setLoop(true); music.setVolume(25); music.play(); }
}

void Game::ResetGame() {
    level.Load(0); 
    if (player) delete player;
    sf::Vector2f startPos = level.GetPlayerSpawn();
    player = new Player(startPos.x, startPos.y); 
    score = 0;
    gameWon = false;
    scoreText.setString("Fresas: 0");
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
        
        if (currentState == GameState::MENU) {
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                ResetGame(); 
                currentState = GameState::PLAYING; 
                sndCollect.play();
            }
        }
        else if (currentState == GameState::PLAYING) {
            if (gameWon && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                currentState = GameState::MENU; 
            }
            // EDITOR: Activar
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E) {
                editorMode = !editorMode;
                if (editorMode) window.setTitle("EDITOR ACTIVADO"); else window.setTitle("Celeste Clone - [JUEGO]");
            }
            // EDITOR: Seleccionar Bloque
            if (editorMode && event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Num1) { selectedTile = '#'; window.setTitle("EDITOR: [1] PASTO #"); }
                if (event.key.code == sf::Keyboard::Num2) { selectedTile = '^'; window.setTitle("EDITOR: [2] PINCHO ^"); }
                if (event.key.code == sf::Keyboard::Num3) { selectedTile = '@'; window.setTitle("EDITOR: [3] FRESA @"); }
                
                // --- NUEVO: TECLA 4 PARA LADRILLO ---
                if (event.key.code == sf::Keyboard::Num4) { 
                    selectedTile = 'B'; 
                    window.setTitle("EDITOR: [4] LADRILLO B"); 
                }
            }
        }
    }
    
    if (currentState == GameState::PLAYING && editorMode) {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos, camera);
        int gridX = static_cast<int>(worldPos.x / level.GetTileSize());
        int gridY = static_cast<int>(worldPos.y / level.GetTileSize());
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) level.SetTile(gridX, gridY, selectedTile);
        else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) level.SetTile(gridX, gridY, '.');
    }
}

void Game::Update(float dt) {
    if (dt > 0.05f) dt = 0.05f;

    if (currentState == GameState::MENU) {
        menuTimer += dt * 3.0f;
        titleText.setPosition(400.f, 200.f + std::sin(menuTimer) * 5.f); 
        int alpha = static_cast<int>((std::sin(menuTimer * 2.0f) + 1.0f) * 127.0f);
        sf::Color c = sf::Color::White; c.a = alpha;
        instructionText.setFillColor(c);
        return; 
    }

    if (gameWon) return;

    particleSystem.Update(dt);

    if (player) {
        player->Update(dt, level);

        sf::Vector2f feetPos = player->GetPosition();
        feetPos.x += player->GetHitbox().width / 2.0f;
        feetPos.y += player->GetHitbox().height;

        if (player->eventJumped) { sndJump.play(); particleSystem.Emit(feetPos, 2); }
        if (player->eventDashed) { sndDash.play(); }
        if (player->eventLanded) { particleSystem.Emit(feetPos, 4); }
        
        if (level.CheckCollection(player->GetHitbox())) {
            score++; scoreText.setString("Fresas: " + std::to_string(score));
            sndCollect.play(); 
        }

        if (level.CheckWin(player->GetHitbox())) {
            if (level.IsLastLevel()) {
                gameWon = true; sndCollect.play();
            } else {
                level.NextLevel();
                player->SetSpawnPoint(level.GetPlayerSpawn());
                sndCollect.play();
            }
        }
    }
}

void Game::Render() {
    window.clear(sf::Color::Black);

    if (currentState == GameState::MENU) {
        window.setView(window.getDefaultView());
        window.draw(titleText);
        window.draw(instructionText);
        window.display();
        return; 
    }

    if (player) {
        sf::Vector2f pos = player->GetPosition();
        camera.setCenter(pos.x + 10.f, pos.y + 10.f);
        window.setView(camera);
    }
    
    level.Render(window);
    particleSystem.Render(window); 
    if (player) player->Render(window);

    window.setView(window.getDefaultView());
    window.draw(scoreText);

    if (gameWon) {
        sf::FloatRect textRect = winText.getLocalBounds();
        winText.setOrigin(textRect.width / 2.0f, textRect.height / 2.0f);
        winText.setPosition(400.f, 300.f);
        window.draw(winText);
    }

    window.display();
}