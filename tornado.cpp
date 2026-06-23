#include "tornado.h"
#include <cmath>
#include <cstdlib>
#include <iostream>

//             
//  Static sprite-rect tables  (measured from Tornado_Blue.png 1250×1346)
//             

// WALK – Band 0  y=40 h=113 – 4 frames
const sf::IntRect Tornado::walkRects[4] = {
    sf::IntRect(24, 40,  91, 113),
    sf::IntRect(149, 40, 117, 113),
    sf::IntRect(288, 40, 112, 113),
    sf::IntRect(426, 40, 117, 113),
};

// IDLE – Band 4  y=715 h=109 – 2 frames
const sf::IntRect Tornado::idleRects[2] = {
    sf::IntRect(23, 715, 105, 109),
    sf::IntRect(153, 715, 113, 109),
};

// FLY – Band 3  y=536 h=133 – 2 frames
const sf::IntRect Tornado::flyRects[2] = {
    sf::IntRect(11, 536, 125, 133),
    sf::IntRect(162, 536,  95, 133),
};

// SHOOT – Band 5  y=883 h=109 – first 2 sprites (shoot pose)
const sf::IntRect Tornado::shootRects[2] = {
    sf::IntRect(32, 883, 104, 109),
    sf::IntRect(142, 883, 115, 109),
};

// KNIFE – Band 5  y=883 h=109 – sprites 2-5 (animated projectiles)
const sf::IntRect Tornado::knifeRects[4] = {
    sf::IntRect(284, 883, 60, 109),
    sf::IntRect(355, 883, 61, 109),
    sf::IntRect(427, 883, 60, 109),
    sf::IntRect(498, 883, 61, 109),
};

//  Animation helpers
static void loadTornadoSheet(sf::Texture& tex) {
    if (!tex.loadFromFile("Tornado_Blue.png"))
        std::cout << "ERROR: Tornado_Blue.png not loaded!\n";
    tex.setSmooth(false);
}

void Tornado::applyTornadoScale() {
    sf::IntRect r = sprite.getTextureRect();
    if (r.height <= 0) return;
    const float TARGET_H = 50.0f;
    float s = TARGET_H / static_cast<float>(r.height);
    sprite.setScale(direction > 0 ? -s : s, s);

    sprite.setOrigin(r.width * 0.5f, static_cast<float>(r.height));
}

void Tornado::setTornadoAnim(TornadoAnim a) {
    if (tAnim == a) return;
    tAnim = a;
    tFrame = 0;
    tAnimTimer = 0.0f;
}

void Tornado::updateTornadoAnim(float dt) {
    int frameCount = 1;
    const sf::IntRect* rects = nullptr;

    switch (tAnim) {
    case TornadoAnim::WALK:  rects = walkRects;  frameCount = 4; break;
    case TornadoAnim::IDLE:  rects = idleRects;  frameCount = 2; break;
    case TornadoAnim::FLY:   rects = flyRects;   frameCount = 2; break;
    case TornadoAnim::SHOOT: rects = shootRects; frameCount = 2; break;
    }

    tAnimTimer += dt;
    if (tAnimTimer >= tAnimFrameDuration) {
        tAnimTimer -= tAnimFrameDuration;
        tFrame = (tFrame + 1) % frameCount;
    }

    sprite.setTexture(tornadoSheet);
    sprite.setTextureRect(rects[tFrame]);
    applyTornadoScale();
}

//  Constructor
Tornado::Tornado() : FlyingFooga() {
    loadTornadoSheet(tornadoSheet);

    // Override the sprite texture set by FlyingFooga
    sprite.setTexture(tornadoSheet);
    sprite.setTextureRect(walkRects[0]);

    tAnim = TornadoAnim::WALK;
    tFrame = 0;
    tAnimTimer = 0.0f;
    tAnimFrameDuration = 0.12f;  

    applyTornadoScale();

    // Knife setup – uses same sheet
    knifeSprite.setTexture(tornadoSheet);
    knifeSprite.setTextureRect(knifeRects[0]);
    knifeSprite.setScale(0.5f, 0.5f);
    knifeSprite.setOrigin(
        knifeRects[0].width * 0.5f,
        knifeRects[0].height * 0.5f
    );

    knifeActive = false;
    knifeTimer = 2.0f;
    knifeFrame = 0;
    knifeAnimTimer = 0.0f;
    knifeFrameDuration = 0.08f;   
    knifeSpeedX = 0.0f;
    knifeSpeedY = 0.0f;

    teleportTimer = 0.0f;
    teleportCooldown = 2.5f;
    shootPoseTimer = 0.0f;
}

//  Copy constructor
Tornado::Tornado(const Tornado& other) : FlyingFooga(other) {
    loadTornadoSheet(tornadoSheet);
    sprite.setTexture(tornadoSheet);
    sprite.setPosition(other.sprite.getPosition());

    tAnim = other.tAnim;
    tFrame = other.tFrame;
    tAnimTimer = other.tAnimTimer;
    tAnimFrameDuration = other.tAnimFrameDuration;
    applyTornadoScale();

    knifeSprite.setTexture(tornadoSheet);
    knifeSprite.setTextureRect(knifeRects[0]);
    knifeSprite.setScale(0.5f, 0.5f);
    knifeSprite.setOrigin(knifeRects[0].width * 0.5f, knifeRects[0].height * 0.5f);

    knifeActive = false;
    knifeTimer = 2.0f;
    knifeFrame = 0;
    knifeAnimTimer = 0.0f;
    knifeFrameDuration = 0.08f;
    knifeSpeedX = 0.0f;
    knifeSpeedY = 0.0f;
    teleportTimer = 0.0f;
    teleportCooldown = other.teleportCooldown;
    shootPoseTimer = 0.0f;
}

//  Assignment operator
Tornado& Tornado::operator=(const Tornado& other) {
    if (this != &other) {
        FlyingFooga::operator=(other);
        loadTornadoSheet(tornadoSheet);
        sprite.setTexture(tornadoSheet);
        sprite.setPosition(other.sprite.getPosition());

        tAnim = other.tAnim;
        tFrame = other.tFrame;
        tAnimTimer = other.tAnimTimer;
        tAnimFrameDuration = other.tAnimFrameDuration;
        applyTornadoScale();

        knifeSprite.setTexture(tornadoSheet);
        knifeActive = false;
        knifeTimer = 2.0f;
        knifeFrame = 0;
        knifeAnimTimer = 0.0f;
        knifeSpeedX = 0.0f;
        knifeSpeedY = 0.0f;
        teleportTimer = 0.0f;
        teleportCooldown = other.teleportCooldown;
        shootPoseTimer = 0.0f;
    }
    return *this;
}

//  update  
void Tornado::update(float dt, float playerX, float playerY) {
    if (!alive) return;

    // Frozen / rolling 
    if (frozen || rolling) {
        FlyingFooga::update(dt);
        sprite.setTexture(tornadoSheet);   
        return;
    }

    if (isFlying) {
        moveX = (moveX > 0 ? 80.0f : -80.0f);
        moveY = (moveY > 0 ? 80.0f : -80.0f);
    }

    // Teleport
    teleportTimer += dt;
    if (teleportTimer >= teleportCooldown) {
        teleportTimer = 0.0f;
        float newX = static_cast<float>(rand() % 900 + 50);
        float newY = static_cast<float>(rand() % 500 + 100);
        sprite.setPosition(newX, newY);
        isFlying = false;
        velocityY = 0.0f;
        onGround = true;
        groundTimer = 2.5f;
    }

    // Base movement
    FlyingFooga::update(dt);

    // Clamp to screen bottom
    if (sprite.getPosition().y > 700.0f) {
        sprite.setPosition(sprite.getPosition().x, 700.0f);
        velocityY = 0.0f;
        onGround = true;
    }

    //   Knife shoot        
    knifeTimer -= dt;
    if (knifeTimer <= 0.0f && !knifeActive && alive) {
        knifeActive = true;
        shootPoseTimer = 0.3f;  

        sf::Vector2f pos = sprite.getPosition();
        knifeSprite.setPosition(pos.x, pos.y - 20.0f);

        float dx = playerX - pos.x;
        float dy = playerY - (pos.y - 20.0f);
        float len = std::sqrt(dx * dx + dy * dy);
        if (len > 0.0f) {
            knifeSpeedX = (dx / len) * 350.0f;
            knifeSpeedY = (dy / len) * 350.0f;
        }
        knifeTimer = 3.0f;
    }

    //   Move knife + animate it  
    if (knifeActive) {
        knifeSprite.move(knifeSpeedX * dt, knifeSpeedY * dt);

        // Spin animation
        knifeAnimTimer += dt;
        if (knifeAnimTimer >= knifeFrameDuration) {
            knifeAnimTimer -= knifeFrameDuration;
            knifeFrame = (knifeFrame + 1) % 4;
        }
        knifeSprite.setTexture(tornadoSheet);
        knifeSprite.setTextureRect(knifeRects[knifeFrame]);
        knifeSprite.setOrigin(
            knifeRects[knifeFrame].width * 0.5f,
            knifeRects[knifeFrame].height * 0.5f
        );

        sf::Vector2f kp = knifeSprite.getPosition();
        if (kp.x < -50.f || kp.x > 1050.f || kp.y < -50.f || kp.y > 850.f)
            knifeActive = false;
    }

    //   Shoot-pose cool-down   
    if (shootPoseTimer > 0.0f)
        shootPoseTimer -= dt;

    //   Re-bind our sheet  
    sprite.setTexture(tornadoSheet);

    //   Pick animation state   
    if (shootPoseTimer > 0.0f) {
        setTornadoAnim(TornadoAnim::SHOOT);
    }
    else if (isFlying) {
        setTornadoAnim(TornadoAnim::FLY);
    }
    else if (onGround) {
        // Walking or standing still
        if (speed > 0)
            setTornadoAnim(TornadoAnim::WALK);
        else
            setTornadoAnim(TornadoAnim::IDLE);
    }
    else {
        setTornadoAnim(TornadoAnim::FLY);  
    }

    updateTornadoAnim(dt);
}

//  draw
void Tornado::draw(sf::RenderWindow& window) {
    if (!alive) return;
    window.draw(sprite);
    if (knifeActive)
        window.draw(knifeSprite);
}

//  Getter
bool Tornado::isKnifeActive() { return knifeActive; }

sf::FloatRect Tornado::getKnifeBounds() { return knifeSprite.getGlobalBounds(); }

void Tornado::kill() { alive = false; }