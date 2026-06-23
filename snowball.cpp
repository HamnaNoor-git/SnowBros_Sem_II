#include "snowball.h"
#include <cmath>

Snowball::Snowball() {
    shape.setRadius(10);
    shape.setFillColor(sf::Color::White);

    speed = 8.0f;
    direction = 1;
    active = false;

    startX = 0;
    maxRange = 400;
    powerBoost = false;
    speed = 8.0f; 
}


void Snowball::shoot(float x, float y, int dir, float range) {
    active = true;

    position.x = x;
    position.y = y;

    shape.setPosition(position); 

    direction = dir;
    startX = x;
    maxRange = range;
}

void Snowball::update(float dt) {
    if (!active) return;

    float currentSpeed = speed;

    // 🔥 POWER BOOST SPEED
    if (powerBoost) {
        currentSpeed = speed * 1.8f; // faster when upgraded
    }

    // ❗ REMOVE dt (restore original feel)
    position.x += currentSpeed * direction;

    shape.setPosition(position);

    if (abs(position.x - startX) > maxRange) {
        deactivate();
    }
}

void Snowball::draw(sf::RenderWindow& window) {
    if (active)
        window.draw(shape);
}

bool Snowball::isActive() {
    return active;
}

sf::FloatRect Snowball::getBounds() {
    return shape.getGlobalBounds();
}

void Snowball::deactivate() {
    active = false;
}