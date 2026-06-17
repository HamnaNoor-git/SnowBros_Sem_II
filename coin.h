#pragma once
#include <SFML/Graphics.hpp>

class Coin {
private:
    sf::CircleShape body;

    float velocityY;
    float gravity;

    bool active;

public:
    Coin();

    void spawn(float x, float y);
    void update(float dt);
    void draw(sf::RenderWindow& window);

    bool isActive();
    void collect();

    sf::FloatRect getBounds();
};