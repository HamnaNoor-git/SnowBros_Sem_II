#pragma once
#include <SFML/Graphics.hpp>
using namespace std;

class Snowball {
private:
    sf::CircleShape shape;

    float speed;
    int direction;
    bool active;


    float maxDistance;
    float maxRange;
    float startX;
    sf::Vector2f position;

public:
    Snowball();


    void update(float dt);
    bool powerBoost;

    void draw(sf::RenderWindow& window);
    void shoot(float x, float y, int dir, float range);
    bool isActive();
    sf::FloatRect getBounds();
    void deactivate();
};