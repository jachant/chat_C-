#pragma once

#include <iostream>
#include <vector>

using namespace std;

class Chat {
public:
    Chat();
    void add_message(int id, std::string message);

    vector<pair<int, string>> get_chat();
    pair<int, string> get_last_message();
    std::string get_name();
    int get_id();
    void set_id(int id);
    void set_name(string name);

private:
    std::vector<pair<int, std::string>> chat;
    std::string name;
    int id;
};
