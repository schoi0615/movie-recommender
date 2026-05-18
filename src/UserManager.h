#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include "BaseManager.h"
#include "user.h"
#include <vector>
#include <string>

class UserManager : public BaseManager {
private:
    std::vector<User> users;

public:
    void addUser(const User& user);
    void printAll() const;
    User findById(int id) const;

    void loadFromFile(const std::string& filename) override;
    void saveToFile(const std::string& filename) const override;
    int size() const override;
};

#endif