
#include "mogera.h"
#include <iostream>

sf::Texture Mogera::sharedTexture;
bool        Mogera::textureLoaded = false;


static const sf::IntRect MOGERA_IDLE[3] = {
    sf::IntRect(42,   5, 539, 462),
    sf::IntRect(586,  5, 539, 462),
    sf::IntRect(1167, 5, 538, 462)
};

static const sf::IntRect MOGERA_DYING[2] = {
    sf::IntRect(0,    528, 845, 584),
    sf::IntRect(1009, 528, 781, 584)
};

//     

Mogera::Mogera() {

    if (!textureLoaded) {
        if (sharedTexture.loadFromFile("mogera.png")) {
            sharedTexture.setSmooth(false);
            textureLoaded = true;
            std::cout << "[Mogera] Texture loaded OK\n";
        }
        else {
            std::cout << "[Mogera] ERROR: mogera.png not found!\n";
        }
    }

    sprite.setTexture(sharedTexture);
    sprite.setTextureRect(MOGERA_IDLE[0]);

    sprite.setScale(0.33f, 0.33f);   
    sprite.setPosition(700.f, 150.f);

    health = 10;
    maxHealth = 10;
    spawnTimer = 0.f;
    alive = true;
    isDying = false;
    dyingTimer = 0.f;
    currentAnim = MogeraAnim::Idle;
    frameIndex = 0;
    animTimer = 0.f;
}


void Mogera::applyFrame() {
    if (currentAnim == MogeraAnim::Idle) {
        sprite.setTextureRect(MOGERA_IDLE[frameIndex]);
        sprite.setScale(0.33f, 0.33f);
    }
    else {
        sprite.setTextureRect(MOGERA_DYING[frameIndex]);
        float sc = (frameIndex == 0) ? 0.21f : 0.23f;
        sprite.setScale(sc, sc);
    }
}

void Mogera::updateAnimation(float dt) {
    animTimer += dt;

    if (currentAnim == MogeraAnim::Idle) {
        if (animTimer >= 0.22f) {
            animTimer -= 0.22f;
            frameIndex = (frameIndex + 1) % 3;
            applyFrame();
        }
    }
    else {
        if (frameIndex == 0 && animTimer >= 0.5f) {
            frameIndex = 1;
            animTimer = 0.f;
            applyFrame();
        }
    }
}


void Mogera::update(float dt) {
    if (!alive) return;

    spawnTimer += dt;

    if (isDying) {
        updateAnimation(dt);
        dyingTimer += dt;
        if (dyingTimer >= 1.8f)
            alive = false;
        return;
    }

    updateAnimation(dt);
}

void Mogera::draw(sf::RenderWindow& window) {
    if (!alive)         return;
    if (!textureLoaded) return;
    window.draw(sprite);
}

void Mogera::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

sf::FloatRect Mogera::getBounds() {
    return sprite.getGlobalBounds();
}


void Mogera::takeDamage() {
    if (isDying) return;
    health ;
    if (health <= 0) {
        isDying = true;
        dyingTimer = 0.f;
        animTimer = 0.f;
        frameIndex = 0;
        currentAnim = MogeraAnim::Dying;
        applyFrame();
    }
}

bool Mogera::isAlive() { return alive; }
int  Mogera::getHealth() { return health; }
int  Mogera::getMaxHealth() { return maxHealth; }

bool Mogera::shouldSpawnChild() {
    if (isDying) return false;
    if (spawnTimer >= 3.0f) {
        spawnTimer = 0;
        return true;
    }
    return false;
}