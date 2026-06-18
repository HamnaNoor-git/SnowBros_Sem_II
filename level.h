#pragma once
#include "platform.h"
#include "botom.h"
#include "flying_fooga.h"
#include "tornado.h"
#include "mogera.h"
#include "mogera_child.h"
#include "coin.h"
#include "gamakichi.h"

class Level {
private:
    Platform platforms[10];
    int platformCount;

    int currentLevel;

    Botom botoms[20];
    int botomCount;

    FlyingFooga foogas[20];
    int foogaCount;

    Tornado tornados[10];
    int tornadoCount;

    Mogera* mogera;
    bool hasMogera;

    MogeraChild children[20];
    int childCount;

    int coins;

    Coin coinsDrop[20];
    int coinDropCount;

    Gamakichi* gamakichi;
    bool hasGamakichi;

    sf::Texture platformTex1;
    sf::Texture platformTex2;

    Enemy* allEnemies[50];
    int allEnemyCount;

public:
    Level();
    ~Level();   

    void loadLevel(int levelNumber);

    Platform* getPlatforms();
    int getPlatformCount();

    int getCurrentLevel();

    Botom* getBotoms();
    int getBotomCount();

    FlyingFooga* getFoogas();
    int getFoogaCount();

    Tornado* getTornados();
    int getTornadoCount();

    Mogera* getMogera();
    bool isMogeraActive();

    MogeraChild* getChildren();
    int getChildCount();
    void incrementChildCount();

    int getCoins();
    void addCoins(int c);

    Coin* getCoinsDrop();
    int getCoinDropCount();
    void setCoinDropCount(int c);

    Gamakichi* getGamakichi();
    bool isGamakichiActive();

    Enemy** getAllEnemies();
    int getAllEnemyCount();
    void addEnemy(Enemy* enemy);
};