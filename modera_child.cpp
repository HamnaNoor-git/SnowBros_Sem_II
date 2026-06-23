#include "mogera_child.h"
#include <iostream>

  

static const sf::IntRect CHILD_WALK_RIGHT[2] = {
    sf::IntRect(1968, 779, 168, 197),
    sf::IntRect(2140, 779, 173, 197)
};

static const sf::IntRect CHILD_WALK_LEFT[2] = {
    sf::IntRect(1795, 800, 169, 176),
    sf::IntRect(2318, 779, 152, 197)
};

//     

MogeraChild::MogeraChild() {
    if (!texture.loadFromFile("mogera.png")) {
        std::cout << "ERROR: Mogera spritesheet not loaded!\n";
    }
    texture.setSmooth(false);

    sprite.setTexture(texture);
    sprite.setTextureRect(CHILD_WALK_RIGHT[0]);

    // Scale down — frame is ~170x197, want ~60px on screen
    sprite.setScale(0.35f, 0.35f);
    sprite.setPosition(100, 100);

    speed = 150;
    direction = 1;
    alive = true;
    velocityY = 0;
    gravity = 800;
    onGround = false;

    frameIndex = 0;
    animTimer = 0;
    frameCount = 2;
    currentAnim = ChildAnim::WalkRight;
}

//     

void MogeraChild::setAnimation(ChildAnim anim) {
    if (currentAnim == anim) return;
    currentAnim = anim;
    frameIndex = 0;
    animTimer = 0;
    frameCount = 2;

    if (anim == ChildAnim::WalkRight)
        sprite.setTextureRect(CHILD_WALK_RIGHT[0]);
    else
        sprite.setTextureRect(CHILD_WALK_LEFT[0]);
}

void MogeraChild::updateAnimation(float dt) {
    animTimer += dt;
    if (animTimer >= 0.15f) {
        animTimer -= 0.15f;
        frameIndex = (frameIndex + 1) % frameCount;

        if (currentAnim == ChildAnim::WalkRight)
            sprite.setTextureRect(CHILD_WALK_RIGHT[frameIndex]);
        else
            sprite.setTextureRect(CHILD_WALK_LEFT[frameIndex]);
    }
}

//     

void MogeraChild::update(float dt) {
    if (!alive) return;

    // Gravity
    velocityY += gravity * dt;
    if (onGround) velocityY = 0;
    sprite.move(0, velocityY * dt);

    // Horizontal
    sprite.move(direction * speed * dt, 0);

    // Wall bounce
    float x = sprite.getPosition().x;
    if (x < 0) {
        sprite.setPosition(0, sprite.getPosition().y);
        direction = 1;
    }
    if (x > 960) {
        sprite.setPosition(960, sprite.getPosition().y);
        direction = -1;
    }

    // Ground safety
    if (sprite.getPosition().y > 750) {
        sprite.setPosition(sprite.getPosition().x,
            750 - sprite.getGlobalBounds().height);
        velocityY = 0;
        onGround = true;
    }

    // Switch animation by direction
    ChildAnim desired = (direction > 0)
        ? ChildAnim::WalkRight
        : ChildAnim::WalkLeft;

    if (currentAnim != desired)
        setAnimation(desired);

    updateAnimation(dt);
}


void MogeraChild::draw(sf::RenderWindow& window) {
    if (alive) window.draw(sprite);
}

sf::FloatRect MogeraChild::getBounds() {
    return sprite.getGlobalBounds();
}

void MogeraChild::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

void MogeraChild::setDirection(int dir) {
    direction = dir;
}

void MogeraChild::takeDamage() { alive = false; }
void MogeraChild::kill() { alive = false; }