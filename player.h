
#pragma once
#include <SFML/Graphics.hpp>

enum class AnimState {
    IDLE,
    WALK,
    JUMP,
    RUN,
    ATTACK,
    THROW_SNOW,
    HIT,
    WALK_WEAPON,
    WIN,
    BIG_NICK,
    DEATH
};

class Player {
public:
    static const int MAX_FRAMES = 8;

    struct FrameSet {
        sf::IntRect frames[MAX_FRAMES];
        int         frameCount;
        float       fps;
        bool        loops;
    };

private:
    sf::Texture texture;
    sf::Sprite  sprite;
    int characterID;
    float speed;
    float velocityY;
    float gravity;
    float jumpPower;
    bool  isOnGround;
    int   direction;
    float speedMultiplier;

    AnimState curState;
    int       frameIdx;
    float     frameTimer;

    bool isAttacking;
    bool isHit;
    bool isWinning;
    bool isDead;
    bool isBig;

    static const FrameSet ANIMS[(int)AnimState::DEATH + 1];

    void setState(AnimState s);
    void updateAnim(float dt);
    void applyRect();



    int characterType;

    sf::Texture altTexture;
    sf::Sprite  altSprite;

    bool useAltSprite;

public:
    Player();
    void setCharacter(int type);
    int getCharacter() const;

    void handleInput(float dt);
    void update(float dt);
    void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds();
    void  setPosition(float x, float y);
    float getVelocityY();
    void  setVelocityY(float v);
    void  setOnGround(bool val);
    void  setSpeedMultiplier(float m);
    float getSpeedMultiplier() const;

    void triggerAttack();
    void triggerHit();
    void triggerWin();
    void triggerDeath();
    void triggerBig(bool on);

    int       getDirection() const { return direction; }
    float     getX()         const { return sprite.getPosition().x; }
    float     getY()         const { return sprite.getPosition().y; }
    AnimState getState()     const { return curState; }
    bool      getIsOnGround() const { return isOnGround; }

    // Bonus: double-jump support — called from game.cpp when DJ power-up is active
    void tryDoubleJump(bool& usedDoubleJump);
};