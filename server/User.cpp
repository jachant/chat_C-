#include "defines.h"
#include "User.h"


using namespace std;

User::User(string username,string login, string password, int id,vector<int>&chats)
{
    this->username = username;
    this->login = login;
    this->password = password;
    this->id = id;
    this->user_chats = chats;
}

string User::get_username()
{
    return this->username;
}


string User::get_login()
{
    return this->login;
}

string User::get_password()
{
    return this->password;
}

int User::get_id()
{
    return this->id;
}

vector<int> User::get_user_chats()
{
    return this->user_chats;
}

void User::select_chat(int id)
{
    this->selected_chat = id;
}

void User::add_chat(int id) {
    this->user_chats.push_back(id);
}

