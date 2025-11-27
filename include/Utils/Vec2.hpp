#pragma once
#include <cmath>

struct Vec2 {
    float x, y;

    Vec2() : x(0), y(0) {}
    Vec2(float x, float y) : x(x), y(y) {}

    // Sobrecarga de operadores para facilitar la física
    Vec2 operator+(const Vec2& other) const { return Vec2(x + other.x, y + other.y); }
    Vec2 operator-(const Vec2& other) const { return Vec2(x - other.x, y - other.y); }
    Vec2 operator*(float scalar) const { return Vec2(x * scalar, y * scalar); }
    
    // Para sumar velocidad a la posición: pos += vel;
    void operator+=(const Vec2& other) { x += other.x; y += other.y; }

    // Magnitud (útil para limitar la velocidad máxima)
    float Length() const { return std::sqrt(x * x + y * y); }
};