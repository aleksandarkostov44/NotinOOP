#pragma once
#include <string>
#include <iostream>

class User {
protected:
    unsigned int userId;
    std::string username;
    std::string password;

public:
    User(unsigned int id, const std::string& uname, const std::string& pass);

    virtual ~User() = default;

    unsigned int getUserId() const;
    const std::string& getUsername() const;

    bool checkPassword(const std::string& pass) const;

    virtual void help() const = 0;

    const std::string& getPassword() const;
};