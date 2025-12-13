#include <SFML/Graphics.hpp>
#include "../../include/Entities/Player.hpp"
#include <iostream>
#include <cmath> 

const float SPEED = 140.0f;              
const float GRAVITY = 900.0f;            
const float JUMP_FORCE = -300.0f;        
const float DASH_SPEED = 400.0f;         
const float DASH_DURATION = 0.15f;       
const float WALL_SLIDE_SPEED = 30.0f;    
const float WALL_JUMP_FORCE_X = 300.0f;  
const float WALL_JUMP_FORCE_Y = -320.0f; 
const float WALL_JUMP_TIME = 0.15f;      
const float COYOTE_TIME = 0.1f;          
const float JUMP_BUFFER_TIME = 0.1f;     
const float ANIM_SPEED = 0.08f;          

Player::Player(float x, float y) 
    : Actor(x, y, 16.f, 32.f) 
{
    spawnPosition = {x, y};
    velocity = {0.f, 0.f};
    isGrounded = false;
    hasDash = true; isDashing = false; dashTimer = 0.f;
    wallDir = 0; wallJumpTimer = 0.f; coyoteTimer = 0.f; jumpBufferTimer = 0.f;

    // --- CAMBIO: RUTA DE IMAGEN ---
    if (!texture.loadFromFile("Assets/imagenes/CHIAPASIANOTE.png")) { 
        std::cout << "[ERROR] Falta Assets/imagenes/CHIAPASIANOTE.png" << std::endl;
        texture.create(32, 32);
    } 
    sprite.setTexture(texture);

    int numFrames = 12; int frameW = 16; int frameH = 32; 
    animationFrames.clear();
    for (int i = 0; i < numFrames; i++) {
        int rectX = i * frameW;
        if (rectX + frameW <= (int)texture.getSize().x) {
            animationFrames.push_back(sf::IntRect(rectX, 0, frameW, frameH));
        }
    }
    currentFrame = 0; animationTimer = 0.0f; facingDir = 1;

    if (!animationFrames.empty()) {
        sprite.setTextureRect(animationFrames[0]);
        sprite.setOrigin(frameW / 2.f, (float)frameH);
    }
}

void Player::Update(float dt, Level& level) {
    eventJumped = false; eventDashed = false; eventLanded = false; 

    if (isDashing) {
        dashTimer -= dt;
        if (dashTimer <= 0) { isDashing = false; velocity.x = 0; velocity.y = 0; }
    }
    if (wallJumpTimer > 0) wallJumpTimer -= dt;
    if (coyoteTimer > 0) coyoteTimer -= dt;
    if (jumpBufferTimer > 0) jumpBufferTimer -= dt;

    wallDir = 0;
    sf::FloatRect box = GetHitbox();
    sf::FloatRect boxLeft = box; boxLeft.left -= 2.0f; 
    if (CheckCollision(boxLeft, level)) wallDir = -1;
    sf::FloatRect boxRight = box; boxRight.left += 2.0f; 
    if (CheckCollision(boxRight, level)) wallDir = 1;

    HandleInput();

    if (!isDashing) {
        if (velocity.y > 0 && wallDir != 0 && !isGrounded) velocity.y = WALL_SLIDE_SPEED; 
        else velocity.y += GRAVITY * dt;   
    }

    MoveX(velocity.x * dt, level, [this]() { velocity.x = 0; });
    
    isGrounded = false;
    MoveY(velocity.y * dt, level, [this]() {
        if (velocity.y > 0) { 
            isGrounded = true; hasDash = true; isDashing = false; coyoteTimer = COYOTE_TIME; 
            if (velocity.y > 200.f) eventLanded = true; 
        }
        velocity.y = 0;
    });

    if (!animationFrames.empty()) {
        bool isMoving = std::abs(velocity.x) > 10.f;
        bool isOnFloor = coyoteTimer > 0.0f; 
        if (isOnFloor) {
            if (isMoving) {
                animationTimer += dt;
                if (animationTimer >= ANIM_SPEED) {
                    animationTimer = 0; currentFrame++;
                    if (currentFrame >= animationFrames.size()) currentFrame = 1; 
                }
            } else { currentFrame = 0; animationTimer = 0; }
        } else {
            if (velocity.y < 0) currentFrame = 3; 
            else currentFrame = 8;
            if (currentFrame >= animationFrames.size()) currentFrame = 0;
        }
        sprite.setTextureRect(animationFrames[currentFrame]);
    }
    CheckDeath(level);
}

void Player::Render(sf::RenderWindow& window) {
    sprite.setPosition(position.x + (hitboxSize.x / 2.0f), position.y + hitboxSize.y);
    sprite.setScale(std::abs(sprite.getScale().x) * facingDir, sprite.getScale().y);
    sprite.setColor(sf::Color::White);
    if (isDashing) sprite.setColor(sf::Color::Cyan);
    else if (!hasDash) sprite.setColor(sf::Color(100, 100, 200));
    window.draw(sprite);
}

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
        eventDashed = true; 
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
        if (coyoteTimer > 0) { 
            velocity.y = JUMP_FORCE; jumpBufferTimer = 0; coyoteTimer = 0; isGrounded = false; 
            eventJumped = true; 
        }
        else if (wallDir != 0) { 
            velocity.y = WALL_JUMP_FORCE_Y; velocity.x = -wallDir * WALL_JUMP_FORCE_X; wallJumpTimer = WALL_JUMP_TIME; facingDir = -wallDir; jumpBufferTimer = 0; 
            eventJumped = true; 
        }
    }
    static bool wasXPressed = false; bool isXPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::X);
    if (isXPressed && !wasXPressed) StartDash();
    wasXPressed = isXPressed;
}