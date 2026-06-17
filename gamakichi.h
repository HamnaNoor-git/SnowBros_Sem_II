#pragma once
#include <SFML/Graphics.hpp>
#include "enemy.h"
#include "blast.h"



class Gamakichi : public Enemy {
private:
    sf::RectangleShape body;
    sf::Texture        texture;
    sf::Sprite         sprite;

    int   health;
    int   maxHealth;
    float moveDir;

    Blast blasts[10];
    float attackTimer;
    float shootTimer;

    static const int FACE_FRAMES = 2;
    sf::IntRect faceFrames[FACE_FRAMES];
    int   currentFaceFrame;
    bool  attacking;
    float attackFlash;

    static const int BOMB_FRAMES = 8;
    sf::IntRect bombFrames[BOMB_FRAMES];
    sf::Sprite  bombSprite;
    int         bombCurrentFrame;
    float       bombAnimTimer;
    float       bombAnimSpeed;

    static const int EXPL_FRAMES = 2;
    sf::IntRect explFrames[EXPL_FRAMES];
    sf::Sprite  explSprite;
    int         explCurrentFrame;
    float       explAnimTimer;
    float       explAnimSpeed;

public:
    Gamakichi();

    void update(float dt) override {}
    void update(float dt, float playerX, float playerY);
    void update(float dt, float playerX, float playerY,
        Platform* platforms, int platformCount);

    void draw(sf::RenderWindow& window) override;
    void takeDamage() override;
    sf::FloatRect getBounds() override;
    void setPosition(float x, float y) override;

    bool isAlive()    override;
    int  getHealth();
    int  getMaxHealth();
    void setHealth(int h) { health = h; maxHealth = h; }
    Blast* getBlasts();

    sf::CircleShape* getBullets() { return nullptr; }
    bool* getBulletStates() { return nullptr; }
};