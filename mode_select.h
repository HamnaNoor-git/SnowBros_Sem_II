#pragma once
#include <SFML/Graphics.hpp>
#include <string>
using namespace std;


class ModeSelect {
public:
    ModeSelect();

    void handleInput(sf::Event& event,
        bool& chose1P,
        bool& chose2P,
        bool& showHelp,
        bool& goBack,
        sf::RenderWindow& window);

    void draw(sf::RenderWindow& window);

    void moveUp();
    void moveDown();

    int getSelectedIndex() const;

private:
    sf::Font font;

    static const int OPT_COUNT = 3;
    sf::Text options[OPT_COUNT];
    string   optionNames[OPT_COUNT] = { "1 Player", "2 Players", "Help" };

    int selectedIndex = 0;

    bool helpVisible = false;

    void drawHelp(sf::RenderWindow& window);
};