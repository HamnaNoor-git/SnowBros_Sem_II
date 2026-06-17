#pragma once
#include <SFML/Graphics.hpp>

class Enemy {
protected:
    float velocityY;
    float speed;
    int direction;
    bool alive;
    bool onGround;

public:
    Enemy() : velocityY(0), speed(100), direction(1), alive(true), onGround(false) {}
    virtual ~Enemy() {}

    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void takeDamage() = 0;
    virtual sf::FloatRect getBounds() = 0;
    virtual void setPosition(float x, float y) = 0;

    virtual bool isAlive() { return alive; }
    virtual bool isFrozen() const { return false; }
    virtual bool isRolling() const { return false; }

    float getVelocityY() { return velocityY; }
    void setVelocityY(float v) { velocityY = v; }
    void setOnGround(bool val) { onGround = val; }

    float getSpeed() const { return speed; }
    void setSpeed(float s) { speed = s; }

    virtual void kill() { alive = false; }
};