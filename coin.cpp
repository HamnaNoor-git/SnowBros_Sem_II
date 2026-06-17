#include "coin.h"

Coin::Coin() {

    body.setRadius(8);
    body.setFillColor(sf::Color::Yellow);

    velocityY = 0;
    gravity = 200; //  SLOW FALL

    active = false;
}

void Coin::spawn(float x, float y) {

    body.setPosition(x, y);
    velocityY = 0;
    active = true;
}

void Coin::update(float dt) {

    if (!active) return;

    velocityY += gravity * dt;

    body.move(0, velocityY * dt);

    // stop on ground
    if (body.getPosition().y > 720) {
        body.setPosition(body.getPosition().x, 720);
        velocityY = 0;
    }
}

void Coin::draw(sf::RenderWindow& window) {
    if (active)
        window.draw(body);
}

bool Coin::isActive() {
    return active;
}

void Coin::collect() {
    active = false;
}

sf::FloatRect Coin::getBounds() {
    return body.getGlobalBounds();
}