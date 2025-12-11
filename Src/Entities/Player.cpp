#include <SFML/Graphics.hpp>
#include "../../include/Entities/Player.hpp"
#include <iostream>
#include <cmath>

// Ya no necesitamos variables globales aquí arriba porque ya están en el .hpp

Player::Player(float x, float y) 
    : Actor(x, y, 30.f, 64.f) // Hitbox rectangular (Ancho 30, Alto 64)
{
    spawnPosition = {x, y};
    velocity = {0.f, 0.f};
    isGrounded = false;
    
    // Inicializar mecánicas
    hasDash = true; isDashing = false; dashTimer = 0.0f;
    wallDir = 0; wallJumpTimer = 0.0f;
    coyoteTimer = 0.0f; jumpBufferTimer = 0.0f;

    // 1. CARGAR IMAGEN
    if (!texture.loadFromFile("Assets/player_run.png")) {
        std::cout << "ERROR: No se encontró Assets/player_run.png" << std::endl;
        texture.create(64, 64);
    }
    sprite.setTexture(texture);

    // 2. CONFIGURACIÓN DE ANIMACIÓN
    sf::Vector2u size = texture.getSize();

    // --- AJUSTA ESTOS DOS NÚMEROS A TU IMAGEN ---
    numCols = 1;  // ¿Cuántos monitos hay a lo ancho?
    numRows = 1;  // ¿Cuántas filas hay hacia abajo?
    // --------------------------------------------

    // Cálculo automático
    frameWidth = size.x / numCols;
    frameHeight = size.y / numRows;
    numFrames = numCols * numRows;

    currentFrame = 0;
    animationTimer = 0.0f;

    // Poner el punto de pivote en los PIES del personaje
    sprite.setOrigin(frameWidth / 2.0f, (float)frameHeight);
    
    facingDir = 1; 
    sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
}

void Player::Update(float dt, Level& level) {
    // --- TIMERS ---
    if (isDashing) {
        dashTimer -= dt;
        if (dashTimer <= 0) { isDashing = false; velocity.x = 0; velocity.y = 0; }
    }
    if (wallJumpTimer > 0) wallJumpTimer -= dt;
    if (coyoteTimer > 0) coyoteTimer -= dt;
    if (jumpBufferTimer > 0) jumpBufferTimer -= dt;

    // --- PAREDES ---
    wallDir = 0;
    sf::FloatRect box = GetHitbox();
    sf::FloatRect boxLeft = box; boxLeft.left -= 2.0f; 
    if (CheckCollision(boxLeft, level)) wallDir = -1;
    sf::FloatRect boxRight = box; boxRight.left += 2.0f; 
    if (CheckCollision(boxRight, level)) wallDir = 1;

    // --- INPUTS ---
    HandleInput();

    // --- FÍSICA ---
    if (!isDashing) {
        if (velocity.y > 0 && wallDir != 0 && !isGrounded) velocity.y = WALL_SLIDE_SPEED; 
        else velocity.y += GRAVITY * dt;   
    }

    MoveX(velocity.x * dt, level, [this]() { velocity.x = 0; });
    isGrounded = false;
    MoveY(velocity.y * dt, level, [this]() {
        if (velocity.y > 0) { 
            isGrounded = true; hasDash = true; isDashing = false; coyoteTimer = COYOTE_TIME; 
        }
        velocity.y = 0;
    });

    // --- ANIMACIÓN ---
    if (velocity.x != 0 && isGrounded) {
        animationTimer += dt;
        if (animationTimer >= ANIM_SPEED) {
            animationTimer = 0;
            currentFrame++;
            if (currentFrame >= numFrames) currentFrame = 0;
        }
    } else {
        currentFrame = 0; 
        animationTimer = 0;
    }

    // Calcular recorte
    int currentColumn = currentFrame % numCols;
    int currentRow = currentFrame / numCols;
    int frameX = currentColumn * frameWidth;
    int frameY = currentRow * frameHeight;

    sprite.setTextureRect(sf::IntRect(frameX, frameY, frameWidth, frameHeight));

    CheckDeath(level);
}

void Player::Render(sf::RenderWindow& window) {
    // A. Posicionar
    sprite.setPosition(position.x + (hitboxSize.x / 2.0f), position.y + hitboxSize.y);
    
    // B. Voltear
    sprite.setScale(std::abs(sprite.getScale().x) * facingDir, sprite.getScale().y);

    // C. Colores
    sprite.setColor(sf::Color::White); 
    if (isDashing) sprite.setColor(sf::Color::Cyan); 
    else if (!hasDash) sprite.setColor(sf::Color(100, 100, 200));

    // D. Dibujar
    window.draw(sprite);

    // E. Dibujar Hitbox Verde
    Actor::Render(window); 
}

// --- FUNCIONES EXTRA ---
void Player::CheckDeath(Level& level) {
    if (position.y > 800) { Respawn(); return; }
    sf::FloatRect box = GetHitbox();
    float ts = level.GetTileSize();
    box.left += 4; box.width -= 8; box.top += 4; box.height -= 8;
    int left = (int)(box.left / ts); int right = (int)((box.left + box.width) / ts);
    int top = (int)(box.top / ts); int bottom = (int)((box.top + box.height) / ts);
    for (int i = left; i <= right; i++) {
        for (int j = top; j <= bottom; j++) {
            if (level.IsHazard(i, j)) { Respawn(); return; }
        }
    }
}
void Player::Respawn() {
    position = spawnPosition; velocity = {0, 0};
    hasDash = true; isDashing = false; wallJumpTimer = 0; coyoteTimer = 0; jumpBufferTimer = 0;
}
void Player::StartDash() {
    if (hasDash) {
        isDashing = true; hasDash = false; dashTimer = DASH_DURATION; wallJumpTimer = 0; 
        float dirX = 0; float dirY = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) dirX = 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  dirX = -1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    dirY = -1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  dirY = 1;
        if (dirX == 0 && dirY == 0) dirX = facingDir; 
        velocity.x = dirX * DASH_SPEED; velocity.y = dirY * DASH_SPEED;
    }
}
void Player::HandleInput() {
    if (isDashing) return;
    if (wallJumpTimer <= 0) {
        velocity.x = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) { velocity.x = SPEED; facingDir = 1; }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) { velocity.x = -SPEED; facingDir = -1; }
    }
    static bool wasZPressed = false; bool isZPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
    if (isZPressed && !wasZPressed) jumpBufferTimer = JUMP_BUFFER_TIME;
    wasZPressed = isZPressed;
    if (jumpBufferTimer > 0) {
        if (coyoteTimer > 0) { velocity.y = JUMP_FORCE; jumpBufferTimer = 0; coyoteTimer = 0; isGrounded = false; }
        else if (wallDir != 0) { velocity.y = WALL_JUMP_FORCE_Y; velocity.x = -wallDir * WALL_JUMP_FORCE_X; wallJumpTimer = WALL_JUMP_TIME; facingDir = -wallDir; jumpBufferTimer = 0; }
    }
    static bool wasXPressed = false; bool isXPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::X);
    if (isXPressed && !wasXPressed) StartDash();
    wasXPressed = isXPressed;
}