#pragma once
// =============================================================================
//  bonus.h  —  Extra features for SnowBros project
//
//  Features implemented:
//    1. Monkey enemy     — zig-zag movement, uses monkey.png (same stats as Mogera)
//    2. ShineEffect      — brief white/gold flash drawn on screen when a power-up is collected
//    3. DoubleJumpPowerup — pink circular collectible; grants 10-sec double-jump to the player
//    4. ScreenShake      — camera-shake offset applied when Gamakichi is defeated
//    5. BossHPBar        — HP bar helper for Level 5 (Mogera) and Level 10 (Gamakichi)
//                         used by both single-player (game.cpp) and two-player (twoplayer.cpp)
// =============================================================================

#include <SFML/Graphics.hpp>
#include "enemy.h"
#include <string>
#include <cmath>

using namespace std;

// ─────────────────────────────────────────────────────────────────────────────
//  1. Monkey — zig-zag enemy (same health / damage as Mogera, uses monkey.png)
// ─────────────────────────────────────────────────────────────────────────────
class Monkey : public Enemy {
private:
    static sf::Texture sharedTexture;
    static bool        textureLoaded;

    sf::Sprite  sprite;

    int   health;
    int   maxHealth;

    // Zig-zag movement
    float zigzagTimer;          // tracks time within one zig or zag segment
    float zigzagSegment;        // duration of each segment (seconds)
    int   zigzagDir;            // +1 = moving right, -1 = moving left
    float verticalAmp;          // pixel amplitude of vertical oscillation
    float verticalFreq;         // oscillation frequency (radians/sec)
    float baseY;                // Y anchor for vertical oscillation
    float timeAccum;            // total time for sin() calculation

    // Death animation
    bool  isDying;
    float dyingTimer;

    // Simple 2-frame idle animation
    static const sf::IntRect IDLE_FRAMES[2];
    int   frameIndex;
    float animTimer;

    void applyFrame();

public:
    Monkey();

    // Copyable (no raw pointer members — safe to copy/assign)
    Monkey(const Monkey&) = default;
    Monkey& operator=(const Monkey&) = default;

    // Re-initialise in-place without requiring copy (avoids texture reload)
    void reset(float x, float y);

    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    void takeDamage() override;
    sf::FloatRect getBounds() override;
    void setPosition(float x, float y) override;

    bool isAlive() override;
    int  getHealth()    const { return health; }
    int  getMaxHealth() const { return maxHealth; }
    void setHealth(int h) { health = h; maxHealth = h; }
};

// ─────────────────────────────────────────────────────────────────────────────
//  2. ShineEffect — drawn over the player for ~0.6 s after any power-up pickup
// ─────────────────────────────────────────────────────────────────────────────
class ShineEffect {
private:
    float timer;          // countdown
    float duration;       // total duration (seconds)
    sf::CircleShape ring; // expanding golden ring

public:
    ShineEffect();

    // Call this when a power-up is collected
    void trigger();

    // Call every frame; returns true while still active
    bool update(float dt);

    // Draw centred on (cx, cy)
    void draw(sf::RenderWindow& window, float cx, float cy);

    bool isActive() const { return timer > 0.f; }
};

// ─────────────────────────────────────────────────────────────────────────────
//  3. DoubleJumpPowerup — pink circular collectible that grants double-jump
//     * Spawns as a pink/magenta filled circle on screen
//     * When collected → grants 10 seconds of double-jump to the collector
// ─────────────────────────────────────────────────────────────────────────────
class DoubleJumpPowerup {
private:
    sf::CircleShape shape;
    bool active;
    float bobTimer;     // for a gentle up/down bob animation

public:
    DoubleJumpPowerup();

    void spawn(float x, float y);
    void update(float dt);
    void draw(sf::RenderWindow& window);

    bool isActive() const { return active; }
    void collect() { active = false; }
    sf::FloatRect getBounds() const;
};

// ─────────────────────────────────────────────────────────────────────────────
//  4. ScreenShake — apply a random offset to window.setView() for a duration
//     Usage:
//       ScreenShake shake;
//       // when Gamakichi dies:
//       shake.trigger(0.6f, 10.f);   // duration=0.6s, amplitude=10px
//       // in update():
//       shake.update(dt);
//       // in render(), before drawing anything:
//       sf::View v = window.getDefaultView();
//       shake.applyToView(v);
//       window.setView(v);
//       // after window.display():
//       window.setView(window.getDefaultView());
// ─────────────────────────────────────────────────────────────────────────────
class ScreenShake {
private:
    float timer;
    float duration;
    float amplitude;
    float offsetX;
    float offsetY;

public:
    ScreenShake();

    void trigger(float duration = 0.5f, float amplitude = 12.f);
    void update(float dt);
    void applyToView(sf::View& view) const;
    bool isActive() const { return timer > 0.f; }
};

// ─────────────────────────────────────────────────────────────────────────────
//  5. BossHPBar — renders a labelled health bar for level-5 and level-10 bosses
//     Works for both TwoPlayerGame and single-player Game.
// ─────────────────────────────────────────────────────────────────────────────
class BossHPBar {
public:
    // Draw a boss HP bar at the top-centre of the screen.
    //   bossName  — label ("MOGERA" / "GAMAKICHI")
    //   health    — current HP
    //   maxHealth — max HP
    //   font      — sf::Font already loaded by the caller
    //   window    — render target
    static void draw(sf::RenderWindow& window,
        const sf::Font& font,
        const string& bossName,
        float             health,
        float             maxHealth);
};
