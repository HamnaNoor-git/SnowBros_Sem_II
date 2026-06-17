#pragma once
#include <SFML/Graphics.hpp>

class Collectible {
private:
    sf::Sprite sprite;
    sf::Texture* texture;
    bool active;

public:
    Collectible() {
        active = false;
        texture = nullptr;
    }

    void spawn(sf::Texture* tex, float x, float y) {
        texture = tex;
        sprite.setTexture(*texture);
        sprite.setPosition(x, y);
        active = true;
        sprite.setScale(0.4f, 0.4f);  
    }

    void draw(sf::RenderWindow& window) {
        if (active)
            window.draw(sprite);
    }

    sf::FloatRect getBounds() {
        return sprite.getGlobalBounds();
    }

    bool isActive() {
        return active;
    }

    void collect() {
        active = false;
    }
};