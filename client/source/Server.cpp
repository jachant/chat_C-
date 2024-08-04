//
// Created by Андрей Степанов on 13.05.2024.
//

#include "Server.h"
#include "App.h"

sf::TcpSocket Server::socket;
sf::IpAddress Server::ip = sf::IpAddress::getLocalAddress();
sf::Socket::Status Server::status = Server::socket.connect(ip, 2000);
int Server::id;

int Server::check_operation(sf::Packet &packet) {
    int operation;
    packet >> operation;
    return operation;
}

void Server::send_message(sf::Packet packet) {
    socket.send(packet);
}

sf::Packet Server::receive_packet() {
    sf::Packet packet;
    if (socket.receive(packet) == sf::Socket::Done) {
        return packet;
    }
}

void Server::updateOperations() {
    sf::Packet packet;
    while (true) {
        if (socket.receive(packet) == sf::Socket::Done) {
            int operation = check_operation(packet);
            if (operation == 0) {
                int chat_id, client_id;
                string message;
                packet >> chat_id >> client_id >> message;
                cout<<"new message from "<<chat_id<<" by "<<client_id<<": "<<message<<endl;
                for (int i = 0; i < chats.size(); i++) {
                    if (chats[i].get_id() == chat_id) {
                        Server::chats[i].add_message(client_id, message);
                        lastMessageUserId = client_id;
                        break;
                    }
                }
                if (chats[App::currentChat].get_id() == chat_id) messageCum = true;
            } else if (operation == 1) {
                int chatId;
                packet >> chatId >> chatId;
                cout<<"chat was made: "<<chatId<<endl;
                Chat curChat;
                curChat.set_id(chatId);
                Server::chats.push_back(curChat);
                flagChatId = 1;
            } else if (operation == 2) {
                int chatId;
                string chatName;
                Chat newChat;
                packet >> chatId >> chatName;
                cout<<"was connected to the chat "<<chatId<<endl;
                newChat.set_id(chatId);
                newChat.set_name(chatName);
                int smsCount;
                packet >> smsCount;
                int curId;
                string curMes;
                for (int i = 0; i < smsCount; i++){
                    packet >> curId >> curMes;
                    newChat.add_message(curId, curMes);
                }
                chats.push_back(newChat);
                flagAddUser = 1;
            } else if (operation == 3) {
                int count;
                packet >> count;
                string user_login;
                cout<<"table is uploading"<<endl;
                for (int i = 0; i < count; i++) {
                    packet >> user_login;
                    username_table.push_back(user_login);
                }
                Server::flag_prereload = 1;
                cout<<"table was uploaded"<<endl;
                for(int i=0;i<username_table.size();i++){
                    cout<<username_table[i]<<" "<<i<<endl;
                }
            } else if (operation == 4) {
                string username;
                packet >> username;
                cout<<"new user in the table"<<endl;
                cout<<username<<endl;
                username_table.push_back(username);
            } else if (operation == 100) {
                packet >> cur_online >> cur_online;
                cout<<"number of online users has been changed"<<endl;
                cout << endl << "|||" << " " << cur_online << " " << "|||" << endl;
            }
        }
    }
}


vector<Chat> Server::chats;
bool Server::messageCum = false;
bool Server::chatCum = false;
int Server::lastMessageUserId;
int Server::cur_online;
bool Server::flag_prereload = 0;
bool Server::flagChatId = 0;
bool Server::flagAddUser = 0;
string Server::get_login(int id) {
    return username_table[id];
}

vector<string> Server::username_table;
