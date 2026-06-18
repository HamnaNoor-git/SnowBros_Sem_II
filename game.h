#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>
#include "player.h"
#include "snowball.h"
#include "level.h"
#include "botom.h"
#include "menu.h"
#include "leaderboard.h"
#include "collectible.h"
#include <SFML/Audio.hpp>
#include "auth.h"
#include "mode_select.h"   
#include "twoplayer.h"      
#include "settings.h"
#include "bonus.h"

using namespace std;

class Game {
private:
    sf::RenderWindow window;

    sf::Texture bgTexture;
    sf::Texture bgTextureLevel5;
    sf::Texture bgTextureLevel6to9;
    sf::Texture bgTextureLevel10;
    sf::Texture menuBgTexture;
    sf::Sprite menuBgSprite;
    sf::Sprite bgSprite;



    sf::Texture loginBgTexture;
    sf::Sprite loginBgSprite;
    int menuIndex = 0;
    sf::Texture leaderboardBgTexture;
    sf::Sprite leaderboardBgSprite;

    sf::Texture shopBgTexture;
    sf::Sprite shopBgSprite;

    sf::Music bgMusic;

    Player player;
    Snowball snowball;
    Level level;
    Menu menu;

    enum GameState {
        SPLASH,
        LOGIN,
        SIGNUP,
        CHARACTER_SELECT,
        MENU,
        MODE_SELECT,
        LEVEL_SELECT,
        LEVEL_SELECT_2P,
        PLAYING,
        SHOP,
        VICTORY,
        SETTINGS
    };
    int levelIndex;

    sf::Texture splashTexture;
    sf::Sprite splashSprite;

    GameState currentState;

    sf::Clock clock;
    float dt;

    bool spacePressed;
    bool levelChanging;

    sf::Font font;
    sf::Text levelText;
    sf::Text livesText;

    int playerLives;
    float damageCooldown;
    float damageCooldownReset;


    int gems;

    sf::Text scoreText;
    sf::Text gemText;
    sf::Text levelCenterText;
    sf::Text powerUpText;

    float bossHealth;
    float bossMaxHealth;
    sf::RectangleShape bossBar;
    sf::RectangleShape bossBarBg;
    bool isPaused;
    sf::Text pauseText;




    Leaderboard leaderboard;
    bool showLeaderboard;
    int score;

    ModeSelect modeSelect;

    Settings settings;
    Difficulty currentDifficulty;
    float musicVolume;

    sf::Music music;

    int currentMusicID;

    int selectedCharacter;

    Auth auth;
    string currentUser;
    bool isLoggedIn;
    bool isGuest;

    string inputUsername;
    string inputPassword;
    bool typingUsername;
    bool typingPassword;


    int pauseIndex;




    int shopIndex;

    string shopItems[5] = {
        "Extra Life - 50",
        "Speed Boost (30s) - 20",
        "Snowball Power - 30",
        "Distance Increase - 25",
        "Balloon Mode (30s) - 35"
    };

    bool speedBoostActive;
    float speedTimer;

    bool balloonModeActive;
    float balloonTimer;

    bool snowballPower;
    bool maxDistance;

    void handleEvents();
    void update();
    void render();
    void applyDifficultyToLevel();

    string inputPassword2;
    bool typingConfirm;


    string pauseOptions[5] = {
        "Resume",
        "Open Shop",
        "Save Game",
        "Logout",
        "Exit to Main Menu"
    };


    Collectible collectibles[20];
    int collectibleCount;

    sf::Texture collectibleTextures[4];

    float collectibleSpawnTimer;
    float collectibleSpawnDelay;



    sf::Texture starTexture;
    sf::Sprite starSprite;

    bool starActive;
    bool starSpawned;

    float starSpawnTimer;
    float starSpawnDelay;



    bool bonusSpawned;

    int totalBonusItems;
    int collectedBonusItems;
    sf::Texture victoryBgTexture;
    sf::Sprite victoryBgSprite;


    sf::Texture charSelectBgTexture;
    sf::Sprite  charSelectBgSprite;
    sf::Texture charTextures[3];
    sf::Sprite  charSprites[3];
    sf::Texture charSelectTextures[3];

    // ── Bonus systems (bonus.h) ───────────────────────────────────────────────
    ScreenShake       screenShake1P;
    ShineEffect       shineEffect1P;
    DoubleJumpPowerup djPowerup1P;
    bool              djSpawned1P = false;
    bool              doubleJumpActive1P = false;
    float             doubleJumpTimer1P = 0.f;
    bool              usedDoubleJump1P = false;
    bool              gamakichiWasDead1P = false;
    Monkey            monkeys1P[3];
    int               monkeyCount1P = 0;

public:
    Game();
    void run();

    void playMusic(int id);

    Level& getLevel();
    void setStateToPlaying();


};