#include "twoplayer.h"
#include "bonus.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <sstream>

using namespace std;

//  Score table
int TwoPlayerGame::getScoreForEnemy(const string& type) const {
    if (type == "Botom")     return 100 + rand() % 401;
    if (type == "Fooga")     return 200 + rand() % 601;
    if (type == "Tornado")   return 300 + rand() % 901;
    if (type == "Mogera")    return 5000;
    if (type == "Gamakichi") return 10000;
    if (type == "Monkey")    return 300 + rand() % 501;
    return 0;
}

//  TwoPlayerController
TwoPlayerController::TwoPlayerController() {
    fallbackRect.setSize({ 40.f, 60.f });
}

bool TwoPlayerController::loadTexture(const string& path,
    float scaleX, float scaleY,
    sf::Color fallbackColor) {
    fallbackRect.setFillColor(fallbackColor);
    if (!texture.loadFromFile(path)) {
        textureLoaded = false;
        return false;
    }
    sprite.setTexture(texture);
    sprite.setScale(scaleX, scaleY);
    textureLoaded = true;
    return true;
}

void TwoPlayerController::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

sf::FloatRect TwoPlayerController::getBounds() const {
    if (textureLoaded)
        return sprite.getGlobalBounds();
    // fallback rect
    sf::FloatRect fb;
    fb.left = sprite.getPosition().x;
    fb.top = sprite.getPosition().y;
    fb.width = fallbackRect.getSize().x;
    fb.height = fallbackRect.getSize().y;
    return fb;
}

float TwoPlayerController::getX() const {

    return sprite.getGlobalBounds().left;
}
float TwoPlayerController::getY() const {
    return sprite.getGlobalBounds().top;
}

void TwoPlayerController::draw(sf::RenderWindow& window) {
    bool visible = true;
    if (hitFlashTimer > 0.f) {
        float t = hitFlashTimer;
        visible = (int(t * 10.f) % 2 == 0);
    }
    if (!visible) return;

    if (textureLoaded) {
        window.draw(sprite);
    }
    else {
        fallbackRect.setPosition(sprite.getPosition());
        window.draw(fallbackRect);
    }
}

//  Static helpers 
static void handlePlayerInput(TwoPlayerController& p, float dt,
    sf::Keyboard::Key left,
    sf::Keyboard::Key right,
    sf::Keyboard::Key jump) {
    float eff = p.speed * p.speedMultiplier;

    if (sf::Keyboard::isKeyPressed(left)) {
        p.sprite.move(-eff * dt, 0.f);
        p.direction = -1;
        // Flip sprite horizontally
        float sx = std::abs(p.sprite.getScale().x);
        float sy = p.sprite.getScale().y;
        p.sprite.setScale(-sx, sy);
    }
    if (sf::Keyboard::isKeyPressed(right)) {
        p.sprite.move(eff * dt, 0.f);
        p.direction = 1;
        float sx = std::abs(p.sprite.getScale().x);
        float sy = p.sprite.getScale().y;
        p.sprite.setScale(sx, sy);
    }
    // Check jump key pressed this frame (edge detection via static map would be ideal,
    // but since handlePlayerInput has no state, we use velocityY == 0 heuristic)
    static bool jumpHeld1 = false, jumpHeld2 = false;
    bool& jumpHeld = (jump == sf::Keyboard::Space) ? jumpHeld1 : jumpHeld2;
    bool jumpPressed = sf::Keyboard::isKeyPressed(jump);
    bool jumpEdge = jumpPressed && !jumpHeld;
    jumpHeld = jumpPressed;

    float jp = p.balloonModeActive ? p.jumpPower * 0.6f : p.jumpPower;

    if (jumpEdge) {
        if (p.isOnGround) {
            p.velocityY = jp;
            p.isOnGround = false;
            p.usedDoubleJump = false;   // reset double-jump on landing
        }
        else if (p.doubleJumpActive && !p.usedDoubleJump) {
            // Second jump — slightly weaker, only if power-up is active
            p.velocityY = jp * 0.85f;
            p.usedDoubleJump = true;
        }
    }
}

static void updatePlayerPhysics(TwoPlayerController& p, float dt) {
    // Balloon mode: reduced gravity
    float grav = p.balloonModeActive ? p.gravity * 0.35f : p.gravity;
    p.velocityY += grav * dt;
    p.sprite.move(0.f, p.velocityY * dt);

    // Screen bounds
    sf::FloatRect b = p.getBounds();
    // Horizontal bounds 
    float leftEdge = b.left;
    float rightEdge = b.left + b.width;
    if (leftEdge < 0)           p.sprite.move(-leftEdge, 0.f);
    if (rightEdge > 1000.f)     p.sprite.move(1000.f - rightEdge, 0.f);
    // Vertical bounds
    if (b.top < 0) {
        p.sprite.move(0.f, -b.top);
        if (p.velocityY < 0.f) p.velocityY = 0.f;
    }
    // Bottom fallback
    if (b.top > 820.f) {
        p.setPosition(b.left, 600.f);
        p.velocityY = 0.f;
    }

    // Tick timers
    if (p.hitFlashTimer > 0.f) p.hitFlashTimer -= dt;

    if (p.speedBoostActive) {
        p.speedTimer -= dt;
        p.speedMultiplier = 2.f;
        if (p.speedTimer <= 0.f) {
            p.speedBoostActive = false;
            p.speedMultiplier = 1.f;
        }
    }
    if (p.balloonModeActive) {
        p.balloonTimer -= dt;
        if (p.balloonTimer <= 0.f) p.balloonModeActive = false;
    }
    // Double-jump power-up timer
    if (p.doubleJumpActive) {
        p.doubleJumpTimer -= dt;
        if (p.doubleJumpTimer <= 0.f) {
            p.doubleJumpActive = false;
            p.usedDoubleJump = false;
        }
    }
    // Reset usedDoubleJump when player lands
    if (p.isOnGround) p.usedDoubleJump = false;
}

//  TwoPlayerGame constructor
TwoPlayerGame::TwoPlayerGame(sf::RenderWindow& win, int startLevel) : window(win) {
    currentLevel = startLevel;

    font.loadFromFile("arial.ttf");

    //   Backgrounds   
    bgTex.loadFromFile("bg.png");
    bgTex5.loadFromFile("bg5.png");
    bgTex6.loadFromFile("bg6.png");
    bgTex10.loadFromFile("bg10.png");
    bgSprite.setTexture(bgTex);
    bgSprite.setScale(1000.f / bgTex.getSize().x,
        800.f / bgTex.getSize().y);

    //   Shop background        
    shopBgTexture.loadFromFile("shop.png");
    shopBgSprite.setTexture(shopBgTexture);
    if (shopBgTexture.getSize().x > 0) {
        shopBgSprite.setScale(1000.f / shopBgTexture.getSize().x,
            800.f / shopBgTexture.getSize().y);
    }

    //   Collectible textures   
    const char* cPaths[4] = { "c1.png","c2.png","c3.png","star.png" };
    for (int i = 0; i < 4; i++)
        collectibleTextures[i].loadFromFile(cPaths[i]);

    //   Player sprites  
    bool p1ok = p1.loadTexture("man.png", 0.16f, 0.16f, sf::Color(80, 160, 255));
    if (!p1ok) p1.loadTexture("nick.png", 0.15f, 0.15f, sf::Color(80, 160, 255));

    bool p2ok = p2.loadTexture("elsa.png", 0.15f, 0.15f, sf::Color(255, 100, 80));
    if (!p2ok) p2.loadTexture("diago.png", 0.15f, 0.15f, sf::Color(255, 100, 80));

    //   Starting positions    
    p1.setPosition(100.f, 600.f);
    p2.setPosition(800.f, 600.f);

    //   Level             
    level.loadLevel(currentLevel);
    // Set correct background for the starting level immediately
    if (currentLevel == 5)                            bgSprite.setTexture(bgTex5);
    else if (currentLevel >= 6 && currentLevel <= 9)  bgSprite.setTexture(bgTex6);
    else if (currentLevel == 10)                      bgSprite.setTexture(bgTex10);
    else                                              bgSprite.setTexture(bgTex);
    if (bgSprite.getTexture() && bgSprite.getTexture()->getSize().x > 0)
        bgSprite.setScale(1000.f / bgSprite.getTexture()->getSize().x,
            800.f / bgSprite.getTexture()->getSize().y);

    // Bonus: spawn initial Monkey if applicable
    monkeyCount = 0;
    if (currentLevel != 4 && currentLevel != 5 &&
        currentLevel != 9 && currentLevel != 10) {

        monkeys[0].reset(500.f + (float)(rand() % 300), 300.f);
        monkeyCount = 1;
    }

    //   Leaderboard          
    leaderboard.loadFromFile();

    // Victory screen texture
    if (victoryBgTex.loadFromFile("end.png")) {
        victoryBgSprite.setTexture(victoryBgTex);
        victoryBgSprite.setScale(1000.f / victoryBgTex.getSize().x,
            800.f / victoryBgTex.getSize().y);
    }
    if (leaderboardBgTex.loadFromFile("lead.png")) {
        leaderboardBgSprite.setTexture(leaderboardBgTex);
        leaderboardBgSprite.setScale(1000.f / leaderboardBgTex.getSize().x,
            800.f / leaderboardBgTex.getSize().y);
    }
}

//  Music
void TwoPlayerGame::playMusic(int id) {
    if (id == currentMusicID) return;
    currentMusicID = id;
    music.stop();
    switch (id) {
    case 0:  music.openFromFile("splash.ogg");      break;
    case 1:  music.openFromFile("menu.ogg");        break;
    case 2:  music.openFromFile("leaderboard.ogg"); break;
    case 3:  music.openFromFile("level13.ogg");     break;
    case 4:  music.openFromFile("level2.ogg");      break;
    case 5:  music.openFromFile("bonus.ogg");       break;
    case 6:  music.openFromFile("boss.ogg");        break;
    case 7:  music.openFromFile("level6.ogg");      break;
    case 8:  music.openFromFile("level7.ogg");      break;
    case 9:  music.openFromFile("level8.ogg");      break;
    case 10: music.openFromFile("end.ogg");         break;
    default: return;
    }
    music.setLoop(true);
    music.play();

}

//  Run loop
void TwoPlayerGame::run() {
    clock.restart();
    while (window.isOpen() && !sessionDone) {
        dt = clock.restart().asSeconds();
        if (dt > 0.05f) dt = 0.05f;
        handleEvents();
        if (!isPaused && !isInShop && !showVictory && !showLeaderboard) update();
        render();
    }
}

//  Handle Events
void TwoPlayerGame::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {

        if (event.type == sf::Event::Closed) {
            window.close();
            sessionDone = true;
            return;
        }

        if (event.type == sf::Event::KeyPressed) {

            // Victory screen input
            if (showVictory) {
                if (event.key.code == sf::Keyboard::Enter) {
                    showVictory = false;
                    showLeaderboard = true;  // show leaderboard next
                }
                else if (event.key.code == sf::Keyboard::Escape) {
                    sessionDone = true;
                }
                return;
            }
            // Leaderboard screen input
            if (showLeaderboard) {
                if (event.key.code == sf::Keyboard::Enter ||
                    event.key.code == sf::Keyboard::Escape) {
                    sessionDone = true;
                }
                return;
            }

            //   Shop navigation  
            if (isInShop) {
                if (event.key.code == sf::Keyboard::Up) {
                    shopIndex ;
                    if (shopIndex < 0) shopIndex = SHOP_ITEMS - 1;
                }
                if (event.key.code == sf::Keyboard::Down ||
                    event.key.code == sf::Keyboard::S) {
                    shopIndex++;
                    if (shopIndex >= SHOP_ITEMS) shopIndex = 0;
                }
                if (event.key.code == sf::Keyboard::Num1) {
                    shopPlayer = 0;
                    buyShopItem(shopIndex, shopPlayer);
                }
                if (event.key.code == sf::Keyboard::Num2) {
                    shopPlayer = 1;
                    buyShopItem(shopIndex, shopPlayer);
                }
                if (event.key.code == sf::Keyboard::Escape ||
                    event.key.code == sf::Keyboard::P) {
                    isInShop = false;
                    isPaused = false;
                }
                continue;  // eat remaining events while in shop
            }

            //   Pause toggle     -
            if (event.key.code == sf::Keyboard::Escape ||
                event.key.code == sf::Keyboard::P) {
                isPaused = !isPaused;
                if (isPaused) clock.restart();
            }

            //   Pause menu navigation               
            if (isPaused && !isInShop) {
                if (event.key.code == sf::Keyboard::Up) {
                    pauseIndex ;
                    if (pauseIndex < 0) pauseIndex = PAUSE_OPTS - 1;
                }
                if (event.key.code == sf::Keyboard::Down) {
                    pauseIndex++;
                    if (pauseIndex >= PAUSE_OPTS) pauseIndex = 0;
                }
                if (event.key.code == sf::Keyboard::Enter) {
                    if (pauseIndex == 0) {               // Resume
                        isPaused = false;
                    }
                    else if (pauseIndex == 1) {        // Open Shop
                        isInShop = true;
                        shopIndex = 0;
                        shopPlayer = 0;
                    }
                    else if (pauseIndex == 2) {        // Restart Level
                        level.loadLevel(currentLevel);
                        bonusSpawned = false;
                        collectibleCount = 0;
                        currentMusicID = -1;
                        p1.setPosition(100.f, 600.f);
                        p2.setPosition(800.f, 600.f);
                        p1.velocityY = p2.velocityY = 0.f;
                        p1Lives = p2Lives = 3;
                        sb1.deactivate(); sb2.deactivate();
                        isPaused = false;
                    }
                    else {                             // Exit to Menu
                        sessionDone = true;
                    }
                }
            }
        }
    }

    //   Snowball throws           
    if (!isPaused && !isInShop) {
        // P1 – Space
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            if (!space1Pressed && !sb1.isActive()) {
                float range = p1.maxDistance ? 900.f : 600.f;
                sf::FloatRect b1 = p1.getBounds();
                float sx1 = b1.left + b1.width * 0.5f;
                float sy1 = b1.top + b1.height * 0.5f;
                sb1.shoot(sx1, sy1, p1.direction, range);
                sb1.powerBoost = p1.snowballPower;
                space1Pressed = true;
            }
        }
        else {
            space1Pressed = false;
        }

        // P2 – N
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::N)) {
            if (!nKeyPressed && !sb2.isActive()) {
                float range = p2.maxDistance ? 900.f : 600.f;
                sf::FloatRect b2 = p2.getBounds();
                float sx2 = b2.left + b2.width * 0.5f;
                float sy2 = b2.top + b2.height * 0.5f;
                sb2.shoot(sx2, sy2, p2.direction, range);
                sb2.powerBoost = p2.snowballPower;
                nKeyPressed = true;
            }
        }
        else {
            nKeyPressed = false;
        }
    }
}

//  Shop: buy item
void TwoPlayerGame::buyShopItem(int item, int player) {
    int& gems = (player == 0) ? p1Gems : p2Gems;
    int& lives = (player == 0) ? p1Lives : p2Lives;
    TwoPlayerController& p = (player == 0) ? p1 : p2;

    switch (item) {
    case 0:  // Extra Life
        if (gems >= 50) {
            p1Lives++;
            p2Lives++;
            gems -= 50;
        }
        break;
    case 1:  // Speed Boost
        if (gems >= 20) {
            p.speedBoostActive = true;
            p.speedTimer = 30.f;
            gems -= 20;
        }
        break;
    case 2:  // Snowball Power
        if (gems >= 30) {
            p.snowballPower = true;
            gems -= 30;
        }
        break;
    case 3:  // Distance Increase
        if (gems >= 25) {
            p.maxDistance = true;
            gems -= 25;
        }
        break;
    case 4:  // Balloon Mode
        if (gems >= 35) {
            p.balloonModeActive = true;
            p.balloonTimer = 30.f;
            gems -= 35;
        }
        break;
    }
    (void)lives;  // silence unused warning
}

//  Platform collision
void TwoPlayerGame::applyPlatformCollision(TwoPlayerController& p) {
    p.isOnGround = false;
    for (int i = 0; i < level.getPlatformCount(); i++) {
        sf::FloatRect pb = level.getPlatforms()[i].getBounds();
        sf::FloatRect cb = p.getBounds();
        if (cb.intersects(pb) && p.velocityY > 0) {
            p.setPosition(cb.left, pb.top - cb.height);
            p.velocityY = 0.f;
            p.isOnGround = true;
        }
    }
}

//  Enemy damage — players LOSE A LIFE on contact
void TwoPlayerGame::applyEnemyDamage(TwoPlayerController& p, int& lives,
    float& cooldown, int& score, int& gems) {
    cooldown -= dt;
    if (cooldown > 0.f) return;  // still invincible

    sf::FloatRect pb = p.getBounds();

    auto hit = [&]() {
        lives ;
        cooldown = 1.5f;
        p.hitFlashTimer = 1.5f;    // visual flash
        float rx = (&p == &p1) ? 100.f : 800.f;
        p.setPosition(rx, 600.f);
        p.velocityY = 0.f;
        };

    for (int i = 0; i < level.getBotomCount(); i++) {
        Botom& b = level.getBotoms()[i];
        if (b.isAlive() && !b.isFrozen() && !b.isRolling() &&
            pb.intersects(b.getBounds())) {
            hit(); return;
        }
    }

    //   Foogas      
    for (int i = 0; i < level.getFoogaCount(); i++) {
        if (level.getFoogas()[i].isAlive() &&
            pb.intersects(level.getFoogas()[i].getBounds())) {
            hit(); return;
        }
    }

    //   Mogera children  
    for (int i = 0; i < level.getChildCount(); i++) {
        if (level.getChildren()[i].isAlive() &&
            pb.intersects(level.getChildren()[i].getBounds())) {
            hit(); return;
        }
    }

    //   Mogera boss               
    if (level.isMogeraActive() && level.getMogera()->isAlive() &&
        pb.intersects(level.getMogera()->getBounds())) {
        hit(); return;
    }

    //   Tornado bodies         
    for (int i = 0; i < level.getTornadoCount(); i++) {
        if (level.getTornados()[i].isAlive() &&
            pb.intersects(level.getTornados()[i].getBounds())) {
            hit(); return;
        }
    }

    //   Tornado knives         
    for (int i = 0; i < level.getTornadoCount(); i++) {
        if (level.getTornados()[i].isKnifeActive() &&
            pb.intersects(level.getTornados()[i].getKnifeBounds())) {
            hit(); return;
        }
    }

    //   Gamakichi boss (direct contact)                    
    if (level.isGamakichiActive() && level.getGamakichi()->isAlive() &&
        pb.intersects(level.getGamakichi()->getBounds())) {
        hit(); return;
    }

    //   Gamakichi blasts        
    if (level.isGamakichiActive()) {
        Gamakichi* g = level.getGamakichi();
        for (int i = 0; i < 10; i++) {
            if (g->getBlasts()[i].isActive() &&
                g->getBlasts()[i].getBounds().intersects(pb)) {
                hit(); return;
            }
        }
    }

    (void)score; (void)gems;
}

//  Snowball vs enemies
void TwoPlayerGame::applySnowballVsEnemies(Snowball& sb, bool /*powerEnabled*/,
    bool /*useMaxRange*/,
    int& score, int& gems) {
    if (!sb.isActive()) return;
    sf::FloatRect sb_b = sb.getBounds();

    // Botoms
    for (int i = 0; i < level.getBotomCount(); i++) {
        if (level.getBotoms()[i].isAlive() &&
            sb_b.intersects(level.getBotoms()[i].getBounds())) {
            level.getBotoms()[i].takeDamage();
            if (!level.getBotoms()[i].isAlive()) {
                score += getScoreForEnemy("Botom");
                gems += 15;
            }
            sb.deactivate(); return;
        }
    }
    // Foogas
    for (int i = 0; i < level.getFoogaCount(); i++) {
        if (level.getFoogas()[i].isAlive() &&
            sb_b.intersects(level.getFoogas()[i].getBounds())) {
            level.getFoogas()[i].kill();
            score += getScoreForEnemy("Fooga");
            gems += 20;
            sb.deactivate(); return;
        }
    }
    // Mogera children
    for (int i = 0; i < level.getChildCount(); i++) {
        if (level.getChildren()[i].isAlive() &&
            sb_b.intersects(level.getChildren()[i].getBounds())) {
            level.getChildren()[i].takeDamage();
            sb.deactivate(); return;
        }
    }
    // Tornados
    for (int i = 0; i < level.getTornadoCount(); i++) {
        if (level.getTornados()[i].isAlive() &&
            sb_b.intersects(level.getTornados()[i].getBounds())) {
            level.getTornados()[i].kill();
            score += getScoreForEnemy("Tornado");
            gems += 25;
            sb.deactivate(); return;
        }
    }
    // Mogera boss
    if (level.isMogeraActive() && level.getMogera()->isAlive() &&
        sb_b.intersects(level.getMogera()->getBounds())) {
        level.getMogera()->takeDamage();
        if (!level.getMogera()->isAlive()) {
            score += getScoreForEnemy("Mogera");
            gems += 30;
        }
        sb.deactivate(); return;
    }
    // Gamakichi boss
    if (level.isGamakichiActive() && level.getGamakichi()->isAlive() &&
        sb_b.intersects(level.getGamakichi()->getBounds())) {
        level.getGamakichi()->takeDamage();
        if (!level.getGamakichi()->isAlive()) {
            score += getScoreForEnemy("Gamakichi");
            gems += 50;
        }
        sb.deactivate(); return;
    }
    // Gamakichi blasts
    if (level.isGamakichiActive()) {
        Gamakichi* g = level.getGamakichi();
        for (int i = 0; i < 10; i++) {
            if (g->getBlasts()[i].isActive() &&
                sb_b.intersects(g->getBlasts()[i].getBounds())) {
                g->getBlasts()[i].deactivate();
                score += 50;
                sb.deactivate(); return;
            }
        }
    }
}

//  Collectibles
void TwoPlayerGame::applyCollectible(TwoPlayerController& p,
    int& score, int& gems) {
    for (int i = 0; i < collectibleCount; i++) {
        if (collectibles[i].isActive() &&
            p.getBounds().intersects(collectibles[i].getBounds())) {
            collectibles[i].collect();
            score += 100;
            gems += 5;
            for (int j = i; j < collectibleCount - 1; j++)
                collectibles[j] = collectibles[j + 1];
            collectibleCount ;
            i ;
        }
    }
}

//  Level transitions
void TwoPlayerGame::loadNextLevel() {
    currentLevel++;
    if (currentLevel > 10) {
        leaderboard.addScore("P1-2P", p1Score);
        leaderboard.addScore("P2-2P", p2Score);
        leaderboard.saveToFile();
        showVictory = true;
        currentMusicID = -1;  // force music restart
        playMusic(10);        // play end.ogg immediately
        return;
    }
    level.loadLevel(currentLevel);
    bonusSpawned = false;
    collectibleCount = 0;
    levelChanging = false;
    p1.setPosition(100.f, 600.f);
    p2.setPosition(800.f, 600.f);
    p1.velocityY = p2.velocityY = 0.f;
    sb1.deactivate(); sb2.deactivate();

    // Bonus: reset DJ power-up and tracking flags for new level
    djSpawned = false;
    gamakichiWasDead = false;

    // Bonus: spawn 1 Monkey enemy per level (not on boss/bonus levels)
    monkeyCount = 0;
    if (currentLevel != 4 && currentLevel != 5 &&
        currentLevel != 9 && currentLevel != 10) {

        monkeys[0].reset(500.f + (float)(rand() % 300), 300.f);
        monkeyCount = 1;
    }
}

void TwoPlayerGame::checkLevelComplete() {
    bool allCleared = true;

    if (currentLevel == 4 || currentLevel == 9) {
        allCleared = (collectibleCount <= 0);
    }
    else {
        for (int i = 0; i < level.getAllEnemyCount(); i++) {
            if (level.getAllEnemies()[i]->isAlive()) {
                allCleared = false;
                break;
            }
        }
    }

    if (allCleared && !levelChanging) {
        levelChanging = true;
        loadNextLevel();
    }
    if (!allCleared) levelChanging = false;
}

//  Update
void TwoPlayerGame::update() {

    //   Music       
    if (showVictory || showLeaderboard) {
        playMusic(10);  // end.ogg
    }
    else if (!isPaused && !isInShop) {
        int id = 3;
        if (currentLevel == 2)
            id = 4;
        else if (currentLevel == 4 || currentLevel == 9)
            id = 5;  // bonus.ogg
        else if (currentLevel == 5 || currentLevel == 10)
            id = 6;  // boss.ogg
        else if (currentLevel == 6)
            id = 7;  // level6.ogg
        else if (currentLevel == 7)
            id = 8;  // level7.ogg
        else if (currentLevel == 8)
            id = 9;  // level8.ogg
        playMusic(id);
    }

    //   Background           
    if (currentLevel == 5)                 bgSprite.setTexture(bgTex5);
    else if (currentLevel >= 6 && currentLevel <= 9) bgSprite.setTexture(bgTex6);
    else if (currentLevel == 10)           bgSprite.setTexture(bgTex10);
    else                                   bgSprite.setTexture(bgTex);
    bgSprite.setScale(1000.f / bgSprite.getTexture()->getSize().x,
        800.f / bgSprite.getTexture()->getSize().y);

    //   Player input + physics     
    handlePlayerInput(p1, dt, sf::Keyboard::A, sf::Keyboard::D, sf::Keyboard::W);
    handlePlayerInput(p2, dt, sf::Keyboard::Left, sf::Keyboard::Right, sf::Keyboard::Up);

    updatePlayerPhysics(p1, dt);
    updatePlayerPhysics(p2, dt);

    applyPlatformCollision(p1);
    applyPlatformCollision(p2);

    //   Snowballs     
    sb1.update(dt);
    sb2.update(dt);

    applySnowballVsEnemies(sb1, p1.snowballPower, p1.maxDistance, p1Score, p1Gems);
    applySnowballVsEnemies(sb2, p2.snowballPower, p2.maxDistance, p2Score, p2Gems);

    //   Rolling Botom kills others   
    for (int i = 0; i < level.getBotomCount(); i++) {
        if (!level.getBotoms()[i].isAlive() || !level.getBotoms()[i].isRolling())
            continue;
        sf::FloatRect roller = level.getBotoms()[i].getBounds();
        for (int j = 0; j < level.getBotomCount(); j++) {
            if (i != j && level.getBotoms()[j].isAlive() &&
                roller.intersects(level.getBotoms()[j].getBounds())) {
                level.getBotoms()[j].kill();
                int pts = getScoreForEnemy("Botom") / 2;
                p1Score += pts;
                p2Score += pts;
            }
        }
    }

    //   Enemy updates   
    for (int i = 0; i < level.getAllEnemyCount(); i++)
        if (level.getAllEnemies()[i]->isAlive())
            level.getAllEnemies()[i]->update(dt);

    // Gamakichi targets closer player
    if (level.isGamakichiActive()) {
        float d1 = std::abs(p1.getX() - level.getGamakichi()->getBounds().left);
        float d2 = std::abs(p2.getX() - level.getGamakichi()->getBounds().left);
        float px = (d1 < d2) ? p1.getX() : p2.getX();
        float py = (d1 < d2) ? p1.getY() : p2.getY();
        level.getGamakichi()->update(dt, px, py,
            level.getPlatforms(),
            level.getPlatformCount());
    }

    // Tornados track closer player
    for (int i = 0; i < level.getTornadoCount(); i++) {
        if (level.getTornados()[i].isAlive()) {
            float d1 = std::abs(p1.getX() - level.getTornados()[i].getBounds().left);
            float d2 = std::abs(p2.getX() - level.getTornados()[i].getBounds().left);
            float tx = (d1 < d2) ? p1.getX() : p2.getX();
            float ty = (d1 < d2) ? p1.getY() : p2.getY();
            level.getTornados()[i].update(dt, tx, ty);
        }
    }

    //   Platform collision for enemies                     
    for (int i = 0; i < level.getBotomCount(); i++) {
        level.getBotoms()[i].setOnGround(false);
        for (int j = 0; j < level.getPlatformCount(); j++) {
            sf::FloatRect b = level.getBotoms()[i].getBounds();
            sf::FloatRect pl = level.getPlatforms()[j].getBounds();
            if (b.intersects(pl)) {
                float bot = b.top + b.height;
                float prevBot = bot - level.getBotoms()[i].getVelocityY() * dt;
                if (prevBot <= pl.top) {
                    level.getBotoms()[i].setPosition(b.left, pl.top - b.height);
                    level.getBotoms()[i].setVelocityY(0);
                    level.getBotoms()[i].setOnGround(true);
                }
            }
        }
    }
    for (int i = 0; i < level.getFoogaCount(); i++) {
        level.getFoogas()[i].setOnGround(false);
        for (int j = 0; j < level.getPlatformCount(); j++) {
            sf::FloatRect f = level.getFoogas()[i].getBounds();
            sf::FloatRect pl = level.getPlatforms()[j].getBounds();
            if (f.intersects(pl)) {
                float bot = f.top + f.height;
                float prevBot = bot - level.getFoogas()[i].getVelocityY() * dt;
                if (prevBot <= pl.top) {
                    level.getFoogas()[i].setPosition(f.left + f.width * 0.5f, pl.top);
                    level.getFoogas()[i].setVelocityY(0);
                    level.getFoogas()[i].setOnGround(true);
                }
            }
        }
    }

    //   Kick frozen botoms       
    for (int i = 0; i < level.getBotomCount(); i++) {
        Botom& b = level.getBotoms()[i];
        if (b.isAlive() && b.isFrozen()) {
            if (p1.getBounds().intersects(b.getBounds())) b.kick(p1.direction);
            if (p2.getBounds().intersects(b.getBounds())) b.kick(p2.direction);
        }
    }

    //   Mogera child spawn       
    if (level.isMogeraActive() && level.getMogera()->isAlive()) {
        if (level.getMogera()->shouldSpawnChild()) {
            for (int k = 0; k < 2 && level.getChildCount() < 20; k++) {
                MogeraChild& c = level.getChildren()[level.getChildCount()];
                float mx = level.getMogera()->getBounds().left;
                float my = level.getMogera()->getBounds().top;
                c.setPosition(mx, my);
                float avgX = (p1.getX() + p2.getX()) / 2.f;
                c.setDirection(avgX < mx ? -1 : 1);
                level.incrementChildCount();
                level.addEnemy(&c);
            }
        }
    }

    //   Bonus collectibles (levels 4 and 9)            
    if (currentLevel == 4 || currentLevel == 9) {
        if (!bonusSpawned) {
            collectibleCount = 0;
            float pos[15][2] = {
                {200,650},{400,650},{600,650},{800,650},
                {200,450},{400,450},{600,450},{800,450},
                {300,250},{500,250},
                {350,550},{650,550},{350,350},{650,350},{500,150}
            };
            for (int i = 0; i < 10; i++) {
                collectibles[collectibleCount].spawn(
                    &collectibleTextures[0], pos[i][0], pos[i][1]);
                collectibleCount++;
            }
            for (int i = 10; i < 15; i++) {
                collectibles[collectibleCount].spawn(
                    &collectibleTextures[1], pos[i][0], pos[i][1]);
                collectibleCount++;
            }
            bonusSpawned = true;
        }
    }

    //   Enemy damage to players    -
    applyEnemyDamage(p1, p1Lives, damageCooldown1, p1Score, p1Gems);
    applyEnemyDamage(p2, p2Lives, damageCooldown2, p2Score, p2Gems);

    //   Collectible pickup
    {
        int prev1 = p1Score, prev2 = p2Score;
        applyCollectible(p1, p1Score, p1Gems);
        applyCollectible(p2, p2Score, p2Gems);
        if (p1Score != prev1) shineEffect1.trigger();  // Bonus: shine on collectible
        if (p2Score != prev2) shineEffect2.trigger();  // Bonus: shine on collectible
    }

    //   Bonus: Double-jump pink power-up spawn + pickup  
    if (!djSpawned && currentLevel != 4 && currentLevel != 9) {
        float spawnX = 200.f + (float)(rand() % 600);
        float spawnY = 550.f;
        djPowerup.spawn(spawnX, spawnY);
        djSpawned = true;
    }
    djPowerup.update(dt);

    if (djPowerup.isActive()) {
        if (p1.getBounds().intersects(djPowerup.getBounds())) {
            djPowerup.collect();
            p1.doubleJumpActive = true;
            p1.doubleJumpTimer = 10.f;
            p1.usedDoubleJump = false;
            shineEffect1.trigger();     // visual shine for P1
        }
        if (djPowerup.isActive() &&
            p2.getBounds().intersects(djPowerup.getBounds())) {
            djPowerup.collect();
            p2.doubleJumpActive = true;
            p2.doubleJumpTimer = 10.f;
            p2.usedDoubleJump = false;
            shineEffect2.trigger();     // visual shine for P2
        }
    }

    //   Bonus: Shine effect update   
    shineEffect1.update(dt);
    shineEffect2.update(dt);

    //   Bonus: Screen shake update   
    screenShake.update(dt);

    //   Bonus: Detect Gamakichi death → trigger screen shake          
    if (level.isGamakichiActive()) {
        bool nowDead = !level.getGamakichi()->isAlive();
        if (nowDead && !gamakichiWasDead) {
            screenShake.trigger(0.65f, 14.f);   // shake: 0.65 s, 14 px amplitude
            gamakichiWasDead = true;
        }
    }
    else {
        gamakichiWasDead = false;  // reset for next playthrough
    }

    //   Bonus: Monkey enemy update   
    for (int i = 0; i < monkeyCount; i++) {
        if (monkeys[i].isAlive())
            monkeys[i].update(dt);
    }

    // Monkey vs players (damage)
    for (int i = 0; i < monkeyCount; i++) {
        if (!monkeys[i].isAlive()) continue;
        sf::FloatRect mb = monkeys[i].getBounds();
        if (damageCooldown1 <= 0.f && p1.getBounds().intersects(mb)) {
            p1Lives ;
            damageCooldown1 = 2.f;
            p1.hitFlashTimer = 1.f;
        }
        if (damageCooldown2 <= 0.f && p2.getBounds().intersects(mb)) {
            p2Lives ;
            damageCooldown2 = 2.f;
            p2.hitFlashTimer = 1.f;
        }
    }
    // Monkey vs snowballs
    for (int i = 0; i < monkeyCount; i++) {
        if (!monkeys[i].isAlive()) continue;
        if (sb1.isActive() && sb1.getBounds().intersects(monkeys[i].getBounds())) {
            monkeys[i].takeDamage();
            if (!monkeys[i].isAlive()) { p1Score += getScoreForEnemy("Monkey"); p1Gems += 3; }
            sb1.deactivate();
        }
        if (sb2.isActive() && sb2.getBounds().intersects(monkeys[i].getBounds())) {
            monkeys[i].takeDamage();
            if (!monkeys[i].isAlive()) { p2Score += getScoreForEnemy("Monkey"); p2Gems += 3; }
            sb2.deactivate();
        }
    }


    if (p1Lives <= 0) {
        p1Lives = 3;
        p1Score = 0;
        p1Gems = 0;
        // Reload level 1 for both (shared world resets)
        currentLevel = 1;
        level.loadLevel(1);
        bonusSpawned = false;
        collectibleCount = 0;
        levelChanging = false;
        currentMusicID = -1;

        p1.setPosition(100.f, 600.f);
        p2.setPosition(800.f, 600.f);
        p1.velocityY = p2.velocityY = 0.f;
        sb1.deactivate(); sb2.deactivate();
        damageCooldown1 = 2.f;  // brief invincibility
        return;
    }
    // P2 ran out of lives: respawn at level 1 with 3 lives
    if (p2Lives <= 0) {
        p2Lives = 3;
        p2Score = 0;
        p2Gems = 0;
        currentLevel = 1;
        level.loadLevel(1);
        bonusSpawned = false;
        collectibleCount = 0;
        levelChanging = false;
        currentMusicID = -1;  // force music to restart for level 1
        p1.setPosition(100.f, 600.f);
        p2.setPosition(800.f, 600.f);
        p1.velocityY = p2.velocityY = 0.f;
        sb1.deactivate(); sb2.deactivate();
        damageCooldown2 = 2.f;  // brief invincibility
        return;
    }

    //   Level complete         
    checkLevelComplete();
}

//  Render
void TwoPlayerGame::render() {
    window.clear();

    // Bonus: apply screen shake offset
    if (screenShake.isActive()) {
        sf::View v = window.getDefaultView();
        screenShake.applyToView(v);
        window.setView(v);
    }

    // Leaderboard screen (post-victory)
    if (showLeaderboard) {
        if (leaderboardBgTex.getSize().x > 0)
            window.draw(leaderboardBgSprite);
        else
            window.clear(sf::Color(5, 18, 5));
        leaderboard.draw(window, font);
        // hint
        sf::Text hint2;
        hint2.setFont(font);
        hint2.setCharacterSize(16);
        hint2.setFillColor(sf::Color(62, 207, 90, 200));
        hint2.setString("Press ENTER or ESC to exit");
        auto hb2 = hint2.getLocalBounds();
        hint2.setOrigin(hb2.left + hb2.width / 2.f, hb2.top);
        hint2.setPosition(500.f, 760.f);
        window.draw(hint2);
        window.display();
        return;
    }

    // Victory screen
    if (showVictory) {
        if (victoryBgTex.getSize().x > 0)
            window.draw(victoryBgSprite);
        else
            window.clear(sf::Color(10, 30, 10));


        window.display();
        return;
    }

    // Background
    window.draw(bgSprite);

    // Platforms
    for (int i = 0; i < level.getPlatformCount(); i++)
        level.getPlatforms()[i].draw(window);

    // Enemies
    for (int i = 0; i < level.getAllEnemyCount(); i++)
        if (level.getAllEnemies()[i]->isAlive())
            level.getAllEnemies()[i]->draw(window);

    // Collectibles
    for (int i = 0; i < collectibleCount; i++)
        collectibles[i].draw(window);

    // Snowballs
    sb1.draw(window);
    sb2.draw(window);

    // Players
    p1.draw(window);
    p2.draw(window);

    // Bonus: draw monkey enemies
    for (int i = 0; i < monkeyCount; i++)
        monkeys[i].draw(window);

    // Bonus: draw double-jump power-up (pink circle)
    djPowerup.draw(window);

    // Bonus: draw shine effects centred on each player
    if (shineEffect1.isActive())
        shineEffect1.draw(window,
            p1.getBounds().left + p1.getBounds().width * 0.5f,
            p1.getBounds().top + p1.getBounds().height * 0.5f);
    if (shineEffect2.isActive())
        shineEffect2.draw(window,
            p2.getBounds().left + p2.getBounds().width * 0.5f,
            p2.getBounds().top + p2.getBounds().height * 0.5f);

    // Level 5 boss — Mogera HP bar (2-player)
    if (level.isMogeraActive() && level.getMogera()->isAlive()) {
        BossHPBar::draw(window, font, "MOGERA",
            (float)level.getMogera()->getHealth(),
            (float)level.getMogera()->getMaxHealth());
    }

    // Level 10 boss — Gamakichi HP bar (2-player)
    if (level.isGamakichiActive() && level.getGamakichi()->isAlive()) {
        BossHPBar::draw(window, font, "GAMAKICHI",
            (float)level.getGamakichi()->getHealth(),
            (float)level.getGamakichi()->getMaxHealth());
    }

    renderHUD();

    if (isInShop) { renderShop();  window.display(); window.setView(window.getDefaultView()); return; }
    if (isPaused) renderPause();
    window.display();
    window.setView(window.getDefaultView());  // restore after screen shake
}

//  HUD
void TwoPlayerGame::renderHUD() {
    const float WW = 1000.f, HH = 800.f;
    const float HUD_H = 60.f;

    //   Top HUD bar background    
    sf::RectangleShape hudBg({ WW, HUD_H });
    hudBg.setPosition(0.f, 0.f);
    hudBg.setFillColor(sf::Color(0, 0, 0, 175));
    window.draw(hudBg);

    // Subtle divider line at bottom of HUD
    sf::RectangleShape divider({ WW, 2.f });
    divider.setPosition(0.f, HUD_H - 2.f);
    divider.setFillColor(sf::Color(255, 255, 255, 40));
    window.draw(divider);

    // Helper: draw text
    auto txt = [&](const string& s, float x, float y,
        sf::Color col, int sz = 15) {
            sf::Text t;
            t.setFont(font);
            t.setCharacterSize(sz);
            t.setFillColor(col);
            t.setString(s);
            t.setPosition(x, y);
            window.draw(t);
        };

    // Helper: right-aligned text
    auto txtR = [&](const string& s, float rightX, float y,
        sf::Color col, int sz = 15) {
            sf::Text t;
            t.setFont(font);
            t.setCharacterSize(sz);
            t.setFillColor(col);
            t.setString(s);
            auto b = t.getLocalBounds();
            t.setPosition(rightX - b.width - b.left, y);
            window.draw(t);
        };

    auto hearts = [](int n) -> string {
        string s;
        for (int i = 0; i < n && i < 5; i++) s += "<3 ";
        if (n <= 0) s = "OUT";
        return s;
        };

    //   LEFT SIDE: P1 lives + level           
    // Section label
    txt("P1", 10.f, 5.f, sf::Color(130, 220, 255, 180), 11);

    // Lives (hearts) — large, prominent
    string p1hearts = hearts(p1Lives);
    txt(p1hearts, 10.f, 18.f, sf::Color(255, 80, 80), 18);

    // Level below lives
    txt("LVL " + to_string(currentLevel), 10.f, 40.f, sf::Color(255, 230, 80), 13);

    //   RIGHT SIDE: P1 gems + power + score       
    // Build power string
    string p1powers = "";
    if (p1.speedBoostActive)  p1powers += "SPD ";
    if (p1.balloonModeActive) p1powers += "BAL ";
    if (p1.snowballPower)     p1powers += "PWR ";
    if (p1.maxDistance)       p1powers += "RNG ";

    float rightEdge1 = WW * 0.42f;  // P1 info ends at ~42% from left

    // Score
    txtR("SC: " + to_string(p1Score), rightEdge1, 5.f, sf::Color(255, 255, 255), 14);
    // Gems
    txtR("Gems: " + to_string(p1Gems), rightEdge1, 25.f, sf::Color(100, 220, 255), 14);
    // Power-ups
    if (!p1powers.empty())
        txtR(p1powers, rightEdge1, 43.f, sf::Color(180, 255, 120), 11);

    //   CENTRE DIVIDER        
    sf::RectangleShape vdiv({ 2.f, HUD_H - 10.f });
    vdiv.setPosition(WW / 2.f - 1.f, 5.f);
    vdiv.setFillColor(sf::Color(255, 255, 255, 50));
    window.draw(vdiv);

    //   RIGHT HALF: P2 section    
    float leftEdge2 = WW * 0.58f;

    // P2 lives — left of right section
    txt("P2", leftEdge2, 5.f, sf::Color(255, 160, 130, 180), 11);
    txt(hearts(p2Lives), leftEdge2, 18.f, sf::Color(255, 80, 80), 18);
    // No need to repeat level for P2, show "CO-OP" label
    txt("CO-OP", leftEdge2, 40.f, sf::Color(200, 200, 200, 140), 11);

    // P2 score/gems/powers on extreme right
    string p2powers = "";
    if (p2.speedBoostActive)  p2powers += "SPD ";
    if (p2.balloonModeActive) p2powers += "BAL ";
    if (p2.snowballPower)     p2powers += "PWR ";
    if (p2.maxDistance)       p2powers += "RNG ";

    txtR("SC: " + to_string(p2Score), WW - 8.f, 5.f, sf::Color(255, 255, 255), 14);
    txtR("Gems: " + to_string(p2Gems), WW - 8.f, 25.f, sf::Color(255, 180, 100), 14);
    if (!p2powers.empty())
        txtR(p2powers, WW - 8.f, 43.f, sf::Color(180, 255, 120), 11);

    //   Dead notices         
    if (p1Lives <= 0)
        txt("P1 OUT!", WW / 2.f - 150.f, HH / 2.f - 20.f, sf::Color::Red, 36);
    if (p2Lives <= 0)
        txt("P2 OUT!", WW / 2.f + 20.f, HH / 2.f - 20.f, sf::Color(255, 80, 0), 36);

    // Bottom controls hint (small, unobtrusive)
    sf::RectangleShape hintBg({ WW, 18.f });
    hintBg.setPosition(0.f, HH - 18.f);
    hintBg.setFillColor(sf::Color(0, 0, 0, 130));
    window.draw(hintBg);
    txt("ESC=Pause  WASD+Space=P1  Arrows+N=P2",
        WW / 2.f - 185.f, HH - 17.f, sf::Color(90, 90, 90), 11);
}

//  Pause overlay
void TwoPlayerGame::renderPause() {
    //   DARK OVERLAY         
    sf::RectangleShape overlay(sf::Vector2f(1000, 800));
    overlay.setFillColor(sf::Color(0, 10, 30, 180));
    window.draw(overlay);

    //   PANEL                   
    float PW = 420, PH = 380;
    float PX = 500 - PW / 2.f;
    float PY = 400 - PH / 2.f;

    sf::RectangleShape panel(sf::Vector2f(PW, PH));
    panel.setPosition(PX, PY);
    panel.setFillColor(sf::Color(10, 20, 50, 220));
    panel.setOutlineThickness(2);
    panel.setOutlineColor(sf::Color(80, 180, 255));
    window.draw(panel);

    //   CORNER ACCENTS        
    auto corner = [&](float x, float y) {
        sf::RectangleShape h(sf::Vector2f(30, 2));
        h.setPosition(x, y);
        h.setFillColor(sf::Color(80, 180, 255));
        window.draw(h);
        sf::RectangleShape v(sf::Vector2f(2, 30));
        v.setPosition(x, y);
        v.setFillColor(sf::Color(80, 180, 255));
        window.draw(v);
        };

    corner(PX, PY);
    corner(PX + PW - 30, PY);
    corner(PX, PY + PH - 30);
    corner(PX + PW - 30, PY + PH - 30);

    //   TITLE (with glow)             
    sf::Text title;
    title.setFont(font);
    title.setCharacterSize(40);
    title.setString("PAUSED");

    sf::FloatRect tb = title.getLocalBounds();
    title.setOrigin(tb.left + tb.width / 2.f, tb.top);
    title.setPosition(500, PY + 30);

    sf::Text glow = title;
    glow.setFillColor(sf::Color(80, 180, 255, 80));
    glow.move(2, 2);
    window.draw(glow);

    title.setFillColor(sf::Color::White);
    window.draw(title);

    //   OPTIONS                  
    for (int i = 0; i < PAUSE_OPTS; i++) {
        float y = PY + 100 + i * 50;

        sf::Text option;
        option.setFont(font);
        option.setCharacterSize(24);
        option.setString(pauseOptions[i]);

        sf::FloatRect ob = option.getLocalBounds();
        option.setOrigin(ob.left + ob.width / 2.f, ob.top);
        option.setPosition(500, y);

        if (i == pauseIndex) {
            sf::RectangleShape highlight(sf::Vector2f(320, 40));
            highlight.setPosition(500 - 160, y - 5);
            highlight.setFillColor(sf::Color(80, 180, 255, 40));
            highlight.setOutlineThickness(1);
            highlight.setOutlineColor(sf::Color(80, 180, 255));
            window.draw(highlight);
            option.setFillColor(sf::Color::White);
        }
        else {
            option.setFillColor(sf::Color(120, 160, 220));
        }

        window.draw(option);
    }

    //   HINT             
    sf::Text hint;
    hint.setFont(font);
    hint.setCharacterSize(13);
    hint.setFillColor(sf::Color(60, 120, 180));
    hint.setString("UP/DOWN navigate  |  ENTER select");
    sf::FloatRect hb = hint.getLocalBounds();
    hint.setOrigin(hb.left + hb.width / 2.f, hb.top);
    hint.setPosition(500, PY + PH - 28.f);
    window.draw(hint);
}

//  Shop overlay
void TwoPlayerGame::renderShop() {
    // Background
    if (shopBgTexture.getSize().x > 0)
        window.draw(shopBgSprite);


    // Soft overlay
    sf::RectangleShape ov({ 1000.f,800.f });
    ov.setFillColor(sf::Color(20, 10, 5, 150));
    window.draw(ov);

    // Panel
    const float PW = 520.f, PH = 520.f;
    const float PX = 500.f - PW / 2.f, PY = 400.f - PH / 2.f;
    sf::RectangleShape panel({ PW,PH });
    panel.setPosition(PX, PY);
    panel.setFillColor(sf::Color(40, 25, 15, 230));
    panel.setOutlineThickness(2);
    panel.setOutlineColor(sf::Color(139, 94, 60));
    window.draw(panel);

    // Title
    sf::Text title;
    title.setFont(font); title.setCharacterSize(36);
    title.setFillColor(sf::Color(255, 240, 210));
    title.setString("TWO-PLAYER SHOP");
    auto tb = title.getLocalBounds();
    title.setOrigin(tb.left + tb.width / 2.f, tb.top);
    title.setPosition(500.f, PY + 20.f);
    window.draw(title);

    // Gems display
    sf::Text gemsT;
    gemsT.setFont(font); gemsT.setCharacterSize(19);
    gemsT.setFillColor(sf::Color(205, 170, 125));
    gemsT.setString("P1 Gems: " + to_string(p1Gems) +
        "   |   P2 Gems: " + to_string(p2Gems));
    gemsT.setPosition(PX + 20.f, PY + 72.f);
    window.draw(gemsT);

    // Items
    for (int i = 0; i < SHOP_ITEMS; i++) {
        float y = PY + 130.f + i * 60.f;
        sf::Text item;
        item.setFont(font); item.setCharacterSize(22);
        item.setString(shopItems[i]);
        auto ob = item.getLocalBounds();
        item.setOrigin(ob.left + ob.width / 2.f, ob.top);
        item.setPosition(500.f, y);

        if (i == shopIndex) {
            sf::RectangleShape hl({ 380.f, 46.f });
            hl.setPosition(500.f - 190.f, y - 8.f);
            hl.setFillColor(sf::Color(255, 220, 170, 40));
            hl.setOutlineThickness(1);
            hl.setOutlineColor(sf::Color(205, 170, 125));
            window.draw(hl);
            item.setFillColor(sf::Color(255, 240, 210));
        }
        else {
            item.setFillColor(sf::Color(180, 140, 100));
        }
        window.draw(item);
    }

    // Instructions
    sf::Text inst;
    inst.setFont(font); inst.setCharacterSize(15);
    inst.setFillColor(sf::Color(150, 120, 80));
    inst.setString("UP/DOWN: navigate   [1] P1 buys   [2] P2 buys   ESC: close");
    auto ib = inst.getLocalBounds();
    inst.setOrigin(ib.left + ib.width / 2.f, ib.top);
    inst.setPosition(500.f, PY + PH - 35.f);
    window.draw(inst);
}