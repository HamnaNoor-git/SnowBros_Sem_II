#pragma once
#include <SFML/Graphics.hpp>
using namespace std;

class Platform {
private:
    sf::RectangleShape body;

    static sf::Texture texture;
    sf::Sprite sprite;


    sf::Texture platformTex1;  
    sf::Texture platformTex2;   


public:
    Platform(); 
    Platform(float x, float y, float width, float height);

    sf::FloatRect getBounds();
    void draw(sf::RenderWindow& window);



    void setTexture(sf::Texture* tex);
};