#include "flying_fooga.h"
#include <cstdlib>
#include <iostream>

// Seconds the fooga walks on a platform before intentionally dropping off
const float FlyingFooga::WALK_BEFORE_DROP = 3.0f;

const sf::IntRect FlyingFooga::animRects[5][3] = {
    // IDLE
    { sf::IntRect(63,  37,  81, 147),
      sf::IntRect(224, 24, 152, 159),
      sf::IntRect(436, 28, 127, 145) },
      // WALK
      { sf::IntRect(48,  244, 121, 126),
        sf::IntRect(240, 264, 121, 106),
        sf::IntRect(416, 230, 136, 140) },
        // FLY (3rd slot unused – zeroed)
        { sf::IntRect(29,  432, 149, 124),
          sf::IntRect(220, 432, 150, 119),
          sf::IntRect(0,   0,   0,   0)   },
          // ROLL
          { sf::IntRect(24,  613, 159, 131),
            sf::IntRect(230, 643, 141, 102),
            sf::IntRect(422, 667, 131,  78) },
            // SQUISH (2nd & 3rd unused)
            { sf::IntRect(53,  815, 126, 145),
              sf::IntRect(0,   0,   0,   0),
              sf::IntRect(0,   0,   0,   0)   }
};

const int FlyingFooga::animFrameCounts[5] = { 3, 3, 2, 3, 1 };

void FlyingFooga::applyFoogaScale(int dir) {
    sf::IntRect r = sprite.getTextureRect();
    if (r.height <= 0) return;

    const float TARGET_H = 50.0f;
    float s = TARGET_H / static_cast<float>(r.height);

    // Negative X scale = horizontal flip (face left)
    sprite.setScale(dir * s, s);

    // Bottom-centre origin so setPosition lands the feet on the platform
    sprite.setOrigin(r.width * 0.5f, static_cast<float>(r.height));
}
//  setAnimState – only resets if actually changing state
            
void FlyingFooga::setAnimState(FoogaAnim anim) {
    if (currentAnim == anim) return;
    currentAnim = anim;
    animFrame = 0;
    animTimer = 0.0f;
}

// updateAnimation
      
void FlyingFooga::updateAnimation(float dt) {
    animTimer += dt;
    if (animTimer >= animFrameDuration) {
        animTimer -= animFrameDuration;
        int s = static_cast<int>(currentAnim);
        animFrame = (animFrame + 1) % animFrameCounts[s];
    }

    int s = static_cast<int>(currentAnim);
    sprite.setTextureRect(animRects[s][animFrame]);


    applyFoogaScale(direction);
}
 
//  loadSheet         
static void loadFoogaSheet(sf::Texture& tex) {
    if (!tex.loadFromFile("FlyingFoogaFoog_Blue.png"))
        std::cout << "ERROR: FlyingFoogaFoog_Blue.png not loaded!\n";
    tex.setSmooth(false);
}

//  Constructor
             
FlyingFooga::FlyingFooga() : Botom() {
    onGround = true;
    alive = true;
    frozen = false;
    rolling = false;
    dropped = false;
    direction = 1;
    speed = 80;
    velocityY = 0;

    isFlying = false;
    flightTimer = 0.0f;
    groundTimer = 2.5f;
    moveX = 0.0f;
    moveY = 0.0f;

    walkTimer = 0.0f;
    droppingOff = false;

    currentAnim = FoogaAnim::IDLE;
    animFrame = 0;
    animTimer = 0.0f;
    animFrameDuration = 0.15f;   

    loadFoogaSheet(foogaSheet);

    // Override the texture Botom set 
    sprite.setTexture(foogaSheet);
    sprite.setTextureRect(animRects[0][0]);
    applyFoogaScale(1);
    sprite.setPosition(100, 100);
}


//  Copy constructor           
FlyingFooga::FlyingFooga(const FlyingFooga& other) : Botom(other) {
    isFlying = other.isFlying;
    flightTimer = other.flightTimer;
    groundTimer = other.groundTimer;
    moveX = other.moveX;
    moveY = other.moveY;
    walkTimer = other.walkTimer;
    droppingOff = other.droppingOff;
    currentAnim = other.currentAnim;
    animFrame = other.animFrame;
    animTimer = other.animTimer;
    animFrameDuration = other.animFrameDuration;

    loadFoogaSheet(foogaSheet);
    sprite.setTexture(foogaSheet);

    int s = static_cast<int>(currentAnim);
    sprite.setTextureRect(animRects[s][animFrame]);
    applyFoogaScale(direction);
    sprite.setPosition(other.sprite.getPosition());
}

//  Assignment operator
FlyingFooga& FlyingFooga::operator=(const FlyingFooga& other) {
    if (this != &other) {
        Botom::operator=(other);
        isFlying = other.isFlying;
        flightTimer = other.flightTimer;
        groundTimer = other.groundTimer;
        moveX = other.moveX;
        moveY = other.moveY;
        walkTimer = other.walkTimer;
        droppingOff = other.droppingOff;
        currentAnim = other.currentAnim;
        animFrame = other.animFrame;
        animTimer = other.animTimer;
        animFrameDuration = other.animFrameDuration;

        loadFoogaSheet(foogaSheet);
        sprite.setTexture(foogaSheet);

        int s = static_cast<int>(currentAnim);
        sprite.setTextureRect(animRects[s][animFrame]);
        applyFoogaScale(direction);
        sprite.setPosition(other.sprite.getPosition());
    }
    return *this;
}

//  update 
void FlyingFooga::update(float dt) {
    if (!alive) return;

    //   frozen / rolling     
    if (frozen || rolling) {
        setAnimState(FoogaAnim::ROLL);
        // Botom
        Botom::update(dt);
        
        sprite.setTexture(foogaSheet);
        int s = static_cast<int>(currentAnim);
        sprite.setTextureRect(animRects[s][animFrame]);
        updateAnimation(dt);
        return;
    }

    //   FLY MODE           
    if (isFlying) {
        setAnimState(FoogaAnim::FLY);

        sprite.move(moveX * dt, moveY * dt);

        sf::Vector2f pos = sprite.getPosition();
        if (pos.x < 0) { sprite.setPosition(0, pos.y); moveX *= -1; }
        if (pos.x > 960) { sprite.setPosition(960, pos.y); moveX *= -1; }
        if (pos.y < 0) { sprite.setPosition(pos.x, 0);   moveY *= -1; }
        if (pos.y > 700) { sprite.setPosition(pos.x, 700); moveY *= -1; }

        flightTimer -= dt;
        if (flightTimer <= 0.0f) {
            isFlying = false;
            velocityY = 0.0f;
            groundTimer = 2.5f;
            walkTimer = 0.0f;
            droppingOff = false;
        }

        updateAnimation(dt);
        return;
    }

    //   GROUND MODE   

    // Gravity
    velocityY += 800.0f * dt;
    bool wasOnGround = onGround;         
    if (onGround) velocityY = 0.0f;
    sprite.move(0.0f, velocityY * dt);

    // Just landed after flying, give a fresh wait before next takeoff
    if (!wasOnGround && onGround) {
        groundTimer = 2.5f;
        walkTimer = 0.0f;
        droppingOff = false;
        direction = (rand() % 2 == 0) ? -1 : 1;
        moveTimer = 2.0f;
    }

    // Random direction change
    moveTimer -= dt;
    if (moveTimer <= 0.0f) {
        direction = (rand() % 2 == 0) ? -1 : 1;
        moveTimer = 2.0f;
    }

    //   Platform-drop logic    
    if (onGround) {
        walkTimer += dt;
        if (walkTimer >= WALK_BEFORE_DROP) {
            droppingOff = true;   // keep walking until we fall off
        }
    }
    else {
        if (!droppingOff) 
            walkTimer = 0.0f;
        
    }

    // When we've just landed after a drop, clear the drop flag
    if (droppingOff && onGround && velocityY == 0.0f && walkTimer >= WALK_BEFORE_DROP) {

        droppingOff = false;
        walkTimer = 0.0f;
    }

    sprite.move(direction * speed * dt, 0.0f);

    // Wall bounds 
    if (!droppingOff) {
        float x = sprite.getPosition().x;
        if (x < 0) { sprite.setPosition(0, sprite.getPosition().y); direction = 1; }
        if (x > 960) { sprite.setPosition(960, sprite.getPosition().y); direction = -1; }
    }

    //   Take-off         
    if (onGround) groundTimer -= dt;

    if (groundTimer <= 0.0f && onGround) {
        isFlying = true;
        flightTimer = 3.0f;
        groundTimer = 2.5f;
        walkTimer = 0.0f;
        droppingOff = false;

        int dirs[8][2] = {
            {1,0},{-1,0},{0,1},{0,-1},
            {1,1},{-1,1},{1,-1},{-1,-1}
        };
        int r = rand() % 8;
        moveX = dirs[r][0] * 80.0f;
        moveY = dirs[r][1] * 80.0f;
    }

    //   Animation state        
    setAnimState(FoogaAnim::WALK);
    updateAnimation(dt);
}

//             
//  takeDamage / draw
//             
void FlyingFooga::takeDamage() {
    Botom::takeDamage();
}

void FlyingFooga::draw(sf::RenderWindow& window) {
    if (alive)
        window.draw(sprite);
}