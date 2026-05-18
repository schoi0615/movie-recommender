#ifndef USER_H
#define USER_H

#include <string>
#include <iostream>

class User {
private:
    int id;
    std::string name;
    int age;

public:
    User() : id(0), name(""), age(0) {}
    User(int id, const std::string& name, int age)
        : id(id), name(name), age(age) {}

    int getId() const { return id; }
    std::string getName() const { return name; }
    int getAge() const { return age; }

    friend std::ostream& operator<<(std::ostream& os, const User& u) {
        os << "[" << u.id << "] " << u.name << " (" << u.age << "세)";
        return os;
    }
};

#endif