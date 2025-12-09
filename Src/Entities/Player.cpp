#include <SFML/Graphics.hpp>
#include "../../include/Entities/Player.hpp"
#include <iostream>
#include <cmath> // Necesario para std::abs

// Variable auxiliar para columnas (solo se usa en este archivo)
int numCols = 1; 

Player::Player(float x, float y) 
    : Actor(x, y, 16.f, 16.f) 
{
    spawnPosition = {x, y};
    velocity = {0.f, 0.f};
    isGrounded = false;
    
    // Inicializar mecánicas
    hasDash = true; isDashing = false; dashTimer = 0.0f;
    wallDir = 0; wallJumpTimer = 0.0f;
    coyoteTimer = 0.0f; jumpBufferTimer = 0.0f;

    // --- 1. CARGAR HOJA DE SPRITES ---
    // Asegúrate de que esta sea la imagen de la cuadrícula
    if (!texture.loadFromFile("Assets/player_run.png")) {
        std::cout << "Error: No se encontró Assets/player_run.png" << std::endl;
        texture.create(32, 32); // Textura de respaldo
    }
    sprite.setTexture(texture);

    // --- 2. CONFIGURACIÓN DE LA ANIMACIÓN (¡AJUSTA ESTO!) ---
    // Abre tu imagen en Paint y cuenta los píxeles si se ve raro.
    
    frameWidth = 32;    // Ancho de UN cuadrito individual
    frameHeight = 32;   // Alto de UN cuadrito individual
    
    numCols = 8;        // ¿Cuántos muñequitos hay en una sola fila horizontal?
    numFrames = 8;      // ¿Cuántos cuadros TOTALES tiene la animación de correr?

    currentFrame = 0;
    animationTimer = 0.0f;

    // --- 3. ALINEACIÓN (PIVOTE EN LOS PIES) ---
    // Esto es vital para que al girar no se mueva de lugar
    sprite.setOrigin(frameWidth / 2.0f, (float)frameHeight);

    facingDir = 1; // 1 = Derecha, -1 = Izquierda

    // Recorte inicial (Primer cuadro)
    sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
}

void Player::Update(float dt, Level& level) {
    // --- GESTIÓN DE TIMERS ---
    if (isDashing) {
        dashTimer -= dt;
        if (dashTimer <= 0) { isDashing = false; velocity.x = 0; velocity.y = 0; }
    }
    if (wallJumpTimer > 0) wallJumpTimer -= dt;
    if (coyoteTimer > 0) coyoteTimer -= dt;
    if (jumpBufferTimer > 0) jumpBufferTimer -= dt;

    // --- DETECCIÓN DE PAREDES ---
    wallDir = 0;
    sf::FloatRect box = GetHitbox();
    sf::FloatRect boxLeft = box; boxLeft.left -= 2.0f; 
    if (CheckCollision(boxLeft, level)) wallDir = -1;
    sf::FloatRect boxRight = box; boxRight.left += 2.0f; 
    if (CheckCollision(boxRight, level)) wallDir = 1;

    // --- INPUTS ---
    HandleInput();

    // --- FÍSICA (Gravedad y Wall Slide) ---
    if (!isDashing) {
        // Wall Slide: Caer lento si tocamos pared y no estamos en suelo
        if (velocity.y > 0 && wallDir != 0 && !isGrounded) {
            velocity.y = WALL_SLIDE_SPEED; 
        } else {
            velocity.y += GRAVITY * dt;   
        }
    }

    // --- MOVIMIENTO ---
    MoveX(velocity.x * dt, level, [this]() { velocity.x = 0; });

    isGrounded = false;
    MoveY(velocity.y * dt, level, [this]() {
        if (velocity.y > 0) { 
            isGrounded = true; 
            hasDash = true; 
            isDashing = false; 
            coyoteTimer = COYOTE_TIME; // Recargar Coyote Time al tocar suelo
        }
        velocity.y = 0;
    });


    // --- SISTEMA DE ANIMACIÓN (GRID) ---
    // Solo animamos si nos movemos horizontalmente y estamos en el suelo
    if (velocity.x != 0 && isGrounded) { 
        animationTimer += dt;
        if (animationTimer >= ANIM_SPEED) {
            animationTimer = 0;
            currentFrame++;
            // Loop de animación
            if (currentFrame >= numFrames) {
                currentFrame = 0;
            }
        }
    } 
    else {
        // Si estamos quietos, saltando o cayendo -> Frame 0 (Idle)
        currentFrame = 0;
        animationTimer = 0;
    }

    // CÁLCULO DE FILA Y COLUMNA
    // Esto permite leer imágenes que tienen varias filas
    int currentColumn = currentFrame % numCols;  // Columna actual
    int currentRow = currentFrame / numCols;     // Fila actual

    int frameX = currentColumn * frameWidth;
    int frameY = currentRow * frameHeight;

    sprite.setTextureRect(sf::IntRect(frameX, frameY, frameWidth, frameHeight));

    // --- CHEQUEO DE MUERTE ---
    CheckDeath(level);
}

void Player::Render(sf::RenderWindow& window) {
    // 1. Sincronizar posición visual con la física (Alinear Pies)
    sprite.setPosition(position.x + (hitboxSize.x / 2.0f), position.y + hitboxSize.y);

    // 2. Voltear Sprite (Flip) según dirección
    sprite.setScale(std::abs(sprite.getScale().x) * facingDir, sprite.getScale().y);

    // 3. Efectos de Color (Feedback visual)
    sprite.setColor(sf::Color::White); // Color normal

    if (isDashing) {
        sprite.setColor(sf::Color::Cyan); // Dash activo
    } 
    else if (!hasDash) {
        sprite.setColor(sf::Color(100, 100, 200)); // Sin Dash (Oscuro)
    }

    window.draw(sprite);
    
    // DEBUG: Descomenta esto si quieres ver la caja de colisión verde
    // Actor::Render(window); 
}

// --- FUNCIONES DE SOPORTE (Lógica interna) ---

void Player::CheckDeath(Level& level) {
    if (position.y > 800) { Respawn(); return; } // Caída al vacío

    sf::FloatRect box = GetHitbox();
    float ts = level.GetTileSize();
    // Hitbox indulgente (más pequeña que el jugador)
    box.left += 4; box.width -= 8; box.top += 4; box.height -= 8;

    int left = (int)(box.left / ts);
    int right = (int)((box.left + box.width) / ts);
    int top = (int)(box.top / ts);
    int bottom = (int)((box.top + box.height) / ts);

    for (int i = left; i <= right; i++) {
        for (int j = top; j <= bottom; j++) {
            if (level.IsHazard(i, j)) { Respawn(); return; }
        }
    }
}

void Player::Respawn() {
    position = spawnPosition;
    velocity = {0, 0};
    hasDash = true; isDashing = false;
    wallJumpTimer = 0; coyoteTimer = 0; jumpBufferTimer = 0;
}

void Player::StartDash() {
    if (hasDash) {
        isDashing = true; hasDash = false;
        dashTimer = DASH_DURATION; wallJumpTimer = 0; 

        float dirX = 0; float dirY = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) dirX = 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  dirX = -1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    dirY = -1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  dirY = 1;

        if (dirX == 0 && dirY == 0) dirX = facingDir; // Dash hacia donde miramos

        velocity.x = dirX * DASH_SPEED;
        velocity.y = dirY * DASH_SPEED;
    }
}

void Player::HandleInput() {
    if (isDashing) return; // No controlar durante dash

    // Movimiento Horizontal
    if (wallJumpTimer <= 0) {
        velocity.x = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            velocity.x = SPEED;
            facingDir = 1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            velocity.x = -SPEED;
            facingDir = -1;
        }
    }

    // Salto (Con Buffer)
    static bool wasZPressed = false;
    bool isZPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
    
    if (isZPressed && !wasZPressed) jumpBufferTimer = JUMP_BUFFER_TIME;
    wasZPressed = isZPressed;

    if (jumpBufferTimer > 0) {
        // Salto Normal (Con Coyote Time)
        if (coyoteTimer > 0) { 
            velocity.y = JUMP_FORCE;
            jumpBufferTimer = 0; coyoteTimer = 0; isGrounded = false;
        }
        // Wall Jump
        else if (wallDir != 0) { 
            velocity.y = WALL_JUMP_FORCE_Y;
            velocity.x = -wallDir * WALL_JUMP_FORCE_X;
            wallJumpTimer = WALL_JUMP_TIME;
            
            facingDir = -wallDir; // Voltear personaje
            jumpBufferTimer = 0; 
        }
    }

    // Dash
    static bool wasXPressed = false;
    bool isXPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::X);
    if (isXPressed && !wasXPressed) StartDash();
    wasXPressed = isXPressed;
}