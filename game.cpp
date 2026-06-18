#include "game.h"
#include "bonus.h"



int getScoreForEnemy(string type) {
    if (type == "Botom")
        return 100 + rand() % 401;

    if (type == "Fooga")
        return 200 + rand() % 601;

    if (type == "Tornado")
        return 300 + rand() % 901;

    if (type == "Mogera")
        return 5000;

    if (type == "Gamakichi")
        return 10000;

    if (type == "Monkey")
        return 300 + rand() % 501;   // Monkey: same score range as Tornado


    return 0;
}

Game::Game() : window(sf::VideoMode(1000, 800), "Snow Bros") {

    menuBgTexture.loadFromFile("menu.png");

    menuBgSprite.setTexture(menuBgTexture);
    menuBgSprite.setScale(1000.0f / menuBgTexture.getSize().x, 800.0f / menuBgTexture.getSize().y);

    bgTexture.loadFromFile("bg.png");

    bgSprite.setTexture(bgTexture);
    bgSprite.setScale(1000.0f / bgTexture.getSize().x, 800.0f / bgTexture.getSize().y
    );

    bgTextureLevel5.loadFromFile("bg5.png");
    bgTextureLevel6to9.loadFromFile("bg6.png");
    bgTextureLevel10.loadFromFile("bg10.png");

    font.loadFromFile("arial.ttf");

    levelText.setFont(font);
    levelText.setCharacterSize(13);
    levelText.setPosition(10, 40);
    levelText.setFillColor(sf::Color(255, 230, 80));

    livesText.setFont(font);
    livesText.setCharacterSize(18);
    livesText.setPosition(10, 18);
    livesText.setFillColor(sf::Color(255, 80, 80));

    level.loadLevel(1);
    bonusSpawned = false;
    playerLives = 2;
    damageCooldown = 0;
    damageCooldownReset = 1.0f;

    spacePressed = false;
    levelChanging = false;


    typingConfirm = false;
    showLeaderboard = false;
    leaderboard.loadFromFile();
    score = 0;

    selectedCharacter = 0;


    isLoggedIn = false;
    isGuest = false;

    typingUsername = true;
    typingPassword = false;

    isPaused = false;
    pauseIndex = 0;

    currentDifficulty = Difficulty::NORMAL;
    musicVolume = 100.f;

    menuIndex = 0;
    shopIndex = 0;
    gems = 0;

    speedBoostActive = false;
    speedTimer = 0;

    balloonModeActive = false;
    balloonTimer = 0;

    snowballPower = false;
    maxDistance = false;


    loginBgTexture.loadFromFile("login.png");
    loginBgSprite.setTexture(loginBgTexture);
    loginBgSprite.setScale(
        1000.0f / loginBgTexture.getSize().x,
        800.0f / loginBgTexture.getSize().y
    );


    totalBonusItems = 0;
    collectedBonusItems = 0;


    leaderboardBgTexture.loadFromFile("lead.png");
    leaderboardBgSprite.setTexture(leaderboardBgTexture);

    leaderboardBgSprite.setScale(
        1000.0f / leaderboardBgTexture.getSize().x,
        800.0f / leaderboardBgTexture.getSize().y
    );


    shopBgTexture.loadFromFile("shop.png");
    shopBgSprite.setTexture(shopBgTexture);

    shopBgSprite.setScale(
        1000.0f / shopBgTexture.getSize().x,
        800.0f / shopBgTexture.getSize().y
    );


    splashTexture.loadFromFile("splash.png");
    splashSprite.setTexture(splashTexture);

    splashSprite.setScale(
        1000.0f / splashTexture.getSize().x,
        800.0f / splashTexture.getSize().y
    );


    levelIndex = 0;
    currentState = SPLASH;



    collectibleTextures[0].loadFromFile("c1.png");
    collectibleTextures[1].loadFromFile("c2.png");
    collectibleTextures[2].loadFromFile("c3.png");
    collectibleTextures[3].loadFromFile("c4.png");

    collectibleSpawnTimer = 0;
    collectibleSpawnDelay = 2 + rand() % 3;
    collectibleCount = 0;


    currentUser = "Guest";


    starTexture.loadFromFile("star.png");
    starSprite.setTexture(starTexture);

    starSprite.setScale(0.15f, 0.15f);

    starActive = false;
    starSpawnTimer = 0;
    starSpawnDelay = 6 + rand() % 5;
    starSpawned = false;

    victoryBgTexture.loadFromFile("end.png");
    victoryBgSprite.setTexture(victoryBgTexture);

    victoryBgSprite.setScale(
        1000.0f / victoryBgTexture.getSize().x,
        800.0f / victoryBgTexture.getSize().y
    );


    charSelectBgTexture.loadFromFile("charr.jpeg");
    charSelectBgSprite.setTexture(charSelectBgTexture);
    charSelectBgSprite.setScale(
        1000.0f / charSelectBgTexture.getSize().x,
        800.0f / charSelectBgTexture.getSize().y
    );

    charTextures[0].loadFromFile("nick.png");
    charTextures[1].loadFromFile("elsa.png");
    charTextures[2].loadFromFile("man.png");



    charSelectTextures[0].loadFromFile("nicl.png");
    charSelectTextures[1].loadFromFile("elsa.png");
    charSelectTextures[2].loadFromFile("man.png");


    for (int i = 0; i < 3; i++) {

        charSprites[i].setTexture(charSelectTextures[i]);

        sf::Vector2u sz = charSelectTextures[i].getSize();

        float targetHeight = 200.f;

        float scale = targetHeight / sz.y;

        if (i == 0) {
            scale *= 1.15f;
        }
        if (i == 1) {
            scale *= 1.15f;
        }
        charSprites[i].setScale(scale, scale);
    }

    currentMusicID = -1;
}


void Game::playMusic(int id) {

    if (currentMusicID == id) return;

    currentMusicID = id;

    music.stop();

    switch (id) {

    case 0:
        music.openFromFile("splash.ogg");
        break;

    case 1:
        music.openFromFile("menu.ogg");
        break;

    case 2:
        music.openFromFile("leaderboard.ogg");
        break;

    case 3:
        music.openFromFile("level13.ogg");
        break;

    case 4:
        music.openFromFile("level2.ogg");
        break;

    case 5:
        music.openFromFile("bonus.ogg");
        break;

    case 6:
        music.openFromFile("boss.ogg");
        break;

    case 7:
        music.openFromFile("level6.ogg");
        break;

    case 8:
        music.openFromFile("level7.ogg");
        break;

    case 9:
        music.openFromFile("level8.ogg");
        break;

    case 10:
        music.openFromFile("end.ogg");
        break;
    }

    music.setLoop(true);
    music.play();
}


// RUN
void Game::run() {
    while (window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}

// EVENTS
void Game::handleEvents() {

    sf::Event event;

    if (currentState == SPLASH) {

        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Enter) {

                currentState = LOGIN;
            }
        }

        return;
    }

    // 🔐 LOGIN / SIGNUP

    if (currentState == LOGIN || currentState == SIGNUP) {

        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::TextEntered) {

                auth.message = "";

                char typed = static_cast<char>(event.text.unicode);

                if (typed == '\b') {
                    if (typingUsername && !inputUsername.empty())
                        inputUsername.pop_back();
                    else if (typingPassword && !inputPassword.empty())
                        inputPassword.pop_back();
                    else if (typingConfirm && !inputPassword2.empty())
                        inputPassword2.pop_back();
                }
                else if (typed >= 32 && typed <= 126) {
                    if (typingUsername)
                        inputUsername += typed;
                    else if (typingPassword)
                        inputPassword += typed;
                    else if (typingConfirm)
                        inputPassword2 += typed;
                }
            }

            if (event.type == sf::Event::KeyPressed) {

                if (event.key.code == sf::Keyboard::Tab) {

                    if (currentState == LOGIN) {
                        typingUsername = !typingUsername;
                        typingPassword = !typingPassword;
                    }
                    else {
                        if (typingUsername) {
                            typingUsername = false;
                            typingPassword = true;
                        }
                        else if (typingPassword) {
                            typingPassword = false;
                            typingConfirm = true;
                        }
                        else {
                            typingConfirm = false;
                            typingUsername = true;
                        }
                    }
                }

                if (event.key.code == sf::Keyboard::G) {
                    currentUser = "Guest";
                    isGuest = true;
                    isLoggedIn = false;
                    currentState = MENU;
                }

                if (event.key.code == sf::Keyboard::S) {
                    currentState = SIGNUP;
                }
            }

            //MOUSE INPUT
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {

                int mx = event.mouseButton.x;
                int my = event.mouseButton.y;

                bool isLogin = (currentState == LOGIN);
                float btnY = isLogin ? 420 : 494;

                //BUTTON CLICK
                if (mx >= 270 && mx <= 728 &&
                    my >= btnY && my <= btnY + 48) {

                    // LOGIN
                    if (currentState == LOGIN) {

                        if (inputUsername.empty() || inputPassword.empty()) {
                            auth.message = "Fill all fields";
                        }
                        else if (auth.login(inputUsername, inputPassword)) {

                            currentUser = inputUsername;
                            cout << "LOGGED IN USER = [" << inputUsername << "]" << endl;
                            if (currentUser.empty())
                                currentUser = "Guest";
                            isLoggedIn = true;
                            isGuest = false;

                            inputUsername = "";
                            inputPassword = "";

                            currentState = CHARACTER_SELECT;

                            return;
                        }
                    }

                    // SIGNUP
                    else {

                        if (inputUsername.empty() || inputPassword.empty() || inputPassword2.empty()) {
                            auth.message = "Fill all fields";
                        }
                        else if (inputPassword != inputPassword2) {
                            auth.message = "Passwords do not match";
                        }
                        else if (auth.signup(inputUsername, inputPassword)) {

                            auth.message = "Account created";
                            currentUser = inputUsername;

                            if (currentUser.empty())
                                currentUser = "Guest";
                            isLoggedIn = true;
                            isGuest = false;
                            inputUsername = "";
                            inputPassword = "";
                            inputPassword2 = "";
                            currentState = CHARACTER_SELECT;
                        }
                    }
                }

                //LOGIN TAB
                if (mx >= 252 && mx <= 490 &&
                    my >= 200 && my <= 236) {
                    currentState = LOGIN;
                }

                //SIGNUP TAB
                if (mx >= 490 && mx <= 728 &&
                    my >= 200 && my <= 236) {
                    currentState = SIGNUP;
                }

                //CLICK USERNAME BOX
                if (mx >= 270 && mx <= 728 &&
                    my >= 268 && my <= 310) {

                    typingUsername = true;
                    typingPassword = false;
                    typingConfirm = false;
                }

                //CLICK PASSWORD BOX
                if (mx >= 270 && mx <= 728 &&
                    my >= 342 && my <= 384) {

                    typingUsername = false;
                    typingPassword = true;
                    typingConfirm = false;
                }

                //CLICK CONFIRM (SIGNUP)
                if (currentState == SIGNUP &&
                    mx >= 270 && mx <= 728 &&
                    my >= 416 && my <= 458) {

                    typingUsername = false;
                    typingPassword = false;
                    typingConfirm = true;
                }
            }
        }
        return;
    }

    //SHOP
    if (currentState == SHOP) {

        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {

                if (event.key.code == sf::Keyboard::Up) {
                    shopIndex ;
                    if (shopIndex < 0) shopIndex = 4;
                }

                if (event.key.code == sf::Keyboard::Down) {
                    shopIndex++;
                    if (shopIndex > 4) shopIndex = 0;
                }

                if (event.key.code == sf::Keyboard::Enter) {

                    switch (shopIndex) {

                    case 0: //EXTRA LIFE
                        if (gems >= 50) {
                            playerLives++;
                            gems -= 50;
                        }
                        break;

                    case 1: //SPEED
                        if (gems >= 20) {
                            speedBoostActive = true;
                            speedTimer = 30;
                            gems -= 20;
                        }
                        break;

                    case 2: //POWER
                        if (gems >= 30) {
                            snowballPower = true;
                            gems -= 30;
                        }
                        break;

                    case 3: //RANGE
                        if (gems >= 25) {
                            maxDistance = true;
                            gems -= 25;
                        }
                        break;

                    case 4: //BALLOON
                        if (gems >= 35) {
                            balloonModeActive = true;
                            balloonTimer = 30;
                            gems -= 35;
                        }
                        break;
                    }
                }

                if (event.key.code == sf::Keyboard::Escape) {
                    currentState = PLAYING;
                }
            }
        }
        return;
    }
    if (currentState == CHARACTER_SELECT) {

        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {

                // choose character
                if (event.key.code == sf::Keyboard::Num1)
                    selectedCharacter = 0;

                if (event.key.code == sf::Keyboard::Num2)
                    selectedCharacter = 1;

                if (event.key.code == sf::Keyboard::Num3)
                    selectedCharacter = 2;

                // confirm selection
                if (event.key.code == sf::Keyboard::Enter) {

                    player.setCharacter(selectedCharacter);

                    currentState = MENU;

                    return;
                }
            }
        }

        return;
    }

    //MENU
    if (currentState == MENU) {

        bool startGame = false;
        bool exitGame = false;
        bool showLeaderboardLocal = false;
        bool showSettingsLocal = false;

        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape) {
                showLeaderboard = false;
            }

            menu.handleInput(event, startGame, exitGame, showLeaderboardLocal, showSettingsLocal, window);
        }

        if (showLeaderboardLocal)
            showLeaderboard = true;

        if (showSettingsLocal)
            currentState = SETTINGS;

        if (startGame) {

            if (menu.getSelectedIndex() == 0) {
                currentState = MODE_SELECT;
            }
            else if (menu.getSelectedIndex() == 1) {
                currentState = PLAYING;
            }
        }

        if (exitGame)
            window.close();

        return;
    }

    //SETTINGS
    if (currentState == SETTINGS) {

        bool goBack = false;

        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)
                window.close();

            settings.handleInput(event, goBack, music);
        }

        if (goBack) {
            currentDifficulty = settings.getDifficulty();
            musicVolume = settings.getVolume();
            currentState = MENU;
        }

        return;
    }


    // MODE SELECT  (1P / 2P / Help)
    if (currentState == MODE_SELECT) {

        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)
                window.close();

            bool chose1P = false;
            bool chose2P = false;
            bool showHelp = false;
            bool goBack = false;

            modeSelect.handleInput(event, chose1P, chose2P,
                showHelp, goBack, window);

            if (goBack) {
                currentState = MENU;
                return;
            }

            if (chose1P) {
                currentState = LEVEL_SELECT;
                return;
            }

            if (chose2P) {
                // Go to 2P level select first
                currentState = LEVEL_SELECT_2P;
                levelIndex = 0;
                return;
            }
        }

        return;
    }

    // LEVEL SELECT
    if (currentState == LEVEL_SELECT) {

        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {

                if (event.key.code == sf::Keyboard::Up) {
                    levelIndex ;
                    if (levelIndex < 0) levelIndex = 9;
                }

                if (event.key.code == sf::Keyboard::Down) {
                    levelIndex++;
                    if (levelIndex > 9) levelIndex = 0;
                }

                if (event.key.code == sf::Keyboard::Enter) {

                    level.loadLevel(levelIndex + 1);
                    applyDifficultyToLevel();

                    player.setPosition(100, 600);
                    playerLives = 2;

                    currentState = PLAYING;
                }

                if (event.key.code == sf::Keyboard::Escape) {
                    currentState = MENU;
                }
            }
        }

        return;
    }

    // LEVEL SELECT – TWO PLAYER

    if (currentState == LEVEL_SELECT_2P) {

        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {

                if (event.key.code == sf::Keyboard::Up) {
                    levelIndex ;
                    if (levelIndex < 0) levelIndex = 9;
                }

                if (event.key.code == sf::Keyboard::Down) {
                    levelIndex++;
                    if (levelIndex > 9) levelIndex = 0;
                }

                if (event.key.code == sf::Keyboard::Enter) {
                    // Stop 1P music before handing control to 2P
                    music.stop();
                    currentMusicID = -1;
                    // Launch two-player session starting at chosen level
                    TwoPlayerGame tp(window, levelIndex + 1);
                    tp.run();

                    currentMusicID = -1;
                    music.stop();
                    currentState = MENU;
                    return;
                }

                if (event.key.code == sf::Keyboard::Escape) {
                    currentState = MODE_SELECT;
                }
            }
        }

        return;
    }

    //PLAYING + PAUSE

    while (window.pollEvent(event)) {

        // 🎉 VICTORY STATE EVENTS
        if (currentState == VICTORY) {

            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Enter) {

                showLeaderboard = true;
                currentState = MENU;
            }

            return;
        }

        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::KeyPressed) {

            // ENTER PAUSE
            if (!isPaused && event.key.code == sf::Keyboard::Escape) {
                isPaused = true;
            }

            // PAUSE MENU
            else if (isPaused) {

                if (event.key.code == sf::Keyboard::Up) {
                    pauseIndex ;
                    if (pauseIndex < 0) pauseIndex = 4;
                }

                if (event.key.code == sf::Keyboard::Down) {
                    pauseIndex++;
                    if (pauseIndex > 4) pauseIndex = 0;
                }

                if (event.key.code == sf::Keyboard::Enter) {

                    switch (pauseIndex) {

                    case 0: isPaused = false; break;
                    case 1: currentState = SHOP; isPaused = false; break;

                    case 2: {
                        std::ofstream save("save.txt");
                        save << currentUser << " "
                            << level.getCurrentLevel() << " "
                            << playerLives << " "
                            << score;
                        save.close();
                    } break;

                    case 3:
                    {
                        currentUser = "";
                        isGuest = false;
                        isLoggedIn = false;
                        currentState = LOGIN;
                        isPaused = false;
                    }
                    break;

                    case 4: currentState = MENU; isPaused = false; break;
                    }
                }

                // ESC -> resume
                if (event.key.code == sf::Keyboard::Escape) {
                    isPaused = false;
                }
            }
        }
    }
}

// UPDATE
void Game::update() {
    //MUSIC
    // SPLASH
    if (currentState == SPLASH) {
        playMusic(0);
    }

    // MENU + LOGIN + SELECT SCREENS
    else if (currentState == LOGIN ||
        currentState == SIGNUP ||
        currentState == MENU ||
        currentState == SETTINGS ||
        currentState == MODE_SELECT ||
        currentState == LEVEL_SELECT ||
        currentState == LEVEL_SELECT_2P ||
        currentState == CHARACTER_SELECT) {

        // leaderboard special case
        if (showLeaderboard)
            playMusic(2);
        else
            playMusic(1);
    }

    // END
    else if (currentState == VICTORY) {
        playMusic(10);
    }

    // GAMEPLAY
    else if (currentState == PLAYING) {

        int lvl = level.getCurrentLevel();

        if (lvl == 1 || lvl == 3)
            playMusic(3);

        else if (lvl == 2)
            playMusic(4);

        else if (lvl == 4 || lvl == 9)
            playMusic(5);

        else if (lvl == 5 || lvl == 10)
            playMusic(6);

        else if (lvl == 6)
            playMusic(7);

        else if (lvl == 7)
            playMusic(8);

        else if (lvl == 8)
            playMusic(9);
    }

    if (currentState != PLAYING) {
        return;
    }

    if (isPaused) {
        clock.restart();  // prevents time jump
        return;
    }
    dt = clock.restart().asSeconds();

    // SHOP EFFECTS
// Speed Boost
    if (speedBoostActive) {
        speedTimer -= dt;

        player.setSpeedMultiplier(2.0f);

        if (speedTimer <= 0) {
            speedBoostActive = false;
            player.setSpeedMultiplier(1.0f);
        }
    }

    // Balloon Mode
    if (balloonModeActive) {
        balloonTimer -= dt;

        if (balloonTimer <= 0) {
            balloonModeActive = false;
        }
    }

    // BACKGROUND SELECTION
    int lvl = level.getCurrentLevel();
    if (lvl == 5)
        bgSprite.setTexture(bgTextureLevel5);
    else if (lvl >= 6 && lvl <= 9)
        bgSprite.setTexture(bgTextureLevel6to9);
    else if (lvl == 10)
        bgSprite.setTexture(bgTextureLevel10);
    else
        bgSprite.setTexture(bgTexture);




    // STAR SPAWN (ONLY ONCE PER LEVEL)
    starSpawnTimer += dt;

    if (!starSpawned && starSpawnTimer >= starSpawnDelay) {

        float x = rand() % 900 + 50;
        float y = rand() % 600 + 100;

        starSprite.setPosition(x, y);

        starActive = true;
        starSpawned = true;
    }


    // PLAYER UPDATE

    player.handleInput(dt);
    // Bonus: double-jump (only when power-up is active and player is airborne)
    if (doubleJumpActive1P)
        player.tryDoubleJump(usedDoubleJump1P);
    player.update(dt);
    if (balloonModeActive) {
        player.setVelocityY(player.getVelocityY() * 0.9f);
    }
    player.setOnGround(false);

    // Platform collision
    for (int i = 0; i < level.getPlatformCount(); i++) {
        if (player.getBounds().intersects(level.getPlatforms()[i].getBounds())) {
            if (player.getVelocityY() > 0) {

                player.setPosition(
                    player.getBounds().left,
                    level.getPlatforms()[i].getBounds().top - player.getBounds().height
                );

                player.setVelocityY(0);
                player.setOnGround(true);
            }
        }
    }

    // Fall safety
    if (player.getBounds().top > 800) {
        player.setPosition(100, 600);
        player.setVelocityY(0);
    }

    // SNOWBALL
    snowball.powerBoost = snowballPower;
    snowball.update(dt);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        if (!spacePressed && !snowball.isActive()) {
            float px = player.getX();
            float py = player.getY();
            float range = maxDistance ? 1200.f : 600.f;
            snowball.shoot(px, py + 45, player.getDirection(), range);
            spacePressed = true;
        }
    }
    else {
        spacePressed = false;
    }

    // UPDATE ALL ENEMIES USING UNIFIED ARRAY
    for (int i = 0; i < level.getAllEnemyCount(); i++) {
        if (level.getAllEnemies()[i]->isAlive()) {
            level.getAllEnemies()[i]->update(dt);
        }
    }


    //  Gamakichi needs player position update
    if (level.isGamakichiActive()) {
        level.getGamakichi()->update(
            dt,
            player.getX(),
            player.getY(),
            level.getPlatforms(),
            level.getPlatformCount()
        );
    }

    if (level.getCurrentLevel() == 4 || level.getCurrentLevel() == 9) {

        if (!bonusSpawned) {

            collectibleCount = 0;
            float positions[15][2] = {
                {200,650},{400,650},{600,650},{800,650},
                {200,450},{400,450},{600,450},{800,450},
                {300,250},{500,250},
                {350,550},{650,550},{350,350},{650,350},{500,150}
            };

            //GEMS
            for (int i = 0; i < 10; i++) {
                collectibles[collectibleCount].spawn(
                    &collectibleTextures[0],
                    positions[i][0],
                    positions[i][1]
                );
                collectibleCount++;
            }

            // POWER-UPS
            for (int i = 10; i < 15; i++) {
                collectibles[collectibleCount].spawn(
                    &collectibleTextures[1],
                    positions[i][0],
                    positions[i][1]
                );
                collectibleCount++;
            }
            bonusSpawned = true;
        }

    }

    // SPECIAL UPDATES FOR TORNADO (needs player position)
    for (int i = 0; i < level.getTornadoCount(); i++) {
        if (level.getTornados()[i].isAlive()) {
            level.getTornados()[i].update(dt, player.getX(), player.getY());
        }
    }


    // MOGERA CHILD SPAWN
    if (level.isMogeraActive() && level.getMogera()->isAlive()) {
        if (level.getMogera()->shouldSpawnChild()) {
            for (int k = 0; k < 2; k++) {
                if (level.getChildCount() < 20) {
                    MogeraChild& c = level.getChildren()[level.getChildCount()];
                    float mx = level.getMogera()->getBounds().left;
                    float my = level.getMogera()->getBounds().top;
                    c.setPosition(mx, my);
                    if (player.getX() < mx)
                        c.setDirection(-1);
                    else
                        c.setDirection(1);
                    level.incrementChildCount();

                    level.addEnemy(&c);
                }
            }
        }
    }

    // PLATFORM COLLISION FOR BOTOMS
    for (int i = 0; i < level.getBotomCount(); i++) {
        level.getBotoms()[i].setOnGround(false);
        for (int j = 0; j < level.getPlatformCount(); j++) {
            sf::FloatRect b = level.getBotoms()[i].getBounds();
            sf::FloatRect p = level.getPlatforms()[j].getBounds();
            if (b.intersects(p)) {
                float bottom = b.top + b.height;
                float prevBottom = bottom - level.getBotoms()[i].getVelocityY() * dt;
                if (prevBottom <= p.top) {
                    level.getBotoms()[i].setPosition(b.left, p.top - b.height);
                    level.getBotoms()[i].setVelocityY(0);
                    level.getBotoms()[i].setOnGround(true);
                }
            }
        }
        if (level.getBotoms()[i].getBounds().top > 750) {
            level.getBotoms()[i].setPosition(
                level.getBotoms()[i].getBounds().left,
                750 - level.getBotoms()[i].getBounds().height
            );
            level.getBotoms()[i].setVelocityY(0);
            level.getBotoms()[i].setOnGround(true);
        }
    }


    // PLATFORM COLLISION FOR FOOGA

    for (int i = 0; i < level.getFoogaCount(); i++) {
        level.getFoogas()[i].setOnGround(false);
        for (int j = 0; j < level.getPlatformCount(); j++) {
            sf::FloatRect f = level.getFoogas()[i].getBounds();
            sf::FloatRect p = level.getPlatforms()[j].getBounds();
            if (f.intersects(p)) {
                float bottom = f.top + f.height;
                float prevBottom = bottom - level.getFoogas()[i].getVelocityY() * dt;
                if (prevBottom <= p.top) {

                    level.getFoogas()[i].setPosition(f.left + f.width * 0.5f, p.top);

                    level.getFoogas()[i].setVelocityY(0);
                    level.getFoogas()[i].setOnGround(true);
                }
            }
        }
    }

    // KICK FROZEN BOTOMS
    for (int i = 0; i < level.getBotomCount(); i++) {
        Botom& b = level.getBotoms()[i];
        if (b.isAlive() && b.isFrozen() && player.getBounds().intersects(b.getBounds())) {
            b.kick(player.getDirection());
        }
    }


    // ROLLING BOTOM KILLS OTHER ENEMIES

    for (int i = 0; i < level.getBotomCount(); i++) {
        if (!level.getBotoms()[i].isAlive() || !level.getBotoms()[i].isRolling())
            continue;
        sf::FloatRect rollerBounds = level.getBotoms()[i].getBounds();
        for (int j = 0; j < level.getBotomCount(); j++) {
            if (i != j && level.getBotoms()[j].isAlive() && rollerBounds.intersects(level.getBotoms()[j].getBounds())) {
                level.getBotoms()[j].kill();
                score += getScoreForEnemy("Botom");
                gems += 10;
            }
        }
        for (int j = 0; j < level.getFoogaCount(); j++) {
            if (level.getFoogas()[j].isAlive() && rollerBounds.intersects(level.getFoogas()[j].getBounds())) {
                level.getFoogas()[j].kill();
                score += getScoreForEnemy("Fooga");
                gems += 20;

            }
        }
    }


    // SNOWBALL COLLISION WITH ENEMIES

    if (snowball.isActive()) {
        // Botom collision
        for (int i = 0; i < level.getBotomCount(); i++) {
            if (level.getBotoms()[i].isAlive() && snowball.getBounds().intersects(level.getBotoms()[i].getBounds())) {
                level.getBotoms()[i].takeDamage();

                if (snowballPower) {
                    level.getBotoms()[i].takeDamage();
                }


                snowball.deactivate();
                break;
            }
        }
        // Fooga collision
        for (int i = 0; i < level.getFoogaCount(); i++) {
            if (level.getFoogas()[i].isAlive() && snowball.getBounds().intersects(level.getFoogas()[i].getBounds())) {
                level.getFoogas()[i].kill();
                score += getScoreForEnemy("Fooga");
                gems += 20;
                snowball.deactivate();
                break;
            }
        }
        // child collision
        for (int i = 0; i < level.getChildCount(); i++) {
            if (level.getChildren()[i].isAlive() && snowball.getBounds().intersects(level.getChildren()[i].getBounds())) {
                level.getChildren()[i].takeDamage();
                snowball.deactivate();
                break;
            }
        }
        // Tornado collision
        for (int i = 0; i < level.getTornadoCount(); i++) {
            if (level.getTornados()[i].isAlive() && snowball.getBounds().intersects(level.getTornados()[i].getBounds())) {
                //level.getTornados()[i].kill();
                level.getTornados()[i].kill();
                score += getScoreForEnemy("Tornado");
                gems += 25;
                snowball.deactivate();
                break;
            }
        }

        if (level.isMogeraActive() && level.getMogera()->isAlive() &&
            snowball.getBounds().intersects(level.getMogera()->getBounds())) {

            level.getMogera()->takeDamage();

            if (snowballPower) {
                level.getMogera()->takeDamage(); // extra hit
            }


            if (!level.getMogera()->isAlive()) {
                score += getScoreForEnemy("Mogera");
                gems += 30;
            }

            snowball.deactivate();
        }


        if (level.isGamakichiActive() && level.getGamakichi()->isAlive() &&
            snowball.getBounds().intersects(level.getGamakichi()->getBounds())) {

            level.getGamakichi()->takeDamage();

            if (!level.getGamakichi()->isAlive()) {

                score += getScoreForEnemy("Gamakichi");

                currentState = VICTORY;   //VICTORY SCREEN
                return;
            }

            snowball.deactivate();
        }
        // SNOWBALL vs GAMAKICHI BLASTS
        if (level.isGamakichiActive()) {

            Gamakichi* g = level.getGamakichi();

            for (int i = 0; i < 10; i++) {

                if (g->getBlasts()[i].isActive() &&
                    snowball.getBounds().intersects(g->getBlasts()[i].getBounds())) {

                    //destroy blast
                    g->getBlasts()[i].deactivate();

                    // optional reward
                    score += 50;

                    // snowball disappears after hit
                    snowball.deactivate();

                    break;
                }
            }
        }
    }


    //COLLECTIBLE COLLISION
    for (int i = 0; i < collectibleCount; i++) {

        if (collectibles[i].isActive() &&
            player.getBounds().intersects(collectibles[i].getBounds())) {

            collectibles[i].collect();
            score += 100;
            shineEffect1P.trigger();   // Bonus: shine on collectible pickup

            // REMOVE + SHIFT ARRAY
            for (int j = i; j < collectibleCount - 1; j++) {
                collectibles[j] = collectibles[j + 1];
            }

            collectibleCount ;
            i ;
        }
    }


    // STAR COLLISION
    if (starActive &&
        player.getBounds().intersects(starSprite.getGlobalBounds())) {

        starActive = false;

        int power = rand() % 3;

        if (power == 0) {
            speedBoostActive = true;
            speedTimer = 5;
        }
        else if (power == 1) {
            balloonModeActive = true;
            balloonTimer = 5;
        }
        else {
            snowballPower = true;
        }

        score += 200;
        shineEffect1P.trigger();   // Bonus: visual shine when star collected
    }

    shineEffect1P.update(dt);

    //   Bonus: Double-jump pink power-up (1-player)    -
    if (!djSpawned1P) {
        int lvl = level.getCurrentLevel();
        if (lvl != 4 && lvl != 5 && lvl != 9 && lvl != 10) {
            djPowerup1P.spawn(150.f + (float)(rand() % 700), 550.f);
            djSpawned1P = true;
        }
    }
    djPowerup1P.update(dt);
    if (djPowerup1P.isActive() &&
        player.getBounds().intersects(djPowerup1P.getBounds())) {
        djPowerup1P.collect();
        doubleJumpActive1P = true;
        doubleJumpTimer1P = 10.f;
        usedDoubleJump1P = false;
        shineEffect1P.trigger();
    }
    if (doubleJumpActive1P) {
        doubleJumpTimer1P -= dt;
        if (doubleJumpTimer1P <= 0.f) {
            doubleJumpActive1P = false;
            usedDoubleJump1P = false;
        }
    }
    if (player.getVelocityY() == 0.f) usedDoubleJump1P = false; // landed

    //   Bonus: Screen shake update (1-player)       -
    screenShake1P.update(dt);

    // Detect Gamakichi death for screen shake
    if (level.isGamakichiActive()) {
        bool nowDead = !level.getGamakichi()->isAlive();
        if (nowDead && !gamakichiWasDead1P) {
            screenShake1P.trigger(0.65f, 14.f);
            gamakichiWasDead1P = true;
        }
    }
    else {
        gamakichiWasDead1P = false;
    }

    //   Bonus: Monkey enemy update (1-player)       -
    for (int i = 0; i < monkeyCount1P; i++) {
        if (monkeys1P[i].isAlive()) monkeys1P[i].update(dt);
    }
    // Monkey vs player damage
    for (int i = 0; i < monkeyCount1P; i++) {
        if (!monkeys1P[i].isAlive()) continue;
        if (damageCooldown <= 0.f &&
            player.getBounds().intersects(monkeys1P[i].getBounds())) {
            playerLives ;
            damageCooldown = damageCooldownReset;
        }
    }
    // Monkey vs snowball
    for (int i = 0; i < monkeyCount1P; i++) {
        if (!monkeys1P[i].isAlive()) continue;
        if (snowball.isActive() &&
            snowball.getBounds().intersects(monkeys1P[i].getBounds())) {
            monkeys1P[i].takeDamage();
            if (!monkeys1P[i].isAlive())
                score += getScoreForEnemy("Monkey");
            snowball.deactivate();
        }
    }


    // PLAYER DAMAGE FROM ENEMIES

    damageCooldown -= dt;

    // Botom damage
    for (int i = 0; i < level.getBotomCount(); i++) {
        Botom& b = level.getBotoms()[i];
        if (b.isAlive() && !b.isFrozen() && !b.isRolling() && player.getBounds().intersects(b.getBounds())) {
            if (damageCooldown <= 0) {
                playerLives ;
                damageCooldown = damageCooldownReset;
                player.setPosition(100, 600);
            }
        }
    }

    // Fooga damage
    for (int i = 0; i < level.getFoogaCount(); i++) {
        if (level.getFoogas()[i].isAlive() && player.getBounds().intersects(level.getFoogas()[i].getBounds())) {
            if (damageCooldown <= 0) {
                playerLives ;
                damageCooldown = damageCooldownReset;
                player.setPosition(100, 600);
            }
        }
    }
    for (int i = 0; i < level.getChildCount(); i++) {
        if (level.getChildren()[i].isAlive() && player.getBounds().intersects(level.getChildren()[i].getBounds())) {
            if (damageCooldown <= 0) {
                playerLives ;
                damageCooldown = damageCooldownReset;
                player.setPosition(100, 600);
            }
        }
    }
    // Tornado knife damage
    for (int i = 0; i < level.getTornadoCount(); i++) {
        if (level.getTornados()[i].isKnifeActive() && player.getBounds().intersects(level.getTornados()[i].getKnifeBounds())) {
            if (damageCooldown <= 0) {
                playerLives ;
                damageCooldown = damageCooldownReset;
                player.setPosition(100, 600);
            }
        }
    }


    // GAMAKICHI BOMB DAMAGE
    if (level.isGamakichiActive()) {

        Gamakichi* g = level.getGamakichi();

        for (int i = 0; i < 10; i++) {

            if (g->getBlasts()[i].isActive() &&
                g->getBlasts()[i].getBounds().intersects(player.getBounds())) {

                if (damageCooldown <= 0) {
                    playerLives ;
                    damageCooldown = damageCooldownReset;
                    player.setPosition(100, 600);
                }
            }
        }
    }

    // CHECK LEVEL COMPLETION
    bool allCleared = true;

    // BONUS LEVEL CHECK (FIXED)
    if (level.getCurrentLevel() == 4 || level.getCurrentLevel() == 9) {

        if (collectibleCount <= 0)
            allCleared = true;
        else
            allCleared = false;
    }
    else {

        for (int i = 0; i < level.getAllEnemyCount(); i++) {
            if (level.getAllEnemies()[i]->isAlive()) {
                allCleared = false;
                break;
            }
        }

    }

    int currentLvl = level.getCurrentLevel();

    if (allCleared && !levelChanging && currentLvl != 10)
    {
        levelChanging = true;
        collectibleCount = 0;
        starActive = false;
        starSpawned = false;
        starSpawnTimer = 0;
        int nextLevel = level.getCurrentLevel() + 1;
        if (nextLevel <= 10) {
            // RESET COLLECTIBLES FOR NEXT LEVEL
            collectibleCount = 0;

            level.loadLevel(nextLevel);
            applyDifficultyToLevel();
            bonusSpawned = false;
            collectibleCount = 0;
            player.setPosition(100, 600);
            snowball.deactivate();
            playerLives = 2;
            // Bonus: reset per-level bonus state
            djSpawned1P = false;
            gamakichiWasDead1P = false;
            monkeyCount1P = 0;
            if (nextLevel != 4 && nextLevel != 5 &&
                nextLevel != 9 && nextLevel != 10) {

                monkeys1P[0].reset(400.f + (float)(rand() % 400), 300.f);
                monkeyCount1P = 1;
            }
        }
    }
    if (!allCleared)
        levelChanging = false;

    if (playerLives <= 0) {
        collectibleCount = 0;
        starActive = false;
        starSpawned = false;
        starSpawnTimer = 0;

        if (isGuest)
            leaderboard.addScore("Guest", score);
        else

        {
            string nameToSave = currentUser;

            if (nameToSave.empty())
                nameToSave = "Guest";

            leaderboard.addScore(nameToSave, score);
        }
        score = 0;

        playerLives = 2;
        level.loadLevel(1);
        applyDifficultyToLevel();
        player.setPosition(100, 600);
        snowball.deactivate();



        return;
    }



}


// APPLY DIFFICULTY TO LEVEL

void Game::applyDifficultyToLevel() {

    float speedMult = 1.0f;
    float cooldownMult = 1.0f;  //cooldown reset

    if (currentDifficulty == Difficulty::EASY) {
        speedMult = 0.70f;
        cooldownMult = 1.60f;   // longer cooldown 
    }
    else if (currentDifficulty == Difficulty::NORMAL) {
        speedMult = 1.00f;
        cooldownMult = 1.00f;
    }
    else { // HARD
        speedMult = 1.40f;
        cooldownMult = 0.60f;   // shorter cooldown
    }


    damageCooldownReset = 1.0f * cooldownMult;

    // Apply speed to all enemies currently in the level
    for (int i = 0; i < level.getAllEnemyCount(); i++) {
        Enemy* e = level.getAllEnemies()[i];
        if (e) {
            float base = e->getSpeed();

            e->setSpeed(base * speedMult);
        }
    }

    // Extra enemy on Hard: spawn one more Botom at a spare position if room
    if (currentDifficulty == Difficulty::HARD) {
        int bc = level.getBotomCount();
        // Only add if we have spare slots (array max is 20)
        if (bc < 18) {
            Botom* bArr = level.getBotoms();
            bArr[bc].reset();
            bArr[bc].setPosition(500, 100);
            bArr[bc].setSpeed(80.f * speedMult);
            level.addEnemy(&bArr[bc]);

        }
    }

    // Boss health multiplier
    float bossHpMult = 1.0f;
    if (currentDifficulty == Difficulty::EASY)   bossHpMult = 0.60f;
    else if (currentDifficulty == Difficulty::HARD) bossHpMult = 1.50f;

    if (level.isMogeraActive() && level.getMogera()) {
        int newHp = static_cast<int>(10 * bossHpMult);
        if (newHp < 1) newHp = 1;
        level.getMogera()->setHealth(newHp);
    }
    if (level.isGamakichiActive() && level.getGamakichi()) {
        int newHp = static_cast<int>(25 * bossHpMult);
        if (newHp < 1) newHp = 1;
        level.getGamakichi()->setHealth(newHp);
    }
}

// RENDER
void Game::render() {

    window.clear();

    if (currentState == CHARACTER_SELECT) {

        // BACKGROUND 
        window.draw(charSelectBgSprite);

        // DARK OVERLAY for readability
        sf::RectangleShape overlay(sf::Vector2f(1000, 800));
        overlay.setFillColor(sf::Color(0, 0, 0, 140));
        window.draw(overlay);

        // TITLE
        sf::Text title;
        title.setFont(font);
        title.setCharacterSize(42);
        title.setString("SELECT YOUR CHARACTER");
        title.setFillColor(sf::Color(255, 220, 60));
        title.setOutlineColor(sf::Color::Black);
        title.setOutlineThickness(3);
        sf::FloatRect tb = title.getLocalBounds();
        title.setOrigin(tb.left + tb.width / 2.f, tb.top);
        title.setPosition(500, 40);
        window.draw(title);

        // CHARACTER DATA
        const char* names[3] = { "Nick", "elsa", "Diego" };
        const char* stats[3] = { "Balanced", "Fast", "Strong" };
        const char* descs[3] = {
            "The iconic Snow Bros hero.\nWell-balanced\nand reliable in all situations.",
            "Brave and adventurous.\nHigh speed lets her\ndodge and dash with ease.",
            "Fearless and powerful.\nSnowball attacks hit \nharder against tough enemies."
        };

        float cardW = 240.f, cardH = 380.f;
        float startX = 500.f - (3 * cardW + 2 * 30.f) / 2.f;  // 3 cards + 2 gaps of 30
        float cardY = 160.f;

        for (int i = 0; i < 3; i++) {

            float cx = startX + i * (cardW + 30.f);
            bool  selected = (selectedCharacter == i);

            // CARD BACKGROUND
            sf::RectangleShape card(sf::Vector2f(cardW, cardH));
            card.setPosition(cx, cardY);

            if (selected) {
                card.setFillColor(sf::Color(255, 220, 60, 50));
                card.setOutlineColor(sf::Color(255, 220, 60));
                card.setOutlineThickness(3);
            }
            else {
                card.setFillColor(sf::Color(10, 20, 50, 180));
                card.setOutlineColor(sf::Color(80, 130, 200, 160));
                card.setOutlineThickness(1.5f);
            }
            window.draw(card);

            // CHARACTER PORTRAIT 
            sf::FloatRect bounds = charSprites[i].getLocalBounds();

            // center origin
            charSprites[i].setOrigin(bounds.width / 2, bounds.height / 2);

            // center inside card
            charSprites[i].setPosition(cx + cardW / 2.f, cardY + 120.f);

            window.draw(charSprites[i]);
            //   NUMBER BADGE
            sf::CircleShape badge(18);
            badge.setPosition(cx + cardW - 42.f, cardY + 8.f);
            badge.setFillColor(selected ? sf::Color(255, 220, 60) : sf::Color(40, 60, 120, 200));
            badge.setOutlineColor(sf::Color::White);
            badge.setOutlineThickness(1.5f);
            window.draw(badge);

            sf::Text num;
            num.setFont(font);
            num.setCharacterSize(20);
            num.setString(std::to_string(i + 1));
            num.setFillColor(selected ? sf::Color::Black : sf::Color::White);
            num.setPosition(cx + cardW - 32.f, cardY + 12.f);
            window.draw(num);

            //   NAME  
            sf::Text nameText;
            nameText.setFont(font);
            nameText.setCharacterSize(24);
            nameText.setString(names[i]);
            nameText.setFillColor(selected ? sf::Color(255, 220, 60) : sf::Color::White);
            nameText.setOutlineColor(sf::Color::Black);
            nameText.setOutlineThickness(2);
            sf::FloatRect nb = nameText.getLocalBounds();
            nameText.setOrigin(nb.left + nb.width / 2.f, nb.top);
            nameText.setPosition(cx + cardW / 2.f, cardY + 248.f);
            window.draw(nameText);

            //   STAT BADGE                   
            sf::RectangleShape statBadge(sf::Vector2f(120, 22));
            statBadge.setFillColor(selected ? sf::Color(255, 180, 0, 180) : sf::Color(40, 80, 160, 180));
            statBadge.setPosition(cx + cardW / 2.f - 60.f, cardY + 278.f);
            window.draw(statBadge);

            sf::Text stat;
            stat.setFont(font);
            stat.setCharacterSize(14);
            stat.setString(stats[i]);
            stat.setFillColor(sf::Color::White);
            sf::FloatRect sb = stat.getLocalBounds();
            stat.setOrigin(sb.left + sb.width / 2.f, sb.top);
            stat.setPosition(cx + cardW / 2.f, cardY + 281.f);
            window.draw(stat);

            //   DESCRIPTION                 
            sf::Text desc;
            desc.setFont(font);
            desc.setCharacterSize(13);
            desc.setString(descs[i]);
            desc.setFillColor(sf::Color(200, 215, 255));
            desc.setPosition(cx + 12.f, cardY + 308.f);
            window.draw(desc);
        }

        //   BOTTOM HINT                    
        sf::Text hint;
        hint.setFont(font);
        hint.setCharacterSize(18);
        hint.setFillColor(sf::Color(180, 180, 180));
        hint.setString("Press 1 / 2 / 3 to choose     ENTER to confirm");
        sf::FloatRect hb = hint.getLocalBounds();
        hint.setOrigin(hb.left + hb.width / 2.f, hb.top);
        hint.setPosition(500, 720);
        window.draw(hint);

        //   SELECTED INDICATOR           
        sf::Text selInfo;
        selInfo.setFont(font);
        selInfo.setCharacterSize(16);
        selInfo.setFillColor(sf::Color(255, 220, 60));
        selInfo.setString("Selected: " + std::string(names[selectedCharacter]));
        sf::FloatRect si = selInfo.getLocalBounds();
        selInfo.setOrigin(si.left + si.width / 2.f, si.top);
        selInfo.setPosition(500, 755);
        window.draw(selInfo);

        window.display();
        return;
    }





    if (currentState == SPLASH) {

        window.draw(splashSprite);

        window.display();
        return;
    }



    if (currentState == LOGIN || currentState == SIGNUP) {

        window.clear();
        window.draw(loginBgSprite);

        //   BACKGROUND GRID  
        for (int i = -20; i < 40; i++) {
            sf::RectangleShape line(sf::Vector2f(1200, 1));
            line.setFillColor(sf::Color(80, 170, 255, 18));
            line.setPosition(-100 + i * 22, 0);
            line.setRotation(45);
            window.draw(line);
        }

        //   GLOW     
        sf::CircleShape glowTop(220);
        glowTop.setFillColor(sf::Color(80, 170, 255, 30));
        glowTop.setPosition(280, -180);
        window.draw(glowTop);

        sf::CircleShape glowBot(180);
        glowBot.setFillColor(sf::Color(120, 200, 255, 20));
        glowBot.setPosition(480, 390);
        window.draw(glowBot);

        //   PANEL                     
        sf::RectangleShape panel;
        panel.setSize(sf::Vector2f(500, 510));
        panel.setFillColor(sf::Color(10, 20, 40, 230));
        panel.setOutlineThickness(1.5f);
        panel.setOutlineColor(sf::Color(80, 170, 255));
        panel.setPosition(250, 60);
        window.draw(panel);

        //   CORNERS                    
        auto drawCorner = [&](float x, float y) {
            sf::RectangleShape h(sf::Vector2f(36, 2));
            h.setFillColor(sf::Color(120, 200, 255));
            h.setPosition(x, y);
            window.draw(h);

            sf::RectangleShape v(sf::Vector2f(2, 36));
            v.setFillColor(sf::Color(120, 200, 255));
            v.setPosition(x, y);
            window.draw(v);
            };

        drawCorner(250, 60);
        drawCorner(714, 60);
        drawCorner(250, 534);
        drawCorner(714, 534);

        //   TITLE                     
        sf::Text titleGlow;
        titleGlow.setFont(font);
        titleGlow.setCharacterSize(42);
        titleGlow.setFillColor(sf::Color(80, 170, 255, 60));
        titleGlow.setString("   SNOW BROS");
        titleGlow.setPosition(300, 109);
        window.draw(titleGlow);

        sf::Text title;
        title.setFont(font);
        title.setCharacterSize(42);
        title.setFillColor(sf::Color::White);
        title.setString("   SNOW BROS");
        title.setPosition(298, 107);
        window.draw(title);

        sf::Text subtitle;
        subtitle.setFont(font);
        subtitle.setCharacterSize(13);
        subtitle.setFillColor(sf::Color(120, 200, 255));
        subtitle.setLetterSpacing(5.0f);
        subtitle.setString("PRESS \"G\" FOR GUEST MODE");
        subtitle.setPosition(330, 158);
        window.draw(subtitle);

        //   TABS        
        auto drawTab = [&](const std::string& label, float x, bool active) {

            sf::RectangleShape tab(sf::Vector2f(238, 36));
            tab.setPosition(x, 200);

            if (active)
                tab.setFillColor(sf::Color(80, 170, 255));
            else
                tab.setFillColor(sf::Color(0, 0, 0, 0));

            tab.setOutlineThickness(1);
            tab.setOutlineColor(sf::Color(80, 170, 255, 60));
            window.draw(tab);

            sf::Text t;
            t.setFont(font);
            t.setCharacterSize(13);
            t.setLetterSpacing(2.0f);
            t.setFillColor(active ? sf::Color::White : sf::Color(140, 170, 200));
            t.setString(label);
            t.setPosition(x + 70, 209);
            window.draw(t);
            };

        bool isLogin = (currentState == LOGIN);
        drawTab("LOGIN", 252, isLogin);
        drawTab("SIGN UP", 490, !isLogin);

        //   INPUT FIELD                  
        auto drawField = [&](const std::string& label,
            const std::string& content,
            float y,
            bool focused,
            bool isPassword) {

                sf::Text lbl;
                lbl.setFont(font);
                lbl.setCharacterSize(11);
                lbl.setLetterSpacing(2.0f);
                lbl.setFillColor(sf::Color(120, 200, 255));
                lbl.setString(label);
                lbl.setPosition(270, y - 20);
                window.draw(lbl);

                sf::RectangleShape box(sf::Vector2f(458, 42));
                box.setPosition(270, y);
                box.setFillColor(sf::Color(10, 20, 40, 200));
                box.setOutlineThickness(1.5f);
                box.setOutlineColor(focused ? sf::Color(80, 170, 255)
                    : sf::Color(80, 170, 255, 70));
                window.draw(box);

                if (focused) {
                    sf::RectangleShape glow(sf::Vector2f(458, 42));
                    glow.setPosition(270, y);
                    glow.setFillColor(sf::Color(80, 170, 255, 25));
                    window.draw(glow);
                }

                std::string display = isPassword
                    ? std::string(content.length(), '*')
                    : content;

                sf::Text txt;
                txt.setFont(font);
                txt.setCharacterSize(16);
                txt.setFillColor(sf::Color::White);
                txt.setString(display);
                txt.setPosition(305, y + 10);
                window.draw(txt);
            };

        drawField("USERNAME", inputUsername, 268, typingUsername, false);
        drawField("PASSWORD", inputPassword, 342, typingPassword, true);

        if (!isLogin)
            drawField("CONFIRM PASSWORD", inputPassword2, 416, typingConfirm, true);

        //   BUTTON        
        float btnY = isLogin ? 420 : 494;

        sf::RectangleShape btn(sf::Vector2f(458, 48));
        btn.setPosition(270, btnY);
        btn.setFillColor(sf::Color(80, 170, 255));
        window.draw(btn);

        sf::Text btnText;
        btnText.setFont(font);
        btnText.setCharacterSize(14);
        btnText.setLetterSpacing(3.0f);
        btnText.setFillColor(sf::Color::White);
        btnText.setString(isLogin ? "LOG IN"
            : "CREATE ACCOUNT");
        btnText.setPosition(310, btnY + 14);
        window.draw(btnText);

        //   MESSAGE                    
        if (!auth.message.empty()) {
            sf::Text msg;
            msg.setFont(font);
            msg.setCharacterSize(14);
            msg.setPosition(290, btnY + 52);

            bool success =
                auth.message.find("successful") != std::string::npos ||
                auth.message.find("created") != std::string::npos;

            msg.setFillColor(success
                ? sf::Color(120, 255, 200)
                : sf::Color(255, 120, 120));

            msg.setString(auth.message);
            window.draw(msg);
        }

        window.display();
        return;
    }



    if (currentState == SHOP) {

        //   BACKGROUND   
        window.draw(shopBgSprite);

        //   SOFT OVERLAY       
        sf::RectangleShape overlay(sf::Vector2f(1000, 800));
        overlay.setFillColor(sf::Color(20, 10, 5, 150));
        window.draw(overlay);

        //   PANEL                  
        float PW = 500, PH = 500;
        float PX = 500 - PW / 2.f;
        float PY = 400 - PH / 2.f;

        sf::RectangleShape panel(sf::Vector2f(PW, PH));
        panel.setPosition(PX, PY);
        panel.setFillColor(sf::Color(40, 25, 15, 230));
        panel.setOutlineThickness(2);
        panel.setOutlineColor(sf::Color(139, 94, 60));
        window.draw(panel);

        //   TITLE                  
        sf::Text title;
        title.setFont(font);
        title.setCharacterSize(36);
        title.setString("SHOP");

        sf::FloatRect tb = title.getLocalBounds();
        title.setOrigin(tb.left + tb.width / 2.f, tb.top);
        title.setPosition(500, PY + 30);

        // glow
        sf::Text glow = title;
        glow.setFillColor(sf::Color(255, 220, 170, 80));
        glow.move(2, 2);
        window.draw(glow);

        title.setFillColor(sf::Color(255, 240, 210));
        window.draw(title);

        //   GEMS            
        sf::Text gemText;
        gemText.setFont(font);
        gemText.setCharacterSize(22);
        gemText.setString("Gems: " + std::to_string(gems));
        gemText.setFillColor(sf::Color(205, 170, 125));
        gemText.setPosition(PX + 20, PY + 80);
        window.draw(gemText);

        //   ITEMS                  
        for (int i = 0; i < 5; i++) {

            float y = PY + 140 + i * 60;

            sf::Text item;
            item.setFont(font);
            item.setCharacterSize(24);
            item.setString(shopItems[i]);

            sf::FloatRect ob = item.getLocalBounds();
            item.setOrigin(ob.left + ob.width / 2.f, ob.top);
            item.setPosition(500, y);

            if (i == shopIndex) {

                // highlight
                sf::RectangleShape hl(sf::Vector2f(360, 45));
                hl.setPosition(500 - 180, y - 8);
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

        window.display();
        return;
    }





    if (currentState == LEVEL_SELECT) {

        window.clear();

        // use menu bg
        window.draw(menuBgSprite);

        //   PANEL     
        sf::RectangleShape panel(sf::Vector2f(500, 600));
        panel.setPosition(250, 100);
        panel.setFillColor(sf::Color(10, 25, 10, 220));
        panel.setOutlineThickness(2);
        panel.setOutlineColor(sf::Color(62, 207, 90));
        window.draw(panel);

        //   TITLE       
        sf::Text title;
        title.setFont(font);
        title.setCharacterSize(36);
        title.setString("SELECT LEVEL");
        title.setPosition(350, 120);
        window.draw(title);

        //   LEVEL LIST           
        for (int i = 0; i < 10; i++) {

            sf::Text lvl;
            lvl.setFont(font);
            lvl.setCharacterSize(22);
            lvl.setString("LEVEL " + std::to_string(i + 1));
            lvl.setPosition(380, 200 + i * 40);

            if (i == levelIndex)
                lvl.setFillColor(sf::Color::Yellow);
            else
                lvl.setFillColor(sf::Color(120, 200, 140));

            window.draw(lvl);
        }

        //   HELP              
        sf::Text help;
        help.setFont(font);
        help.setCharacterSize(16);
        help.setString("UP/DOWN: Select   ENTER: Play   ESC: Back");
        help.setPosition(260, 720);
        window.draw(help);

        window.display();
        return;
    }

    if (currentState == LEVEL_SELECT_2P) {

        window.clear();
        window.draw(menuBgSprite);

        //   PANEL   
        sf::RectangleShape panel2p(sf::Vector2f(500, 640));
        panel2p.setPosition(250, 80);
        panel2p.setFillColor(sf::Color(1, 13, 2, 220));
        panel2p.setOutlineThickness(2);
        panel2p.setOutlineColor(sf::Color(62, 207, 90));
        window.draw(panel2p);

        //   TITLE   
        sf::Text title2p;
        title2p.setFont(font);
        title2p.setCharacterSize(30);
        title2p.setFillColor(sf::Color::White);
        title2p.setString("SELECT LEVEL  -  2 PLAYER");
        {
            auto tb = title2p.getLocalBounds();
            title2p.setOrigin(tb.left + tb.width / 2.f, tb.top);
            title2p.setPosition(500, 100);
        }
        window.draw(title2p);

        sf::Text sub2p;
        sub2p.setFont(font);
        sub2p.setCharacterSize(12);
        sub2p.setLetterSpacing(2.f);
        sub2p.setFillColor(sf::Color(62, 207, 90));
        sub2p.setString("P1: WASD + SPACE          P2: ARROWS + N");
        {
            auto sb2 = sub2p.getLocalBounds();
            sub2p.setOrigin(sb2.left + sb2.width / 2.f, sb2.top);
            sub2p.setPosition(500, 142);
        }
        window.draw(sub2p);

        sf::RectangleShape div2p(sf::Vector2f(420, 1));
        div2p.setPosition(290, 165);
        div2p.setFillColor(sf::Color(30, 102, 40, 160));
        window.draw(div2p);

        //   LEVEL LIST        
        for (int i = 0; i < 10; i++) {
            float iy = 180.f + i * 44.f;
            float ix = 290.f;
            float iw = 420.f;

            if (i == levelIndex) {
                sf::RectangleShape hl(sf::Vector2f(iw, 36));
                hl.setPosition(ix, iy);
                hl.setFillColor(sf::Color(62, 207, 90, 28));
                hl.setOutlineThickness(1.5f);
                hl.setOutlineColor(sf::Color(62, 207, 90));
                window.draw(hl);
                sf::CircleShape dotL(5);
                dotL.setFillColor(sf::Color(100, 160, 255));
                dotL.setPosition(ix - 18.f, iy + 13.f);
                window.draw(dotL);
                sf::CircleShape dotR(5);
                dotR.setFillColor(sf::Color(100, 160, 255));
                dotR.setPosition(ix + iw + 8.f, iy + 13.f);
                window.draw(dotR);
            }

            sf::Text lvl2p;
            lvl2p.setFont(font);
            lvl2p.setCharacterSize(20);
            std::string label2p = "LEVEL " + std::to_string(i + 1);
            if (i + 1 == 4 || i + 1 == 9) label2p += "";
            lvl2p.setString(label2p);
            {
                auto lb = lvl2p.getLocalBounds();
                lvl2p.setOrigin(lb.left + lb.width / 2.f, lb.top + lb.height / 2.f);
                lvl2p.setPosition(500, iy + 18.f);
            }
            lvl2p.setFillColor(i == levelIndex ? sf::Color::Yellow : sf::Color(93, 184, 112));
            window.draw(lvl2p);
        }

        sf::Text help2p;
        help2p.setFont(font);
        help2p.setCharacterSize(14);
        help2p.setFillColor(sf::Color(62, 207, 90, 200));
        help2p.setString("UP/DOWN: Select    ENTER: Play    ESC: Back");
        {
            auto hb = help2p.getLocalBounds();
            help2p.setOrigin(hb.left + hb.width / 2.f, hb.top);
            help2p.setPosition(500, 660);
        }
        window.draw(help2p);

        window.display();
        return;
    }

    if (currentState == MENU && showLeaderboard) {

        window.draw(leaderboardBgSprite);   // ✅ NEW BG
        leaderboard.draw(window, font);

        window.display();
        return;   // ❗ VERY IMPORTANT
    }

    //   MODE SELECT          
    if (currentState == MODE_SELECT) {
        window.draw(menuBgSprite);
        modeSelect.draw(window);
        window.display();
        return;
    }


    if (currentState == MENU) {

        window.draw(menuBgSprite);

        // ONLY ONE THING SHOULD DRAW
        if (showLeaderboard) {
            leaderboard.draw(window, font);
        }
        else {
            menu.draw(window);
        }

        window.display();
        return;
    }

    // SETTINGS SCREEN
    if (currentState == SETTINGS) {
        window.draw(menuBgSprite);
        settings.draw(window);
        window.display();
        return;
    }
    // VICTORY SCREEN
    if (currentState == VICTORY) {

        window.draw(victoryBgSprite);

        window.display();
        return;
    }

    // PLAYING STATE
    else {
        window.draw(bgSprite);

        // Draw platforms
        for (int i = 0; i < level.getPlatformCount(); i++)
            level.getPlatforms()[i].draw(window);

        // Draw snowball
        snowball.draw(window);


        // Bonus: apply screen shake
        if (screenShake1P.isActive()) {
            sf::View v = window.getDefaultView();
            screenShake1P.applyToView(v);
            window.setView(v);
        }

        // DRAW STAR
        if (starActive) {
            window.draw(starSprite);
        }

        // Bonus: draw double-jump pink power-up
        djPowerup1P.draw(window);

        // DRAW ALL ENEMIES USING THE UNIFIED ARRAY
        for (int i = 0; i < level.getAllEnemyCount(); i++) {
            if (level.getAllEnemies()[i]->isAlive()) {
                level.getAllEnemies()[i]->draw(window);
            }
        }

        // DRAW COLLECTIBLES
        for (int i = 0; i < collectibleCount; i++) {
            collectibles[i].draw(window);
        }

        // Bonus: draw monkey enemies
        for (int i = 0; i < monkeyCount1P; i++)
            monkeys1P[i].draw(window);

        // Bonus: draw shine effect on player
        if (shineEffect1P.isActive()) {
            sf::FloatRect pb = player.getBounds();
            shineEffect1P.draw(window,
                pb.left + pb.width * 0.5f,
                pb.top + pb.height * 0.5f);
        }

        // Level 5 boss — Mogera HP bar (1-player)
        if (level.isMogeraActive()) {
            BossHPBar::draw(window, font,
                "MOGERA",
                (float)level.getMogera()->getHealth(),
                (float)level.getMogera()->getMaxHealth());
        }

        // GAMAKICHI HEALTH BAR  (Level 10, 1-player)
        if (level.isGamakichiActive()) {
            BossHPBar::draw(window, font,
                "GAMAKICHI",
                (float)level.getGamakichi()->getHealth(),
                (float)level.getGamakichi()->getMaxHealth());
        }


        // Draw player
        player.draw(window);

        //   TOP HUD BAR    -
        const float WW = 1000.f, HH = 800.f;
        const float HUD_H = 60.f;

        sf::RectangleShape hudBg({ WW, HUD_H });
        hudBg.setPosition(0.f, 0.f);
        hudBg.setFillColor(sf::Color(0, 0, 0, 175));
        window.draw(hudBg);

        sf::RectangleShape hudDiv({ WW, 2.f });
        hudDiv.setPosition(0.f, HUD_H - 2.f);
        hudDiv.setFillColor(sf::Color(255, 255, 255, 40));
        window.draw(hudDiv);

        //   LEFT: Lives + Level  
        string heartsStr = "";
        for (int i = 0; i < playerLives && i < 5; i++) heartsStr += "<3 ";
        if (playerLives <= 0) heartsStr = "OUT";
        livesText.setString(heartsStr);
        livesText.setPosition(10.f, 18.f);
        window.draw(livesText);

        levelText.setString("LVL " + to_string(level.getCurrentLevel()));
        levelText.setPosition(10.f, 42.f);
        window.draw(levelText);

        //   RIGHT: Score + Gems + Powers
        auto txtR1P = [&](const string& s, float rightX, float y,
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

        txtR1P("SC: " + to_string(score), WW - 8.f, 5.f, sf::Color(255, 255, 255), 14);
        txtR1P("Gems: " + to_string(gems), WW - 8.f, 25.f, sf::Color(100, 220, 255), 14);

        std::string powers = "";
        if (speedBoostActive)  powers += "SPD ";
        if (balloonModeActive) powers += "BAL ";
        if (snowballPower)     powers += "PWR ";
        if (maxDistance)       powers += "RNG ";
        if (!powers.empty())
            txtR1P(powers, WW - 8.f, 43.f, sf::Color(180, 255, 120), 11);

        // Bottom controls hint strip
        sf::RectangleShape hintBg1P({ WW, 18.f });
        hintBg1P.setPosition(0.f, HH - 18.f);
        hintBg1P.setFillColor(sf::Color(0, 0, 0, 130));
        window.draw(hintBg1P);
        sf::Text hintT1P;
        hintT1P.setFont(font); hintT1P.setCharacterSize(11);
        hintT1P.setFillColor(sf::Color(90, 90, 90));
        hintT1P.setString("ESC=Pause  Arrow Keys / WASD = Move  Space = Throw");
        auto hb1p = hintT1P.getLocalBounds();
        hintT1P.setPosition(WW / 2.f - hb1p.width / 2.f, HH - 17.f);
        window.draw(hintT1P);

        // DEBUG TEXT - Shows how many enemies are alive (REMOVE AFTER TESTING)
        sf::Text debugText;
        debugText.setFont(font);
        debugText.setCharacterSize(14);
        debugText.setPosition(10, 700);
        debugText.setFillColor(sf::Color::White);

        int aliveCount = 0;
        for (int i = 0; i < level.getAllEnemyCount(); i++) {
            if (level.getAllEnemies()[i]->isAlive()) aliveCount++;
        }
        debugText.setString("Alive: " + std::to_string(aliveCount) + "/" + std::to_string(level.getAllEnemyCount()));

        window.draw(debugText);

        if (isPaused) {


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

            // glow
            sf::Text glow = title;
            glow.setFillColor(sf::Color(80, 180, 255, 80));
            glow.move(2, 2);

            window.draw(glow);

            title.setFillColor(sf::Color::White);
            window.draw(title);

            //   OPTIONS                  
            for (int i = 0; i < 5; i++) {

                float y = PY + 100 + i * 50;

                sf::Text option;
                option.setFont(font);
                option.setCharacterSize(24);
                option.setString(pauseOptions[i]);

                // center text
                sf::FloatRect ob = option.getLocalBounds();
                option.setOrigin(ob.left + ob.width / 2.f, ob.top);
                option.setPosition(500, y);

                if (i == pauseIndex) {
                    // highlight bar
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


        }

    }
    window.display();
    window.setView(window.getDefaultView());  // restore after screen shake (1P)
}



Level& Game::getLevel() {
    return level;
}

void Game::setStateToPlaying() {
    currentState = PLAYING;
}