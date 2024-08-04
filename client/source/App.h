//
// Created by Андрей Степанов on 16.04.2024.
//

#ifndef CHAT_APP_H
#define CHAT_APP_H

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Reader.h"
#include "Textbox.h"
#include "Button.h"
#include "Bubble.h"
#include "Login.h"
#include "ChatLabel.h"
#include "Server.h"
#include "RoundedRect.h"

class App {
private:
    const Reader reader;
    static Textbox *textbox1;
    static Textbox *newChatTextbox;
    static Textbox *newUserTextbox;
    static sf::Text *usersOnline;
    static Button *send;
    static Button *newChat;
    static Button *newUser;
    static Button *textboxButton;
    static RoundRect *chatsNotSelectedRect;
    static sf::Text *chatsNotSelected;
    static sf::Text *chatsEmpty;

    static sf::Texture backgroundTexture;
    static sf::Texture uiGroupsTexture;
    static sf::Sprite background;
    static sf::Sprite uiGroups;
    sf::RenderWindow *window{};
    Login login;
    sf::Event sfEvent{};

    void initWindow();
    static void initChats();
    static void initTextFields();
    static void initButtons();
    void updateSFMLEvents();

    static void onSendClick();
    static void receiveMessage();
    static void onNewChatClick();
    static void onNewPersonClick();
    static void onTextboxClick();

    static float yBubbles;
    static float yChats;
    static bool isScrollable;
    static bool isChatsScrollable;
public:
    App();
    ~App();
    void update();
    void render();
    void run();
    static int currentChat;
};

#endif //CHAT_APP_H
