//
// Created by Андрей Степанов on 13.05.2024.
//

#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H

#include "SFML/Network.hpp"
#include "iostream"
#include "Chat.h"
class Server {
public:
    static vector<Chat>chats;
    static sf::IpAddress ip;
    static sf::TcpSocket socket;
    static sf::Socket::Status status;
    static int id;

    static int check_operation(sf::Packet &packet);

    static void send_message(sf::Packet packet);

    static void get_message();

    static sf::Packet receive_packet();

    static void updateOperations();

    static bool messageCum;
    static bool chatCum;
    static int lastMessageUserId;
    static vector<string> username_table;

    static string get_login(int id);
    static int cur_online;
    static bool flag_prereload;
    static bool flagChatId;
    static bool flagAddUser;
};
#endif //CHAT_SERVER_H
