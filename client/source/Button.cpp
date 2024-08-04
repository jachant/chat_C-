//
// Created by Андрей Степанов on 18.04.2024.
//

#include "Button.h"

Button::Button(std::string t, sf::Vector2f size, int charSize) {
    text.setString(t);
    text.setFillColor(sf::Color::Black);
    text.setCharacterSize(charSize);

    button.setSize(size);
    button.setFillColor(sf::Color::White);
}

Button::Button(sf::Vector2f size) {
    button.setSize(size);
    button.setFillColor(sf::Color::White);
}

void Button::setFont(sf::Font &font) {
    text.setFont(font);
}

void Button::setBackgroundColor(sf::Color color) {
    button.setFillColor(color);
}

void Button::setTextColor(sf::Color color) {
    text.setFillColor(color);
}

void Button::setPosition(sf::Vector2f pos) {
    button.setPosition(pos);
    float xPos = (pos.x + button.getLocalBounds().width / 2) - (text.getLocalBounds().width / 2);
    float yPos = (pos.y + button.getLocalBounds().height / 2) - (text.getLocalBounds().height / 2);
    text.setPosition(xPos, yPos);
}

void Button::drawTo(sf::RenderWindow *window) {
    window->draw(button);
    if(text.getString() != "")
        window->draw(text);
}

bool Button::isMouseOver(sf::RenderWindow *window) {
    int mouseX = sf::Mouse::getPosition(*window).x;
    int mouseY = sf::Mouse::getPosition(*window).y;

    float btnPosX = button.getPosition().x;
    float btnPosY = button.getPosition().y;

    float btnXPosWidth = btnPosX + button.getLocalBounds().width;
    float btnYPosHeight = btnPosY + button.getLocalBounds().height;

    if (mouseX < btnXPosWidth && mouseX > btnPosX && mouseY < btnYPosHeight && mouseY > btnPosY){
        return true;
    }
    return false;
}

void Button::setTexture(const std::string& path) {
#ifndef _WIN32
    delete texture;
#endif
    texture = nullptr;
    texture = new sf::Texture;
    texture->loadFromFile(path);
    button.setTexture(texture);
}

void Button::setFunction(void (*func)()) {
    function = func;
}

void Button::doFunction() const {
    function();
}

void Button::move(sf::Vector2f offset) {
    this->button.move(offset);
    this->text.move(offset);
}
