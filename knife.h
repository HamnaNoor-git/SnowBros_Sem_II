#pragma once
#include <SFML/Graphics.hpp>
using namespace std;

class Knife {
private:
    sf::RectangleShape body;

    float speedX;
    float speedY;

    bool active;

public:
    Knife();

    void shoot(float startX, float startY, float targetX, float targetY);
    void update(float dt);
    void draw(sf::RenderWindow& window);

    bool isActive();
    sf::FloatRect getBounds();
};