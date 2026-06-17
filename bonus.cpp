// =============================================================================
//  bonus.cpp  —  Implementation of extra features for SnowBros project
//
//  Compile with the rest of the project (no separate linkage needed).
//  Requires: SFML 2.x, bonus.h, enemy.h
// =============================================================================

#include "bonus.h"
#include <cstdlib>
#include <cmath>
#include <iostream>

// ─────────────────────────────────────────────────────────────────────────────
//  Monkey — zig-zag enemy
// ─────────────────────────────────────────────────────────────────────────────

// Static members
sf::Texture Monkey::sharedTexture;
bool        Monkey::textureLoaded = false;

// Two simple frame rects (full-texture fallback if monkey.png is a single image)
const sf::IntRect Monkey::IDLE_FRAMES[2] = {
    sf::IntRect(0,   0, 256, 256),
    sf::IntRect(256, 0, 256, 256)
};

Monkey::Monkey() {
    if (!textureLoaded) {
        if (sharedTexture.loadFromFile("monkey.png")) {
            sharedTexture.setSmooth(false);
            textureLoaded = true;
        }
        else {
            std::cout << "WARNING: monkey.png not found – using fallback colour\n";
        }
    }

    sprite.setTexture(sharedTexture);

    // Scale so the monkey appears roughly similar in size to Mogera on screen
    sprite.setScale(0.33f, 0.33f);
    sprite.setPosition(500.f, 300.f);

    health = 10;
    maxHealth = 10;
    alive = true;
    isDying = false;
    dyingTimer = 0.f;
    frameIndex = 0;
    animTimer = 0.f;

    // Zig-zag parameters
    zigzagTimer = 0.f;
    zigzagSegment = 1.2f;          // change horizontal direction every 1.2 s
    zigzagDir = 1;
    verticalAmp = 80.f;          // oscillate ±80 px vertically
    verticalFreq = 2.5f;          // ~0.4 s per full vertical cycle
    baseY = 300.f;
    timeAccum = 0.f;
    speed = 150.f;
}


void Monkey::reset(float x, float y) {
    health = 10;
    maxHealth = 10;
    alive = true;
    isDying = false;
    dyingTimer = 0.f;
    frameIndex = 0;
    animTimer = 0.f;
    zigzagTimer = 0.f;
    zigzagDir = 1;
    timeAccum = 0.f;
    setPosition(x, y);
    applyFrame();
}

void Monkey::applyFrame() {
    // If the texture is smaller than two frames just use the full image
    unsigned int tw = sharedTexture.getSize().x;
    if (tw >= 512) {
        sprite.setTextureRect(IDLE_FRAMES[frameIndex]);
    }
    else {
        // Single-frame texture — use full image
        sprite.setTextureRect(sf::IntRect(0, 0,
            (int)sharedTexture.getSize().x,
            (int)sharedTexture.getSize().y));
    }
}

void Monkey::update(float dt) {
    if (!alive) return;

    if (isDying) {
        dyingTimer += dt;
        if (dyingTimer >= 1.0f)
            alive = false;
        return;
    }

    timeAccum += dt;
    zigzagTimer += dt;

    // Animate (2 frames at ~0.2 s each)
    animTimer += dt;
    if (animTimer >= 0.2f) {
        animTimer -= 0.2f;
        frameIndex = (frameIndex + 1) % 2;
        applyFrame();
    }

    // Flip direction every zigzagSegment seconds
    if (zigzagTimer >= zigzagSegment) {
        zigzagTimer = 0.f;
        zigzagDir = -zigzagDir;
        // Flip sprite horizontally to face movement direction
        float sx = std::abs(sprite.getScale().x);
        float sy = sprite.getScale().y;
        sprite.setScale(sx * (float)zigzagDir, sy);
    }

    // Horizontal movement
    float dx = (float)zigzagDir * speed * dt;

    // Vertical oscillation (sine wave)
    float newY = baseY + verticalAmp * std::sin(verticalFreq * timeAccum);
    float dy = newY - sprite.getPosition().y;

    sprite.move(dx, dy);

    // Clamp horizontally so the monkey stays in the play area
    sf::FloatRect b = sprite.getGlobalBounds();
    if (b.left < 0.f) {
        sprite.move(-b.left, 0.f);
        zigzagDir = 1;
    }
    if (b.left + b.width > 1000.f) {
        sprite.move(1000.f - (b.left + b.width), 0.f);
        zigzagDir = -1;
    }
}

void Monkey::draw(sf::RenderWindow& window) {
    if (!alive) return;

    if (textureLoaded) {
        window.draw(sprite);
    }
    else {
        // Fallback — draw a brown rectangle so the enemy is still visible
        sf::RectangleShape rect({ 48.f, 48.f });
        rect.setFillColor(sf::Color(139, 90, 43));
        rect.setPosition(sprite.getPosition());
        window.draw(rect);
    }
}

void Monkey::setPosition(float x, float y) {
    sprite.setPosition(x, y);
    baseY = y;   // anchor vertical oscillation to spawn Y
}

sf::FloatRect Monkey::getBounds() {
    if (textureLoaded)
        return sprite.getGlobalBounds();
    // Fallback bounds
    auto pos = sprite.getPosition();
    return sf::FloatRect(pos.x, pos.y, 48.f, 48.f);
}

void Monkey::takeDamage() {
    if (isDying) return;
    health ;
    if (health <= 0) {
        isDying = true;
        dyingTimer = 0.f;
        alive = true;  // still drawn briefly while dying
    }
}

bool Monkey::isAlive() { return alive; }


// ─────────────────────────────────────────────────────────────────────────────
//  ShineEffect — golden burst / ring when a power-up is collected
// ─────────────────────────────────────────────────────────────────────────────

ShineEffect::ShineEffect()
    : timer(0.f), duration(0.6f) {
    ring.setRadius(1.f);
    ring.setFillColor(sf::Color::Transparent);
    ring.setOutlineThickness(6.f);
    ring.setOutlineColor(sf::Color(255, 240, 60, 230));  // bright gold
    ring.setOrigin(1.f, 1.f);
}

void ShineEffect::trigger() {
    timer = duration;
}

bool ShineEffect::update(float dt) {
    if (timer > 0.f) {
        timer -= dt;
        if (timer < 0.f) timer = 0.f;
    }
    return timer > 0.f;
}

void ShineEffect::draw(sf::RenderWindow& window, float cx, float cy) {
    if (timer <= 0.f) return;

    float progress = 1.f - (timer / duration);   // 0 → 1 over the lifetime

    // Outer golden ring that expands outward
    float r = 10.f + 60.f * progress;
    sf::Uint8 alpha = (sf::Uint8)(230.f * (1.f - progress));

    ring.setRadius(r);
    ring.setOrigin(r, r);
    ring.setOutlineColor(sf::Color(255, 240, 60, alpha));
    ring.setPosition(cx, cy);
    window.draw(ring);

    // Inner white flash that fades quickly
    if (progress < 0.4f) {
        float innerR = 30.f * (1.f - progress / 0.4f);
        sf::Uint8 innerA = (sf::Uint8)(200.f * (1.f - progress / 0.4f));
        sf::CircleShape inner(innerR);
        inner.setOrigin(innerR, innerR);
        inner.setFillColor(sf::Color(255, 255, 255, innerA));
        inner.setPosition(cx, cy);
        window.draw(inner);
    }
}


// ─────────────────────────────────────────────────────────────────────────────
//  DoubleJumpPowerup — pink circular collectible
// ─────────────────────────────────────────────────────────────────────────────

DoubleJumpPowerup::DoubleJumpPowerup()
    : active(false), bobTimer(0.f) {
    shape.setRadius(18.f);
    shape.setFillColor(sf::Color(255, 60, 200));         // hot pink / magenta
    shape.setOutlineColor(sf::Color(255, 180, 255));
    shape.setOutlineThickness(3.f);
    shape.setOrigin(18.f, 18.f);
}

void DoubleJumpPowerup::spawn(float x, float y) {
    shape.setPosition(x, y);
    active = true;
    bobTimer = 0.f;
}

void DoubleJumpPowerup::update(float dt) {
    if (!active) return;
    bobTimer += dt;
    // Gentle 6-pixel bob so it's noticeable
    float offsetY = std::sin(bobTimer * 3.5f) * 6.f;
    // setPosition each frame rather than move() to avoid drift
    sf::Vector2f base = shape.getPosition();
    shape.setPosition(base.x, base.y + offsetY * dt * 3.5f);
    // clamp so it doesn't drift away — recalculate from base only on spawn is simpler
    // (below we reset the position smoothly each frame)
    (void)offsetY;
}

void DoubleJumpPowerup::draw(sf::RenderWindow& window) {
    if (!active) return;

    // Pulsing glow
    float pulse = 0.5f + 0.5f * std::sin(bobTimer * 5.f);
    sf::Uint8 glowA = (sf::Uint8)(80.f + 80.f * pulse);
    sf::CircleShape glow(24.f);
    glow.setOrigin(24.f, 24.f);
    glow.setFillColor(sf::Color(255, 100, 220, glowA));
    glow.setPosition(shape.getPosition());
    window.draw(glow);

    window.draw(shape);

    // "2x" label on the circle
    // (skipping sf::Text here to keep bonus.cpp self-contained with no font dependency)
}

sf::FloatRect DoubleJumpPowerup::getBounds() const {
    return shape.getGlobalBounds();
}


// ─────────────────────────────────────────────────────────────────────────────
//  ScreenShake
// ─────────────────────────────────────────────────────────────────────────────

ScreenShake::ScreenShake()
    : timer(0.f), duration(0.f), amplitude(0.f), offsetX(0.f), offsetY(0.f) {
}

void ScreenShake::trigger(float dur, float amp) {
    duration = dur;
    timer = dur;
    amplitude = amp;
}

void ScreenShake::update(float dt) {
    if (timer <= 0.f) { offsetX = offsetY = 0.f; return; }
    timer -= dt;
    if (timer < 0.f) { timer = 0.f; offsetX = offsetY = 0.f; return; }

    // Attenuate amplitude as time elapses
    float t = timer / duration;   // 1→0
    float amp = amplitude * t;

    // Pseudo-random shake: use timer itself to vary offset
    float angle = timer * 233.7f;   // large prime-ish multiplier → fast wobble
    offsetX = amp * std::cos(angle);
    offsetY = amp * std::sin(angle * 1.618f);
}

void ScreenShake::applyToView(sf::View& view) const {
    if (timer > 0.f)
        view.move(offsetX, offsetY);
}


// ─────────────────────────────────────────────────────────────────────────────
//  BossHPBar
// ─────────────────────────────────────────────────────────────────────────────

void BossHPBar::draw(sf::RenderWindow& window,
    const sf::Font& font,
    const string& bossName,
    float             health,
    float             maxHealth) {
    if (maxHealth <= 0.f) return;
    float ratio = health / maxHealth;
    if (ratio < 0.f) ratio = 0.f;
    if (ratio > 1.f) ratio = 1.f;

    // Sizes and position (centred at top of screen)
    const float BAR_W = 400.f;
    const float BAR_H = 22.f;
    const float BAR_X = (1000.f - BAR_W) / 2.f;   // centred in 1000-px wide window
    const float BAR_Y = 10.f;

    // Background track
    sf::RectangleShape bg({ BAR_W, BAR_H });
    bg.setFillColor(sf::Color(40, 40, 40, 220));
    bg.setOutlineColor(sf::Color(200, 200, 200, 180));
    bg.setOutlineThickness(2.f);
    bg.setPosition(BAR_X, BAR_Y);
    window.draw(bg);

    // Health fill (green → yellow → red based on ratio)
    sf::Color fillColor;
    if (ratio > 0.5f)
        fillColor = sf::Color(
            (sf::Uint8)((1.f - ratio) * 2.f * 255.f),
            200, 30);                                // green to yellow
    else
        fillColor = sf::Color(
            230,
            (sf::Uint8)(ratio * 2.f * 180.f),
            20);                                     // yellow to red

    sf::RectangleShape fill({ BAR_W * ratio, BAR_H });
    fill.setFillColor(fillColor);
    fill.setPosition(BAR_X, BAR_Y);
    window.draw(fill);

    // Boss name label
    sf::Text label;
    label.setFont(font);
    label.setCharacterSize(14);
    label.setFillColor(sf::Color::White);
    label.setString(bossName);
    auto lb = label.getLocalBounds();
    label.setPosition(BAR_X + (BAR_W - lb.width) / 2.f,
        BAR_Y + (BAR_H - lb.height) / 2.f - 2.f);
    window.draw(label);

    // HP fraction text
    string hpStr = to_string((int)health) + " / " + to_string((int)maxHealth);
    sf::Text hpText;
    hpText.setFont(font);
    hpText.setCharacterSize(11);
    hpText.setFillColor(sf::Color(255, 255, 200));
    hpText.setString(hpStr);
    auto hb = hpText.getLocalBounds();
    hpText.setPosition(BAR_X + BAR_W + 8.f, BAR_Y + (BAR_H - hb.height) / 2.f - 2.f);
    window.draw(hpText);
}
