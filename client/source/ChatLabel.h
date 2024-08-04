//
// Created by Андрей Степанов on 06.05.2024.
//
#ifndef CHAT_CHATLABEL_H
#define CHAT_CHATLABEL_H

#include "SFML/Graphics.hpp"
#include "Button.h"
#include "Reader.h"
#include "Server.h"
#include "Bubble.h"

class ChatLabel {
public:
    ChatLabel(sf::Vector2f pos, sf::String label, sf::String lastMessage);
    void drawTo(sf::RenderWindow *target);

    void setSelected(bool sel);
    void doFunc();
    bool isMouseOver(sf::RenderWindow *target);
    bool isScrollable();
    void updateLastMessage(sf::String newLastMessage);
    static void move(float delta);
    static std::vector<ChatLabel*> chatLabels;
    static int maxId;
private:
    sf::RectangleShape outline;
    sf::RectangleShape base;
    Textbox label;
    Textbox lastMessage;
    Button *button;
    bool isSelected = false;
    int id;
    float yBubbles = 60;
    bool isScroll = false;

    static void moveUp(float delta);
};


#endif //CHAT_CHATLABEL_H
