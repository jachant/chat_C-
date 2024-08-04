#pragma once
#include "defines.h"
#include "User.h"
#include "Chat.h"

using namespace std;

class Server
{
public:
    // ��� �������� ���������� ������ ���������� � 1 �������!!!!!!!
    Server();
    void run();

    void op_log(sf::TcpSocket& socket, string login,string password);
    void op_reg(sf::TcpSocket& scoket, string username, string login, string password);
    int get_login_id(string login);


    void send_message(int chat_id, int user_id, string & message);//

    void server_off();


    //Network objects
    void connect_client(sf::TcpSocket& socket);
    sf::Packet receive_packet(sf::TcpSocket& socket);
    int check_operation(sf::Packet& packet);
    pair<int,string> parce_message(sf::Packet& packet);
    bool join_account(sf::TcpSocket& socket);

    void send_message_for_online(int chat_id,int client_id,string message);
    void send_chat_for_online(int chat_id, int client_id);
private:
    //network objects
    vector<pair<int,sf::TcpSocket*>> clients;
    sf::TcpListener listener;
    void set_server_online();
    void get_chats(sf::Packet &packet, int ind);
    void users_online(vector<pair<int,sf::TcpSocket*>>& clients);

    int user_count = 0;
    vector <User> users;
    void reset_user_base();
    void set_user_from_file(ifstream& file);

    int chat_count = 0;
    vector <Chat> chats;
    void reset_chat_base();
    void set_chat_from_file(ifstream& file);

    void send_current_online();
};

