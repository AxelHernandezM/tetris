// Src/Test.cpp
#include <SFML/Graphics.hpp>
#include <iostream>

void TestSFML() {
    sf::Event event; // Si esto falla, SFML no está instalado bien
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        std::cout << "Espacio presionado" << std::endl;
    }
}