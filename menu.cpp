#include "menu.h"
#include "leaderboard.h"

Menu::Menu() {

    font.loadFromFile("arial.ttf");

    // TITLE
    title.setFont(font);
    title.setString("SNOW BROS");
    title.setCharacterSize(60);
    title.setFillColor(sf::Color::White);
    title.setPosition(300, 100);

    // OPTIONS
    for (int i = 0; i < 5; i++) {

        options[i].setFont(font);
        options[i].setString(optionNames[i]);
        options[i].setCharacterSize(30);

        options[i].setPosition(400, 250 + i * 60);
    }

    selectedIndex = 0;
    options[0].setFillColor(sf::Color::Yellow);

    volume = 100;
}


// INPUT

void Menu::handleInput(sf::Event& event, bool& startGame, bool& exitGame, bool& showLeaderboard, bool& showSettings, sf::RenderWindow& window)
{

    if (event.type == sf::Event::KeyPressed) {

        if (event.key.code == sf::Keyboard::Up)
            moveUp();

        if (event.key.code == sf::Keyboard::Down)
            moveDown();

        if (event.key.code == sf::Keyboard::Enter) {

            if (selectedIndex == 0) {
                startGame = true;
            }
            else if (selectedIndex == 1) {
                startGame = true; 
            }
            else if (selectedIndex == 2) {
                showLeaderboard = true;
            }
            else if (selectedIndex == 3) {
                showSettings = true;
            }
            else if (selectedIndex == 4) {
                exitGame = true;
            }
        }
    }


// MOUSE CLICK

    if (event.type == sf::Event::MouseButtonPressed) {

        int mouseX = sf::Mouse::getPosition(window).x;
        int mouseY = sf::Mouse::getPosition(window).y;

        for (int i = 0; i < 5; i++) {

            if (options[i].getGlobalBounds().contains(mouseX, mouseY)) {

                selectedIndex = i;

                if (i == 0 || i == 1)
                    startGame = true;

                else if (i == 4)
                    exitGame = true;

                else if (i == 2)
                    showLeaderboard = true;

                else if (i == 3)
                    showSettings = true;
            }
        }
    }
}


// DRAW

void Menu::draw(sf::RenderWindow& window) {

    const float WW = static_cast<float>(window.getSize().x); 
    const float WH = static_cast<float>(window.getSize().y); 
    const float CX = WW / 2.f;


    //   PANEL 
    const float PW = 500, PH = 480;
    const float PX = CX - PW / 2.f;
    const float PY = WH / 2.f - PH / 2.f;

    sf::RectangleShape panel(sf::Vector2f(PW, PH));
    panel.setPosition(PX, PY);
    panel.setFillColor(sf::Color(1, 13, 2, 220));
    panel.setOutlineThickness(1.5f);
    panel.setOutlineColor(sf::Color(26, 92, 32));
    window.draw(panel);

    // Corner brackets
    auto corner = [&](float x, float y, float sx, float sy) {
        sf::RectangleShape h(sf::Vector2f(30, 2));
        h.setPosition(x, y); h.setFillColor(sf::Color(62, 207, 90));
        window.draw(h);
        sf::RectangleShape v(sf::Vector2f(2, 30));
        v.setPosition(x, y); v.setFillColor(sf::Color(62, 207, 90));
        window.draw(v);
        (void)sx; (void)sy;
        };
    corner(PX, PY, 1, 1);
    corner(PX + PW - 30, PY, -1, 1);
    corner(PX, PY + PH - 30, 1, -1);
    corner(PX + PW - 30, PY + PH - 30, -1, -1);

    //   BADGE 
    sf::RectangleShape badge(sf::Vector2f(180, 20));
    badge.setPosition(CX - 90, PY + 22);
    badge.setFillColor(sf::Color(62, 207, 90, 18));
    badge.setOutlineThickness(1);
    badge.setOutlineColor(sf::Color(62, 207, 90, 60));
    window.draw(badge);

    sf::Text badgeTxt;
    badgeTxt.setFont(font);
    badgeTxt.setCharacterSize(11);
    badgeTxt.setLetterSpacing(3.5f);
    badgeTxt.setFillColor(sf::Color(62, 207, 90));
    badgeTxt.setString("* SNOW BROS *");
    auto bBounds = badgeTxt.getLocalBounds();
    badgeTxt.setPosition(CX - bBounds.width / 2.f, PY + 24);
    window.draw(badgeTxt);

    //   TITLE (centred) 
    title.setFillColor(sf::Color(255, 255, 255));
    title.setCharacterSize(42);
    {
        auto tb = title.getLocalBounds();
        title.setOrigin(tb.left + tb.width / 2.f, tb.top);
        title.setPosition(CX, PY + 50);
    }
    window.draw(title);

    //   SUBTITLE      
    sf::Text sub;
    sub.setFont(font);
    sub.setCharacterSize(12);
    sub.setLetterSpacing(5.0f);
    sub.setFillColor(sf::Color(62, 207, 90));
    sub.setString("FROZEN  REALM");
    {
        auto sb = sub.getLocalBounds();
        sub.setOrigin(sb.left + sb.width / 2.f, sb.top);
        sub.setPosition(CX, PY + 100);
    }
    window.draw(sub);

    //   DIVIDER  
    const float divY = PY + 126;
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

    //   MENU OPTIONS (centred)             
    const float ITEM_W = 340;
    const float ITEM_H = 44;
    const float ITEM_GAP = 10;
    const float ITEMS_START_Y = PY + 148;

    for (int i = 0; i < 5; i++) {
        float itemX = CX - ITEM_W / 2.f;
        float itemY = ITEMS_START_Y + i * (ITEM_H + ITEM_GAP);

        if (i == selectedIndex) {
            // Active highlight bar
            sf::RectangleShape hlBar(sf::Vector2f(ITEM_W, ITEM_H));
            hlBar.setPosition(itemX, itemY);
            hlBar.setFillColor(sf::Color(62, 207, 90, 25));
            hlBar.setOutlineThickness(1.5f);
            hlBar.setOutlineColor(sf::Color(62, 207, 90));
            window.draw(hlBar);

            // Side indicator dots
            sf::CircleShape dotL(5);
            dotL.setFillColor(sf::Color(62, 207, 90));
            dotL.setPosition(itemX - 18, itemY + ITEM_H / 2.f - 5);
            window.draw(dotL);

            sf::CircleShape dotR(5);
            dotR.setFillColor(sf::Color(62, 207, 90));
            dotR.setPosition(itemX + ITEM_W + 8, itemY + ITEM_H / 2.f - 5);
            window.draw(dotR);

            options[i].setFillColor(sf::Color(255, 255, 255));
        }
        else {
            options[i].setFillColor(sf::Color(93, 184, 112));
        }

        // Centre each option text
        options[i].setCharacterSize(16);
        options[i].setLetterSpacing(2.5f);
        {
            auto ob = options[i].getLocalBounds();
            options[i].setOrigin(ob.left + ob.width / 2.f, ob.top + ob.height / 2.f);
            options[i].setPosition(CX, itemY + ITEM_H / 2.f);
        }
        window.draw(options[i]);
    }

    //   HOTKEY ROW (centred)                    
    auto drawKey = [&](const std::string& key, const std::string& desc, float x, float y) {
        sf::RectangleShape kb(sf::Vector2f(32, 18));
        kb.setPosition(x, y);
        kb.setFillColor(sf::Color(62, 207, 90, 20));
        kb.setOutlineThickness(1);
        kb.setOutlineColor(sf::Color(62, 207, 90, 50));
        window.draw(kb);

        sf::Text kt;
        kt.setFont(font); kt.setCharacterSize(10);
        kt.setFillColor(sf::Color(62, 207, 90));
        kt.setString(key);
        kt.setPosition(x + 5, y + 3);
        window.draw(kt);

        sf::Text dt;
        dt.setFont(font); dt.setCharacterSize(11);
        dt.setFillColor(sf::Color(42, 107, 48));
        dt.setString(desc);
        dt.setPosition(x + 37, y + 2);
        window.draw(dt);
        };

    float helpY = PY + PH - 44;
    drawKey("W/S", "navigate", CX - 170, helpY);
    drawKey("ENT", "select", CX - 50, helpY);
    drawKey("ESC", "back", CX + 60, helpY);
}


// NAVIGATION

void Menu::moveUp() {

    options[selectedIndex].setFillColor(sf::Color::White);

    selectedIndex ;

    if (selectedIndex < 0)
        selectedIndex = 4;

    options[selectedIndex].setFillColor(sf::Color::Yellow);
}

void Menu::moveDown() {

    options[selectedIndex].setFillColor(sf::Color::White);

    selectedIndex++;

    if (selectedIndex > 4)
        selectedIndex = 0;

    options[selectedIndex].setFillColor(sf::Color::Yellow);
}

int Menu::getSelectedIndex() {
    return selectedIndex;
}