#include "leaderboard.h"

Leaderboard::Leaderboard() {
    count = 0;
}

void Leaderboard::loadFromFile() {
    ifstream file("leaderboard.txt");

    count = 0;

    if (!file.is_open())
        return;

    while (file >> entries[count].name >> entries[count].score) {
        count++;
        if (count >= MAX) break;
    }

    file.close();
}

void Leaderboard::saveToFile() {
    ofstream file("leaderboard.txt");

    for (int i = 0; i < count; i++) {
        file << entries[i].name << " " << entries[i].score << endl;
    }

    file.close();
}

void Leaderboard::addScore(string name, int score) {
    if (count < MAX) {
        entries[count].name = name;
        entries[count].score = score;
        count++;
    }
    else {
        // replace lowest if new score is higher
        int minIndex = 0;
        for (int i = 1; i < MAX; i++) {
            if (entries[i].score < entries[minIndex].score)
                minIndex = i;
        }

        if (score > entries[minIndex].score) {
            entries[minIndex].name = name;
            entries[minIndex].score = score;
        }
    }

    sortScores();
    saveToFile();
}

void Leaderboard::sortScores() {
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (entries[j].score > entries[i].score) {
                swap(entries[i], entries[j]);
            }
        }
    }
}


void Leaderboard::draw(sf::RenderWindow& window, sf::Font& font) {

    float WW = window.getSize().x;
    float WH = window.getSize().y;
    float CX = WW / 2.f;

    // SOFT DARK FADE (so text is readable on bg)
    sf::RectangleShape fade(sf::Vector2f(WW, WH));
    fade.setFillColor(sf::Color(0, 0, 0, 90));
    window.draw(fade);

    //  PANEL (CENTER BOX)
    float PW = 550, PH = 650;
    float PX = CX - PW / 2.f;
    float PY = 100;

    sf::RectangleShape panel(sf::Vector2f(PW, PH));
    panel.setPosition(PX, PY);
    panel.setFillColor(sf::Color(0, 20, 5, 220));
    panel.setOutlineThickness(2);
    panel.setOutlineColor(sf::Color(62, 207, 90));
    window.draw(panel);

    // TITLE (with glow)
    

    sf::Text title;
    title.setFont(font);
    title.setCharacterSize(40);
    title.setFillColor(sf::Color::White);
    title.setString("LEADERBOARD");
    title.setPosition(CX - 152, PY + 18);
    window.draw(title);

    //  DIVIDER
    
    sf::RectangleShape line(sf::Vector2f(400, 1));
    line.setPosition(CX - 200, PY + 80);
    line.setFillColor(sf::Color(62, 207, 90, 150));
    window.draw(line);

    // ENTRIES
    
    for (int i = 0; i < count; i++) {

        float y = PY + 110 + i * 45;

        // Highlight top 3
        sf::Color textColor = sf::Color(150, 255, 180);

        if (i == 0) textColor = sf::Color(255, 215, 0);    
        else if (i == 1) textColor = sf::Color(200, 200, 200); 
        else if (i == 2) textColor = sf::Color(205, 127, 50); 

        // Background highlight (only for selected ranks)
        if (i < 3) {
            sf::RectangleShape highlight(sf::Vector2f(480, 36));
            highlight.setPosition(CX - 240, y - 5);
            highlight.setFillColor(sf::Color(62, 207, 90, 25));
            window.draw(highlight);
        }

        sf::Text entry;
        entry.setFont(font);
        entry.setCharacterSize(24);

        string line =
            to_string(i + 1) + ". " +
            entries[i].name +
            "     " +
            to_string(entries[i].score);

        entry.setString(line);
        entry.setFillColor(textColor);
        entry.setPosition(CX - 220, y);

        window.draw(entry);
    }

    // BACK BUTTON (BOTTOM)
    sf::RectangleShape backBox(sf::Vector2f(300, 40));
    backBox.setPosition(CX - 150, PY + PH - 70);
    backBox.setFillColor(sf::Color(62, 207, 90, 30));
    backBox.setOutlineThickness(1);
    backBox.setOutlineColor(sf::Color(62, 207, 90));
    window.draw(backBox);

    sf::Text back;
    back.setFont(font);
    back.setCharacterSize(18);
    back.setFillColor(sf::Color::White);
    back.setString("PRESS ESC TO GO BACK");
    back.setPosition(CX - 130, PY + PH - 60);

    window.draw(back);
}