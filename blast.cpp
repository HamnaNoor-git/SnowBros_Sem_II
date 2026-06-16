#include "blast.h"

Blast::Blast() {

    body.setRadius(30);
    body.setFillColor(sf::Color::Yellow);

    speedX = 0;
    speedY = 0;

    active = false;
    exploded = false;
    explosionTimer = 0;
    onGround = false;
    walkDir = 1;
}

void Blast::shoot(float x, float y, float dirX, float dirY) {
    

    body.setPosition(x, y);


    speedX = dirX * 10;
    speedY = dirY * 10;

    active = true;
    onGround = false;
    walkDir = (rand() % 2 == 0) ? -1 : 1;
    exploded = false;
}


void Blast::update(float dt, Platform* platforms, int platformCount) {

    if (!active) return;
    onGround = false;  
    speedY += 200 * dt;

    body.move(speedX * dt, speedY * dt);



    for (int i = 0; i < platformCount; i++) {

        sf::FloatRect b = body.getGlobalBounds();
        sf::FloatRect p = platforms[i].getBounds();

        float bottom = b.top + b.height;
        float prevBottom = bottom - speedY * dt;

        if (b.intersects(p)) {

            if (speedY > 0 && prevBottom <= p.top) {

                body.setPosition(
                    b.left,
                    p.top - b.height
                );

                speedY = 0;
                onGround = true;

                speedX = 80 * walkDir;
            }
        }
    }

 
    if (body.getPosition().x < 50 || body.getPosition().x > 950) {
        walkDir *= -1;
        speedX = 80 * walkDir;
    }

    if (!exploded) {

        if (body.getPosition().x < 0 || body.getPosition().x > 1000 ||
            body.getPosition().y < 0 || body.getPosition().y > 800) {

            exploded = true;
            explosionTimer = 0.5f;

            body.setRadius(20);
            body.setFillColor(sf::Color::Red);
        }
    }
    else {
        explosionTimer -= dt;

        if (explosionTimer <= 0) {
            active = false;
        }
    }




    if (body.getPosition().y > 750) {
        body.setPosition(body.getPosition().x, 750);
        speedY = 0;
        onGround = true;
    }
}
void Blast::draw(sf::RenderWindow& window) {
    if (active)
        window.draw(body);
}

bool Blast::isActive() {
    return active;
}

bool Blast::isExploded() {
    return exploded;
}

sf::FloatRect Blast::getBounds() {
    return body.getGlobalBounds();
}

void Blast::deactivate() {
    active = false;
}