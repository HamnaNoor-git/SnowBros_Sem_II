#include "level.h"
#include <iostream>

Level::Level() {
    platformCount = 0;
    currentLevel = 1;
    botomCount = 0;
    foogaCount = 0;
    tornadoCount = 0;
    childCount = 0;
    coinDropCount = 0;
    hasMogera = false;
    hasGamakichi = false;
    coins = 0;
    allEnemyCount = 0;

    //initialise pointers to nullptr — never leave dangling
    mogera = nullptr;
    gamakichi = nullptr;

    platformTex1.loadFromFile("plateform.png");
    platformTex2.loadFromFile("plateform2.png");
}

// destructor cleans up heap objects
Level::~Level() {
    delete mogera;
    delete gamakichi;
}

void Level::loadLevel(int levelNumber) {
    //   RESET     
    allEnemyCount = 0;
    currentLevel = levelNumber;
    platformCount = 0;
    botomCount = 0;
    foogaCount = 0;
    tornadoCount = 0;
    childCount = 0;
    coinDropCount = 0;

    // delete old boss objects before creating new ones
    delete mogera;
    mogera = nullptr;
    hasMogera = false;

    delete gamakichi;
    gamakichi = nullptr;
    hasGamakichi = false;

    // LEVEL 1
    if (levelNumber == 1) {
        platformCount = 4;
        platforms[0] = Platform(0, 750, 1000, 50);
        platforms[1] = Platform(0, 600, 250, 40);
        platforms[2] = Platform(750, 300, 250, 40);
        platforms[3] = Platform(400, 450, 250, 40);

        botomCount = 2;
        botoms[0].reset();
        botoms[0].setPosition(300, 100);
        addEnemy(&botoms[0]);

        botoms[1].reset();
        botoms[1].setPosition(600, 100);
        addEnemy(&botoms[1]);
    }

    // LEVEL 2
    else if (levelNumber == 2) {
        platformCount = 5;
        platforms[0] = Platform(0, 750, 1000, 50);
        platforms[1] = Platform(150, 600, 250, 40);
        platforms[2] = Platform(600, 600, 250, 40);
        platforms[3] = Platform(150, 400, 250, 40);
        platforms[4] = Platform(600, 400, 250, 40);

        botomCount = 3;
        botoms[0].reset(); botoms[0].setPosition(200, 100); addEnemy(&botoms[0]);
        botoms[1].reset(); botoms[1].setPosition(500, 100); addEnemy(&botoms[1]);
        botoms[2].reset(); botoms[2].setPosition(800, 100); addEnemy(&botoms[2]);

        foogaCount = 2;
        foogas[0] = FlyingFooga(); foogas[0].setPosition(300, 100); addEnemy(&foogas[0]);
        foogas[1] = FlyingFooga(); foogas[1].setPosition(700, 100); addEnemy(&foogas[1]);
    }

    // LEVEL 3
    else if (levelNumber == 3) {
        platformCount = 6;
        platforms[0] = Platform(0, 750, 1000, 50);
        platforms[1] = Platform(150, 600, 250, 40);
        platforms[2] = Platform(600, 600, 250, 40);
        platforms[3] = Platform(150, 300, 250, 40);
        platforms[4] = Platform(600, 300, 250, 40);
        platforms[5] = Platform(370, 450, 250, 40);

        botomCount = 3;
        botoms[0].reset(); botoms[0].setPosition(150, 100); addEnemy(&botoms[0]);
        botoms[1].reset(); botoms[1].setPosition(450, 100); addEnemy(&botoms[1]);
        botoms[2].reset(); botoms[2].setPosition(750, 100); addEnemy(&botoms[2]);

        foogaCount = 2;
        foogas[0] = FlyingFooga(); foogas[0].setPosition(250, 100); addEnemy(&foogas[0]);
        foogas[1] = FlyingFooga(); foogas[1].setPosition(650, 100); addEnemy(&foogas[1]);
    }

    
    // LEVEL 4

    else if (levelNumber == 4) {
        platformCount = 4;

        platforms[0] = Platform(0, 750, 1000, 50);
        platforms[1] = Platform(150, 500, 250, 40);
        platforms[2] = Platform(600, 500, 250, 40);
        platforms[3] = Platform(350, 300, 300, 40);

        botomCount = 0;
        foogaCount = 0;
        tornadoCount = 0;
    }
    // LEVEL 5 — MOGERA BOSS
    else if (levelNumber == 5) {
        platformCount = 7;
        platforms[0] = Platform(0, 750, 1000, 50);
        platforms[1] = Platform(0, 500, 300, 40);
        platforms[2] = Platform(850, 400, 170, 40);
        platforms[3] = Platform(850, 450, 170, 40);
        platforms[4] = Platform(300, 600, 300, 40);
        platforms[5] = Platform(200, 350, 300, 40);
        platforms[6] = Platform(0, 200, 300, 40);

        hasMogera = true;
        mogera = new Mogera();
        float px = platforms[2].getBounds().left;
        float py = platforms[2].getBounds().top;
        mogera->setPosition(px, py - 120);
        addEnemy(mogera);
    }

    // LEVEL 6
    else if (levelNumber == 6) {
        platformCount = 7;
        platforms[0] = Platform(0, 750, 1000, 50);
        platforms[1] = Platform(150, 600, 250, 40);
        platforms[2] = Platform(600, 600, 250, 40);
        platforms[3] = Platform(150, 400, 250, 40);
        platforms[4] = Platform(600, 400, 250, 40);
        platforms[5] = Platform(150, 200, 250, 40);
        platforms[6] = Platform(600, 200, 250, 40);

        botomCount = 3;
        botoms[0].reset(); botoms[0].setPosition(150, 100); addEnemy(&botoms[0]);
        botoms[1].reset(); botoms[1].setPosition(450, 100); addEnemy(&botoms[1]);
        botoms[2].reset(); botoms[2].setPosition(750, 100); addEnemy(&botoms[2]);

        foogaCount = 2;
        foogas[0] = FlyingFooga(); foogas[0].setPosition(250, 100); addEnemy(&foogas[0]);
        foogas[1] = FlyingFooga(); foogas[1].setPosition(650, 100); addEnemy(&foogas[1]);
    }

    // LEVEL 7
    else if (levelNumber == 7) {
        platformCount = 7;
        platforms[0] = Platform(0, 750, 1000, 50);
        platforms[1] = Platform(0, 600, 300, 40);
        platforms[2] = Platform(700, 600, 300, 40);
        platforms[3] = Platform(420, 600, 200, 40);
        platforms[4] = Platform(150, 400, 300, 40);
        platforms[5] = Platform(600, 400, 300, 40);
        platforms[6] = Platform(350, 200, 350, 40);

        botomCount = 3;
        botoms[0].reset(); botoms[0].setPosition(150, 100); addEnemy(&botoms[0]);
        botoms[1].reset(); botoms[1].setPosition(450, 100); addEnemy(&botoms[1]);
        botoms[2].reset(); botoms[2].setPosition(750, 100); addEnemy(&botoms[2]);

        foogaCount = 2;
        foogas[0] = FlyingFooga(); foogas[0].setPosition(250, 100); addEnemy(&foogas[0]);
        foogas[1] = FlyingFooga(); foogas[1].setPosition(650, 100); addEnemy(&foogas[1]);
    }

    // LEVEL 8
    else if (levelNumber == 8) {
        platformCount = 6;
        platforms[0] = Platform(0, 750, 1000, 50);
        platforms[1] = Platform(0, 600, 300, 40);
        platforms[2] = Platform(0, 200, 300, 40);
        platforms[3] = Platform(300, 320, 300, 40);
        platforms[4] = Platform(300, 470, 300, 40);
        platforms[5] = Platform(700, 400, 300, 40);

        botomCount = 3;
        botoms[0].reset(); botoms[0].setPosition(150, 100); addEnemy(&botoms[0]);
        botoms[1].reset(); botoms[1].setPosition(450, 100); addEnemy(&botoms[1]);
        botoms[2].reset(); botoms[2].setPosition(750, 100); addEnemy(&botoms[2]);

        foogaCount = 2;
        foogas[0] = FlyingFooga(); foogas[0].setPosition(250, 100); addEnemy(&foogas[0]);
        foogas[1] = FlyingFooga(); foogas[1].setPosition(650, 100); addEnemy(&foogas[1]);
    }

    // LEVEL 9

    else if (levelNumber == 9) {
        platformCount = 4;

        platforms[0] = Platform(0, 750, 1000, 50);
        platforms[1] = Platform(150, 500, 250, 40);
        platforms[2] = Platform(600, 500, 250, 40);
        platforms[3] = Platform(350, 300, 300, 40);

        botomCount = 0;
        foogaCount = 0;
        tornadoCount = 0;
        }


    // LEVEL 10 — GAMAKICHI BOSS
    else if (levelNumber == 10) {
        platformCount = 6;
        platforms[0] = Platform(0, 750, 1000, 50);
        platforms[1] = Platform(200, 300, 600, 40);
        platforms[2] = Platform(200, 600, 100, 40);
        platforms[3] = Platform(700, 600, 100, 40);
        platforms[4] = Platform(0, 450, 300, 40);
        platforms[5] = Platform(700, 450, 300, 40);

        hasGamakichi = true;
        gamakichi = new Gamakichi();
        float groundTop = platforms[0].getBounds().top;
        gamakichi->setPosition(400, groundTop - 150);
        addEnemy(gamakichi);
    }

    // TORNADO — levels 3-9 except boss levels

    if (levelNumber >= 3 && levelNumber != 5 && levelNumber != 10
        && levelNumber != 4 && levelNumber != 9){
        tornadoCount = 1 + (levelNumber % 2);
        for (int i = 0; i < tornadoCount; i++) {
            tornados[i] = Tornado();
            tornados[i].setPosition(400 + i * 200, 100);
            addEnemy(&tornados[i]);
        }
    }

    // PLATFORM TEXTURE

    sf::Texture* currentTex = (levelNumber <= 5) ? &platformTex1 : &platformTex2;
    for (int i = 0; i < platformCount; i++)
        platforms[i].setTexture(currentTex);
}

// GETTERS
Platform* Level::getPlatforms() { 
    return platforms; 
}

int Level::getPlatformCount() { 
    return platformCount; 
}

int Level::getCurrentLevel() { 
    return currentLevel; }

Botom* Level::getBotoms() { 
    return botoms; 
}

int Level::getBotomCount() { 
    return botomCount; 
}

FlyingFooga* Level::getFoogas() { 
    return foogas; 
}

int Level::getFoogaCount() { 
    return foogaCount; 
}

Tornado* Level::getTornados() { 
    return tornados; 
}

int Level::getTornadoCount() { 
    return tornadoCount; 
}

Mogera* Level::getMogera() { 
    return mogera; 
}  

bool Level::isMogeraActive() { 
    return hasMogera; 
}

MogeraChild* Level::getChildren() { 
    return children; 
}

int Level::getChildCount() { 
    return childCount; 
}

void Level::incrementChildCount() { 
    if (childCount < 20) 
        childCount++; 
}

int Level::getCoins() { 
    return coins; 
}

void Level::addCoins(int c) { 
    coins += c; 
}

Coin* Level::getCoinsDrop() { 
    return coinsDrop; 
}

int Level::getCoinDropCount() { 
    return coinDropCount; 
}

void Level::setCoinDropCount(int c) { 
    coinDropCount = c; 
}

Gamakichi* Level::getGamakichi() { 
    return gamakichi; 
} 

bool Level::isGamakichiActive() { 
    return hasGamakichi; 
}

Enemy** Level::getAllEnemies() { 
    return allEnemies; 
}

int Level::getAllEnemyCount() { 
    return allEnemyCount; 
}

void Level::addEnemy(Enemy* enemy) {
    if (allEnemyCount < 50) {
        allEnemies[allEnemyCount] = enemy;
        allEnemyCount++;
    }
}