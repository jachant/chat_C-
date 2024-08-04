//
// Created by Андрей Степанов on 29.04.2024.
//

#ifndef CHAT_BUBBLE_H
#define CHAT_BUBBLE_H

#include "SFML/Graphics.hpp"
#include "RoundedRect.h"
#include "Reader.h"
#include "Textbox.h"


class Bubble {
public:
    enum owner{
        me, mynigga
    };

    Bubble(const sf::String& txt, owner own, float y, const std::string& name = "");

    float getY();
    void drawTo(sf::RenderWindow *window);
    void moveUp(const float y = -45);
    void setName(const sf::String& name);

    static std::vector<Bubble*> bubbles;
    owner getOwner();
private:
    RoundRect bubble;
    sf::Text userName;
    Textbox text;
    owner ownership;
};


#endif //CHAT_BUBBLE_H
