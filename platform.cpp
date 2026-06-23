#include "platform.h"
#include<iostream>
using namespace std;
sf::Texture Platform::texture;

// DEFAULT CONSTRUCTOR
Platform::Platform() {
    body.setSize(sf::Vector2f(0, 0));\

    platformTex1.loadFromFile("plateform.png");
    platformTex2.loadFromFile("plateform2.png");
}



Platform::Platform(float x, float y, float width, float height) {

//COLLISION 
    body.setPosition(x, y);
    body.setSize(sf::Vector2f(width, height));

   
    static bool loaded = false;

    platformTex1.loadFromFile("plateform.png");
    platformTex2.loadFromFile("plateform2.png");


    if (!loaded) {
        if (!texture.loadFromFile("plateform.png")) {
            std::cout << "PLATFORM IMAGE FAILED\n";
        }
        texture.setSmooth(false);
        loaded = true;
    }
   
    sprite.setTexture(texture);
    sprite.setPosition(x, y);

    sprite.setScale(
        width / texture.getSize().x,
        height / texture.getSize().y
    );
}


sf::FloatRect Platform::getBounds() {
    return sprite.getGlobalBounds();
}
void Platform::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Platform::setTexture(sf::Texture* tex) {
    sprite.setTexture(*tex);

    sprite.setScale(
        body.getSize().x / tex->getSize().x,
        body.getSize().y / tex->getSize().y
    );
}