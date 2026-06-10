#include "User.h"

User::User(unsigned int id, const std::string& uname, const std::string& pass)
    : userId(id), username(uname), password(pass)
{}

unsigned int User::getUserId() const {
    return userId;
}

const std::string& User::getUsername() const {
    return username;
}

bool User::checkPassword(const std::string& pass) const {
    return password == pass;
}