#pragma once
#include <SFML/Graphics.hpp>
#include "enemy.h"

enum class MogeraAnim {
    Idle = 0,
    Dying = 1
};

class Mogera : public Enemy {
private:

    static sf::Texture sharedTexture;
    static bool        textureLoaded;

    sf::Sprite  sprite;

    int   health;
    int   maxHealth;
    float spawnTimer;

    MogeraAnim currentAnim;
    int   frameIndex;
    float animTimer;
    bool  isDying;
    float dyingTimer;

    void applyFrame();
    void updateAnimation(float dt);

public:
    Mogera();

    Mogera(const Mogera&) = delete;
    Mogera& operator=(const Mogera&) = delete;

    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    void takeDamage() override;
    sf::FloatRect getBounds() override;
    void setPosition(float x, float y) override;

    bool isAlive() override;
    bool shouldSpawnChild();
    int  getHealth();
    int  getMaxHealth();
    void setHealth(int h) { health = h; maxHealth = h; }
};