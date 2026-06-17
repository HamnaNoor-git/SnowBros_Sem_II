#include "gamakichi.h"
#include "blast.h"
#include "platform.h"
#include <cmath>
#include <cstdlib>
#include <iostream>
using namespace std;


static void setSpriteFromRect(sf::Sprite& spr, const sf::IntRect& rect,
    float displaySize)
{
    spr.setTextureRect(rect);
    spr.setScale(displaySize / rect.width, displaySize / rect.height);
    spr.setOrigin(rect.width / 2.f, rect.height / 2.f);
}


Gamakichi::Gamakichi()
{
    //   Collision body         

    body.setSize(sf::Vector2f(210, 160));
    body.setFillColor(sf::Color::Transparent);
    body.setPosition(100, 100);

    //   Texture      
    if (!texture.loadFromFile("gamakichi.png"))
        cout << "Failed to load gamakichi.png\n";
    texture.setSmooth(false);

    //   Face frames    
    faceFrames[0] = sf::IntRect(64, 7, 1151, 648);   // idle  (mouth closed)
    faceFrames[1] = sf::IntRect(1445, 7, 1082, 632);   // attack (mouth open)

    currentFaceFrame = 0;
    attacking = false;
    attackFlash = 0.f;

    sprite.setTexture(texture);
    sprite.setTextureRect(faceFrames[0]);
    sprite.setScale(
        body.getSize().x / (float)faceFrames[0].width,
        body.getSize().y / (float)faceFrames[0].height
    );
    sprite.setOrigin(0.f, 0.f);

    //   Flying bomb frames       
    const int BOMB_Y = 1145, BOMB_H = 300;//286
    bombFrames[0] = sf::IntRect(48, BOMB_Y, 253, BOMB_H);
    bombFrames[1] = sf::IntRect(347, BOMB_Y, 194, BOMB_H);
    bombFrames[2] = sf::IntRect(615, BOMB_Y, 177, BOMB_H);
    bombFrames[3] = sf::IntRect(934, BOMB_Y, 176, BOMB_H);
    bombFrames[4] = sf::IntRect(1157, BOMB_Y, 193, BOMB_H);
    bombFrames[5] = sf::IntRect(1408, BOMB_Y, 253, BOMB_H);
    bombFrames[6] = sf::IntRect(1678, BOMB_Y, 194, BOMB_H);
    bombFrames[7] = sf::IntRect(1919, BOMB_Y, 235, BOMB_H);

    bombCurrentFrame = 0;
    bombAnimTimer = 0.f;
    bombAnimSpeed = 0.10f;   // switch frame every 

    bombSprite.setTexture(texture);
    setSpriteFromRect(bombSprite, bombFrames[0], 56.f); 

    //   Explosion frames        
    const int EXPL_Y = 1431, EXPL_H = 286;
    explFrames[0] = sf::IntRect(45, EXPL_Y, 206, EXPL_H);
    explFrames[1] = sf::IntRect(315, EXPL_Y, 179, EXPL_H);

    explCurrentFrame = 0;
    explAnimTimer = 0.f;
    explAnimSpeed = 0.12f;

    explSprite.setTexture(texture);
    setSpriteFromRect(explSprite, explFrames[0], 72.f); 

    //   Stats       
    maxHealth = 25;
    health = maxHealth;
    attackTimer = 4.0f;
    shootTimer = 0.f;
    alive = true;
    moveDir = 1.f;
}

void Gamakichi::setPosition(float x, float y)
{
    body.setPosition(x, y);
    sprite.setPosition(x, y);
}

void Gamakichi::update(float dt, float playerX, float playerY,
    Platform* platforms, int platformCount)
{
    if (!alive) return;

    //   Vertical bob          
    body.move(0.f, moveDir * 100.f * dt);
    sprite.setPosition(body.getPosition());

    if (body.getPosition().y < 100.f) moveDir = 1.f;
    if (body.getPosition().y > 500.f) moveDir = -1.f;

    //   Face
    if (attacking) {
        attackFlash -= dt;
        if (attackFlash <= 0.f) {
            attacking = false;
            currentFaceFrame = 0;
            sprite.setTextureRect(faceFrames[0]);
        }
    }

    //   Advance flying bomb animation frame            
    bombAnimTimer += dt;
    if (bombAnimTimer >= bombAnimSpeed) {
        bombAnimTimer = 0.f;
        bombCurrentFrame = (bombCurrentFrame + 1) % BOMB_FRAMES;
        setSpriteFromRect(bombSprite, bombFrames[bombCurrentFrame], 56.f);
    }

    // explosion animation frame             
    explAnimTimer += dt;
    if (explAnimTimer >= explAnimSpeed) {
        explAnimTimer = 0.f;
        explCurrentFrame = (explCurrentFrame + 1) % EXPL_FRAMES;
        setSpriteFromRect(explSprite, explFrames[explCurrentFrame], 72.f);
    }

    //   Attack timer     
    attackTimer -= dt;
    if (attackTimer <= 0.f) {

        // Spawn from the centre of the body
        float cx = body.getPosition().x + body.getSize().x / 2.f;
        float cy = body.getPosition().y + body.getSize().y / 2.f;

        for (int i = 0; i < 10; i++) {
            if (!blasts[i].isActive()) {
                float dx = playerX - cx;
                float dy = playerY - cy;
                float len = sqrtf(dx * dx + dy * dy);
                if (len > 0.f) { dx /= len; dy /= len; }
                blasts[i].shoot(cx, cy, dx * 50.f, dy * 50.f);
                break;
            }
        }

        // Show open-mouth
        attacking = true;
        attackFlash = 0.5f;
        currentFaceFrame = 1;
        sprite.setTextureRect(faceFrames[1]);

        attackTimer = 4.0f;
    }

    //   Update all blasts 
    for (int i = 0; i < 10; i++)
        blasts[i].update(dt, platforms, platformCount);
}

//             

void Gamakichi::draw(sf::RenderWindow& window)
{
    if (!alive) return;

    // Draw boss face at body position
    sprite.setPosition(body.getPosition());
    const sf::IntRect& fr = faceFrames[currentFaceFrame];
    sprite.setScale(
        body.getSize().x / (float)fr.width,
        body.getSize().y / (float)fr.height
    );
    window.draw(sprite);

    for (int i = 0; i < 10; i++) {
        if (!blasts[i].isActive()) continue;

        sf::FloatRect bb = blasts[i].getBounds();
        float cx = bb.left + bb.width / 2.f;
        float cy = bb.top + bb.height / 2.f;

        if (blasts[i].isExploded()) {
            // Show explosion sprite
            explSprite.setPosition(cx, cy);
            window.draw(explSprite);
        }
        else {
            // Show flying bomb sprite
            bombSprite.setPosition(cx, cy);
            window.draw(bombSprite);
        }
    }
}


bool Gamakichi::isAlive() { return alive; }
int  Gamakichi::getHealth() { return health; }
int  Gamakichi::getMaxHealth() { return maxHealth; }

void Gamakichi::takeDamage()
{
    health ;
    if (health <= 0) alive = false;
}

sf::FloatRect Gamakichi::getBounds() { return body.getGlobalBounds(); }
Blast* Gamakichi::getBlasts() { return blasts; }