#include "mode_select.h"

//  Constructor
ModeSelect::ModeSelect() {
    font.loadFromFile("arial.ttf");

    for (int i = 0; i < OPT_COUNT; i++) {
        options[i].setFont(font);
        options[i].setString(optionNames[i]);
        options[i].setCharacterSize(22);
    }

    // Highlight first option
    options[0].setFillColor(sf::Color::Yellow);
    for (int i = 1; i < OPT_COUNT; i++)
        options[i].setFillColor(sf::Color(93, 184, 112));
}

//  Input
void ModeSelect::handleInput(sf::Event& event,
    bool& chose1P,
    bool& chose2P,
    bool& showHelp,
    bool& goBack,
    sf::RenderWindow& window) {
    if (helpVisible) {
        if (event.type == sf::Event::KeyPressed ||
            event.type == sf::Event::MouseButtonPressed) {
            helpVisible = false;
        }
        return;
    }

    if (event.type == sf::Event::KeyPressed) {

        if (event.key.code == sf::Keyboard::Up)    moveUp();
        if (event.key.code == sf::Keyboard::Down)  moveDown();
        if (event.key.code == sf::Keyboard::Escape) goBack = true;

        if (event.key.code == sf::Keyboard::Enter) {
            if (selectedIndex == 0) chose1P = true;
            else if (selectedIndex == 1) chose2P = true;
            else { helpVisible = true; showHelp = true; }
        }
    }

    // Mouse click
    if (event.type == sf::Event::MouseButtonPressed) {
        int mx = sf::Mouse::getPosition(window).x;
        int my = sf::Mouse::getPosition(window).y;
        for (int i = 0; i < OPT_COUNT; i++) {
            if (options[i].getGlobalBounds().contains((float)mx, (float)my)) {
                // De-highlight old
                options[selectedIndex].setFillColor(sf::Color(93, 184, 112));
                selectedIndex = i;
                options[selectedIndex].setFillColor(sf::Color::White);

                if (i == 0) chose1P = true;
                else if (i == 1) chose2P = true;
                else { helpVisible = true; showHelp = true; }
            }
        }
    }
}

//  Navigation
void ModeSelect::moveUp() {
    options[selectedIndex].setFillColor(sf::Color(93, 184, 112));
    selectedIndex = (selectedIndex - 1 + OPT_COUNT) % OPT_COUNT;
    options[selectedIndex].setFillColor(sf::Color::Yellow);
}

void ModeSelect::moveDown() {
    options[selectedIndex].setFillColor(sf::Color(93, 184, 112));
    selectedIndex = (selectedIndex + 1) % OPT_COUNT;
    options[selectedIndex].setFillColor(sf::Color::Yellow);
}

int ModeSelect::getSelectedIndex() const { return selectedIndex; }

//  Draw
void ModeSelect::draw(sf::RenderWindow& window) {
    const float WW = (float)window.getSize().x;
    const float WH = (float)window.getSize().y;
    const float CX = WW / 2.f;

    //   Panel       
    const float PW = 480.f, PH = 380.f;
    const float PX = CX - PW / 2.f;
    const float PY = WH / 2.f - PH / 2.f;

    sf::RectangleShape panel({ PW, PH });
    panel.setPosition(PX, PY);
    panel.setFillColor(sf::Color(1, 13, 2, 230));
    panel.setOutlineThickness(2.f);
    panel.setOutlineColor(sf::Color(26, 92, 32));
    window.draw(panel);

    // Corner brackets
    auto drawBracket = [&](float x, float y) {
        sf::RectangleShape h({ 28.f, 2.f });
        h.setPosition(x, y); h.setFillColor(sf::Color(62, 207, 90));
        window.draw(h);
        sf::RectangleShape v({ 2.f, 28.f });
        v.setPosition(x, y); v.setFillColor(sf::Color(62, 207, 90));
        window.draw(v);
        };
    drawBracket(PX, PY);
    drawBracket(PX + PW - 28, PY);
    drawBracket(PX, PY + PH - 28);
    drawBracket(PX + PW - 28, PY + PH - 28);

    //   Title       
    sf::Text title;
    title.setFont(font);
    title.setCharacterSize(36);
    title.setFillColor(sf::Color::White);
    title.setString("SELECT MODE");
    {
        auto tb = title.getLocalBounds();
        title.setOrigin(tb.left + tb.width / 2.f, tb.top);
        title.setPosition(CX, PY + 30.f);
    }
    window.draw(title);

    // Subtitle
    sf::Text sub;
    sub.setFont(font);
    sub.setCharacterSize(12);
    sub.setLetterSpacing(4.f);
    sub.setFillColor(sf::Color(62, 207, 90));
    sub.setString("SNOW  BROS");
    {
        auto sb2 = sub.getLocalBounds();
        sub.setOrigin(sb2.left + sb2.width / 2.f, sb2.top);
        sub.setPosition(CX, PY + 78.f);
    }
    window.draw(sub);

    // Divider
    sf::RectangleShape div({ 340.f, 1.f });
    div.setPosition(CX - 170.f, PY + 104.f);
    div.setFillColor(sf::Color(30, 102, 40, 160));
    window.draw(div);

    //   Options      
    const float ITEM_W = 320.f;
    const float ITEM_H = 50.f;
    const float ITEM_GAP = 14.f;
    const float START_Y = PY + 120.f;

    for (int i = 0; i < OPT_COUNT; i++) {
        float ix = CX - ITEM_W / 2.f;
        float iy = START_Y + i * (ITEM_H + ITEM_GAP);

        if (i == selectedIndex) {
            sf::RectangleShape hl({ ITEM_W, ITEM_H });
            hl.setPosition(ix, iy);
            hl.setFillColor(sf::Color(62, 207, 90, 28));
            hl.setOutlineThickness(1.5f);
            hl.setOutlineColor(sf::Color(62, 207, 90));
            window.draw(hl);

            // Side indicator dots
            sf::CircleShape dotL(5);
            dotL.setFillColor(sf::Color(62, 207, 90));
            dotL.setPosition(ix - 18.f, iy + ITEM_H / 2.f - 5.f);
            window.draw(dotL);
            sf::CircleShape dotR(5);
            dotR.setFillColor(sf::Color(62, 207, 90));
            dotR.setPosition(ix + ITEM_W + 8.f, iy + ITEM_H / 2.f - 5.f);
            window.draw(dotR);

            options[i].setFillColor(sf::Color::White);
        }
        else {
            options[i].setFillColor(sf::Color(93, 184, 112));
        }

        options[i].setCharacterSize(20);
        options[i].setLetterSpacing(2.f);
        {
            auto ob = options[i].getLocalBounds();
            options[i].setOrigin(ob.left + ob.width / 2.f,
                ob.top + ob.height / 2.f);
            options[i].setPosition(CX, iy + ITEM_H / 2.f);
        }
        window.draw(options[i]);
    }

    //   Hotkey row           
    auto drawKey = [&](const string& key, const string& desc, float x, float y) {
        sf::RectangleShape kb({ 32.f, 18.f });
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

        sf::Text dt2;
        dt2.setFont(font); dt2.setCharacterSize(11);
        dt2.setFillColor(sf::Color(42, 107, 48));
        dt2.setString(desc);
        dt2.setPosition(x + 37, y + 2);
        window.draw(dt2);
        };

    float helpY = PY + PH - 38.f;
    drawKey("W/S", "navigate", CX - 160.f, helpY);
    drawKey("ENT", "select", CX - 40.f, helpY);
    drawKey("ESC", "back", CX + 68.f, helpY);

    //   Help overlay (if open)     
    if (helpVisible)
        drawHelp(window);
}

//  Help overlay – drawn over the mode-select panel
void ModeSelect::drawHelp(sf::RenderWindow& window) {
    const float WW = (float)window.getSize().x;
    const float WH = (float)window.getSize().y;

    // Dim backdrop
    sf::RectangleShape dim({ WW, WH });
    dim.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(dim);

    // Help panel
    const float PW = 640.f, PH = 520.f;
    const float PX = WW / 2.f - PW / 2.f;
    const float PY = WH / 2.f - PH / 2.f;

    sf::RectangleShape panel({ PW, PH });
    panel.setPosition(PX, PY);
    panel.setFillColor(sf::Color(5, 18, 5, 245));
    panel.setOutlineThickness(2.f);
    panel.setOutlineColor(sf::Color(62, 207, 90));
    window.draw(panel);

    // Helper lambda
    auto text = [&](const string& s, float x, float y,
        sf::Color col, int size = 16) {
            sf::Text t;
            t.setFont(font);
            t.setCharacterSize(size);
            t.setFillColor(col);
            t.setString(s);
            t.setPosition(x, y);
            window.draw(t);
        };

    float cx = PX + PW / 2.f;
    float y = PY + 20.f;

    // Title
    {
        sf::Text ttl;
        ttl.setFont(font);
        ttl.setCharacterSize(26);
        ttl.setFillColor(sf::Color::White);
        ttl.setString("HOW TO PLAY – SNOW BROS");
        auto b = ttl.getLocalBounds();
        ttl.setPosition(cx - b.width / 2.f, y);
        window.draw(ttl);
    }
    y += 44.f;

    // Divider
    sf::RectangleShape div({ PW - 40.f, 1.f });
    div.setPosition(PX + 20.f, y);
    div.setFillColor(sf::Color(30, 102, 40, 200));
    window.draw(div);
    y += 14.f;

    //   Controls section        
    text("CONTROLS", PX + 20.f, y, sf::Color(62, 207, 90), 14);
    y += 24.f;

    auto row = [&](const string& action, const string& p1key, const string& p2key) {
        text(action, PX + 20.f, y, sf::Color(180, 180, 180));
        text(p1key, PX + 210.f, y, sf::Color(100, 200, 255));
        text(p2key, PX + 390.f, y, sf::Color(255, 130, 100));
        y += 26.f;
        };

    text("Action", PX + 20.f, y - 2.f, sf::Color(255, 255, 255, 120), 13);
    text("Player 1", PX + 210.f, y - 2.f, sf::Color(100, 200, 255, 160), 13);
    text("Player 2", PX + 390.f, y - 2.f, sf::Color(255, 130, 100, 160), 13);
    y += 20.f;

    row("Move Left", "A", "Left Arrow");
    row("Move Right", "D", "Right Arrow");
    row("Jump", "W", "Up Arrow");
    row("Throw Snowball", "Space", "N");
    row("Pause", "ESC / P", "ESC / P");
    y += 8.f;

    // Divider
    sf::RectangleShape div2({ PW - 40.f, 1.f });
    div2.setPosition(PX + 20.f, y);
    div2.setFillColor(sf::Color(30, 102, 40, 200));
    window.draw(div2);
    y += 14.f;

    //   Rules section   
    text("RULES", PX + 20.f, y, sf::Color(62, 207, 90), 14);
    y += 24.f;

    auto bullet = [&](const string& s) {
        text("*  " + s, PX + 24.f, y, sf::Color(190, 190, 190), 14);
        y += 22.f;
        };

    bullet("Throw snowballs to freeze enemies, then kick them to defeat them.");
    bullet("Any enemy contact (unfrozen) costs a life.");
    bullet("Both players share enemies and collectibles on the same level.");
    bullet("The game ends when both players run out of lives.");
    bullet("Defeat all enemies on a level to advance to the next.");
    bullet("Bonus levels (4 & 9): collect all items to advance.");
    bullet("Each player tracks their own score – highest wins!");

    //   Close hint           
    text("Press any key to close", PX + PW / 2.f - 90.f,
        PY + PH - 28.f, sf::Color(62, 207, 90, 180), 13);
}