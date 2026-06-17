#include "botom.h"
#include <iostream>
#include <cstdlib>

sf::Texture  Botom::sheetBotom;
sf::Texture  Botom::sheetNick;
bool         Botom::sheetsLoaded = false;

sf::IntRect  Botom::walkRects[Botom::WALK_COUNT];
sf::IntRect  Botom::rollRects[Botom::ROLL_COUNT];
sf::IntRect  Botom::freezeRects[Botom::FREEZE_COUNT];
sf::IntRect  Botom::frozenRects[Botom::FROZEN_COUNT];
sf::IntRect  Botom::thawRects[Botom::THAW_COUNT];
sf::IntRect  Botom::eggRect;
bool         Botom::rectsInited = false;

const float  Botom::FREEZE_FRAME_DUR = 0.10f;  // 10 freeze-in
const float  Botom::THAW_FRAME_DUR = 0.12f;  // 8 unfreeze

static const float SCALE = 0.8f;
static const float EGG_SCALE = 0.15f;

void Botom::loadSheets()
{
    if (sheetsLoaded) return;
    if (!sheetBotom.loadFromFile("Botom_Pink.png"))
        std::cout << "ERROR: Botom_Pink.png not loaded!\n";
    if (!sheetNick.loadFromFile("nick.png"))
        std::cout << "ERROR: nick.png not loaded!\n";
    sheetsLoaded = true;
}

void Botom::initRects()
{
    if (rectsInited) return;

    // Walk frames (11) – Botom_Pink.png
    const int wx[WALK_COUNT] = { 12,106, 12,  6,  6,100,  3,101,198,  4,  9 };
    const int wy[WALK_COUNT] = { 21, 21,138,595,709,709,822,822,822,936,1165 };
    const int ww[WALK_COUNT] = { 79, 79, 79, 84, 87, 88, 84, 83, 83, 89, 84 };
    const int wh[WALK_COUNT] = { 92, 92, 82, 92, 82, 82, 83, 83, 83, 84, 83 };
    for (int i = 0; i < WALK_COUNT; ++i)
        walkRects[i] = sf::IntRect(wx[i], wy[i], ww[i], wh[i]);

    // Roll frames (9) – Botom_Pink.png
    const int rx[ROLL_COUNT] = { 2, 99,193,290,390,476,575,666,757 };
    const int rw[ROLL_COUNT] = { 93, 87, 79, 85, 79, 84, 84, 88, 91 };
    for (int i = 0; i < ROLL_COUNT; ++i)
        rollRects[i] = sf::IntRect(rx[i], 1050, rw[i], 94);

    //   nick.png        
    // FREEZE-IN animation  Band 3 y=978 h=301 – first 7 sprites
    // (enemy arms raise then body gets encased in snowball)
    freezeRects[0] = sf::IntRect(79, 978, 257, 301);
    freezeRects[1] = sf::IntRect(375, 978, 267, 301);
    freezeRects[2] = sf::IntRect(689, 978, 249, 301);
    freezeRects[3] = sf::IntRect(985, 978, 268, 301);
    freezeRects[4] = sf::IntRect(1282, 978, 285, 301);
    freezeRects[5] = sf::IntRect(1587, 978, 286, 301);
    freezeRects[6] = sf::IntRect(1886, 978, 299, 301);

    // FROZEN IDLE loop – Band 10 row2 y=3579 h=288 – 2 white snowball frames
    frozenRects[0] = sf::IntRect(89, 3579, 236, 288);
    frozenRects[1] = sf::IntRect(386, 3579, 214, 288);

    // THAW (unfreeze) animation – Band 9 y=2967 h=297 – first 5 sprites
    // (cracked snowball → normal character emerging)
    thawRects[0] = sf::IntRect(126, 2967, 201, 297);
    thawRects[1] = sf::IntRect(422, 2967, 210, 297);
    thawRects[2] = sf::IntRect(709, 2967, 220, 297);
    thawRects[3] = sf::IntRect(986, 2967, 267, 297);
    thawRects[4] = sf::IntRect(1282, 2967, 286, 297);

    // Legacy egg rect (kept for compatibility with other classes that may use it)
    eggRect = sf::IntRect(89, 3579, 236, 288);

    rectsInited = true;
}

//   applyScaleAndOrigin  
static void applyScaleAndOrigin(sf::Sprite& spr, bool frozen, int direction)
{
    if (frozen) {
        spr.setScale(EGG_SCALE, EGG_SCALE);
    }
    else {
        float sx = (direction < 0) ? -SCALE : SCALE;
        spr.setScale(sx, SCALE);
    }
    sf::FloatRect b = spr.getLocalBounds();
    spr.setOrigin(b.width / 2.f, b.height);
}

//   advanceAnim()     
void Botom::advanceAnim(float dt)
{
    //   FREEZE / THAW phases 
    if (freezePhase == FreezePhase::FROZEN_IDLE) {
        freezeAnimTimer += dt;
        if (freezeAnimTimer >= FREEZE_FRAME_DUR * 3.0f) {  // slow loop 
            freezeAnimTimer = 0.f;
            freezeFrame = (freezeFrame + 1) % FROZEN_COUNT;
        }
        // Use the nick.png snowball frames for the frozen idle look
        sprite.setTexture(sheetNick);
        sprite.setTextureRect(frozenRects[freezeFrame]);
        sprite.setColor(sf::Color::White);   //show  egg colour
        sprite.setScale(EGG_SCALE, EGG_SCALE);
        sf::FloatRect b = sprite.getLocalBounds();
        sprite.setOrigin(b.width / 2.f, b.height);
        // Lock position  absolutely no movement while frozen
        sprite.setPosition(frozenX, frozenY);
        return;
    }

    if (freezePhase == FreezePhase::THAWING) {
        freezeAnimTimer += dt;
        if (freezeAnimTimer >= THAW_FRAME_DUR) {
            freezeAnimTimer -= THAW_FRAME_DUR;
            freezeFrame++;
            if (freezeFrame >= THAW_COUNT) {
                // Thaw done  back to normal walk
                freezePhase = FreezePhase::NONE;
                freezeFrame = 0;
                frozen = false;
                sprite.setColor(sf::Color::White);
                animFrame = 0;
                animTimer = 0.f;
            }
        }
        if (freezePhase == FreezePhase::THAWING) {  

            int f = std::min(freezeFrame, THAW_COUNT - 1);
            // Map thaw frame index into walk frames (spread evenly across walk strip)
            int walkIndex = (f * WALK_COUNT) / THAW_COUNT;
            sprite.setTexture(sheetBotom);
            sprite.setTextureRect(walkRects[walkIndex % WALK_COUNT]);
            // Fade from ice-blue back to white over the thaw frames
            float t = static_cast<float>(f) / static_cast<float>(THAW_COUNT - 1); 
            sf::Uint8 r = static_cast<sf::Uint8>(180 + static_cast<int>(75 * t));
            sf::Uint8 g = static_cast<sf::Uint8>(220 + static_cast<int>(35 * t));
            sf::Uint8 b_col = 255u;
            sprite.setColor(sf::Color(r, g, b_col));
            float sx = (direction < 0) ? -SCALE : SCALE;
            sprite.setScale(sx, SCALE);
            sf::FloatRect b = sprite.getLocalBounds();
            sprite.setOrigin(b.width / 2.f, b.height);
            sprite.setPosition(frozenX, frozenY);
        }
        return;
    }

      
    float fps = rolling ? 12.f : 8.f;
    int   total = rolling ? ROLL_COUNT : WALK_COUNT;

    animTimer += dt;
    if (animTimer >= 1.f / fps) {
        animTimer = 0.f;
        animFrame = (animFrame + 1) % total;
    }

    if (rolling) {
        sprite.setTexture(sheetBotom);
        sprite.setTextureRect(rollRects[animFrame % ROLL_COUNT]);
    }
    else {
        sprite.setTexture(sheetBotom);
        sprite.setTextureRect(walkRects[animFrame % WALK_COUNT]);
    }
    applyScaleAndOrigin(sprite, false, direction);
}

//   Constructor      
Botom::Botom() {
    if (!texture.loadFromFile("botom_red.png"))
        std::cout << "ERROR: Botom image not loaded!\n";

    loadSheets();
    initRects();

    animFrame = 0;
    animTimer = 0.f;

    sprite.setTexture(sheetBotom);
    sprite.setTextureRect(walkRects[0]);
    applyScaleAndOrigin(sprite, false, 1);

    speed = 80;
    moveTimer = 0;
    dropped = false;
    frozen = false;
    frozenTimer = 0;
    rolling = false;
    rollTimer = 0;
    rollDirection = 1;
    alive = true;
    velocityY = 0;
    direction = 1;
    onGround = false;

    freezePhase = FreezePhase::NONE;
    freezeFrame = 0;
    freezeAnimTimer = 0.f;
    frozenX = 0.f;
    frozenY = 0.f;
}

//   Copy constructor          
Botom::Botom(const Botom& other) : Enemy(other) {
    loadSheets();
    initRects();

    animFrame = other.animFrame;
    animTimer = other.animTimer;

    sprite.setTexture(sheetBotom);
    sprite.setTextureRect(walkRects[0]);
    applyScaleAndOrigin(sprite, false, other.direction);
    sprite.setPosition(other.sprite.getPosition());
    sprite.setColor(other.sprite.getColor());

    moveTimer = other.moveTimer;
    dropped = other.dropped;
    frozen = other.frozen;
    frozenTimer = other.frozenTimer;
    rolling = other.rolling;
    rollTimer = other.rollTimer;
    rollDirection = other.rollDirection;
    speed = other.speed;
    direction = other.direction;
    velocityY = other.velocityY;
    alive = other.alive;
    onGround = other.onGround;

    freezePhase = other.freezePhase;
    freezeFrame = other.freezeFrame;
    freezeAnimTimer = other.freezeAnimTimer;
    frozenX = other.frozenX;
    frozenY = other.frozenY;
}

//   Assignment operator  
Botom& Botom::operator=(const Botom& other) {
    if (this != &other) {
        loadSheets();
        initRects();

        animFrame = other.animFrame;
        animTimer = other.animTimer;

        sprite.setTexture(sheetBotom);
        sprite.setTextureRect(walkRects[0]);
        applyScaleAndOrigin(sprite, false, other.direction);
        sprite.setPosition(other.sprite.getPosition());
        sprite.setColor(other.sprite.getColor());

        moveTimer = other.moveTimer;
        dropped = other.dropped;
        frozen = other.frozen;
        frozenTimer = other.frozenTimer;
        rolling = other.rolling;
        rollTimer = other.rollTimer;
        rollDirection = other.rollDirection;
        speed = other.speed;
        direction = other.direction;
        velocityY = other.velocityY;
        alive = other.alive;
        onGround = other.onGround;

        freezePhase = other.freezePhase;
        freezeFrame = other.freezeFrame;
        freezeAnimTimer = other.freezeAnimTimer;
        frozenX = other.frozenX;
        frozenY = other.frozenY;
    }
    return *this;
}

//   reset()        
void Botom::reset() {
    alive = true;
    frozen = false;
    rolling = false;
    dropped = false;
    velocityY = 0;
    direction = 1;
    onGround = false;
    freezePhase = FreezePhase::NONE;
    freezeFrame = 0;
    freezeAnimTimer = 0.f;
    sprite.setColor(sf::Color::White);
    animFrame = 0;
    animTimer = 0.f;
}

//   update()              
void Botom::update(float dt) {
    if (!alive) return;

    advanceAnim(dt);

    // Gravity always applies, but frozen enemy position is clamped by advanceAnim
    velocityY += 800 * dt;
    if (onGround) velocityY = 0;
    sprite.move(0, velocityY * dt);

    // ROLLING
    if (rolling) {
        sprite.move(rollDirection * 300.f * dt, 0);
        rollTimer -= dt;
        if (rollTimer <= 0) {
            alive = false;
            dropped = true;
        }
        return;
    }

    // FROZEN 
    if (frozen) {
        frozenTimer -= dt;
        if (frozenTimer <= 0 && freezePhase == FreezePhase::FROZEN_IDLE) {
            // Start thaw animation
            freezePhase = FreezePhase::THAWING;
            freezeFrame = 0;
            freezeAnimTimer = 0.f;
        }
        // Keep sprite at locked position 

        sprite.setPosition(frozenX, frozenY);
        return;
    }

    // NORMAL movement
    moveTimer -= dt;
    if (moveTimer <= 0) {
        direction = (rand() % 2 == 0) ? -1 : 1;
        moveTimer = 2.0f;
    }

    sprite.move(direction * speed * dt, 0);
    sprite.move(direction * speed * dt, 0);  


    // Flip sprite to face movement direction
    sf::Vector2f scale = sprite.getScale();
    if (direction == -1)
        sprite.setScale(std::abs(scale.x), scale.y);
    else
        sprite.setScale(-std::abs(scale.x), scale.y);

    sf::FloatRect bounds = sprite.getGlobalBounds();
    if (bounds.left < 0) {
        sprite.setPosition(bounds.width / 2.f, sprite.getPosition().y);
        direction = 1;
    }
    if (bounds.left + bounds.width > 1000) {
        sprite.setPosition(1000 - bounds.width / 2.f, sprite.getPosition().y);
        direction = -1;
    }
}

//   takeDamage          
void Botom::takeDamage() {

    if (frozen && !rolling && freezePhase == FreezePhase::FROZEN_IDLE) {
        frozen = false;
        rolling = true;
        rollDirection = direction;
        rollTimer = 4.0f;
        freezePhase = FreezePhase::NONE;
        sprite.setColor(sf::Color(220, 240, 255));
        animFrame = 0;
        animTimer = 0.f;
        return;
    }

    // FIRST HIT , FREEZE 

    if (!frozen && !rolling) {
        frozen = true;
        frozenTimer = 5.0f;
        freezePhase = FreezePhase::FROZEN_IDLE; 
        freezeFrame = 0;
        freezeAnimTimer = 0.f;
        // Snapshot the position so the enemy stays locked here
        frozenX = sprite.getPosition().x;
        frozenY = sprite.getPosition().y;
        sprite.setColor(sf::Color(180, 220, 255));
        animFrame = 0;
        animTimer = 0.f;
    }
}

//   kick()               
void Botom::kick(int dir) {
    if (!frozen || rolling) return;
    if (freezePhase != FreezePhase::FROZEN_IDLE) return;  // can only kick when fully frozen

    frozen = false;
    rolling = true;
    rollDirection = dir;
    rollTimer = 4.0f;
    freezePhase = FreezePhase::NONE;
    animFrame = 0;
    animTimer = 0.f;
}

//   Remaining methods   
void Botom::draw(sf::RenderWindow& window) {
    if (alive) window.draw(sprite);
}

sf::FloatRect Botom::getBounds() { return sprite.getGlobalBounds(); }
void          Botom::setPosition(float x, float y) { sprite.setPosition(x, y); }
bool          Botom::hasDropped() { return dropped; }
void          Botom::resetDrop() { dropped = false; }
bool          Botom::isFrozen()  const { return frozen; }
bool          Botom::isRolling() const { return rolling; }


