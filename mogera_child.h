#pragma once
#include <SFML/Graphics.hpp>
#include "enemy.h"

enum class ChildAnim {
    WalkRight = 0,
    WalkLeft = 1
};

class MogeraChild : public Enemy {
private:
    sf::Texture texture;
    sf::Sprite  sprite;
    float gravity;

    ChildAnim currentAnim;
    int   frameIndex;
    float animTimer;
    int   frameCount;

    void setAnimation(ChildAnim anim);
    void updateAnimation(float dt);

public:
    MogeraChild();

    void setDirection(int dir);
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    void takeDamage() override;
    sf::FloatRect getBounds() override;
    void setPosition(float x, float y) override;
    void kill() override;
};