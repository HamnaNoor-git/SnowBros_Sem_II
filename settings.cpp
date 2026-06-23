#include "settings.h"

Settings::Settings() {
    font.loadFromFile("arial.ttf");

    difficultyNames[0] = "Easy";
    difficultyNames[1] = "Normal";
    difficultyNames[2] = "Hard";

    volume = 100.f;
    selectedRow = 0;
    selectedDiff = 1; // Normal default

    // Title
    titleText.setFont(font);
    titleText.setString("SETTINGS");
    titleText.setCharacterSize(42);
    titleText.setFillColor(sf::Color::White);

    // Volume label
    volumeLabel.setFont(font);
    volumeLabel.setString("VOLUME");
    volumeLabel.setCharacterSize(20);
    volumeLabel.setLetterSpacing(2.5f);

    volumeValue.setFont(font);
    volumeValue.setCharacterSize(20);

    // Volume bar background
    volumeBarBg.setSize(sf::Vector2f(300, 12));
    volumeBarBg.setFillColor(sf::Color(30, 80, 40, 180));
    volumeBarBg.setOutlineThickness(1);
    volumeBarBg.setOutlineColor(sf::Color(62, 207, 90, 100));

    // Volume bar fill
    volumeBar.setSize(sf::Vector2f(300, 12));
    volumeBar.setFillColor(sf::Color(62, 207, 90));

    // Difficulty label
    difficultyLabel.setFont(font);
    difficultyLabel.setString("DIFFICULTY");
    difficultyLabel.setCharacterSize(20);
    difficultyLabel.setLetterSpacing(2.5f);

    for (int i = 0; i < 3; i++) {
        difficultyOptions[i].setFont(font);
        difficultyOptions[i].setString(difficultyNames[i]);
        difficultyOptions[i].setCharacterSize(18);
        difficultyOptions[i].setLetterSpacing(2.0f);
    }

    backText.setFont(font);
    backText.setString("< BACK");
    backText.setCharacterSize(18);
    backText.setLetterSpacing(2.5f);
}

void Settings::applyVolume(sf::Music& music) {
    music.setVolume(volume);
}

void Settings::handleInput(sf::Event& event, bool& goBack, sf::Music& music) {
    if (event.type != sf::Event::KeyPressed) return;

    sf::Keyboard::Key key = event.key.code;

    if (key == sf::Keyboard::Up) {
        selectedRow ;
        if (selectedRow < 0) selectedRow = 2;
    }
    else if (key == sf::Keyboard::Down) {
        selectedRow++;
        if (selectedRow > 2) selectedRow = 0;
    }

    // Volume row
    if (selectedRow == 0) {
        if (key == sf::Keyboard::Left) {
            volume -= 10.f;
            if (volume < 0.f) volume = 0.f;
            applyVolume(music);
        }
        else if (key == sf::Keyboard::Right) {
            volume += 10.f;
            if (volume > 100.f) volume = 100.f;
            applyVolume(music);
        }
    }

    // Difficulty row
    if (selectedRow == 1) {
        if (key == sf::Keyboard::Left) {
            selectedDiff ;
            if (selectedDiff < 0) selectedDiff = 0;
        }
        else if (key == sf::Keyboard::Right) {
            selectedDiff++;
            if (selectedDiff > 2) selectedDiff = 2;
        }
    }

    // Back row
    if (selectedRow == 2 && key == sf::Keyboard::Enter) {
        goBack = true;
    }

    if (key == sf::Keyboard::Escape) {
        goBack = true;
    }
}

void Settings::draw(sf::RenderWindow& window) {
    const float WW = static_cast<float>(window.getSize().x);
    const float WH = static_cast<float>(window.getSize().y);
    const float CX = WW / 2.f;

    // Panel
    const float PW = 520, PH = 500;
    const float PX = CX - PW / 2.f;
    const float PY = WH / 2.f - PH / 2.f;

    sf::RectangleShape panel(sf::Vector2f(PW, PH));
    panel.setPosition(PX, PY);
    panel.setFillColor(sf::Color(1, 13, 2, 230));
    panel.setOutlineThickness(1.5f);
    panel.setOutlineColor(sf::Color(26, 92, 32));
    window.draw(panel);

    // Corner brackets
    auto corner = [&](float x, float y) {
        sf::RectangleShape h(sf::Vector2f(30, 2));
        h.setPosition(x, y); h.setFillColor(sf::Color(62, 207, 90));
        window.draw(h);
        sf::RectangleShape v(sf::Vector2f(2, 30));
        v.setPosition(x, y); v.setFillColor(sf::Color(62, 207, 90));
        window.draw(v);
        };
    corner(PX, PY);
    corner(PX + PW - 30, PY);
    corner(PX, PY + PH - 30);
    corner(PX + PW - 30, PY + PH - 30);

    // Title
    titleText.setCharacterSize(42);
    {
        auto tb = titleText.getLocalBounds();
        titleText.setOrigin(tb.left + tb.width / 2.f, tb.top);
        titleText.setPosition(CX, PY + 30);
    }
    window.draw(titleText);

    // Divider
    float divY = PY + 90;
    sf::RectangleShape dL(sf::Vector2f(160, 1));
    dL.setPosition(CX - 172, divY);
    dL.setFillColor(sf::Color(30, 102, 40, 160));
    window.draw(dL);
    sf::CircleShape dot(4);
    dot.setFillColor(sf::Color(62, 207, 90));
    dot.setPosition(CX - 4, divY - 4);
    window.draw(dot);
    sf::RectangleShape dR(sf::Vector2f(160, 1));
    dR.setPosition(CX + 12, divY);
    dR.setFillColor(sf::Color(30, 102, 40, 160));
    window.draw(dR);

    //   ROW 0: VOLUME     
    float rowY0 = PY + 130;
    bool volSelected = (selectedRow == 0);

    volumeLabel.setFillColor(volSelected ? sf::Color(255, 255, 255) : sf::Color(93, 184, 112));
    {
        auto lb = volumeLabel.getLocalBounds();
        volumeLabel.setOrigin(lb.left + lb.width / 2.f, lb.top);
        volumeLabel.setPosition(CX, rowY0);
    }
    window.draw(volumeLabel);

    // Arrows + bar
    float barW = 300.f;
    float barX = CX - barW / 2.f;
    float barY = rowY0 + 38;

    // Left arrow
    sf::Text arrowL;
    arrowL.setFont(font);
    arrowL.setCharacterSize(22);
    arrowL.setString("<");
    arrowL.setFillColor(volSelected ? sf::Color(62, 207, 90) : sf::Color(40, 100, 50));
    arrowL.setPosition(barX - 30, barY - 4);
    window.draw(arrowL);

    // Right arrow
    sf::Text arrowR;
    arrowR.setFont(font);
    arrowR.setCharacterSize(22);
    arrowR.setString(">");
    arrowR.setFillColor(volSelected ? sf::Color(62, 207, 90) : sf::Color(40, 100, 50));
    arrowR.setPosition(barX + barW + 8, barY - 4);
    window.draw(arrowR);

    // Bar background
    volumeBarBg.setSize(sf::Vector2f(barW, 12));
    volumeBarBg.setPosition(barX, barY);
    window.draw(volumeBarBg);

    // Bar fill
    float fillW = (volume / 100.f) * barW;
    if (fillW < 1.f) fillW = 0.f;
    volumeBar.setSize(sf::Vector2f(fillW, 12));
    volumeBar.setPosition(barX, barY);
    volumeBar.setFillColor(volSelected ? sf::Color(62, 207, 90) : sf::Color(40, 140, 60));
    window.draw(volumeBar);

    // Volume percent
    int volInt = (int)volume;
    string volStr = to_string(volInt) + "%";
    volumeValue.setString(volStr);
    volumeValue.setFillColor(volSelected ? sf::Color(255, 255, 255) : sf::Color(93, 184, 112));
    {
        auto vb = volumeValue.getLocalBounds();
        volumeValue.setOrigin(vb.left + vb.width / 2.f, vb.top);
        volumeValue.setPosition(CX, barY + 20);
    }
    window.draw(volumeValue);

    //   ROW 1: DIFFICULTY   
    float rowY1 = PY + 260;
    bool diffSelected = (selectedRow == 1);

    difficultyLabel.setFillColor(diffSelected ? sf::Color(255, 255, 255) : sf::Color(93, 184, 112));
    {
        auto lb = difficultyLabel.getLocalBounds();
        difficultyLabel.setOrigin(lb.left + lb.width / 2.f, lb.top);
        difficultyLabel.setPosition(CX, rowY1);
    }
    window.draw(difficultyLabel);

    // Draw 3 difficulty boxes
    float boxW = 110, boxH = 44, boxGap = 14;
    float totalW = 3 * boxW + 2 * boxGap;
    float boxStartX = CX - totalW / 2.f;
    float boxY = rowY1 + 36;

    // Arrows for difficulty
    sf::Text arrowDL;
    arrowDL.setFont(font);
    arrowDL.setCharacterSize(22);
    arrowDL.setString("<");
    arrowDL.setFillColor(diffSelected ? sf::Color(62, 207, 90) : sf::Color(40, 100, 50));
    arrowDL.setPosition(boxStartX - 30, boxY + 10);
    window.draw(arrowDL);

    sf::Text arrowDR;
    arrowDR.setFont(font);
    arrowDR.setCharacterSize(22);
    arrowDR.setString(">");
    arrowDR.setFillColor(diffSelected ? sf::Color(62, 207, 90) : sf::Color(40, 100, 50));
    arrowDR.setPosition(boxStartX + totalW + 8, boxY + 10);
    window.draw(arrowDR);

    for (int i = 0; i < 3; i++) {
        float bx = boxStartX + i * (boxW + boxGap);
        bool active = (i == selectedDiff);

        sf::RectangleShape box(sf::Vector2f(boxW, boxH));
        box.setPosition(bx, boxY);
        if (active) {
            // Colour based on difficulty
            if (i == 0) box.setFillColor(sf::Color(0, 120, 60, 220));
            else if (i == 1) box.setFillColor(sf::Color(180, 120, 0, 220));
            else  box.setFillColor(sf::Color(160, 20, 20, 220));
            box.setOutlineThickness(2.f);
            box.setOutlineColor(sf::Color(255, 255, 255, 200));
        }
        else {
            box.setFillColor(sf::Color(10, 30, 15, 180));
            box.setOutlineThickness(1.f);
            box.setOutlineColor(sf::Color(30, 80, 40, 120));
        }
        window.draw(box);

        difficultyOptions[i].setString(difficultyNames[i]);
        if (active) difficultyOptions[i].setFillColor(sf::Color(255, 255, 255));
        else        difficultyOptions[i].setFillColor(sf::Color(60, 140, 80));

        {
            auto ob = difficultyOptions[i].getLocalBounds();
            difficultyOptions[i].setOrigin(ob.left + ob.width / 2.f, ob.top + ob.height / 2.f);
            difficultyOptions[i].setPosition(bx + boxW / 2.f, boxY + boxH / 2.f);
        }
        window.draw(difficultyOptions[i]);
    }

    // Difficulty hint
    sf::Text hint;
    hint.setFont(font);
    hint.setCharacterSize(13);
    hint.setLetterSpacing(1.5f);
    if (selectedDiff == 0)      hint.setString("Slower enemies  |  More cooldown  |  Fewer enemies  |  Less boss HP");
    else if (selectedDiff == 1) hint.setString("Balanced speed  |  Normal cooldown  |  Normal enemies  |  Normal boss HP");
    else                        hint.setString("Faster enemies  |  Less cooldown  |  More enemies  |  More boss HP");
    hint.setFillColor(sf::Color(80, 180, 100, 200));
    {
        auto hb = hint.getLocalBounds();
        hint.setOrigin(hb.left + hb.width / 2.f, hb.top);
        hint.setPosition(CX, boxY + boxH + 10);
    }
    window.draw(hint);

    //   ROW 2: BACK      ─
    float rowY2 = PY + PH - 60;
    bool backSel = (selectedRow == 2);

    if (backSel) {
        sf::RectangleShape hlBar(sf::Vector2f(160, 36));
        hlBar.setPosition(CX - 80, rowY2 - 4);
        hlBar.setFillColor(sf::Color(62, 207, 90, 25));
        hlBar.setOutlineThickness(1.5f);
        hlBar.setOutlineColor(sf::Color(62, 207, 90));
        window.draw(hlBar);
    }

    backText.setFillColor(backSel ? sf::Color(255, 255, 255) : sf::Color(93, 184, 112));
    {
        auto bb = backText.getLocalBounds();
        backText.setOrigin(bb.left + bb.width / 2.f, bb.top);
        backText.setPosition(CX, rowY2);
    }
    window.draw(backText);

    //   HOTKEYS        
    sf::Text hint2;
    hint2.setFont(font);
    hint2.setCharacterSize(11);
    hint2.setFillColor(sf::Color(42, 107, 48));
    hint2.setString("UP/DOWN: navigate    LEFT/RIGHT: change    ENTER/ESC: back");
    {
        auto hb2 = hint2.getLocalBounds();
        hint2.setOrigin(hb2.left + hb2.width / 2.f, hb2.top);
        hint2.setPosition(CX, PY + PH - 22);
    }
    window.draw(hint2);
}
