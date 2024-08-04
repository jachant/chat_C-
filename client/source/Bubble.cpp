//
// Created by Андрей Степанов on 29.04.2024.
//

#include "Bubble.h"

Bubble::Bubble(const sf::String& txt, owner own, float y, const std::string& name) {
    text.setCharacterSize(15);
    text.setString(txt);
    text.setFont(Reader::arial);
    text.setFillColor(sf::Color::Black);
    text.setSelected(false);
    ownership = own;
    if (own == mynigga){
        std::cout << name << '\n';
        userName.setCharacterSize(12);
        userName.setString(name);
        userName.setFont(Reader::arial);
        userName.setFillColor(sf::Color::Black);
        bubble.setSize({text.getLocalBounds().width > userName.getLocalBounds().width ? text.getLocalBounds().width + 20 : userName.getLocalBounds().width + 20 , 50});
    } else {
        bubble.setSize({text.getLocalBounds().width + 20, 35});
    }
    bubble.setRadius(10);
    switch (ownership) {
        case me:
            bubble.setPosition(1000 - bubble.getLocalBounds().width - 10, y);
            bubble.setFillColor(sf::Color(231, 254, 204, 255));
            break;
        case mynigga:
            bubble.setPosition(310, y);
            break;
    }
    if(own == mynigga){
        userName.setPosition(bubble.getPosition().x + 10, y + 3);
        text.setPosition({bubble.getPosition().x + 10, y + 18});
    } else {
        text.setPosition({bubble.getPosition().x + 10, y + 8});
    }
}

void Bubble::drawTo(sf::RenderWindow *window) {
    window->draw(bubble);
    window->draw(userName);
    text.drawTo(window);
}

void Bubble::moveUp(const float y) {
    bubble.move({0, y});
    userName.move({0, y});
    text.move({0, y});
}

float Bubble::getY() {
    return bubble.getPosition().y;
}

void Bubble::setName(const sf::String& name) {
    userName.setString(name);
}

Bubble::owner Bubble::getOwner() {
    return ownership;
}

std::vector<Bubble *> Bubble::bubbles;