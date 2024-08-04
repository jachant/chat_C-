#pragma once
#include "defines.h"

using namespace std;

class Chat
{
public:
    Chat();
    void add_message(int id, string message);
    vector<pair<int, string>> get_chat();
    string get_name();
    void set_name(string name);
    int get_id();
    void set_id(int id);
private:
    vector<pair<int, string>>chat;
    string name;
    int id;
};
