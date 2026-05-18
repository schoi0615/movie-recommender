#include "RatingManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <set>

void RatingManager::addRating(const Rating& rating) {
    ratings.push_back(rating);
}

std::vector<Rating> RatingManager::findByUser(int userId) const {
    std::vector<Rating> userRatings;
    for (const auto& r : ratings) {
        if (r.getUserId() == userId) {
            userRatings.push_back(r);
        }
    }
    return userRatings;
}

std::vector<int> RatingManager::getAllUserIds() const {
    std::set<int> userIds;
    for (const auto& r : ratings) {
        userIds.insert(r.getUserId());
    }
    return std::vector<int>(userIds.begin(), userIds.end());
}

void RatingManager::printRatingsForMovie(int movieId) const {
    bool found = false;
    for (const auto& r : ratings) {
        if (r.getMovieId() == movieId) {
            std::cout << "유저 ID: " << r.getUserId() << " -> 평점: " << r.getScore() << "점" << std::endl;
            found = true;
        }
    }
    if (!found) {
        std::cout << "해당 영화에 등록된 평점이 없습니다." << std::endl;
    }
}

void RatingManager::loadFromFile(const std::string& filename) {
    ratings.clear();
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
        int uId = std::stoi(token);
        
        std::getline(ss, token, ',');
        int mId = std::stoi(token);
        
        std::getline(ss, token, ',');
        int score = std::stoi(token);
        
        ratings.push_back(Rating(uId, mId, score));
    }
    file.close();
    std::cout << filename << " 로드 완료: " << ratings.size() << "건" << std::endl;
}

void RatingManager::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: " << filename << " 저장 실패" << std::endl;
        return;
    }
    file << "userId,movieId,score\n";
    for (const auto& r : ratings) {
        file << r.getUserId() << "," << r.getMovieId() << "," << r.getScore() << "\n";
    }
    file.close();
    std::cout << filename << " 저장 완료: " << ratings.size() << "건" << std::endl;
}

int RatingManager::size() const {
    return ratings.size();
}