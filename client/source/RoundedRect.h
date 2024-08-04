//
// Created by Андрей Степанов on 30.04.2024.
//

#ifndef CHAT_ROUNDEDRECT_H
#define CHAT_ROUNDEDRECT_H

#include <SFML/Graphics.hpp>
#include <cmath>

class RoundRect : public sf::Shape {
public:
    RoundRect(sf::Vector2f size = {0, 0}, float radius = 0);

    void setSize(sf::Vector2f size);
    sf::Vector2f getSize() const;

    void setRadius(float radius);
    float getRadius() const;

    std::size_t getPointCount() const override;
    sf::Vector2f getPoint(std::size_t index) const override;

private:
    sf::Vector2f m_size;
    float m_radius;
};

#endif //CHAT_ROUNDEDRECT_H
