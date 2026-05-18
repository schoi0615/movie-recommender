#include "UserManager.h"
#include <fstream>
#include <sstream>
#include <iostream>

void UserManager::addUser(const User& user) {
    users.push_back(user);
}

void UserManager::printAll() const {
    for (const auto& u : users) {
        std::cout << u << std::endl;
    }
}

User UserManager::findById(int id) const {
    for (const auto& u : users) {
        if (u.getId() == id) return u;
    }
    return User(-1, "Unknown", 0);
}

void UserManager::loadFromFile(const std::string& filename) {
    users.clear();
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: " << filename << " 파일을 열 수 없습니다." << std::endl;
        return;
    }
    std::string line;
    std::getline(file, line); // 헤더 건너뛰기
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string token;
        
        std::getline(ss, token, ',');
        int id = std::stoi(token);
        
        std::getline(ss, token, ',');
        std::string name = token;
        
        std::getline(ss, token, ',');
        int age = std::stoi(token);
        
        users.push_back(User(id, name, age));
    }
    file.close();
    std::cout << filename << " 로드 완료: " << users.size() << "건" << std::endl;
}

void UserManager::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: " << filename << " 저장 실패" << std::endl;
        return;
    }
    file << "id,name,age\n";
    for (const auto& u : users) {
        file << u.getId() << "," << u.getName() << "," << u.getAge() << "\n";
    }
    file.close();
    std::cout << filename << " 저장 완료: " << users.size() << "건" << std::endl;
}

int UserManager::size() const {
    return users.size();
}