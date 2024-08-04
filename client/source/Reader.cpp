//
// Created by Андрей Степанов on 16.04.2024.
//

#include "Reader.h"



Reader::Reader() {
    file.open("config.txt");
    arial.loadFromFile("fonts/SF-Compact-Display-Regular.otf");

    read(WINDOW_HEIGHT, "WINDOW_HEIGHT");
    read(WINDOW_WIDTH, "WINDOW_WIDTH");
}

void Reader::read(int &num, const std::string& name) {
    file >> currentValue;
    file >> currentVariable;
    if (currentVariable != name){
        std::cerr << "config file error";
    } else {
        num = currentValue;
    };
}
sf::Font Reader::arial;