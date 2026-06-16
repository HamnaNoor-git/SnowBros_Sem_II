#pragma once
#include <SFML/Graphics.hpp>
#include "platform.h"
using namespace std;

class Blast {
private:
    sf::CircleShape body;

    float speedX;
    float speedY;

    bool active;
    bool exploded;
    float explosionTimer;
    bool onGround;
    float walkDir;

public:
    Blast();

    void shoot(float x, float y, float dirX, float dirY);
    void update(float dt, Platform* platforms, int platformCount);
    void draw(sf::RenderWindow& window);
    void deactivate();
    bool isActive();
    bool isExploded();

    sf::FloatRect getBounds();
};