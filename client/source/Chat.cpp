#include "Chat.h"

using namespace std;

Chat::Chat()
{

}

void Chat::add_message(int id, string message)
{
    chat.push_back(make_pair(id, message));
}

vector<pair<int, string>> Chat::get_chat()
{
    return this->chat;
}

string Chat::get_name() {
    return this->name;
}

int Chat::get_id() {
    return this->id;
}

void Chat::set_name(string name){
    this->name=name;
}

void Chat::set_id(int id) {
    this->id = id;
}

pair<int, string> Chat::get_last_message() {
    if (this->chat.size()){
        return chat.back();
    }
    return {-1,""};
}


