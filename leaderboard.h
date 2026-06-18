#pragma once
#include <SFML/Graphics.hpp>

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct ScoreEntry {
    string name;
    int score;
};

class Leaderboard {
private:
    static const int MAX = 10;
    ScoreEntry entries[MAX];
    int count;

public:
    Leaderboard();

    void loadFromFile();
    void saveToFile();
    void addScore(string name, int score);
    void sortScores();
    void display();
    void draw(sf::RenderWindow& window, sf::Font& font);
};