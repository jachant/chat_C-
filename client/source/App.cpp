//
// Created by Андрей Степанов on 16.04.2024.
//

#include "App.h"
#include <thread>

void App::initWindow() {
    backgroundTexture.loadFromFile("textures/trueBackground.png");
    backgroundTexture.setRepeated(true);
    background.setTexture(backgroundTexture);
    background.setTextureRect({0, 0, 1000, 800});
    uiGroupsTexture.loadFromFile("textures/uigroups.png");
    uiGroups.setTexture(uiGroupsTexture);
    usersOnline = new sf::Text;
    usersOnline->setFont(Reader::arial);
    usersOnline->setPosition({42, 763});
    usersOnline->setCharacterSize(20);
    usersOnline->setFillColor(sf::Color(93, 143, 194, 255));
    usersOnline->setString("1");

    chatsNotSelectedRect = new RoundRect({155, 40}, 10);
    chatsNotSelectedRect->setPosition({572.5, 377.5});
    chatsNotSelectedRect->setFillColor(sf::Color(0, 0, 0, 30));

    chatsNotSelected = new sf::Text;
    chatsNotSelected->setFont(Reader::arial);
    chatsNotSelected->setPosition({603, 385});
    chatsNotSelected->setCharacterSize(19);
    chatsNotSelected->setFillColor(sf::Color::White);
    chatsNotSelected->setString("Select chat");

    chatsEmpty = new sf::Text;
    chatsEmpty->setFont(Reader::arial);
    chatsEmpty->setPosition({103, 385});
    chatsEmpty->setCharacterSize(19);
    chatsEmpty->setFillColor(sf::Color::Black);
    chatsEmpty->setString("Create chat");
}

void App::initChats() {
    ChatLabel::chatLabels.clear();
    ChatLabel::maxId = -1;
    yChats = -10;
    for (int i = 0; i < Server::chats.size(); i++) {
        if (Server::chats[i].get_last_message().first == -1)
            ChatLabel::chatLabels.push_back(new ChatLabel({0, yChats += 61},
                                                          to_string(Server::chats[i].get_id()) + ": " +  Server::chats[i].get_name(), ""));
        else {
            string name = Server::username_table[Server::chats[i].get_last_message().first];
            if (Server::chats[i].get_last_message().first == Server::id) name = "me";
            ChatLabel::chatLabels.push_back(new ChatLabel({0, yChats += 61}, to_string(Server::chats[i].get_id()) + ": " +  Server::chats[i].get_name(),
                                                           name +
                                                          ": " + Server::chats[i].get_last_message().second));
        }
    }

    if (ChatLabel::chatLabels.size() > 12) {
        isChatsScrollable = true;
    }
}

void App::initTextFields() {
    textbox1 = new Textbox(15, sf::Color::Black);
    textbox1->setFont(Reader::arial);
    textbox1->setPosition({310, 766});
    textbox1->setLimit(true, 650);
    textbox1->setSelected(true);

    newChatTextbox = new Textbox(15, sf::Color::Black);
    newChatTextbox->setFont(Reader::arial);
    newChatTextbox->setPosition({45, 18});
    newChatTextbox->setLimit(true, 250);
    newChatTextbox->setHint("Chat name");
    newChatTextbox->setSelected(false);

    newUserTextbox = new Textbox(15, sf::Color::Black);
    newUserTextbox->setFont(Reader::arial);
    newUserTextbox->setPosition({345, 18});
    newUserTextbox->setLimit(true, 650);
    newUserTextbox->setHint("User name");
    newUserTextbox->setSelected(false);
}

void App::initButtons() {
    send = new Button({36, 36});
    send->setTexture("textures/send.png");
    send->setPosition({958, 758});
    send->setFunction(onSendClick);

    newChat = new Button({35, 35});
    newChat->setTexture("textures/newChat.png");
    newChat->setPosition({8, 8});
    newChat->setFunction(onNewChatClick);

    newUser = new Button({35, 35});
    newUser->setTexture("textures/newPerson.png");
    newUser->setPosition({308, 10});
    newUser->setFunction(onNewPersonClick);

    textboxButton = new Button({650, 50});
    textboxButton->setPosition({300, 750});
    textboxButton->setFunction(onTextboxClick);
}

App::App() {
    initTextFields();
    initButtons();
    initWindow();
}

App::~App() {
    delete this->window;
}

void App::update() {
    receiveMessage();
    updateSFMLEvents();
}

void App::render() {
    this->window->clear();

    this->window->draw(background);

    for (auto i: ChatLabel::chatLabels) {
        i->drawTo(window);
    }
    for (auto i: Bubble::bubbles) {
        i->drawTo(window);
    }

    this->window->draw(uiGroups);
    textbox1->drawTo(window);
    send->drawTo(window);
    newChatTextbox->drawTo(window);
    newChat->drawTo(window);
    newUserTextbox->drawTo(window);
    newUser->drawTo(window);
    this->window->draw(*usersOnline);

    if (currentChat == -1) {
        window->draw(*chatsNotSelectedRect);
        window->draw(*chatsNotSelected);
    }
    if (ChatLabel::chatLabels.empty()) {
        window->draw(*chatsEmpty);
    }

    this->window->display();
}

void App::run() {
    if (Server::status != sf::Socket::Done) {
        std::cout << "error";
    } else {
        //login screen
        login.run();

        std::thread receive_message(Server::updateOperations);
        receive_message.detach();

        while (!Server::flag_prereload);
        initChats();
        //main screen init
        if (Login::isValid()) {
            sf::ContextSettings settings;
            settings.antialiasingLevel = 8;
            this->window = new sf::RenderWindow(sf::VideoMode(reader.WINDOW_WIDTH, reader.WINDOW_HEIGHT), "Chat",
                                                sf::Style::Close, settings);
            //main screen run
            while (this->window->isOpen()) {
                this->update();
                this->render();
            }
        }
    }
}

void App::updateSFMLEvents() {
    usersOnline->setString(to_string(Server::cur_online));
    sf::Packet packet;
    while (this->window->pollEvent(this->sfEvent)) {
        switch (this->sfEvent.type) {
            case sf::Event::Closed:
                this->window->close();
                break;
            case sf::Event::TextEntered:
                if (textbox1->isSelected()) {
                    if (sfEvent.text.unicode == 10) {
                        send->doFunction();
                    } else {
                        textbox1->typedOn(sfEvent);
                    }
                } else if (newUserTextbox->isSelected()) {
                    if (sfEvent.text.unicode == 10) {
                        if (!newUserTextbox->getText().empty()){
                            sf::Packet packet;
                            int i = 0;
                            for (; i < Server::username_table.size(); i++) {
                                cout << "|||" << Server::username_table[i] << " " << newUserTextbox->getText() << "|||" << endl;
                                if (Server::username_table[i] == newUserTextbox->getText())
                                    break;
                            }
                            packet << 2 << Server::chats[currentChat].get_id() << i;
                            Server::socket.send(packet);
                            cout<<"new user "<<i<<" in chat"<<Server::chats[currentChat].get_id()<<endl;
                            newUserTextbox->clear();
                            textbox1->setSelected(true);
                            newUserTextbox->setSelected(false);
                        }
                    } else {
                        newUserTextbox->typedOn(sfEvent);
                    }
                } else if (newChatTextbox->isSelected()) {
                    if (sfEvent.text.unicode == 10) {
                        if (!newChatTextbox->getText().empty()){
                            sf::Packet packet;
                            cout << Server::id;
                            packet << 1 << Server::id << newChatTextbox->getText();
                            cout<<"new chat request client id:"<<Server::id<<" chat name:"<< newChatTextbox->getText()<<endl;
                            Server::socket.send(packet);
                            while (!Server::flagChatId);
                            Server::flagChatId = 0;
                            Server::chats.back().set_name(newChatTextbox->getText());
                            initChats();
                            newChatTextbox->clear();
                            textbox1->setSelected(true);
                            newChatTextbox->setSelected(false);
                        }
                    } else {
                        newChatTextbox->typedOn(sfEvent);
                    }
                }
                break;
            case sf::Event::MouseMoved:
                break;
            case sf::Event::MouseButtonPressed:
                if (send->isMouseOver(window)) {
                    send->doFunction();
                } else if (newChat->isMouseOver(window)) {
                    newChat->doFunction();
                } else if (newUser->isMouseOver(window)) {
                    newUser->doFunction();
                } else if (textboxButton->isMouseOver(window)) {
                    textboxButton->doFunction();
                } else {
                    for (int i = 0; i < ChatLabel::chatLabels.size(); i++) {
                        if (ChatLabel::chatLabels[i]->isMouseOver(window)) {
                            ChatLabel::chatLabels[i]->doFunc();
                            currentChat = i;
                            if (!Bubble::bubbles.empty())
                                isScrollable = Bubble::bubbles.back()->getY() >= 650;
                            else
                                isScrollable = false;
                            break;
                        }
                    }
                }
                break;
            case sf::Event::MouseWheelScrolled:
                if (isScrollable && sf::Mouse::getPosition().x - window->getPosition().x * 2 > 600) {
                    if (sfEvent.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                        if (Bubble::bubbles[0]->getY() >= 60) {
                            if (sfEvent.mouseWheelScroll.delta < 0) {
                                for (auto i: Bubble::bubbles) {
                                    i->moveUp(sfEvent.mouseWheelScroll.delta * 5);
                                }
                            }
                        } else if (Bubble::bubbles.back()->getY() <= 700 &&
                                   Bubble::bubbles.back()->getOwner() == Bubble::me) {
                            if (sfEvent.mouseWheelScroll.delta > 0) {
                                for (auto i: Bubble::bubbles) {
                                    i->moveUp(sfEvent.mouseWheelScroll.delta * 5);
                                }
                            } else if (Bubble::bubbles.back()->getY() <= 685 &&
                                       Bubble::bubbles.back()->getOwner() == Bubble::mynigga) {
                                if (sfEvent.mouseWheelScroll.delta > 0) {
                                    for (auto i: Bubble::bubbles) {
                                        i->moveUp(sfEvent.mouseWheelScroll.delta * 5);
                                    }
                                }
                            }
                        } else {
                            for (auto i: Bubble::bubbles) {
                                i->moveUp(sfEvent.mouseWheelScroll.delta * 5);
                            }
                            if (Bubble::bubbles[0]->getY() >= 60) {
                                float dy = 60 - Bubble::bubbles[0]->getY();
                                for (auto i: Bubble::bubbles) {
                                    i->moveUp(dy);
                                }
                            }
                            if (Bubble::bubbles.back()->getY() <= 700 &&
                                Bubble::bubbles.back()->getOwner() == Bubble::me) {
                                float dy = 700 - Bubble::bubbles.back()->getY();
                                for (auto i: Bubble::bubbles) {
                                    i->moveUp(dy);
                                }
                            } else if (Bubble::bubbles.back()->getY() <= 685 &&
                                       Bubble::bubbles.back()->getOwner() == Bubble::mynigga) {
                                float dy = 685 - Bubble::bubbles.back()->getY();
                                for (auto i: Bubble::bubbles) {
                                    i->moveUp(dy);
                                }
                            }
                        }
                    }
                } else if (isChatsScrollable && sf::Mouse::getPosition().x - window->getPosition().x * 2 < 600) {
                    if (sfEvent.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                        ChatLabel::move(sfEvent.mouseWheelScroll.delta * 5);
                    }
                }
            default:
                break;
        }
    }
}

void App::onSendClick() {
    cout << "!!!" << Server::id << "!!!" << endl;
    if (Server::chats.empty()) return;
    ChatLabel::chatLabels[currentChat]->updateLastMessage("me: " + textbox1->getText());
    Server::chats[currentChat].add_message(Server::id, textbox1->getText());
    sf::Packet packet;
    packet << 0 << Server::chats[currentChat].get_id() << Server::id << textbox1->getText();
    cout << "message sending: "<<Server::id << " " << Server::chats[currentChat].get_id() << " " << textbox1->getText() << endl;
    Server::socket.send(packet);
    if (!Bubble::bubbles.empty())
        yBubbles = Bubble::bubbles.back()->getY() + 45;
    else
        yBubbles = 60;
    if (isScrollable) {
        float dy = 735 - Bubble::bubbles.back()->getY();
        for (auto i: Bubble::bubbles) {
            i->moveUp(dy - 45);
        }
        yBubbles = 735;
    }
    Bubble::bubbles.push_back(new Bubble(textbox1->getSFText(), Bubble::me, yBubbles));
    if (yBubbles > 700) {
        for (auto i: Bubble::bubbles) {
            i->moveUp();
        }
        isScrollable = true;
    } else {
        yBubbles += 45;
    }
    textbox1->clear();
}

void App::receiveMessage() {
    if (Server::flagAddUser) {
        initChats();
        Server::flagAddUser = false;
    }
    if (Server::chatCum) {
        initChats();
        Server::chatCum = false;
    }
    if (Server::messageCum) {
        ChatLabel::chatLabels[currentChat]->updateLastMessage(
                Server::username_table[Server::chats[currentChat].get_last_message().first] + ": " +
                Server::chats[currentChat].get_last_message().second);
        if (!Bubble::bubbles.empty())
            yBubbles = Bubble::bubbles.back()->getY() + 60;
        else
            yBubbles = 60;
        if (isScrollable) {
            float dy = 735 - Bubble::bubbles.back()->getY();
            for (auto i: Bubble::bubbles) {
                i->moveUp(dy - 60);
            }
            yBubbles = 735;
        }
        Bubble::bubbles.push_back(
                new Bubble(Server::chats[currentChat].get_last_message().second, Bubble::mynigga, yBubbles,
                           Server::username_table[Server::lastMessageUserId]));
        if (yBubbles > 700) {
            for (auto i: Bubble::bubbles) {
                i->moveUp(60);
            }
            isScrollable = true;
        } else {
            yBubbles += 60;
        }
    }
    Server::messageCum = false;
}

void App::onNewChatClick() {
    textbox1->setSelected(false);
    newChatTextbox->setSelected(true);
    newUserTextbox->setSelected(false);
}

void App::onNewPersonClick() {
    textbox1->setSelected(false);
    newChatTextbox->setSelected(false);
    newUserTextbox->setSelected(true);
}

void App::onTextboxClick() {
    textbox1->setSelected(true);
    newChatTextbox->setSelected(false);
    newUserTextbox->setSelected(false);
}

sf::Texture App::backgroundTexture;
sf::Texture App::uiGroupsTexture;
sf::Sprite App::background;
sf::Sprite App::uiGroups;

RoundRect *App::chatsNotSelectedRect;
sf::Text *App::chatsNotSelected;
sf::Text *App::chatsEmpty;

Textbox *App::textbox1;
Button *App::textboxButton;
Textbox *App::newChatTextbox;
Textbox *App::newUserTextbox;
sf::Text *App::usersOnline;
Button *App::send;
Button *App::newUser;
Button *App::newChat;

float App::yBubbles = 60;
float App::yChats = -10;
bool App::isScrollable = false;
bool App::isChatsScrollable = false;
int App::currentChat = -1;


