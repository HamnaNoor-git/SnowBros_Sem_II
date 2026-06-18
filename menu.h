#pragma once
#include <SFML/Graphics.hpp>
using namespace std;

class Menu {
private:
    sf::Font font;

    sf::Text title;

    sf::Text options[5];
    string optionNames[5] = {
        "New Game",
        "Continue",
        "Leaderboard",
        "Settings",
        "Exit"
    };

    int selectedIndex;

    float volume;

public:
    Menu();

    void handleInput(sf::Event& event, bool& startGame, bool& exitGame, bool& showLeaderboard, bool& showSettings, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

    void moveUp();
    void moveDown();

    int getSelectedIndex();
};