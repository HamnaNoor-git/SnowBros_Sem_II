#include "player.h"
#include <iostream>
using namespace std;

const Player::FrameSet Player::ANIMS[] = {

    //   IDLE            
    {
        { sf::IntRect(135, 64, 181, 263) },
        1, 4.0f, true
    },

    //   WALK            
    {
        {
            sf::IntRect(135,  64, 181, 263),
            sf::IntRect(431,  64, 136, 263),
            sf::IntRect(736,  64, 164, 263),
            sf::IntRect(1042, 64, 155, 263),
            sf::IntRect(1329, 64, 173, 263),
        },
        5, 8.0f, true
    },

    //   JUMP            
    {
        {
            sf::IntRect(98,   375, 191, 248),
            sf::IntRect(422,  375, 173, 248),
            sf::IntRect(727,  375, 173, 248),
            sf::IntRect(1023, 375, 173, 248),
            sf::IntRect(1347, 375, 173, 248),
            sf::IntRect(1644, 375, 200, 248),
        },
        6, 10.0f, false
    },

    //   RUN      
    {
        {
            sf::IntRect(98,   690, 220, 248),
            sf::IntRect(422,  690, 192, 248),
            sf::IntRect(718,  690, 210, 248),
            sf::IntRect(977,  690, 238, 248),
            sf::IntRect(1616, 690, 210, 248),
            sf::IntRect(1939, 690, 201, 248),
            sf::IntRect(2245, 690, 201, 248),
        },
        7, 12.0f, true
    },

    //   ATTACK           
    {
        {
            sf::IntRect(80,   978, 256, 301),
            sf::IntRect(376,  978, 265, 301),
            sf::IntRect(690,  978, 248, 301),
            sf::IntRect(986,  978, 266, 301),
            sf::IntRect(1283, 978, 284, 301),
            sf::IntRect(1588, 978, 285, 301),
            sf::IntRect(1887, 978, 298, 301),
        },
        7, 10.0f, false
    },

    //   THROW_SNOW         
    {
        {
            sf::IntRect(107,  1284, 192, 265),
            sf::IntRect(375,  1284, 155, 265),
            sf::IntRect(782,  1284, 174, 265),
            sf::IntRect(1004, 1284, 220, 265),
            sf::IntRect(1310, 1284, 173, 265),
            sf::IntRect(1634, 1284, 183, 265),
            sf::IntRect(1939, 1284, 174, 265),
        },
        7, 10.0f, false
    },

    //   HIT      
    {
        {
            sf::IntRect(55,   1589, 253, 274),
            sf::IntRect(421,  1589, 174, 274),
            sf::IntRect(663,  1589, 247, 274),
            sf::IntRect(1032, 1589, 183, 274),
            sf::IntRect(1274, 1589, 256, 274),
            sf::IntRect(1653, 1589, 136, 274),
            sf::IntRect(1913, 1589, 227, 274),
            sf::IntRect(2245, 1589, 192, 274),
        },
        8, 10.0f, false
    },

    //   WALK_WEAPON  
    {
        {
            sf::IntRect(125,  1902, 137, 285),
            sf::IntRect(393,  1902, 220, 285),
            sf::IntRect(699,  1902, 219, 285),
            sf::IntRect(1041, 1902, 183, 285),
            sf::IntRect(1347, 1902, 182, 285),
            sf::IntRect(1634, 1902, 192, 285),
        },
        6, 8.0f, true
    },

    //   WIN      
    {
        {
            sf::IntRect(107,  2200, 192, 299),
            sf::IntRect(431,  2200, 210, 299),
            sf::IntRect(718,  2200, 191, 299),
            sf::IntRect(1356, 2200, 182, 299),
            sf::IntRect(1662, 2200, 192, 299),
        },
        5, 6.0f, true
    },

    //   BIG_NICK          
    {
        {
            sf::IntRect(98,   2507, 228, 445),
            sf::IntRect(366,  2507, 292, 445),
            sf::IntRect(691,  2507, 385, 445),
            sf::IntRect(1125, 2507, 414, 445),
            sf::IntRect(1597, 2507, 405, 445),
            sf::IntRect(2026, 2507, 446, 445),
        },
        6, 8.0f, true
    },

    //   DEATH     
    {
        {
            sf::IntRect(126,  2968, 200, 294),
            sf::IntRect(422,  2968, 210, 294),
            sf::IntRect(709,  2968, 220, 294),
            sf::IntRect(987,  2968, 266, 294),
            sf::IntRect(1282, 2968, 285, 294),
        },
        5, 8.0f, false
    },
};


//  CONSTRUCTOR

Player::Player() {
    if (!texture.loadFromFile("nick.png")) {
        cout << "ERROR: nick.png failed to load!\n";
    }
    texture.setSmooth(false);
    sprite.setTexture(texture);

    speed = 200.0f;
    velocityY = 0.0f;
    gravity = 900.0f;
    jumpPower = -500.0f;
    isOnGround = false;
    direction = 1;
    speedMultiplier = 1.0f;

    isAttacking = false;
    isHit = false;
    isWinning = false;
    isDead = false;
    isBig = false;

    curState = AnimState::IDLE;
    frameIdx = 0;
    frameTimer = 0.0f;

    sprite.setScale(0.35f, 0.35f);
    sprite.setPosition(100.0f, 500.0f);

    applyRect();


    characterType = 0;
    useAltSprite = false;
}

void Player::setState(AnimState s) {
    if (curState == s) return;
    curState = s;
    frameIdx = 0;
    frameTimer = 0.0f;
    applyRect();
}


void Player::applyRect() {
    const FrameSet& anim = ANIMS[(int)curState];
    if (frameIdx < 0 || frameIdx >= anim.frameCount) return;

    sf::IntRect r = anim.frames[frameIdx];

    if (direction == -1) {
        sprite.setTextureRect(r);
    }
    else {
        sprite.setTextureRect(sf::IntRect(r.left + r.width, r.top, -r.width, r.height));
    }
}


void Player::updateAnim(float dt) {
    const FrameSet& anim = ANIMS[(int)curState];
    frameTimer += dt;

    float frameDur = 1.0f / anim.fps;
    if (frameTimer >= frameDur) {
        frameTimer -= frameDur;
        frameIdx++;

        if (frameIdx >= anim.frameCount) {
            if (anim.loops) {
                frameIdx = 0;
            }
            else {
                frameIdx = anim.frameCount - 1;  // hold last frame

                if (curState == AnimState::ATTACK ||
                    curState == AnimState::THROW_SNOW) {
                    isAttacking = false;
                    setState(AnimState::IDLE);
                }
                if (curState == AnimState::HIT) {
                    isHit = false;
                    setState(AnimState::IDLE);
                }
            }
        }
    }

    applyRect();
}

//  HANDLE INPUT

void Player::handleInput(float dt) {
    if (isHit || isWinning || isDead) return;

    bool moving = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {

        sprite.move(-speed * speedMultiplier * dt, 0.0f);
        direction = -1;

        if (useAltSprite)
            altSprite.setScale(-0.18f, 0.18f);  // flip left
        moving = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {

        sprite.move(speed * speedMultiplier * dt, 0.0f);
        direction = 1;

        if (useAltSprite)
            altSprite.setScale(0.18f, 0.18f);   // normal right

        moving = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && isOnGround) {
        velocityY = jumpPower;
        isOnGround = false;
        setState(AnimState::JUMP);
        return;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !isAttacking) {
        triggerAttack();
        return;
    }

    if (isAttacking) return;
    if (!isOnGround) return;

    if (moving) {
        setState(isBig ? AnimState::BIG_NICK : AnimState::WALK);
    }
    else {
        setState(AnimState::IDLE);
    }

}

// UPDATE
void Player::update(float dt) {
    velocityY += gravity * dt;
    sprite.move(0.0f, velocityY * dt);

    if (isOnGround && curState == AnimState::JUMP) {
        setState(AnimState::IDLE);
    }

    updateAnim(dt);

    if (useAltSprite) {
        altSprite.setPosition(sprite.getPosition());
    }
}

//  DRAW


void Player::draw(sf::RenderWindow& window) {

    if (useAltSprite) {

        sf::Sprite temp = altSprite;

        temp.move(0, 150);

        window.draw(temp);
    }
    else {
        window.draw(sprite);
    }
}

//  TRIGGERS
void Player::triggerAttack() {
    if (isAttacking) return;
    isAttacking = true;
    setState(AnimState::ATTACK);
}

void Player::triggerHit() {
    isHit = true;
    isAttacking = false;
    setState(AnimState::HIT);
}

void Player::triggerWin() {
    isWinning = true;
    setState(AnimState::WIN);
}

void Player::triggerDeath() {
    isDead = true;
    setState(AnimState::DEATH);
}

void Player::triggerBig(bool on) {
    isBig = on;
    setState(on ? AnimState::BIG_NICK : AnimState::IDLE);
}

//  GETTERS / SETTERS
sf::FloatRect Player::getBounds() {
    return sprite.getGlobalBounds();
}

void Player::setPosition(float x, float y) {

    sprite.setPosition(x, y);

    if (useAltSprite)
        altSprite.setPosition(x, y);
}
float Player::getVelocityY() { return velocityY; }
void  Player::setVelocityY(float v) { velocityY = v; }
void  Player::setOnGround(bool val) { isOnGround = val; }
void  Player::setSpeedMultiplier(float m) { speedMultiplier = m; }
float Player::getSpeedMultiplier() const { return speedMultiplier; }





void Player::setCharacter(int type) {

    characterType = type;

    //DEFAULT PLAYER 
    if (type == 0) {
        useAltSprite = false;

        texture.loadFromFile("nick.png");
        sprite.setTexture(texture);

        speed = 200;
    }

    //  CHARACTER 2 (simple sprite)
    else if (type == 1) {

        useAltSprite = true;

        altTexture.loadFromFile("elsa.png");
        altSprite.setTexture(altTexture);

        altSprite.setScale(0.18f, 0.18f);

        sf::FloatRect b = altSprite.getLocalBounds();
        altSprite.setOrigin(b.width / 2.f, b.height + 200);

        altSprite.setPosition(sprite.getPosition());

        speed = 240;
    }

    // CHARACTER 3 (simple sprite)
    else if (type == 2) {

        useAltSprite = true;

        altTexture.loadFromFile("man.png");
        altSprite.setTexture(altTexture);

        altSprite.setScale(0.2f, 0.2f);

        sf::FloatRect b = altSprite.getLocalBounds();
        altSprite.setOrigin(b.width / 2.f, b.height + 250);

        altSprite.setPosition(sprite.getPosition());

        speed = 160;
    }
}

int Player::getCharacter() const {
    return characterType;
}
// Bonus: tryDoubleJump — grant a second jump if the player is airborne and
// the W key is freshly pressed while the double-jump power-up is active.
void Player::tryDoubleJump(bool& usedDoubleJump) {
    static bool wHeld = false;
    bool wPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
    bool wEdge = wPressed && !wHeld;
    wHeld = wPressed;

    if (wEdge && !isOnGround && !usedDoubleJump) {
        velocityY = jumpPower * 0.85f;  // slightly weaker second jump
        usedDoubleJump = true;
        setState(AnimState::JUMP);
    }
    // Reset flag when player lands
    if (isOnGround) usedDoubleJump = false;
}
