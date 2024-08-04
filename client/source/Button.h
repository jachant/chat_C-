//
// Created by Андрей Степанов on 18.04.2024.
//

#ifndef CLIENT_BUTTON_H
#define CLIENT_BUTTON_H

#include "SFML/Graphics.hpp"
#include "iostream"

class Button {
public:
    Button();
    Button(std::string t, sf::Vector2f size, int charSize);
    Button(sf::Vector2f size);

    void setFont(sf::Font &font);
    void setBackgroundColor(sf::Color color);
    void setTextColor(sf::Color color);
    void setTexture(const std::string& path);
    void setPosition(sf::Vector2f pos);
    void setFunction(void (*func)());
    void doFunction() const;
    void move(sf::Vector2f offset);
    void drawTo(sf::RenderWindow *window);
    bool isMouseOver(sf::RenderWindow *window);
private:
    void (*function)();
    sf::RectangleShape button;
    sf::Texture *texture;
    sf::Text text;
};


#endif //CLIENT_BUTTON_H
