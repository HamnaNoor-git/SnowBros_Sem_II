#include "knife.h"
#include <cmath>

Knife::Knife() {
    body.setSize(sf::Vector2f(20, 5));
    body.setFillColor(sf::Color::White);

    speedX = 0;
    speedY = 0;
    active = false;
}

void Knife::shoot(float startX, float startY, float targetX, float targetY) {

    body.setPosition(startX, startY);

    float dx = targetX - startX;
    float dy = targetY - startY;

    float length = sqrt(dx * dx + dy * dy);

    if (length != 0) {
        speedX = (dx / length) * 300;
        speedY = (dy / length) * 300;
    }

    active = true;
}

void Knife::update(float dt) {

    if (!active) return;

    body.move(speedX * dt, speedY * dt);

    // deactivate if out of screen
    if (body.getPosition().x < 0 || body.getPosition().x > 1000 ||
        body.getPosition().y < 0 || body.getPosition().y > 800) {

        active = false;
    }
}

void Knife::draw(sf::RenderWindow& window) {
    if (active)
        window.draw(body);
}

bool Knife::isActive() {
    return active;
}

sf::FloatRect Knife::getBounds() {
    return body.getGlobalBounds();
}