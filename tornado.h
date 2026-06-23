#pragma once
#include "flying_fooga.h"

enum class TornadoAnim {
    WALK = 0,  
    IDLE = 1,   
    FLY = 2,  
    SHOOT = 3,   
};

class Tornado : public FlyingFooga {
private:
    sf::Texture tornadoSheet;       

    TornadoAnim tAnim;
    int         tFrame;
    float       tAnimTimer;
    float       tAnimFrameDuration;
    static const sf::IntRect walkRects[4];
    static const sf::IntRect idleRects[2];
    static const sf::IntRect flyRects[2];
    static const sf::IntRect shootRects[2];

    void setTornadoAnim(TornadoAnim a);
    void updateTornadoAnim(float dt);
    void applyTornadoScale();
    static const sf::IntRect knifeRects[4];

    bool          knifeActive;
    float         knifeTimer;
    sf::Sprite    knifeSprite;   
    int           knifeFrame;
    float         knifeAnimTimer;
    float         knifeFrameDuration;
    float         knifeSpeedX;
    float         knifeSpeedY;

    float teleportTimer;
    float teleportCooldown;

    float shootPoseTimer;

public:
    Tornado();
    Tornado(const Tornado& other);
    Tornado& operator=(const Tornado& other);
    ~Tornado() {}

    void update(float dt, float playerX, float playerY);
    void draw(sf::RenderWindow& window) override;
    bool isKnifeActive();
    sf::FloatRect getKnifeBounds();
    void kill();
};