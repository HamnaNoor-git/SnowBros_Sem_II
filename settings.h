#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
using namespace std;

enum class Difficulty {
    EASY = 0,
    NORMAL = 1,
    HARD = 2
};

class Settings {
private:
    sf::Font font;

    // UI elements
    sf::Text titleText;

    // Volume
    sf::Text volumeLabel;
    sf::Text volumeValue;
    sf::RectangleShape volumeBarBg;
    sf::RectangleShape volumeBar;

    // Difficulty
    sf::Text difficultyLabel;
    sf::Text difficultyOptions[3];
    string difficultyNames[3];

    // Back
    sf::Text backText;

    int selectedRow;   // 0 = volume, 1 = difficulty, 2 = back
    int selectedDiff;  // 0=Easy 1=Normal 2=Hard

    float volume;      // 0..100

public:
    Settings();

    void handleInput(sf::Event& event, bool& goBack, sf::Music& music);
    void draw(sf::RenderWindow& window);

    float getVolume() const { return volume; }
    Difficulty getDifficulty() const { return static_cast<Difficulty>(selectedDiff); }

    void applyVolume(sf::Music& music);
};
