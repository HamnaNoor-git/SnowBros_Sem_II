#pragma once
#include "botom.h"
enum class FoogaAnim {
    IDLE = 0,   
    WALK = 1,  
    FLY = 2,   
    ROLL = 3,   
    SQUISH = 4   
};

class FlyingFooga : public Botom {
protected:
    bool  isFlying;
    float flightTimer;
    float groundTimer;   
    float moveX;
    float moveY;

  
    float walkTimer;     
    bool  droppingOff;    
    static const float WALK_BEFORE_DROP;  

    sf::Texture foogaSheet;       
    FoogaAnim   currentAnim;
    int         animFrame;
    float       animTimer;
    float       animFrameDuration;

   
    static const sf::IntRect animRects[5][3];
    static const int         animFrameCounts[5];

    void setAnimState(FoogaAnim anim);
    void updateAnimation(float dt);
    void applyFoogaScale(int dir = 1);   

public:
    FlyingFooga();
    FlyingFooga(const FlyingFooga& other);
    FlyingFooga& operator=(const FlyingFooga& other);
    ~FlyingFooga() {}

    void update(float dt) override;
    void takeDamage() override;
    void draw(sf::RenderWindow& window) override;
};










