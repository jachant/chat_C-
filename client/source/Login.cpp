//
// Created by Андрей Степанов on 03.05.2024.
//

#include "Login.h"
#include "Chat.h"

Login::Login() {
    backgroundTexture.loadFromFile("textures/loginUI.png");
    registerTexture.loadFromFile("textures/registerUI.png");
    background.setTexture(backgroundTexture);

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    initUI();
    this->loginScreen = new sf::RenderWindow(sf::VideoMode(500, 400), "Chat", sf::Style::Close, settings);
    this->loginScreen->setFramerateLimit(60);

    chatTextTexture.loadFromFile("textures/tochat.png");
    chatText.setTexture(chatTextTexture);
    chatText.setPosition({500, 100});
    mouseTexture.loadFromFile("textures/mouse.png");
    mouse.setTexture(mouseTexture);
    mouse.setPosition({560, 125});
}

void Login::initUI() {
    usernameBox = new Textbox(15, sf::Color::Black);
    usernameBox->setFont(Reader::arial);
    usernameBox->setPosition({109, 97.5});
    usernameBox->setLimit(true, 275);

    usernameBoxButton = new Button({300, 50});
    usernameBoxButton->setBackgroundColor(sf::Color::Transparent);
    usernameBoxButton->setPosition({100, 84});
    usernameBoxButton->setFunction(onUsernameBoxClick);

    loginBox = new Textbox(15, sf::Color::Black);
    loginBox->setFont(Reader::arial);
    loginBox->setPosition({109, 164});
    loginBox->setLimit(true, 275);

    loginBoxButton = new Button({300, 50});
    loginBoxButton->setBackgroundColor(sf::Color::Transparent);
    loginBoxButton->setPosition({100, 150});
    loginBoxButton->setFunction(onLoginBoxClick);

    passwordBox = new Textbox(15, sf::Color::Black);
    passwordBox->setSelected(false);
    passwordBox->setFont(Reader::arial);
    passwordBox->setPosition({109, 225});
    passwordBox->setLimit(true, 275);
    passwordBox->setStars(true);

    passwordBoxButton = new Button({300, 50});
    passwordBoxButton->setBackgroundColor(sf::Color::Transparent);
    passwordBoxButton->setPosition({100, 211});
    passwordBoxButton->setFunction(onPasswordBoxClick);

    logInButton = new Button({125, 30.5});
    logInButton->setBackgroundColor(sf::Color::Transparent);
    logInButton->setPosition({187.5, 293});
    logInButton->setFunction(onLogInButtonClick);

    registerButton = new Button({60, 22});
    registerButton->setBackgroundColor(sf::Color::Transparent);
    registerButton->setPosition({220, 333.5});
    registerButton->setFunction(onRegisterButtonClick);

    errorText = new Textbox(15, sf::Color::Red);
    errorText->setFont(Reader::arial);
    errorText->setSelected(false);
    errorText->setLimit(false);
}

void Login::update() {
    while (this->loginScreen->pollEvent(sfEvent)) {
        switch (sfEvent.type) {
            case sf::Event::Closed:
                this->loginScreen->close();
                break;
            case sf::Event::TextEntered:
                if (sfEvent.text.unicode == 10) {
                    switchBox();
                } else {
                    if (loginBox->isSelected())
                        loginBox->typedOn(sfEvent);
                    else if (passwordBox->isSelected())
                        passwordBox->typedOn(sfEvent);
                    else if (usernameBox->isSelected())
                        usernameBox->typedOn(sfEvent);
                }
                break;
            case sf::Event::MouseMoved:
                break;
            case sf::Event::MouseButtonPressed:
                if (logInButton->isMouseOver(loginScreen)) {
                    logInButton->doFunction();
                    if (valid)
                        loginScreen->close();
                } else if (registerButton->isMouseOver(loginScreen)) {
                    registerButton->doFunction();
                } else if (usernameBoxButton->isMouseOver(loginScreen)) {
                    usernameBoxButton->doFunction();
                } else if (loginBoxButton->isMouseOver(loginScreen)) {
                    loginBoxButton->doFunction();
                } else if (passwordBoxButton->isMouseOver(loginScreen)) {
                    passwordBoxButton->doFunction();
                }
                break;
            default:
                break;
        }
    }
}

void Login::render() {
    this->loginScreen->clear();
    this->loginScreen->draw(background);

    if (isLogin) {
        renderChatText();
    }

    if (!isLogin) {
        if (usernameBox->getText().empty()) {
            usernameBox->setHint("Username");
        }
        usernameBox->drawTo(loginScreen);
    }

    if (loginBox->getText().empty()) {
        loginBox->setHint("Login");
    }
    if (passwordBox->getText().empty()) {
        passwordBox->setHint("Password");
    }
    loginBox->drawTo(loginScreen);
    passwordBox->drawTo(loginScreen);

    loginBoxButton->drawTo(loginScreen);
    passwordBoxButton->drawTo(loginScreen);
    logInButton->drawTo(loginScreen);
    registerButton->drawTo(loginScreen);
    errorText->drawTo(loginScreen);

    this->loginScreen->display();
}

void Login::run() {
    while (this->loginScreen->isOpen()) {
        update();
        render();
    }
}

void Login::onLogInButtonClick() {
    //проверка
    if (!isLogin) {
        if (usernameBox->isValid()) {
            setError("username is not valid");
            return;
        }
        else if (loginBox->isValid()) {
            setError("login is not valid");
            return;
        }
        else if (passwordBox->isValid()) {
            setError("password is not valid");
            return;
        }
        else if (passwordBox->getText().size() < 5) {
            setError("password has to have >= 5 symbols");
            return;
        }
    }

    //логика
    sf::Packet packet;
    cout<<"Operation";
    if (!isLogin) {
        packet << 0 << usernameBox->getText();
        cout<<" sign up: name - "<<usernameBox->getText();
    } else {
        packet << 1;
        cout<<" sign in: ";
    }
    packet << loginBox->getText() << passwordBox->getText();
    cout<<" login - "<<loginBox->getText()<<" password - "<<passwordBox->getText()<<endl;
    Server::send_message(packet);
    packet = Server::receive_packet();
    int op = Server::check_operation(packet);
    if (!op) {
        cout<<"Entered account"<<endl;
        packet >> Server::id;
        valid = true;
        if (!isLogin) {return;};
        int chat_count, message_count, cur_id, chat_id;
        std::string name, cur_message;
        packet >> chat_count;
        for (int i = 0; i < chat_count; i++) {
            Chat cur_chat;
            packet >> chat_id >> name >> message_count;
            cur_chat.set_id(chat_id);
            cur_chat.set_name(name);
            for (int j = 0; j < message_count; j++) {
                packet >> cur_id >> cur_message;
                cur_chat.add_message(cur_id, cur_message);
            }
            Server::chats.push_back(cur_chat);
        }
    } else {
        if (!isLogin) {
            setError("Login already exists");
        } else {
            if (op == 1) {
                setError("Login is incorrect");
            } else if (op == 2) {
                setError("Password is incorrect");
            }
        }
    }
}

void Login::onRegisterButtonClick() {
    if (isLogin) {
        isLogin = false;
        background.setTexture(registerTexture);
        usernameBox->clear();
        loginBox->clear();
        passwordBox->clear();
        usernameBox->setSelected(true);
        loginBox->setSelected(false);
        passwordBox->setSelected(false);
    } else {
        isLogin = true;
        background.setTexture(backgroundTexture);
    }
    clock = 0;
}

void Login::onUsernameBoxClick() {
    usernameBox->setSelected(true);
    loginBox->setSelected(false);
    passwordBox->setSelected(false);
}

void Login::onLoginBoxClick() {
    usernameBox->setSelected(false);
    loginBox->setSelected(true);
    passwordBox->setSelected(false);
}

void Login::onPasswordBoxClick() {
    usernameBox->setSelected(false);
    loginBox->setSelected(false);
    passwordBox->setSelected(true);
}

void Login::renderChatText() {
    if (clock == 0) {
        chatText.setPosition({500, 100});
        mouse.setPosition({560, 125});
    }
    if (clock < 146) {
        chatText.move({-2, 0});
        mouse.move({-2, 0});
    } else if (clock > 160 && clock < 400) {
        mouse.move({2, 0});
    }
    loginScreen->draw(chatText);
    loginScreen->draw(mouse);
    clock++;
}

void Login::switchBox() {
    if (isLogin) {
        if (loginBox->isSelected()) {
            loginBox->setSelected(false);
            passwordBox->setSelected(true);
        } else if (passwordBox->isSelected()) {
            loginBox->setSelected(true);
            passwordBox->setSelected(false);
        }
    } else {
        if (usernameBox->isSelected()) {
            usernameBox->setSelected(false);
            loginBox->setSelected(true);
        } else if (loginBox->isSelected()) {
            loginBox->setSelected(false);
            passwordBox->setSelected(true);
        } else if (passwordBox->isSelected()) {
            usernameBox->setSelected(true);
            passwordBox->setSelected(false);
        }
    }
}

void Login::setError(const sf::String &text) {
    errorText->clear();
    errorText->setString(text);
    errorText->setPosition({250 - errorText->getWidth() / 2, 366.5});
}

bool Login::isValid() {
    return valid;
}

sf::Texture Login::backgroundTexture;
sf::Texture Login::registerTexture;
sf::Sprite Login::background;
sf::Texture Login::chatTextTexture;
sf::Sprite Login::chatText;
sf::Texture Login::mouseTexture;
sf::Sprite Login::mouse;

Textbox *Login::usernameBox;
Textbox *Login::loginBox;
Textbox *Login::passwordBox;
Textbox *Login::errorText;
Button *Login::usernameBoxButton;
Button *Login::loginBoxButton;
Button *Login::passwordBoxButton;
Button *Login::logInButton;
Button *Login::registerButton;

int Login::clock = 0;
bool Login::valid = false;
bool Login::isLogin = true;