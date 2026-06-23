#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include "level.h"
#include "snowball.h"
#include "collectible.h"
#include "leaderboard.h"
#include "bonus.h"

using namespace std;

//                    
//  TwoPlayerController
//  Handles a single player's sprite, physics, and animation for two-player mode.
//  P1 uses man.png  (character "man"),  P2 uses elsa.png.
//                    
class TwoPlayerController {
public:
    //   Sprite / texture 
    sf::Texture texture;
    sf::Sprite  sprite;

    //   Physics      
    float speed = 200.f;
    float speedMultiplier = 1.f;
    float velocityY = 0.f;
    float gravity = 800.f;
    float jumpPower = -420.f;
    bool  isOnGround = false;
    int   direction = 1;   // 1 = right, -1 = left

    //   Power-up state         
    bool  speedBoostActive = false;
    float speedTimer = 0.f;
    bool  balloonModeActive = false;
    float balloonTimer = 0.f;
    bool  snowballPower = false;
    bool  maxDistance = false;

    //   Double-jump power-up (granted by pink circular collectible)
    bool  doubleJumpActive = false;
    float doubleJumpTimer = 0.f;
    bool  usedDoubleJump = false;  // consumed once per airborne phase

    //   Hit flash     
    float hitFlashTimer = 0.f;

    //   Fallback rect (when texture missing)           
    sf::RectangleShape fallbackRect;
    bool               textureLoaded = false;

    TwoPlayerController();

    bool loadTexture(const string& path, float scaleX, float scaleY,
        sf::Color fallbackColor);

    void draw(sf::RenderWindow& window);
    void setPosition(float x, float y);
    sf::FloatRect getBounds() const;
    float getX() const;
    float getY() const;
};

//                    
//  TwoPlayerGame
//  Full-featured two-player session with parity to single-player:
//    lives, shop, power-ups, all enemies, bosses, collectibles, leaderboard.
//                    
class TwoPlayerGame {
private:
    sf::RenderWindow& window;

    //   Players      
    TwoPlayerController p1;   // man.png   — WASD + Space
    TwoPlayerController p2;   // elsa.png  — Arrows + N

    //   Snowballs     
    Snowball sb1, sb2;
    bool space1Pressed = false;
    bool nKeyPressed = false;

    //   Level       
    Level level;
    int   currentLevel = 1;
    bool  levelChanging = false;

    //   Lives       
    int   p1Lives = 3, p2Lives = 3;
    float damageCooldown1 = 0.f, damageCooldown2 = 0.f;

    //   Score / Gems          
    int p1Score = 0, p2Score = 0;
    int p1Gems = 0, p2Gems = 0;

    //   Collectibles          
    Collectible collectibles[20];
    int         collectibleCount = 0;
    sf::Texture collectibleTextures[4];
    bool        bonusSpawned = false;

    //   Backgrounds    
    sf::Texture bgTex, bgTex5, bgTex6, bgTex10;
    sf::Sprite  bgSprite;

    //   Shop background  
    sf::Texture shopBgTexture;
    sf::Sprite  shopBgSprite;

    //   Font / clock          
    sf::Font  font;
    sf::Clock clock;
    float     dt = 0.f;

    //   Game states    
    bool isPaused = false;
    bool isInShop = false;
    int  pauseIndex = 0;
    int  shopIndex = 0;
    int  shopPlayer = 0;   // 0=P1, 1=P2

    static const int PAUSE_OPTS = 4;
    string pauseOptions[PAUSE_OPTS] = {
        "Resume", "Open Shop", "Restart Level", "Exit to Menu"
    };

    static const int SHOP_ITEMS = 5;
    string shopItems[SHOP_ITEMS] = {
        "Extra Life (Both) - 50 gems",
        "Speed Boost  (30s)- 20 gems",
        "Snowball Power    - 30 gems",
        "Distance Increase - 25 gems",
        "Balloon Mode (30s)- 35 gems"
    };

    //   Music       
    sf::Music music;
    int currentMusicID = -1;

    //   Leaderboard    
    Leaderboard leaderboard;
    bool sessionDone = false;

    //   Bonus systems (bonus.h)
    ScreenShake       screenShake;
    ShineEffect       shineEffect1;   // P1 shine
    ShineEffect       shineEffect2;   // P2 shine
    DoubleJumpPowerup djPowerup;      // pink circular power-up (spawns once per level)
    bool              djSpawned = false;
    Monkey            monkeys[3];
    int               monkeyCount = 0;
    bool              gamakichiWasDead = false;  // track Gamakichi death for screen shake

    //   Victory      
    bool showVictory = false;
    sf::Texture victoryBgTex;
    sf::Sprite  victoryBgSprite;

    //   Post-victory leaderboard    ─
    bool showLeaderboard = false;
    sf::Texture leaderboardBgTex;
    sf::Sprite  leaderboardBgSprite;

    //   Helpers      
    void handleEvents();
    void update();
    void render();
    void renderHUD();
    void renderPause();
    void renderShop();

    void applyPlatformCollision(TwoPlayerController& p);
    void applyEnemyDamage(TwoPlayerController& p, int& lives,
        float& cooldown, int& score, int& gems);
    void applySnowballVsEnemies(Snowball& sb, bool powerEnabled,
        bool useMaxRange, int& score, int& gems);
    void applyCollectible(TwoPlayerController& p, int& score, int& gems);
    void buyShopItem(int item, int player);
    void loadNextLevel();
    void checkLevelComplete();
    void playMusic(int id);
    int  getScoreForEnemy(const string& type) const;

public:
    explicit TwoPlayerGame(sf::RenderWindow& win, int startLevel = 1);
    void run();
};